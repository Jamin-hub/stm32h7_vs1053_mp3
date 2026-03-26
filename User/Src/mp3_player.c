#include "mp3_player.h"
#include "vs1053.h"

#include "debug_uart.h"
#include "fatfs.h"
#include "ff.h"

#include <string.h>
#include <strings.h>
#include <stdlib.h>

player_t g_player = {
  .current_index = 0,
  .mode = PLAY_MODE_ALL_LOOP,
  .need_open = 1,
  .song_count = 0,
  .state = PLAYER_PLAY,
  .volume = 12
};

song_t song_list[MAX_SONGS];

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

    /* 保存文件名（用于UI显示） */
    strncpy(song_list[g_player.song_count].name, fno.fname, sizeof(song_list[g_player.song_count].name) - 1);

    song_list[g_player.song_count].name[sizeof(song_list[g_player.song_count].name) - 1] = '\0';

    g_player.song_count++;

    // 调试用（可删）
    // u4_printf("add: %s\n", song_list[g_player.song_count - 1].path);
  }

  f_closedir(&dir);

  u4_printf("scan done, total: %d\n", g_player.song_count);
}
