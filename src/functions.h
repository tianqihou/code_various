#ifndef VANILLA_FUNCTIONS
#define VANILLA_FUNCTIONS

#include "types.h"
#include "structs.h"

u16 __attribute__((long_call)) teach_move_in_available_slot(const struct pokemon* , u16 move);
void new_move_for_the_first(const struct pokemon*, u16 move);
void  calculate_stats_pokekmon(struct pokemon* poke);
void* get_item_desc(u16 itemID);
u8 get_item_quality(u16 itemID);
u8 get_item_battle_function(u16 itemID);
u8 checkitem(u16 itemID, u16 quantity);
u16 str_font1_get_min_width(void* string_address, u16 min_width);
u16 str_rbox_max_width(u16 width);
u16 str_rbox_X_pos(u8 x_pos, u16 width);
u8 rbox_create_and_sth(u8 x_pos, u8 y_pos, u8 width, u8 options_no);
void rbox_frame_menupalette(u8 rboxID, u8 vram_3);
void rbox_put_multichoice_text(u8 rboxID, u8 height, struct multichoice_option[]);
void rbox_create_multichoice_cursor(u8 rboxID, u8 options_no, u8 starting_option);
void bg_mark_for_sync(u8 bgID);
void prepare_multichoice_task(u8 canhitB, u8 options_no, u8 rboxID, u8 multichoiceID);
u32 get_attributes(struct pokemon* poke_address, u8 request, void* dst);
void set_attributes(struct pokemon* poke_address, u8 request, void* new_value);
u8 check_if_move_learnt(struct pokemon* poke_address, u16 move);
u8 check_walkrun_bitfield_for(u8 bitfield);
void pokemon_purge_opponent(void);
void pokemon_slot_purge(struct pokemon*);
void pokemon_make_wrapper(struct pokemon* poke, u16 species, u8 lvl, u8 IVs, u8 pid_set, u32 PiD, u8 tid_set, u32 TiD);
u32 calculate_checksum(void* ptr, u32 to_read);
u16 pokemon_calc_checksum(struct pokemon* poke);
u16 rng();
void pokemon_encrypt(struct pokemon* poke);
void pokemon_decrypt(struct pokemon* poke);
void put_money_on_rbox(u8 rboxID, u8 width, u8 unkown, s32 value, u8 unkown2);
void* unsigned_int_to_string(void* destination, u32 num, u8 formatting, u8 max_digits);
u8 pokesummary_create_rbox(struct rbox_rom[], u8 index);
void string_box_build(u8 arg1, void *destination, s8 width_maybe, u8 arg4, u8 arg5, u8 arg6);
u8 bag_add_item(u16 itemID, u16 quantity);
void clear_f7_texts();
void store_f7_text(u8 ID, void* string_ptr);
void display_f7_texts(void* dst, void* src);
void pokemenu_print(u8 arg0, void* string_ptr, u8 arg2, void* ptr);
void fbox_print_sth(u8 rboxID, u8 fontID, u8 arg2, u8 arg3, void* text_clr, u8 arg5, void* text_ptr);
u8 battle_print_on_HPmenu(void* text_ptr, u8 width, u8 height, u8 arg3, struct rbox_rom* rbox);
void update_rtc();
u8 poke_get_gender(struct pokemon* poke);
u8 gender_from_pid(u16 species, u32 PiD);
void nullsub_1485(struct object*);
bool check_move_tutor_compatibility(u16 species, u8 tutor_id);
bool check_TM_compatibility(u16 species, u8 TM_id);
u8 get_egg_moves(struct pokemon* poke, u16* buffer);
u16 script_read_halfword(struct ov_script*);
u32 script_read_word(struct ov_script*);
u16* get_var_address(u16 varID);
u16 get_var_value(u16 varID);
void prepare_string_inserter(u8 caseID, void* text_ptr, u16 species, u8 gender, u32 PiD, void* callback2);
void c2_exit_to_overworld_2_switch(void);
void set_callback2(void* callback2);
void script_exec_return_with_music(void);
void* strcpy_xFF_terminated_0(void* dst, void* src);
void* str_append(void* dst, void* src);
u8 compare_two_strings(void* string1, void* string2);
void sub_81972C4(u8 arg);
void textbox_load_box_graphics(void);
void fdecode_and_copy(void* dst, void* src);
u16 box_related_one(u8 arg0, u8 arg1, void* text, u8 speed, u32 sp_0, u32 sp_4, u32 sp_8, u32 sp_C);
u8 get_text_speed();
u8 task_add(void* function, u8 priority);
void task_add_textbox(void);
void rbox_prepare_textbox(u8 rboxID, u8 arg1);
u16 textbox_print_ret();
void task_delete(u8 taskID);
void prep_standart_textbox(u8 flags);
void menu_option_add(enum menu_options);
void add_to_menu_options(u8* options, u8 amount, enum menu_options);
bool getflag(u16 flag);
void setflag(u16 flag);
void clearflag(u16 flag);
bool pokeblock_give(struct pokeblock*);
void prepare_ov_textbox(u8 rboxID, u8 arg1);
void fish_obj_hero_anim(void);
void put_text_on_rbox(u8 rboxID, u8 font, void* text_ptr, u8 move_right, u8 sp_0, u8 sp_4, u32 sp_8);
bool check_fishing_data_for_map(void);
u8 get_ability_of_poke(struct pokemon*);
u8 sp121_get_player_direction(void);
u8 get_biting_fishing_frame(u8 direction);
u8 get_fishing_direction_frame(u8 direction);
void obj_change_img(struct object*, u8 frame_no);
bool check_fishing_for_feebas(void);
u8 get_wild_pokemon_level(struct wild_pokemon*);
u16 get_wild_data_index_for_map(void);
void increment_encrypted_counter(u8 counter);
u32 get_encrypted_counter(u8 counter);
void wild_battle_or_safari_battle(void);
void rbox_fill(u8 rboxID, u8 what_to_fill);
void textbox_print_char(void);
void npc_release_all(void);
void increment_fishing_tries_consider_tv_report(bool success);
u8 task_find_id_by_funcptr(void* task);
u8 fishing_rod_get_wild_slot(u8 rod);
bool is_pokemon_shiny(struct pokemon*);
bool is_poke_shiny(u32 TiD, u32 PiD);
u8 get_wild_poke_nature(void);
u32 get_full_trainerID(void);
u8 get_sav_data(u8 saveID);
void rbox_to_vram(u8 rboxID, u8 caseID);
void save_hall_of_fame_pokes(u8 taskID);
u8 rbox_create(struct rbox_rom*);
void rbox_default_unpack_fill(u8 rboxID, bool to_vram);
void rbox_800378C(u8 rboxID);
u16 get_string_length(void* text_ptr);
void rbox_delete1(u8 rboxID, bool to_vram);
void rbox_delete2(u8 rboxID, bool to_vram);
void rbox_8003574(u8 rboxID);
u8 str_get_half_width(u8 font, void* text, u8 maxx);
u8 str_get_width(u8 font, void* text, u8 maxx);
u16 species_to_dex_number(u16 species);
u16 make_poke_sprite_appear_as_object(u16 species, u32 tid, u32 pid, bool front, s16 x_jump, s16 y_jump, u8 palsth1, u16 palsth2);
u16 spawn_poke_sprite_as_object(u16 species, u32 tid, u32 pid, bool front, s16 x_jump, s16 y_jump, u8 palsth1, u16 palsth2);
void fame_hall_sprite_animate(u8 objID);
bool fadescreen_related(u32 arg0, u8 arg1, u8 arg2, u8 arg3, u16 arg4);
bool hall_of_fame_build_registering_graphics(void);
void* malloc_and_clear(u32 size_);
u8 save_game_if_memory_present(u8 what_to_save);
void bg_nullify_tilemap(u8 bgID);
void rboxes_free(void);
void play_sound(u16 songID);
void fame_hall_pc_create_rbox(u8 bgID, u8 width_x, u8 y, u8 field_5, u16 vram_tileset);
void fame_hall_corrupted_data_rbox(u8 taskID);
void pokecry_play_sth(u16 species, u8 sth);
void fame_hall_pal_fade_sth(u32, u8, u16 colour);
void pal_fade_sth(u32 arg0, u8 arg1, u16 colour);
void pokecry_stop(void);
void fame_hall_pc_put_text(void* txt, void* text_toadd, u8 arg2, u8 arg3, bool to_vram);
u8 party_move_up_no_free_slots_in_between(void);
void set_poke_to_lvl(struct pokemon* poke, u8 lvl);
u8 get_pokemon_nature(struct pokemon* poke);
u8 generate_pokemon_data_for_player(struct pokemon* poke);
bool get_or_set_pokedex_flags(u16 natNO, u8 caseID);
u8 get_frontier_opponent_class(u16 trainerID);
void* oe_exec(u8 scriptID);
void npc_change_sprite(struct npc_state*, u8 spriteID);
void npc_turn(u8 direction);
u8 get_npc_state_id_by_local_id(u8 localID);
u16 gpu_tile_obj_alloc_tag_and_upload(struct tile_obj*);
u16 gpu_pal_obj_alloc_tag_and_apply(struct tile_pal*);
u8 template_instanciate(struct template*, u16 x, u16 y, u8 priority);
u8 poke_lvl_from_exp(struct pokemon* poke);

void gpu_pal_apply(struct palette* src, u16 index, u16 num_colours);
void gpu_pal_decompress_apply(struct palette* src, u16 index, u16 num_colours);

u32 __udivsi3(u32 numerator, u32 denumerator);
u32 __umodsi3(u32 numerator, u32 denumerator);

#endif /* VANILLA_FUNCTIONS */
