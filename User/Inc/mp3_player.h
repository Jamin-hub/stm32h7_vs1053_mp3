/**
 ****************************************************************************************************
 * @file        demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-04-17
 * @brief       ATK-MO1053 调试实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 H743开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

#include "vs1053.h"


uint8_t PlaySong(const char* filePath);
void PlayMusicList(void);

#endif
