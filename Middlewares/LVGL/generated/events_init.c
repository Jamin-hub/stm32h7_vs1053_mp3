/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static uint8_t current_state = 0; // 0=Play, 1=Pause, 2=Stop

static void screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOAD_START:
    {
        lv_obj_add_state(guider_ui.screen_imgbtn_play, LV_STATE_CHECKED);
        break;
    }
    default:
        break;
    }
}

static void screen_imgbtn_play_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        if (lv_obj_has_state(guider_ui.screen_imgbtn_play, LV_STATE_CHECKED)) {
            // cmd = AUDIO_PLAY; // Play
            // xQueueSend(CmdQueueHandle, &cmd, 100);
        } else {
            // cmd = AUDIO_PAUSE; // Paused
            // xQueueSend(CmdQueueHandle, &cmd, 100);
        }
        break;
    }
    default:
        break;
    }
}

static void screen_imgbtn_last_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        // cmd = AUDIO_PREV; // Previous track
        // xQueueSend(CmdQueueHandle, &cmd, 100);
        lv_obj_add_state(guider_ui.screen_imgbtn_play, LV_STATE_CHECKED);
        break;
    }
    default:
        break;
    }
}

static void screen_imgbtn_next_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        // cmd = AUDIO_NEXT;  // Next Track
        // xQueueSend(CmdQueueHandle, &cmd, 100);
        lv_obj_add_state(guider_ui.screen_imgbtn_play, LV_STATE_CHECKED);
        break;
    }
    default:
        break;
    }
}

static void screen_imgbtn_list_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_del, setup_scr_screen_1, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_btn_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_img_mode0, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_img_mode1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_img_mode2, LV_OBJ_FLAG_HIDDEN);
        // 切换到下一个状态
        current_state = (current_state + 1) % 3;

        switch(current_state) {
        case 0:
            lv_obj_clear_flag(guider_ui.screen_img_mode0, LV_OBJ_FLAG_HIDDEN);
            break;
        case 1:
            lv_obj_clear_flag(guider_ui.screen_img_mode1, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            lv_obj_clear_flag(guider_ui.screen_img_mode2, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        if (lv_obj_has_state(guider_ui.screen_btn_1, LV_STATE_CHECKED)) {
            lv_label_set_text(guider_ui.screen_btn_1_label, ""LV_SYMBOL_MUTE"");

        } else {
            lv_label_set_text(guider_ui.screen_btn_1_label, ""LV_SYMBOL_VOLUME_MAX"");

        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen, screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_play, screen_imgbtn_play_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_last, screen_imgbtn_last_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_next, screen_imgbtn_next_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_list, screen_imgbtn_list_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_mode, screen_btn_mode_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_1, screen_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_1_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_1_del, setup_scr_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_1_list_1_item0_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_1_del, setup_scr_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_1 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_1_btn_1, screen_1_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_1_list_1_item0, screen_1_list_1_item0_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
