//70260
#include "defines.h"
u16 ball_to_item(enum ball_index ball);

enum gender {
	MALE, FEMALE = 0xfe, NO_GENDER = 0xff
};

u16 get_lowest_evo_stage0(u16 species, u16 item)
{
	//New Eevolution to Eevee
	if (species == 0x20b || species == 0x20c || species == 0x2F1) {
		species = 0x85;
		goto END;
	}
	//Special Vivillon to Scatterbug
	if (species >= 0x3f1 && species <= 0x403) {
		species = 0x2cd;
		goto END;
	}
	//Manaphy to Phione
	if (species == 0x21f){
		species = 0x21e;
		goto END;
	}
	for (u16 lower_poke = 1; lower_poke < POKES_NO; lower_poke++) {
		const struct evolution_sub* evos = (*evo_table)[lower_poke];
		for (u8 j = 0; j < 5; j++) {
			const struct evolution_sub* evo = &evos[j];
			if (evo->poke == species && evo->method < 0xFA) {
				if (evo->pad0 == 0x3f && evo->pad1 != item) {
					//Nidoran M/F
					if (species == 0x1d || species == 0x20)
						species = (__umodsi3(rng(),2))? 0x1d: 0x20;
					//Volbeat/Illumise
					if (species == 0x182 || species == 0x183)
						species = (__umodsi3(rng(),2))? 0x182: 0x183;
					goto END;
				}
				species = lower_poke;
				lower_poke = 0;
				break;
			}
		}
	}
	END: return species;
}

u16 get_lowest_evo_stage(void* poke, u8 id)
{
	poke += id * 0x8c;
	u16 species = get_attributes(poke, ATTR_SPECIES, 0);
	u16 item = get_attributes(poke, ATTR_HELD_ITEM, 0);
	return get_lowest_evo_stage0(species, item);
}

bool minior_match(u32 PiD, u16 species) //return 1 when pokemon is a matching minior core or other pokemon
{
	u8 mod = __umodsi3(PiD, 7);
	switch (species) {
	case 0x3CB:
		return (mod == 0);
	case 0x3EB:
		return (mod == 1);
	case 0x3EC:
		return (mod == 2);
	case 0x3ED:
		return (mod == 3);
	case 0x3EE:
		return (mod == 4);
	case 0x3EF:
		return (mod == 5);
	case 0x3F0:
		return (mod == 6);
	default:
		return 1;
	}
}

u32 get_shiny_pid(u32 PiD, u32 TiD, u8 wanted_nature, u8 wanted_gender, s32 pid_tries, u16 species)
{
	do {
		do {
			PiD = rng() | (rng() << 0x10);
		} while ((wanted_gender && wanted_gender != gender_from_pid(species, PiD))
				|| (wanted_nature != 30 && wanted_nature != __umodsi3(PiD, 25))
				|| !minior_match(PiD, species));
		pid_tries--;
	} while (pid_tries >= 0 && !is_poke_shiny(TiD, PiD));
	return PiD;
}

void change_ball(struct pokemon* poke, struct pokemon* target)
{
	u16 item = get_attributes(poke, ATTR_POKEBALL, 0);
	if (item == BALL_MASTER || item == BALL_CHERISH)
		item = BALL_POKE;
	item = ball_to_item(item);
	set_attributes(target, ATTR_POKEBALL, &item);
}

struct pokemon* get_mother(void* poke_parent)
{
	if (get_attributes(poke_parent, ATTR_SPECIES, 0) == POKE_DITTO || poke_get_gender(poke_parent + 0x8c) == FEMALE) {
		poke_parent += 0x8c;
	}
	return poke_parent;
}

u8 get_everstone_nature(void* poke, void* poke_parent)
{
	bool everstone1 = (get_attributes(poke_parent, ATTR_HELD_ITEM, 0) == 0xC3);
	bool everstone2 = (get_attributes(poke_parent + 0x8C, ATTR_HELD_ITEM, 0) == 0xC3);
	u8 nature = __umodsi3(get_attributes(poke, ATTR_PID, 0), 25);
	if (everstone1 || everstone2) {
		if (everstone1)
			nature = __umodsi3(get_attributes(poke_parent, ATTR_PID, 0), 25);
		if (everstone2 > __umodsi3(rng(), everstone1 + everstone2))
			nature = __umodsi3(get_attributes(poke_parent + 0x8C, ATTR_PID, 0), 25);
	}
	return nature;
}

void do_egg_shiny(void* poke, void* poke_parent)
{
	u16 species = get_attributes(poke, ATTR_SPECIES, 0);
	u32 PiD = get_attributes(poke, ATTR_PID, 0);
	u32 TiD = get_full_trainerID();
	u8 nature = get_everstone_nature(poke, poke_parent);
	u8 gender = gender_from_pid(species, PiD);
	PiD = get_shiny_pid(PiD, TiD, nature, gender, 2 * (SHINY_CHARM != 0x0 && checkitem(SHINY_CHARM, 1)), species);
	set_attributes(poke, ATTR_PID, &PiD);
}

bool learned_move(struct pokemon* poke, u16 move)
{
	for (u8 j = 0; j < 4; j++) {
		if (get_attributes(poke, ATTR_ATTACK_1 + j, 0) == move)
			return true;
	}
	return false;
}

void do_moves_heritance(struct pokemon* poke, struct pokemon* poke_parent)
{
	u8 egg_moves_no = get_egg_moves(poke, egg_move_buffer);
	for (u8 i = 0; i < egg_moves_no; i++) {
		u16 move = egg_move_buffer[i];
		if (learned_move(poke_parent, move) && !learned_move(poke, move)
				&& teach_move_in_available_slot(poke, move) == 0xFFFF) {
			new_move_for_the_first(poke, move);
		}
	}
}

void set_egg_poke(struct pokemon* poke)
{
	void* poke_parent = (void*) sav1;
	poke_parent += 0x3030;
	if (get_attributes(poke_parent, ATTR_HELD_ITEM, 0) == 0x3A
			|| get_attributes(poke_parent + 0x8c, ATTR_HELD_ITEM, 0) == 0x3A) {
		u8 times = __umodsi3(rng(), 6);
		for (u8 x = 0; x < 6; x++) {
			if (x == times) {
				continue;
			}
			void* poke_inheritance = poke_parent + 0x8c * (rng() & 1);
			u8 ivs = get_attributes(poke_inheritance, ATTR_HP_IV + x, 0);
			set_attributes(poke, ATTR_HP_IV + x, &ivs);
		}
	}
	poke_parent = get_mother(poke_parent);
	change_ball(poke_parent, poke);
	do_egg_shiny(poke, poke_parent);
	do_moves_heritance(poke, poke_parent);
}

u8* get_flag_address_new(u16 flagID)
{
	if (flagID < 0x4000) { //use standard location ;0 - 0x95F
		u16 flag_byte = flagID >> 3;
		return &sav1->flags[flag_byte];
	} else if (flagID < 0x4680) {
		flagID -= 0x4000;
		u16 flag_byte = flagID >> 3;
		if (flag_byte <= 51)
			return &sav1->seenflags_1[flag_byte];
		flag_byte -= 52;
		if (flag_byte <= 51)
			return &sav1->seenflags_2[flag_byte];
		flag_byte -= 52;
		if (flag_byte <= 51)
			return &sav2->seen_flags[flag_byte];
		flag_byte -= 52;
		if (flag_byte <= 51)
			return &sav2->caught_flags[flag_byte];
		return NULL;
	}
	return NULL;
}

