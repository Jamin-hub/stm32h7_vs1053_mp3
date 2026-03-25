#include "vs1053.h"
#include "patch_flac.h"
#include "mp3_player.h"
#include "fatfs.h"
#include "ff.h"

#include <stdio.h>
#include <string.h>


// 静态缓冲区，避免动态内存分配
#define AUDIO_BUFFER_SIZE 4096
uint8_t audioBuffer[AUDIO_BUFFER_SIZE];



// 播放单个 MP3 文件
uint8_t PlaySong(const char *filePath)
{
  FIL      fmp3; // 文件对象
  UINT     br;   // 实际读取字节数
  uint8_t  res, rval = 0;
  uint16_t i;

  // 初始化 VS1053
  atk_mo1053_restart_play();      // 重启播放
  atk_mo1053_set_all();           // 设置音量等参数
  atk_mo1053_reset_decode_time(); // 重置解码时间

  // 检查文件类型并加载 MP3 补丁（如果需要）
  if (strstr(filePath, ".flac") || strstr(filePath, ".FLAC")) {
    atk_mo1053_load_patch((uint16_t *)vs1053b_patch, VS1053B_PATCHLEN); // 加载 MP3 补丁
  }

  // 打开文件
  res = f_open(&fmp3, filePath, FA_READ);
  if (res != FR_OK) {
    return 0xFF; // 文件打开失败
  } else {
    // 设置 SPI 高速模式
    atk_mo1053_spi_speed_high();
    // 读取并播放文件
    while (rval == 0) {
      res = f_read(&fmp3, audioBuffer, AUDIO_BUFFER_SIZE, &br); // 读取 4096 字节
      // 分块发送数据到 VS1053
      i = 0;
      do /* 主播放循环 */
      {
        if (atk_mo1053_send_music_data(audioBuffer + i) == 0) /* 给VS10XX发送音频数据 */
        {
          i += 32;
        }
      } while (i < 4096);
      if (res != FR_OK || br == 0) {
        rval = (br == 0) ? 0 : 0xFF; // 文件读取完成或出错
        break;
      }
    }
    // 关闭文件
    f_close(&fmp3);
  }
  return rval;
}

// 扫描 SD 卡根目录并播放所有 MP3 文件
void PlayMusicList(void)
{
  DIR     dir;
  FILINFO fno;
  char    path[512] = "0:/Music/"; // SD 卡根目录
  char    filePath[512];

  // 打开根目录
  if (f_opendir(&dir, path) != FR_OK) {
    return; // 打开目录失败
  }

  while (1) {
    // 读取目录项
    if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0) {
      break; // 读取失败或目录末尾
    }

    // 检查是否为 MP3 文件
    if (strstr(fno.fname, ".mp3") || strstr(fno.fname, ".MP3") || strstr(fno.fname, ".flac")
        || strstr(fno.fname, ".FLAC")) {
      // 构造完整文件路径
      strcpy(filePath, path);
      strcat(filePath, fno.fname);
      printf("%s\n",fno.fname);
      // 播放文件
      if (PlaySong(filePath) == 0) {
        // 播放成功，添加延时以区分歌曲
        HAL_Delay(1000);
      }
    }
  }

  f_closedir(&dir);
}
