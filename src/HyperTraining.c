//输入poke，抽取PM的padding_maybe，并取其2到7位，分别代表HP、攻击、防御、速度、特攻、特防是否经历了超级训练
#include "defines.h"

u8 get_ivs(struct pokemon* poke, u8 attr){
	if(poke->padding_maybe >> (attr + 1) & 1)
		return 31;
	return get_attributes(poke, ATTR_HP_IV + attr, 0);
}

void calculate_stats_pokemon(struct pokemon* poke){
	u16 species = get_attributes(poke, ATTR_SPECIES, 0);
	const u8* base =  &(*basestat_table)[species].base_hp;
	u8 level = poke_lvl_from_exp(poke);
	set_attributes(poke, ATTR_LEVEL, &level);
	
	u8 nature = get_pokemon_nature(poke);
	for (u8 i = 0; i < 6; i++){
		u8 ivs = get_ivs(poke, i);
		u8 evs = get_attributes(poke, ATTR_HP_EV + i, 0);
		u16 stats;
		//HP
		if (i == 0){
			//Shedinja
			if (species == POKE_SHEDINJA)
				stats = 1;
			else {
				stats = __udivsi3(((base[i] << 1) + ivs + (evs >> 2)) * level, 100) + 10 + level;
			}
			//Calculate current HP
			u16 curr_hp;
			if (stats - get_attributes(poke, ATTR_TOTAL_HP, 0) > 0)
				curr_hp = get_attributes(poke, ATTR_CURRENT_HP, 0) + stats - get_attributes(poke, ATTR_TOTAL_HP, 0);
			else
				curr_hp = get_attributes(poke, ATTR_CURRENT_HP, 0);
			if (curr_hp > stats)
				curr_hp = stats;
			
			set_attributes(poke, ATTR_TOTAL_HP, &stats);
			set_attributes(poke, ATTR_CURRENT_HP, &curr_hp);
		}
		//Not HP
		else {
			stats = __udivsi3(((base[i] << 1) + ivs + (evs >> 2)) * level, 100) + 5;
			//Calculate Nature Factor
			u8 nat_factor = 10;
			if (__umodsi3(nature, 6) != 0 && __udivsi3(nature, 5) == i - 1){
				nat_factor = 11;
			}
			else if (__umodsi3(nature, 6) != 0 && __umodsi3(nature, 5) == i - 1)
				nat_factor = 9;
			//Apply Nature Factor
			stats = __udivsi3(stats * nat_factor, 10);
		}
		//set stats
		set_attributes(poke, ATTR_TOTAL_HP + i, &stats);
	}
}