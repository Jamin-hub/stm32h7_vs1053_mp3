/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_img_song;
	lv_obj_t *screen_imgbtn_play;
	lv_obj_t *screen_imgbtn_play_label;
	lv_obj_t *screen_imgbtn_last;
	lv_obj_t *screen_imgbtn_last_label;
	lv_obj_t *screen_imgbtn_next;
	lv_obj_t *screen_imgbtn_next_label;
	lv_obj_t *screen_slider_song;
	lv_obj_t *screen_text_now;
	lv_obj_t *screen_text_total;
	lv_obj_t *screen_img_mode2;
	lv_obj_t *screen_img_mode1;
	lv_obj_t *screen_img_mode0;
	lv_obj_t *screen_imgbtn_list;
	lv_obj_t *screen_imgbtn_list_label;
	lv_obj_t *screen_btn_mode;
	lv_obj_t *screen_btn_mode_label;
	lv_obj_t *screen_cont_1;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_line_1;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_btn_1;
	lv_obj_t *screen_btn_1_label;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_cont_1;
	lv_obj_t *screen_1_btn_1;
	lv_obj_t *screen_1_btn_1_label;
	lv_obj_t *screen_1_label_1;
	lv_obj_t *screen_1_label_total;
	lv_obj_t *screen_1_list_1;
	lv_obj_t *screen_1_list_1_item0;
	lv_obj_t *screen_1_list_1_item1;
	lv_obj_t *screen_1_list_1_item2;
	lv_obj_t *screen_1_list_1_item3;
	lv_obj_t *screen_1_list_1_item4;
	lv_obj_t *screen_1_list_1_item5;
	lv_obj_t *screen_1_list_1_item6;
	lv_obj_t *screen_1_list_1_item7;
	lv_obj_t *screen_1_list_1_item8;
	lv_obj_t *screen_1_list_1_item9;
	lv_obj_t *screen_1_list_1_item10;
	lv_obj_t *screen_1_list_1_item11;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
LV_IMG_DECLARE(_cover_alpha_158x158);
LV_IMG_DECLARE(_FluentPlay12Filled_alpha_40x40);
LV_IMG_DECLARE(_FluentPause16Filled_alpha_40x40);
LV_IMG_DECLARE(_FluentPre16Filled_alpha_26x26);
LV_IMG_DECLARE(_FluentNext16Filled_alpha_26x26);
LV_IMG_DECLARE(_FluentArrowShuffle16Filled_alpha_22x22);
LV_IMG_DECLARE(_SolarRepeatOneBold_alpha_22x22);
LV_IMG_DECLARE(_SolarRepeatLinear_alpha_22x22);
LV_IMG_DECLARE(_FluentAppsList20Filled_22x22);

LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_YaHei_10)
LV_FONT_DECLARE(lv_font_YaHei_16)
LV_FONT_DECLARE(lv_font_YaHei_12)


#ifdef __cplusplus
}
#endif
#endif
