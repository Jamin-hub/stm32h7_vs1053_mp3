#include "mp3_player.h"
#include "vs1053.h"

#include "debug_uart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "gbk2utf8.h"

#include <string.h>
#include <strings.h>
#include <stdlib.h>

extern osThreadId_t GuiUpdateTaskHandle;

extern lv_style_t style_screen_1_list_1_extra_btns_main_default;

player_t g_player = {
  .current_index = 0,
  .mode = PLAY_MODE_ALL_LOOP,
  .need_open = 1,
  // .need_seek = 0,
  // .seek_pos = 0,
  // .update_time = 1,
  .song_count = 0,
  .state = PLAYER_PLAY,
  .volume = 12
};
// 歌曲列表
song_t song_list[MAX_SONGS];
// UI 刷新
ui_data_t ui_data;

uint8_t Player_GetNextIndex(void)
{
  switch (g_player.mode) {
    case PLAY_MODE_ALL_LOOP: return (g_player.current_index + 1) % g_player.song_count;

    case PLAY_MODE_LOOP: return g_player.current_index;

    case PLAY_MODE_RANDOM: return rand() % g_player.song_count;
  }

  return 0;
}

uint8_t Player_GetPrevIndex(void)
{
  switch (g_player.mode) {
    case PLAY_MODE_ALL_LOOP: return (g_player.current_index - 1 + g_player.song_count) % g_player.song_count;

    case PLAY_MODE_LOOP: return g_player.current_index;

    case PLAY_MODE_RANDOM: return rand() % g_player.song_count;
  }

  return 0;
}

void Player_SwitchTo(uint8_t index)
{
    if (index >= g_player.song_count)
        return;

    g_player.current_index = index;
    g_player.need_open = 1;
    g_player.state = PLAYER_PLAY;
    ui_data.dirty_flag |= UI_DIRTY_PLAYBTN;
    xTaskNotifyGive(GuiUpdateTaskHandle);
}

void Player_SetVolume(uint8_t vol)
{
    if (vol > 20) vol = 20;

    // 简单“伪对数”优化
    uint8_t vs_vol = 225 - (vol * vol * (225 - 125) / (20 * 20));

    atk_mo1053_set_volume(vs_vol);
}

// 判断扩展名是否为音频文件
static uint8_t is_audio_file(const char *filename)
{
  const char *ext = strrchr(filename, '.');
  if (!ext) return 0;
  return (
    strcasecmp(ext, ".mp3") == 0 ||
    strcasecmp(ext, ".flac") == 0 ||
    strcasecmp(ext, ".wav") == 0
  );
}



/* 递归扫描SD卡目录 */
void ScanMusicFiles(const char *path)
{
  FRESULT res;
  FILINFO fno;
  DIR     dir;

  g_player.song_count = 0; // 每次扫描前清零

  res = f_opendir(&dir, path);
  if (res != FR_OK) {
    u4_printf("open dir fail: %s\n", path);
    return;
  }

  while (1) {
    res = f_readdir(&dir, &fno);
    if (res != FR_OK || fno.fname[0] == 0)
      break;

    /* 跳过目录 */
    if (fno.fattrib & AM_DIR)
      continue;

    /* 判断是否音频文件 */
    if (!is_audio_file(fno.fname))
      continue;

    /* 防止数组越界 */
    if (g_player.song_count >= MAX_SONGS)
      break;

    /* 安全拼接路径 */
    int len = snprintf(song_list[g_player.song_count].path, MAX_PATH, "%s/%s", path, fno.fname);

    if (len < 0 || len >= MAX_PATH) {
      u4_printf("path too long, skip: %s\n", fno.fname);
      continue;
    }
    parse_music_info(song_list[g_player.song_count].path, song_list[g_player.song_count].title, song_list[g_player.song_count].artist);
    g_player.song_count++;

  }

  f_closedir(&dir);

}


void parse_music_info(const char *path, char *title, char *artist)
{
    const char *filename;
    char temp[128];

    // 1. 找最后一个 '/'
    filename = strrchr(path, '/');
    if (filename == NULL)
    {
        filename = path;  // 防止没有 '/'
    }
    else
    {
        filename++; // 跳过 '/'
    }

    // 2. 复制文件名
    strcpy(temp, filename);

    // 3. 去掉 .mp3
    char *dot = strrchr(temp, '.');
    if (dot)
    {
        *dot = '\0';
    }

    // 4. 找 " - "
    char *sep = strstr(temp, " - ");
    if (sep)
    {
        *sep = '\0';              // 左边结束
        strcpy(title, temp);      // 歌名
        strcpy(artist, sep + 3);  // 跳过 " - "
    }
    else
    {
        // 没有分隔符，默认全是歌名
        strcpy(title, temp);
        artist[0] = '\0';
    }
}

void ui_music_list_update(lv_ui *ui)
{
  /* 清空列表 */
  if (ui->screen_1_list_1 == NULL)
    return;
  lv_obj_clean(ui->screen_1_list_1);
  char title_utf8[128];
  for (int i = 0; i < g_player.song_count; i++) {
    str_gbk2utf8(song_list[i].title, title_utf8);
    u4_printf("add: %s\n", title_utf8);
    
    lv_obj_t *btn = lv_list_add_btn(ui->screen_1_list_1, LV_SYMBOL_AUDIO, title_utf8);
    lv_obj_add_style(btn, &style_screen_1_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    /* 绑定点击事件 */
    lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, (void *)i);
  }
}

void list_btn_event_cb(lv_event_t * e)
{
    uint8_t index = (uint32_t)lv_event_get_user_data(e);
    ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_1_del, setup_scr_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 200, 200, true, false);
    Player_SwitchTo(index);
}

