#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

#include <stdint.h>
#include "lvgl.h"
#include "gui_guider.h"
#include "fatfs.h"
#include "ff.h"

#define MAX_PATH 256  // filepath
#define MAX_SONGS 100 // song list

#define UI_DIRTY_TIME        (1 << 0)
#define UI_DIRTY_PROGRESS    (1 << 1)
#define UI_DIRTY_TITLE       (1 << 2)
#define UI_DIRTY_STATE       (1 << 3)
#define UI_DIRTY_ENV         (1 << 4)
#define UI_DIRTY_PLAYBTN     (1 << 5)
#define UI_DIRTY_CLOCK       (1 << 6)
#define UI_DIRTY_LIST        (1 << 7)

/* 当前播放状态 */
typedef enum { 
  PLAYER_STOP = 0, 
  PLAYER_PLAY, 
  PLAYER_PAUSE
} player_state_t;

/* 播放模式 */
typedef enum {
  PLAY_MODE_ALL_LOOP = 0, // 列表循环
  PLAY_MODE_LOOP,         // 单曲循环
  PLAY_MODE_RANDOM        // 随机播放
} play_mode_t;

/* 控制命令 */
typedef enum { 
  CMD_PLAY, 
  CMD_PAUSE, 
  CMD_NEXT, 
  CMD_PREV, 
  CMD_SET_INDEX,
  CMD_SET_MODE,
  CMD_VOL_UP,
  CMD_VOL_DOWN,
  CMD_SET_VOL 
} player_cmd_t;

typedef struct {
  player_cmd_t cmd;
  uint32_t     param;
} player_msg_t;

/* 文件路径 */
typedef struct {
  char path[MAX_PATH]; //完整路径
  char title[64]; // 文件名（用于显示）
  char artist[32]; // 作者
  uint32_t size;
} song_t;

/* 播放器核心结构体 */
typedef struct {
  player_state_t state; // 当前状态（播放/暂停）
  play_mode_t    mode;  // 播放模式

  uint8_t current_index; // 当前歌曲
  uint8_t song_count;    // 歌曲总数
  uint8_t need_open; // 需要打开新文件
  // uint8_t need_seek;
  // uint32_t seek_pos;
  // uint8_t update_time;

  int volume; // 音量（可选）
} player_t;


typedef struct
{
    /* 播放相关 */
    uint32_t cur_time;
    uint32_t total_time;

    char title[64];
    char artist[32];

    uint8_t play_state;

    /* 系统信息 */
    uint8_t temperature;
    uint8_t humidity;

    uint8_t battery;

    /* 时间 */
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    /* 列表 */
    uint16_t song_index;

    /* 标志位（关键） */
    uint32_t dirty_flag;

} ui_data_t;


extern player_t g_player;
extern song_t song_list[MAX_SONGS];
extern ui_data_t ui_data;


uint8_t Player_GetNextIndex(void);
uint8_t Player_GetPrevIndex(void);
void Player_SwitchTo(uint8_t index);
void Player_SetVolume(uint8_t vol);

void ScanMusicFiles(const char *path);
void parse_music_info(const char *path, char *title, char *artist);

void ui_music_list_update(lv_ui *ui);
void list_btn_event_cb(lv_event_t * e);

#endif
