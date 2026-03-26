/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug_uart.h"
#include "fatfs.h"
#include "lwrb.h"
#include "mp3_player.h"

#include "lvgl.h"
#include "lv_demos.h"
#include "touch.h"

#include "vs1053.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
FATFS   fs;
uint8_t flag = 1;
/* Music Data Buffer */
lwrb_t  MusicBuffer;
uint8_t MusicBufferData[4096];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t         defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for FileReadTask */
osThreadId_t         FileReadTaskHandle;
const osThreadAttr_t FileReadTask_attributes = {
    .name       = "FileReadTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for AudioPlayTask */
osThreadId_t         AudioPlayTaskHandle;
const osThreadAttr_t AudioPlayTask_attributes = {
    .name       = "AudioPlayTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t)osPriorityRealtime,
};
/* Definitions for GuiUpdateTask */
osThreadId_t         GuiUpdateTaskHandle;
const osThreadAttr_t GuiUpdateTask_attributes = {
    .name       = "GuiUpdateTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for TouchTask */
osThreadId_t         TouchTaskHandle;
const osThreadAttr_t TouchTask_attributes = {
    .name       = "TouchTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityAboveNormal,
};
/* Definitions for KeyScanTask */
osThreadId_t         KeyScanTaskHandle;
const osThreadAttr_t KeyScanTask_attributes = {
    .name       = "KeyScanTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for SensorTask */
osThreadId_t         SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
    .name       = "SensorTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow7,
};
/* Definitions for RtcTask */
osThreadId_t         RtcTaskHandle;
const osThreadAttr_t RtcTask_attributes = {
    .name       = "RtcTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for ControlTask */
osThreadId_t         ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
    .name       = "ControlTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartFileReadTask(void *argument);
void StartAudioPlayTask(void *argument);
void StartGuiUpdateTask(void *argument);
void StartTouchTask(void *argument);
void StartKeyScanTask(void *argument);
void StartSensorTask(void *argument);
void StartRtcTask(void *argument);
void StartControlTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void vApplicationTickHook(void)
{
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */
  lv_tick_inc(1);
}
/* USER CODE END 3 */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of FileReadTask */
  FileReadTaskHandle = osThreadNew(StartFileReadTask, NULL, &FileReadTask_attributes);

  /* creation of AudioPlayTask */
  AudioPlayTaskHandle = osThreadNew(StartAudioPlayTask, NULL, &AudioPlayTask_attributes);

  /* creation of GuiUpdateTask */
  GuiUpdateTaskHandle = osThreadNew(StartGuiUpdateTask, NULL, &GuiUpdateTask_attributes);

  /* creation of TouchTask */
  TouchTaskHandle = osThreadNew(StartTouchTask, NULL, &TouchTask_attributes);

  /* creation of KeyScanTask */
  KeyScanTaskHandle = osThreadNew(StartKeyScanTask, NULL, &KeyScanTask_attributes);

  /* creation of SensorTask */
  SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);

  /* creation of RtcTask */
  RtcTaskHandle = osThreadNew(StartRtcTask, NULL, &RtcTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartControlTask, NULL, &ControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;) {
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartFileReadTask */
/**
 * @brief Function implementing the FileReadTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFileReadTask */
void StartFileReadTask(void *argument)
{
  /* USER CODE BEGIN StartFileReadTask */
  FRESULT        res;
  FIL            fmp3; // 文件对象
  UINT           br;
  lwrb_sz_t      write_len;
  uint8_t        sd_buf[1024];
  static uint8_t file_opened = 0;

  lwrb_init(&MusicBuffer, MusicBufferData, sizeof(MusicBufferData));

  res = f_mount(&fs, "0:", 1); // 立即挂载
  if (res != FR_OK) {
    u4_printf("sd err\n");
  }
  ScanMusicFiles("0:/Music");

  Player_SwitchTo(5);
  /* Infinite loop */
  for (;;) {
    /* 1. 处理“切歌请求” */
    if (g_player.need_open) {
      g_player.need_open = 0;
      if (file_opened) {
        f_close(&fmp3);
        file_opened = 0;
      }

      lwrb_reset(&MusicBuffer); // 清空缓冲区

      res = f_open(&fmp3, song_list[g_player.current_index].path, FA_READ);
      if (res != FR_OK) {
        u4_printf("file open err\n");
        continue;
      }
      file_opened = 1;
      // 重启解码器
      atk_mo1053_restart_play();
      atk_mo1053_set_all();
      atk_mo1053_reset_decode_time();
      if (strstr(song_list[g_player.current_index].path, ".flac") || strstr(song_list[g_player.current_index].path, ".FLAC")) {
			  atk_mo1053_load_patch((uint16_t *)vs1053b_patch, VS1053B_PATCHLEN); // 加载 MP3 补丁
      } 
      
      g_player.state = PLAYER_PLAY;

      // 预填充 buffer（避免卡顿）
      while (lwrb_get_free(&MusicBuffer) >= 1024) {
        f_read(&fmp3, sd_buf, sizeof(sd_buf), &br);
        if (br > 0) {
          lwrb_write(&MusicBuffer, sd_buf, br);
        } else {
          break;
        }
      }
      xTaskNotifyGive(AudioPlayTaskHandle);
    }
    /* 2. 正常播放状态 */
    if (g_player.state == PLAYER_PLAY && file_opened) {
      // buffer 不满才读
      if (lwrb_get_free(&MusicBuffer) >= 1024) {
        if (f_read(&fmp3, sd_buf, sizeof(sd_buf), &br) == FR_OK) {
          if (br > 0) {
            lwrb_write_ex(&MusicBuffer, sd_buf, sizeof(sd_buf), &write_len, LWRB_FLAG_WRITE_ALL);
          } else {
            g_player.state = PLAYER_STOP;
          }
        }
      }  else osDelay(5);
    } 
    /* 🔵 3. 暂停状态 */
    else if (g_player.state == PLAYER_PAUSE) {
      osDelay(10);
    }

    /* ⚫ 4. 停止状态 */
    else {
      Player_SwitchTo(Player_GetNextIndex());
      osDelay(500);
    }
  }
  /* USER CODE END StartFileReadTask */
}

/* USER CODE BEGIN Header_StartAudioPlayTask */
/**
 * @brief Function implementing the AudioPlayTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAudioPlayTask */
void StartAudioPlayTask(void *argument)
{
  /* USER CODE BEGIN StartAudioPlayTask */
  uint8_t   data[32];
  lwrb_sz_t read_len;
  atk_mo1053_init();
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  atk_mo1053_spi_speed_high();
  /* Infinite loop */
  for (;;) {
    if (VS10XX_DQ == 0) {
      osDelay(1);
      continue;
    }
    if (lwrb_read_ex(&MusicBuffer, data, 32, &read_len, LWRB_FLAG_READ_ALL)) {
      atk_mo1053_send_music_data(data);
    } else {
      osDelay(2);
    }
  }
  /* USER CODE END StartAudioPlayTask */
}

/* USER CODE BEGIN Header_StartGuiUpdateTask */
/**
 * @brief Function implementing the GuiUpdateTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGuiUpdateTask */
void StartGuiUpdateTask(void *argument)
{
  /* USER CODE BEGIN StartGuiUpdateTask */
  lv_demo_benchmark();
  /* Infinite loop */
  for (;;) {
    lv_timer_handler();
    osDelay(5);
  }
  /* USER CODE END StartGuiUpdateTask */
}

/* USER CODE BEGIN Header_StartTouchTask */
/**
 * @brief Function implementing the TouchTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTouchTask */
void StartTouchTask(void *argument)
{
  /* USER CODE BEGIN StartTouchTask */
  /* Infinite loop */
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    tp_dev.scan(0);
    osDelay(50);
  }
  /* USER CODE END StartTouchTask */
}

/* USER CODE BEGIN Header_StartKeyScanTask */
/**
 * @brief Function implementing the KeyScanTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartKeyScanTask */
void StartKeyScanTask(void *argument)
{
  /* USER CODE BEGIN StartKeyScanTask */
  KeyEvent_t key;
  Key_Init();
  /* Infinite loop */
  for (;;) {
    key = Key_Scan();
    if (key.event != KEY_EVENT_NONE) {
      switch (key.key_id) {
        case 1:
          if (key.event == KEY_EVENT_SHORT) {
            u4_printf("1:short\n");
            Player_SwitchTo(Player_GetPrevIndex());
          } else {
            u4_printf("1:long\n");
          }
          break;
        case 2:
          if (key.event == KEY_EVENT_SHORT) {
            u4_printf("2:short\n");
            g_player.state = PLAYER_PAUSE;
          } else {
            u4_printf("2:long\n");
            g_player.state = PLAYER_PLAY;
          }
          break;
        case 3:
          if (key.event == KEY_EVENT_SHORT) {
            u4_printf("3:short\n");
            Player_SwitchTo(Player_GetNextIndex());
          } else {
            u4_printf("3:long\n");
          }
          break;
        default: break;
      }
    }
    osDelay(100);
  }
  /* USER CODE END StartKeyScanTask */
}

/* USER CODE BEGIN Header_StartSensorTask */
/**
 * @brief Function implementing the SensorTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSensorTask */
void StartSensorTask(void *argument)
{
  /* USER CODE BEGIN StartSensorTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartSensorTask */
}

/* USER CODE BEGIN Header_StartRtcTask */
/**
 * @brief Function implementing the RtcTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRtcTask */
void StartRtcTask(void *argument)
{
  /* USER CODE BEGIN StartRtcTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartRtcTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
 * @brief Function implementing the ControlTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
  /* USER CODE BEGIN StartControlTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
