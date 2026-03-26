#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

#include <stdint.h>

#define MAX_PATH 256  // filepath
#define MAX_SONGS 100 // song list

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
  CMD_SET_MODE 
} player_cmd_t;

typedef struct {
  player_cmd_t cmd;
  uint32_t     param;
} player_msg_t;

/* 文件路径 */
typedef struct {
  char path[MAX_PATH];
  char name[64]; // 文件名（用于显示）
} song_t;

/* 播放器核心结构体 */
typedef struct {
  player_state_t state; // 当前状态（播放/暂停）
  play_mode_t    mode;  // 播放模式

  uint8_t current_index; // 当前歌曲
  uint8_t song_count;    // 歌曲总数
  uint8_t need_open; // 需要打开新文件

  uint8_t volume; // 音量（可选）
} player_t;

extern player_t g_player;
extern song_t song_list[MAX_SONGS];



uint8_t Player_GetNextIndex(void);
uint8_t Player_GetPrevIndex(void);
void Player_SwitchTo(uint8_t index);

void ScanMusicFiles(const char *path);

#endif
