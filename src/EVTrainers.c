#include "defines.h"

struct poke_config{
    u8 evs;
    u8 ivs;
    u8 abilityID;
    u8 nature;
    u8 poke_ball;
    u8 shiny;
	u8 filler1;
	u8 filler2;
};
#define RANDOM_TRAINER_FLAG 0x40A4
struct poke_config config_table[256];
u16 ball_to_item(enum ball_index ball);
void create_trainer_pokemon(struct pokemon* poke, u16 trainerID, u8 purge)
{
	/*
	if(getflag(RANDOM_TRAINER_FLAG))
	{
		var_8015_trainer_opponent_A = trainerID = var_8004;
		clearflag(RANDOM_TRAINER_FLAG);
	}
	*/
    if (trainerID != 0x400 && battle_flags.trainer && !(BATTLE_FRONTIER_BATTLE || battle_flags.flagx800 || battle_flags.flag_x4000000))
    {
        struct trainer_data* trainer = &trainer_table[trainerID];
        if (purge)
            pokemon_purge_opponent();
        u8 no_of_pokes = trainer->poke_number;
        if (battle_flags.multibattle && no_of_pokes > 3)
            no_of_pokes = 3;
        u32 TiD = calculate_checksum(trainer, sizeof(struct trainer_data));
		u32 PiD = 0;
        struct poke_trainer_data* poke_info = trainer->poke_data;
        u8 struct_size = 8;
        if (trainer->custom_moves)
            struct_size += 8;
        for (u8 i = 0; i < no_of_pokes; i++)
        {
            struct poke_trainer_data* curr_poke = (struct poke_trainer_data*) ADD_POINTER(poke_info, i * struct_size);
            struct poke_config* config = &config_table[curr_poke->evs_id];
            u32 custom_pid = 0;           
            if (trainer->custom_moves || trainer->custom_item)
            {
                
				if(config->shiny)
				{					
                    PiD=TiD;
                    custom_pid = 1;
				}
                else
                {
					u8 nature = config->nature;
                    while(__umodsi3(PiD, 25) != nature)
                    {
                        PiD = rng() | (rng() << 0x10);
                    }
                    custom_pid = 1;
                }
				
            }
            pokemon_make_wrapper(&poke[i], curr_poke->poke_id, curr_poke->level, 0, custom_pid, PiD, 1, TiD);

            if (trainer->custom_item)
            {
                set_attributes(&poke[i], ATTR_HELD_ITEM, &curr_poke->item_id);
            }
            if (trainer->custom_moves)
            {
                for (u8 j = 0; j < 4; j++)
                {
                    set_attributes(&poke[i], ATTR_ATTACK_1 + j, &curr_poke->move[j]);
                }
            }
            if (trainer->custom_moves || trainer->custom_item)
            {
                for (u8 j = 0; j < 6; j++)
                {
                    set_attributes(&poke[i], ATTR_HP_EV + j, &config->evs);
                    set_attributes(&poke[i], ATTR_HP_IV + j, &config->ivs);
                }
				u16 item=ball_to_item(config->poke_ball);
                set_attributes(&poke[i], ATTR_POKEBALL, &item);
				set_attributes(&poke[i], ATTR_ABILITY_BIT, &config->abilityID);
            }
			//if(curr_poke->filler)
			//	set_attributes(&poke[i], ATTR_NAME, );
			calculate_stats_pokekmon(&poke[i]);
        }
			
        if (trainer->double_battle)
            battle_flags.double_battle = 1;
    }
}

struct poke_config config_table[256] = {
	{0, 0, 0, 13, 1, 0},
	{50, 8, 0, 13, 1, 0},
	{100, 16, 0, 13, 1, 0},
	{200, 24, 0, 13, 1, 0},
	{0, 0, 1, 13, 1, 0},
	{50, 8, 1, 13, 1, 0},
	{100, 16, 1, 13, 1, 0},
	{200, 24, 1, 13, 1, 0},
	{0, 0, 0, 10, 1, 0},
	{50, 8, 0, 10, 1, 0},
	{100, 16, 0, 10, 1, 0},
	{200, 24, 0, 10, 1, 0},
	{0, 0, 1, 10, 1, 0},
	{50, 8, 1, 10, 1, 0},
	{100, 16, 1, 10, 1, 0},
	{200, 24, 1, 10, 1, 0},
	{252, 31, 0, 1, 1, 0},
	{252, 31, 1, 1, 1, 0},
	{252, 31, 2, 1, 1, 0},
	{252, 31, 0, 1, BALL_CHERISH, 0},
	{252, 31, 1, 1, BALL_CHERISH, 0},
	{252, 31, 2, 1, BALL_CHERISH, 0},
	{252, 31, 0, 1, 1, 1},
	{252, 31, 1, 1, 1, 1},
	{252, 31, 2, 1, 1, 1},
	{252, 31, 0, 1, BALL_CHERISH, 1},
	{252, 31, 1, 1, BALL_CHERISH, 1},
	{252, 31, 2, 1, BALL_CHERISH, 1},
	{252, 31, 0, 2, 1, 0},
	{252, 31, 1, 2, 1, 0},
	{252, 31, 2, 2, 1, 0},
	{252, 31, 0, 2, BALL_CHERISH, 0},
	{252, 31, 1, 2, BALL_CHERISH, 0},
	{252, 31, 2, 2, BALL_CHERISH, 0},
	{252, 31, 0, 2, 1, 1},
	{252, 31, 1, 2, 1, 1},
	{252, 31, 2, 2, 1, 1},
	{252, 31, 0, 2, BALL_CHERISH, 1},
	{252, 31, 1, 2, BALL_CHERISH, 1},
	{252, 31, 2, 2, BALL_CHERISH, 1},
	{252, 31, 0, 3, 1, 0},
	{252, 31, 1, 3, 1, 0},
	{252, 31, 2, 3, 1, 0},
	{252, 31, 0, 3, BALL_CHERISH, 0},
	{252, 31, 1, 3, BALL_CHERISH, 0},
	{252, 31, 2, 3, BALL_CHERISH, 0},
	{252, 31, 0, 3, 1, 1},
	{252, 31, 1, 3, 1, 1},
	{252, 31, 2, 3, 1, 1},
	{252, 31, 0, 3, BALL_CHERISH, 1},
	{252, 31, 1, 3, BALL_CHERISH, 1},
	{252, 31, 2, 3, BALL_CHERISH, 1},
	{252, 31, 0, 4, 1, 0},
	{252, 31, 1, 4, 1, 0},
	{252, 31, 2, 4, 1, 0},
	{252, 31, 0, 4, BALL_CHERISH, 0},
	{252, 31, 1, 4, BALL_CHERISH, 0},
	{252, 31, 2, 4, BALL_CHERISH, 0},
	{252, 31, 0, 4, 1, 1},
	{252, 31, 1, 4, 1, 1},
	{252, 31, 2, 4, 1, 1},
	{252, 31, 0, 4, BALL_CHERISH, 1},
	{252, 31, 1, 4, BALL_CHERISH, 1},
	{252, 31, 2, 4, BALL_CHERISH, 1},
	{252, 31, 0, 5, 1, 0},
	{252, 31, 1, 5, 1, 0},
	{252, 31, 2, 5, 1, 0},
	{252, 31, 0, 5, BALL_CHERISH, 0},
	{252, 31, 1, 5, BALL_CHERISH, 0},
	{252, 31, 2, 5, BALL_CHERISH, 0},
	{252, 31, 0, 5, 1, 1},
	{252, 31, 1, 5, 1, 1},
	{252, 31, 2, 5, 1, 1},
	{252, 31, 0, 5, BALL_CHERISH, 1},
	{252, 31, 1, 5, BALL_CHERISH, 1},
	{252, 31, 2, 5, BALL_CHERISH, 1},
	{252, 31, 0, 7, 1, 0},
	{252, 31, 1, 7, 1, 0},
	{252, 31, 2, 7, 1, 0},
	{252, 31, 0, 7, BALL_CHERISH, 0},
	{252, 31, 1, 7, BALL_CHERISH, 0},
	{252, 31, 2, 7, BALL_CHERISH, 0},
	{252, 31, 0, 7, 1, 1},
	{252, 31, 1, 7, 1, 1},
	{252, 31, 2, 7, 1, 1},
	{252, 31, 0, 7, BALL_CHERISH, 1},
	{252, 31, 1, 7, BALL_CHERISH, 1},
	{252, 31, 2, 7, BALL_CHERISH, 1},
	{252, 31, 0, 8, 1, 0},
	{252, 31, 1, 8, 1, 0},
	{252, 31, 2, 8, 1, 0},
	{252, 31, 0, 8, BALL_CHERISH, 0},
	{252, 31, 1, 8, BALL_CHERISH, 0},
	{252, 31, 2, 8, BALL_CHERISH, 0},
	{252, 31, 0, 8, 1, 1},
	{252, 31, 1, 8, 1, 1},
	{252, 31, 2, 8, 1, 1},
	{252, 31, 0, 8, BALL_CHERISH, 1},
	{252, 31, 1, 8, BALL_CHERISH, 1},
	{252, 31, 2, 8, BALL_CHERISH, 1},
	{252, 31, 0, 9, 1, 0},
	{252, 31, 1, 9, 1, 0},
	{252, 31, 2, 9, 1, 0},
	{252, 31, 0, 9, BALL_CHERISH, 0},
	{252, 31, 1, 9, BALL_CHERISH, 0},
	{252, 31, 2, 9, BALL_CHERISH, 0},
	{252, 31, 0, 9, 1, 1},
	{252, 31, 1, 9, 1, 1},
	{252, 31, 2, 9, 1, 1},
	{252, 31, 0, 9, BALL_CHERISH, 1},
	{252, 31, 1, 9, BALL_CHERISH, 1},
	{252, 31, 2, 9, BALL_CHERISH, 1},
	{252, 31, 0, 10, 1, 0},
	{252, 31, 1, 10, 1, 0},
	{252, 31, 2, 10, 1, 0},
	{252, 31, 0, 10, BALL_CHERISH, 0},
	{252, 31, 1, 10, BALL_CHERISH, 0},
	{252, 31, 2, 10, BALL_CHERISH, 0},
	{252, 31, 0, 10, 1, 1},
	{252, 31, 1, 10, 1, 1},
	{252, 31, 2, 10, 1, 1},
	{252, 31, 0, 10, BALL_CHERISH, 1},
	{252, 31, 1, 10, BALL_CHERISH, 1},
	{252, 31, 2, 10, BALL_CHERISH, 1},
	{252, 31, 0, 11, 1, 0},
	{252, 31, 1, 11, 1, 0},
	{252, 31, 2, 11, 1, 0},
	{252, 31, 0, 11, BALL_CHERISH, 0},
	{252, 31, 1, 11, BALL_CHERISH, 0},
	{252, 31, 2, 11, BALL_CHERISH, 0},
	{252, 31, 0, 11, 1, 1},
	{252, 31, 1, 11, 1, 1},
	{252, 31, 2, 11, 1, 1},
	{252, 31, 0, 11, BALL_CHERISH, 1},
	{252, 31, 1, 11, BALL_CHERISH, 1},
	{252, 31, 2, 11, BALL_CHERISH, 1},
	{252, 31, 0, 13, 1, 0},
	{252, 31, 1, 13, 1, 0},
	{252, 31, 2, 13, 1, 0},
	{252, 31, 0, 13, BALL_CHERISH, 0},
	{252, 31, 1, 13, BALL_CHERISH, 0},
	{252, 31, 2, 13, BALL_CHERISH, 0},
	{252, 31, 0, 13, 1, 1},
	{252, 31, 1, 13, 1, 1},
	{252, 31, 2, 13, 1, 1},
	{252, 31, 0, 13, BALL_CHERISH, 1},
	{252, 31, 1, 13, BALL_CHERISH, 1},
	{252, 31, 2, 13, BALL_CHERISH, 1},
	{252, 31, 0, 14, 1, 0},
	{252, 31, 1, 14, 1, 0},
	{252, 31, 2, 14, 1, 0},
	{252, 31, 0, 14, BALL_CHERISH, 0},
	{252, 31, 1, 14, BALL_CHERISH, 0},
	{252, 31, 2, 14, BALL_CHERISH, 0},
	{252, 31, 0, 14, 1, 1},
	{252, 31, 1, 14, 1, 1},
	{252, 31, 2, 14, 1, 1},
	{252, 31, 0, 14, BALL_CHERISH, 1},
	{252, 31, 1, 14, BALL_CHERISH, 1},
	{252, 31, 2, 14, BALL_CHERISH, 1},
	{252, 31, 0, 15, 1, 0},
	{252, 31, 1, 15, 1, 0},
	{252, 31, 2, 15, 1, 0},
	{252, 31, 0, 15, BALL_CHERISH, 0},
	{252, 31, 1, 15, BALL_CHERISH, 0},
	{252, 31, 2, 15, BALL_CHERISH, 0},
	{252, 31, 0, 15, 1, 1},
	{252, 31, 1, 15, 1, 1},
	{252, 31, 2, 15, 1, 1},
	{252, 31, 0, 15, BALL_CHERISH, 1},
	{252, 31, 1, 15, BALL_CHERISH, 1},
	{252, 31, 2, 15, BALL_CHERISH, 1},
	{252, 31, 0, 16, 1, 0},
	{252, 31, 1, 16, 1, 0},
	{252, 31, 2, 16, 1, 0},
	{252, 31, 0, 16, BALL_CHERISH, 0},
	{252, 31, 1, 16, BALL_CHERISH, 0},
	{252, 31, 2, 16, BALL_CHERISH, 0},
	{252, 31, 0, 16, 1, 1},
	{252, 31, 1, 16, 1, 1},
	{252, 31, 2, 16, 1, 1},
	{252, 31, 0, 16, BALL_CHERISH, 1},
	{252, 31, 1, 16, BALL_CHERISH, 1},
	{252, 31, 2, 16, BALL_CHERISH, 1},
	{252, 31, 0, 17, 1, 0},
	{252, 31, 1, 17, 1, 0},
	{252, 31, 2, 17, 1, 0},
	{252, 31, 0, 17, BALL_CHERISH, 0},
	{252, 31, 1, 17, BALL_CHERISH, 0},
	{252, 31, 2, 17, BALL_CHERISH, 0},
	{252, 31, 0, 17, 1, 1},
	{252, 31, 1, 17, 1, 1},
	{252, 31, 2, 17, 1, 1},
	{252, 31, 0, 17, BALL_CHERISH, 1},
	{252, 31, 1, 17, BALL_CHERISH, 1},
	{252, 31, 2, 17, BALL_CHERISH, 1},
	{252, 31, 0, 19, 1, 0},
	{252, 31, 1, 19, 1, 0},
	{252, 31, 2, 19, 1, 0},
	{252, 31, 0, 19, BALL_CHERISH, 0},
	{252, 31, 1, 19, BALL_CHERISH, 0},
	{252, 31, 2, 19, BALL_CHERISH, 0},
	{252, 31, 0, 19, 1, 1},
	{252, 31, 1, 19, 1, 1},
	{252, 31, 2, 19, 1, 1},
	{252, 31, 0, 19, BALL_CHERISH, 1},
	{252, 31, 1, 19, BALL_CHERISH, 1},
	{252, 31, 2, 19, BALL_CHERISH, 1},
	{252, 31, 0, 20, 1, 0},
	{252, 31, 1, 20, 1, 0},
	{252, 31, 2, 20, 1, 0},
	{252, 31, 0, 20, BALL_CHERISH, 0},
	{252, 31, 1, 20, BALL_CHERISH, 0},
	{252, 31, 2, 20, BALL_CHERISH, 0},
	{252, 31, 0, 20, 1, 1},
	{252, 31, 1, 20, 1, 1},
	{252, 31, 2, 20, 1, 1},
	{252, 31, 0, 20, BALL_CHERISH, 1},
	{252, 31, 1, 20, BALL_CHERISH, 1},
	{252, 31, 2, 20, BALL_CHERISH, 1},
	{252, 31, 0, 21, 1, 0},
	{252, 31, 1, 21, 1, 0},
	{252, 31, 2, 21, 1, 0},
	{252, 31, 0, 21, BALL_CHERISH, 0},
	{252, 31, 1, 21, BALL_CHERISH, 0},
	{252, 31, 2, 21, BALL_CHERISH, 0},
	{252, 31, 0, 21, 1, 1},
	{252, 31, 1, 21, 1, 1},
	{252, 31, 2, 21, 1, 1},
	{252, 31, 0, 21, BALL_CHERISH, 1},
	{252, 31, 1, 21, BALL_CHERISH, 1},
	{252, 31, 2, 21, BALL_CHERISH, 1},
	{252, 31, 0, 22, 1, 0},
	{252, 31, 1, 22, 1, 0},
	{252, 31, 2, 22, 1, 0},
	{252, 31, 0, 22, BALL_CHERISH, 0},
	{252, 31, 1, 22, BALL_CHERISH, 0},
	{252, 31, 2, 22, BALL_CHERISH, 0},
	{252, 31, 0, 22, 1, 1},
	{252, 31, 1, 22, 1, 1},
	{252, 31, 2, 22, 1, 1},
	{252, 31, 0, 22, BALL_CHERISH, 1},
	{252, 31, 1, 22, BALL_CHERISH, 1},
	{252, 31, 2, 22, BALL_CHERISH, 1},
	{252, 31, 0, 23, 1, 0},
	{252, 31, 1, 23, 1, 0},
	{252, 31, 2, 23, 1, 0},
	{252, 31, 0, 23, BALL_CHERISH, 0},
	{252, 31, 1, 23, BALL_CHERISH, 0},
	{252, 31, 2, 23, BALL_CHERISH, 0},
	{252, 31, 0, 23, 1, 1},
	{252, 31, 1, 23, 1, 1},
	{252, 31, 2, 23, 1, 1},
	{252, 31, 0, 23, BALL_CHERISH, 1},
	{252, 31, 1, 23, BALL_CHERISH, 1},
	{252, 31, 2, 23, BALL_CHERISH, 1},
};
