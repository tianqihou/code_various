#include "defines.h"

#define EQUAL 0
#define DEF_HIGHER 1
#define ATK_HIGHER 2

enum evo_index{
    LEVEL_UP,
    TRADE,
    STONE_ACTUAL_EVO,
    STONE_CAN_EVO,
};

bool time_check(u8 from, u8 to)
{
    update_rtc();
    u8 hour = rtc_hex.hour;
    if (to >= from)
    {
        if (hour >= from && hour <= to)
            return 1;
        return 0;
    }
    else
    {
        if (hour >= from || hour <= to)
            return 1;
		return 0;
    }
    
}

bool happiness_evo_check(struct pokemon* poke, u8 from, u8 to)
{
    if (time_check(from, to) && get_attributes(poke, ATTR_HAPPINESS, 0) > 219)
        return 1;
    return 0;
}

bool attack_def_evo_check(struct pokemon* poke, u8 condition)
{
    u16 atk = get_attributes(poke, ATTR_ATTACK, 0);
    u16 def = get_attributes(poke, ATTR_DEFENCE, 0);
    if (atk == def && condition == EQUAL)
    {
        return 1;
    }
    else if (atk > def && condition == ATK_HIGHER)
    {
        return 1;
    }
    else if(atk < def && condition == DEF_HIGHER)
    {
		return 1;
    }
	return 0;
}

#if EEVEE_TABLE == true
    #define eevee_evos 8
    struct evolution_sub eevee_table[eevee_evos] =  {
	{7, 0, ITEM_WATERSTONE, POKE_VAPOREON, 0x00},
	{7, 0, ITEM_THUNDERSTONE, POKE_JOLTEON, 0x00},
	{7, 0, ITEM_FIRESTONE, POKE_FLAREON, 0x00},
	{2, 0, 0, POKE_ESPEON, 0x00},
	{3, 0, 0, POKE_UMBREON, 0x00},
	{33, 0x18, 0x01, 0x20c, 0x53}, //Glaceon, Shoal Cave, Ice Room
	{17, 0, 0x3b, 0x20b, 0x00}, //Leafeon, Petalburg Woods
	{29, 0, TYPE_FAIRY, 0x2F1, 0x00}, // XX is the index of Sylveon method
    };
#endif // EEVEE_TABLE

#if SPEWPA_TABLE == true
    #define spewpa_evos 20
    struct evolution_sub spewpa_table[spewpa_evos] =  {
	{32, STEADY_SNOW, 0x0c, 0x3f1, 0x00}, //Polar
	{32, SNOWFLAKES, 0x0c, 0x3f2, 0x00}, //Tundra
	{31, 0xcd, 0x0c, 0x3f3, 0x00}, //Continental
	{31, 0x0f, 0x0c, 0x3f4, 0x00}, //Garden
	{32, STEADY_MIST, 0x0c, 0x3f5, 0x00}, //Elegant
	{33, 0x18, 0x0c, 0x3f6, 0x53}, //Icy Snow
	{31, 0xBB, 0x0c, 0x3f7, 0x00}, //Modern
	{31, 0xcc, 0x0c, 0x3f8, 0x00}, //Marine
	{31, 0x4e, 0x0c, 0x3f9, 0x00}, //Archipelago
	{31, 0x4d, 0x0c, 0x3fa, 0x00}, //HighPlains
	{32, SANDSTORM, 0x0c, 0x3fb, 0x00}, //Sandstorm
	{31, 0x22, 0x0c, 0x3fc, 0x00}, //River
	{24, 0, 0x0c, 0x3fd, 0x00}, //Monsoon
	{31, 0xc9, 0x0c, 0x3fe, 0x00}, //Savanna
	{31, 0x50, 0x0c, 0x3ff, 0x00}, //Sun
	{34, 0x06, 0x0c, 0x400, 0x00}, //Ocean
	{31, 0x3b, 0x0c, 0x401, 0x00}, //Jungle
	{31, 0xd3, 0x0c, 0x402, 0x00}, //Poke Ball
	{31, 0x47, 0x0c, 0x403, 0x00}, //Fancy
	{4, 0, 0x0c, 0x2cf, 0x00}, //Meadow
    };
#endif // SPEWPA_TABLE

u16 cur_map_get_blockid_at(s16 x_to, s16 y_from);
u16 try_evolving_poke(struct pokemon* poke, enum evo_index index, u16 stoneID)
{
    u16 held_item = get_attributes(poke, ATTR_HELD_ITEM, 0);
    if (get_item_battle_function(held_item) == ITEM_EFFECT_EVERSTONE && index != STONE_ACTUAL_EVO && index != STONE_CAN_EVO)
        return 0;
    u16 species = get_attributes(poke, ATTR_SPECIES, 0);
    u16 lvl = get_attributes(poke, ATTR_LEVEL, 0);
    u32 pid_form = __umodsi3(get_attributes(poke, ATTR_PID, 0) >> 0x10, 10);

    const struct evolution_sub* evos = (*evo_table)[species];
    u8 evos_num = EVO_PER_POKE;
    //check eevee
    if (EEVEE_TABLE == true && species == POKE_EEVEE)
    {
        evos = eevee_table;
        evos_num = eevee_evos;
    }
	//check spewpa
    if (SPEWPA_TABLE == true && species == 0x2ce)//POKE_SPEWPA==0x2ce
    {
        evos = spewpa_table;
        evos_num = spewpa_evos;
    }
    for (u8 i = 0; i < evos_num; i++)
    {
        const struct evolution_sub* evo = &evos[i];
        u8 evolving = 0;
        u8 item_delete = 0;
        u8 method = evo->method;
        switch (method)
        {
        case 0: //no evolution
            return 0;
        case 1: //happiness
            evolving = happiness_evo_check(poke, 0, 24);
            break;
        case 2: //happiness daytime
            evolving = happiness_evo_check(poke, DAY_FIRST_HOUR, DAY_LAST_HOUR);
            break;
        case 3: //happiness nighttime
            evolving = happiness_evo_check(poke, NIGHT_FIRST_HOUR, NIGHT_FIRST_HOUR);
            break;
        LEVELUP_EVO:
        case 4: //level up
        case 13: //extra pokemon creation(Nincada to Ninjask)
            if (evo->paramter <= lvl && !(evo->pad0 == 0xff && curr_mapheader.name == 0x06))
                evolving = 1;
            break;
        case 6: //trade with an item
            if (held_item != evo->paramter)
                break;
            item_delete = 1;
        case 5: //trade
            if (index == TRADE)
                evolving = 1;
            break;
        STONE_EVO:
        case 7: //item
            if ((index == STONE_CAN_EVO || index == STONE_ACTUAL_EVO) && stoneID == evo->paramter && !(evo->pad0 == 0xff && curr_mapheader.name == 0x06))
                evolving = 1;
            break;
        case 8: //attack greater than defense
            evolving = attack_def_evo_check(poke, ATK_HIGHER);
			goto LEVELUP_EVO;
            break;
        case 9: //attack equal to defense
            evolving = attack_def_evo_check(poke, EQUAL);
			goto LEVELUP_EVO;
            break;
        case 10: //attack lower than defense
            evolving = attack_def_evo_check(poke, DEF_HIGHER);
			goto LEVELUP_EVO;
            break;
        case 11: //pid <0, 4>
            if (pid_form < 5)
                goto LEVELUP_EVO;
            break;
        case 12: //pid <5, 9>
            if (pid_form >= 5)
                goto LEVELUP_EVO;
            break;
        case 14: //extra pokemon being created(Nincada to Shedinja)
            break;
        case 15: //beauty evolution
            if (get_attributes(poke, ATTR_BEAUTY, 0) >= 170)
                evolving = 1;
            break;
        case 16: //level up when knows a certain move
            for (u8 i = 0; i < 4; i++)
            {
                if (get_attributes(poke, ATTR_ATTACK_1 + i, 0) == evo->paramter)
                {
                    evolving = 1;
                    break;
                }
            }
            break;
        case 17: //level up only in certain area, parameter is map name
           if (curr_mapheader.name == evo->paramter)
                evolving = 1;
            break;
        case 18: //by level at day time
            if (time_check(DAY_FIRST_HOUR, DAY_LAST_HOUR) && !(species == POKE_ROCKRUFF && time_check(17,17)))
                goto LEVELUP_EVO;
            break;
        case 19: //by level at night time
            if (time_check(NIGHT_FIRST_HOUR, NIGHT_LAST_HOUR))
                goto LEVELUP_EVO;
			break;
        case 20: //level up at day when holding an item
            if (time_check(DAY_FIRST_HOUR, DAY_LAST_HOUR) && held_item == evo->paramter)
            {
                evolving = 1;
                item_delete = 1;
            }
            break;
        case 21: //level up at night when holding an item
            if (time_check(NIGHT_FIRST_HOUR, NIGHT_LAST_HOUR) && held_item == evo->paramter)
            {
                evolving = 1;
                item_delete = 1;
            }
            break;
        case 22: //by level only if male
            if (poke_get_gender(poke) == 0)
                goto LEVELUP_EVO;
            break;
        case 23: //by level only if female
            if (poke_get_gender(poke) == 0xFE)
                goto LEVELUP_EVO;
            break;
        case 24: //by level if it's raining (Goodra evo)
            if (sav1->ov_weather == RAINY || sav1->ov_weather == THUNDERSTORM)
                goto LEVELUP_EVO;
            break;
        case 25: //by level if there's specific pokemon in party (Mantyke evo)
            for (u8 i = 0; i < 6; i++)
            {
                struct pokemon* curr_poke = &party_player[i];
                if (get_attributes(curr_poke, ATTR_SPECIES, 0) == evo->paramter && curr_poke != poke)
                {
                    evolving = 1;
                    break;
                }
            }
            break;
        case 26: //level up if there's a pokemon with specific type in user's paty (Pancham's evo); type in pad1
            for (u8 i = 0; i < 6; i++)
            {
                struct pokemon* curr_poke = &party_player[i];
                u16 curr_species = get_attributes(curr_poke, ATTR_SPECIES, 0);
                if (curr_poke != poke && curr_species)
                {
                    const struct poke_basestats* stats = &((*basestat_table)[curr_species]);
                    if (stats->type1 == evo->pad1 || stats->type2 == evo->pad1)
                    {
                        goto LEVELUP_EVO;
                    }
                }
            }
            break;
        case 27: //male by stone
            if (poke_get_gender(poke) == 0)
                goto STONE_EVO;
            break;
        case 28: //female by stone
            if (poke_get_gender(poke) == 0xFE)
                goto STONE_EVO;
            break;
        case 29: //level up when knows move of a type
            for (u8 i = 0; i < 4; i++)
            {
                u16 move = get_attributes(poke, ATTR_ATTACK_1 + i, 0);
                if (move && (*move_table)[move].type == evo->paramter)
                {
                    evolving = 1;
                    break;
                }
            }
            break;
		case 30://level up at dusk »Æ»èÎª17:00-17:59
			if (time_check(17, 17))
                goto LEVELUP_EVO;
            break;
		case 31://certain area
			if (curr_mapheader.name == evo->pad0)
                goto LEVELUP_EVO;
            break;
		case 32://certain weather
            if (sav1->ov_weather == evo->pad0)
                goto LEVELUP_EVO;
            break;
		case 33://certain map_bank & map_no
			if (sav1->map_bank == evo->pad0 && sav1->map_no == evo->pad1)
				goto LEVELUP_EVO;
			break;
		case 34://certain area, dawn or dusk
			if (curr_mapheader.name == evo->pad0 && (time_check(17,17) || time_check(5,5)))
				goto LEVELUP_EVO;
			break;
		/*case 35://certain area, certain tile
			if (curr_mapheader.name == evo->pad0){
				u16 curr_block = cur_map_get_blockid_at(sav1->x_coords + 7, sav1->y_coords + 7);
				if (curr_block == 0x170 || curr_block == 0x208 || curr_block == 0x210){
					goto LEVELUP_EVO;
				}
			}
			break;*/
        }
        if (evolving)
        {
            if (index == LEVEL_UP || (method == 5 || method == 6 || method == 7 || method == 27 || method == 28)) //only via LVL up unless evolution types are different
            {
                if (item_delete)
                {
                    u16 zero = 0;
                    set_attributes(poke, ATTR_HELD_ITEM, &zero);
                }
                return evo->poke;
            }
        }
    }
    return 0;
}
