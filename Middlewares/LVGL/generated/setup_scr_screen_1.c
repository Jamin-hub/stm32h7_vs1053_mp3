/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 480);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1, lv_color_hex(0x0f0f13), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_cont_1
    ui->screen_1_cont_1 = lv_obj_create(ui->screen_1);
    lv_obj_set_pos(ui->screen_1_cont_1, 0, 0);
    lv_obj_set_size(ui->screen_1_cont_1, 320, 44);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_cont_1, 49, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_cont_1, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_1, lv_color_hex(0x141418), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_1
    ui->screen_1_btn_1 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_1_label = lv_label_create(ui->screen_1_btn_1);
    lv_label_set_text(ui->screen_1_btn_1_label, "" LV_SYMBOL_LEFT " 返回");
    lv_label_set_long_mode(ui->screen_1_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_1, 5, 4);
    lv_obj_set_size(ui->screen_1_btn_1, 50, 36);

    //Write style for screen_1_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0x1e1e2a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_1, lv_color_hex(0xe8365d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_1, &lv_font_YaHei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_1
    ui->screen_1_label_1 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_1, "歌曲列表");
    lv_label_set_long_mode(ui->screen_1_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_1, 109, 10);
    lv_obj_set_size(ui->screen_1_label_1, 100, 22);

    //Write style for screen_1_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_1, &lv_font_YaHei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_total
    ui->screen_1_label_total = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_total, "共 8 首 · 本地音乐");
    lv_label_set_long_mode(ui->screen_1_label_total, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_total, 16, 55);
    lv_obj_set_size(ui->screen_1_label_total, 102, 11);

    //Write style for screen_1_label_total, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_total, lv_color_hex(0x5c5b6b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_total, &lv_font_YaHei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_total, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_total, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_total, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_list_1
    ui->screen_1_list_1 = lv_list_create(ui->screen_1);
    ui->screen_1_list_1_item0 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_AUDIO, "save_1");
    ui->screen_1_list_1_item1 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_2");
    ui->screen_1_list_1_item2 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_3");
    ui->screen_1_list_1_item3 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_4");
    ui->screen_1_list_1_item4 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_5");
    ui->screen_1_list_1_item5 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_6");
    ui->screen_1_list_1_item6 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_7");
    ui->screen_1_list_1_item7 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "丰田合肥");
    ui->screen_1_list_1_item8 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_8");
    ui->screen_1_list_1_item9 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_9");
    ui->screen_1_list_1_item10 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_10");
    ui->screen_1_list_1_item11 = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_SAVE, "save_11");
    lv_obj_set_pos(ui->screen_1_list_1, 0, 72);
    lv_obj_set_size(ui->screen_1_list_1, 320, 408);
    lv_obj_set_scrollbar_mode(ui->screen_1_list_1, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_add_flag(ui->screen_1_list_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_list_1_main_main_default
    static lv_style_t style_screen_1_list_1_main_main_default;
    ui_init_style(&style_screen_1_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_1_list_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_list_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_list_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_list_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_list_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_1_list_1_main_main_default, lv_color_hex(0x0f0f13));
    lv_style_set_bg_grad_dir(&style_screen_1_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_1_list_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_1_list_1_main_main_default, 45);
    lv_style_set_border_color(&style_screen_1_list_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_border_side(&style_screen_1_list_1_main_main_default, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_TOP);
    lv_style_set_radius(&style_screen_1_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_1_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_list_1, &style_screen_1_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_list_1_main_scrollbar_default
    static lv_style_t style_screen_1_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_1_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_1_list_1_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_1_list_1_main_scrollbar_default, 128);
    lv_style_set_bg_color(&style_screen_1_list_1_main_scrollbar_default, lv_color_hex(0x5c5b6b));
    lv_style_set_bg_grad_dir(&style_screen_1_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_1_list_1, &style_screen_1_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_list_1_extra_btns_main_default
    static lv_style_t style_screen_1_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_1_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_1_list_1_extra_btns_main_default, 20);
    lv_style_set_pad_left(&style_screen_1_list_1_extra_btns_main_default, 16);
    lv_style_set_pad_right(&style_screen_1_list_1_extra_btns_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_list_1_extra_btns_main_default, 20);
    lv_style_set_border_width(&style_screen_1_list_1_extra_btns_main_default, 1);
    lv_style_set_border_opa(&style_screen_1_list_1_extra_btns_main_default, 58);
    lv_style_set_border_color(&style_screen_1_list_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_border_side(&style_screen_1_list_1_extra_btns_main_default, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_text_color(&style_screen_1_list_1_extra_btns_main_default, lv_color_hex(0xd6d6d6));
    lv_style_set_text_font(&style_screen_1_list_1_extra_btns_main_default, &lv_font_YaHei_16);
    lv_style_set_text_opa(&style_screen_1_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_1_list_1_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_list_1_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_1_list_1_extra_btns_main_default, lv_color_hex(0x0f0f13));
    lv_style_set_bg_grad_dir(&style_screen_1_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_1_list_1_item11, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item10, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item9, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item8, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item7, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item6, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item5, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item4, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item3, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item2, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item1, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_1_list_1_item0, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_list_1_extra_texts_main_default
    static lv_style_t style_screen_1_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_1_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_border_width(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_1_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_1_list_1_extra_texts_main_default, &lv_font_YaHei_16);
    lv_style_set_text_opa(&style_screen_1_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_1_list_1_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_1_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_1_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_1_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

    //Init events for screen.
    events_init_screen_1(ui);
}
