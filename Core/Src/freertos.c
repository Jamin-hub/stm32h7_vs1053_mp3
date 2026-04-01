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
#include "rtc.h"
#include "lwrb.h"
#include "mp3_player.h"
#include "gbk2utf8.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "touch.h"

#include "vs1053.h"
#include "key.h"
#include "sht30.h"
#include "voltage.h"
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
lv_ui   guider_ui;
/* Music Data Buffer */
lwrb_t  MusicBuffer;
uint8_t MusicBufferData[4096];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FileReadTask */
osThreadId_t FileReadTaskHandle;
const osThreadAttr_t FileReadTask_attributes = {
  .name = "FileReadTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for AudioPlayTask */
osThreadId_t AudioPlayTaskHandle;
const osThreadAttr_t AudioPlayTask_attributes = {
  .name = "AudioPlayTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for GuiUpdateTask */
osThreadId_t GuiUpdateTaskHandle;
const osThreadAttr_t GuiUpdateTask_attributes = {
  .name = "GuiUpdateTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for TouchTask */
osThreadId_t TouchTaskHandle;
const osThreadAttr_t TouchTask_attributes = {
  .name = "TouchTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for KeyScanTask */
osThreadId_t KeyScanTaskHandle;
const osThreadAttr_t KeyScanTask_attributes = {
  .name = "KeyScanTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorTask */
osThreadId_t SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
  .name = "SensorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow7,
};
/* Definitions for RtcTask */
osThreadId_t RtcTaskHandle;
const osThreadAttr_t RtcTask_attributes = {
  .name = "RtcTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PlayerCmdQueue */
osMessageQueueId_t PlayerCmdQueueHandle;
const osMessageQueueAttr_t PlayerCmdQueue_attributes = {
  .name = "PlayerCmdQueue"
};
/* Definitions for PlayerTimer */
osTimerId_t PlayerTimerHandle;
const osTimerAttr_t PlayerTimer_attributes = {
  .name = "PlayerTimer"
};
/* Definitions for SensorMutex */
osMutexId_t SensorMutexHandle;
const osMutexAttr_t SensorMutex_attributes = {
  .name = "SensorMutex"
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
void CallbackPlayerTimer(void *argument);

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
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of SensorMutex */
  SensorMutexHandle = osMutexNew(&SensorMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of PlayerTimer */
  PlayerTimerHandle = osTimerNew(CallbackPlayerTimer, osTimerOnce, NULL, &PlayerTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of PlayerCmdQueue */
  PlayerCmdQueueHandle = osMessageQueueNew (8, sizeof(player_msg_t), &PlayerCmdQueue_attributes);

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
    if (g_player.state == PLAYER_PLAY) {
      osDelay(pdMS_TO_TICKS(1000));
      ui_data.cur_time++;
      if (ui_data.cur_time >= ui_data.total_time) {
        ui_data.cur_time = ui_data.total_time;
      }
      ui_data.dirty_flag |= UI_DIRTY_PROGRESS;
      xTaskNotifyGive(GuiUpdateTaskHandle);
    } else osDelay(100);
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

  Player_SwitchTo(0);
  /* Infinite loop */
  for (;;) {
    /*  0. 处理“拖动进度条（seek）请求”  */
    // if (g_player.need_seek && file_opened) {
      
    //   g_player.need_seek = 0;

    //   // 停止当前解码（非常关键）
    //   atk_mo1053_soft_reset(); // 或 restart_play()

    //   // 移动文件指针
    //   f_lseek(&fmp3, g_player.seek_pos);

    //   // 清空环形缓冲区
    //   lwrb_reset(&MusicBuffer);

    //   //（可选）更新时间
    //   ui_data.cur_time = (g_player.seek_pos * ui_data.total_time) / song_list[g_player.current_index].size;
    //   ui_data.dirty_flag |= UI_DIRTY_TIME;
    //   xTaskNotifyGive(GuiUpdateTaskHandle);
    // }

    /* 1. 处理“切歌请求” */
    if (g_player.need_open) {
      g_player.need_open = 0;
      if (file_opened) {
        f_close(&fmp3);
        file_opened = 0;
      }

      lwrb_reset(&MusicBuffer); // 清空缓冲区

      res = f_open(&fmp3, song_list[g_player.current_index].path, FA_READ);
      song_list[g_player.current_index].size = f_size(&fmp3);
      // 通知 lvgl 任务刷新歌曲名字和作者
      str_gbk2utf8(song_list[g_player.current_index].title, ui_data.title);
      str_gbk2utf8(song_list[g_player.current_index].artist, ui_data.artist);
      ui_data.dirty_flag |= UI_DIRTY_TITLE;
      xTaskNotifyGive(GuiUpdateTaskHandle);

      osTimerStart(PlayerTimerHandle, 5000);

      if (res != FR_OK) {
        u4_printf("file open err\n");
      }
      file_opened = 1;
      // 重启解码器
      atk_mo1053_restart_play();
      Player_SetVolume(g_player.volume); /* 设置音量 */
      atk_mo1053_set_all();
      atk_mo1053_reset_decode_time();
      if (strstr(song_list[g_player.current_index].path, ".flac")
          || strstr(song_list[g_player.current_index].path, ".FLAC")) {
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
      ui_data.cur_time = 0;
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
      } else osDelay(3);
    }
    /* 3. 暂停状态 */
    else if (g_player.state == PLAYER_PAUSE) {
      osDelay(100);
    }

    /* 4. 停止状态 */
    else {
      Player_SwitchTo(Player_GetNextIndex());
      osDelay(100);
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
      osDelay(1);
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
  setup_ui(&guider_ui);
  events_init(&guider_ui);
  /* Infinite loop */
  for (;;) {
    lv_timer_handler();
    /* 等待通知 */
    if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100))) {
      uint32_t flag = ui_data.dirty_flag;

      /* 清除标志 */
      ui_data.dirty_flag = 0;

      if (flag & UI_DIRTY_TIME) {
        lv_label_set_text_fmt(guider_ui.screen_text_total, "%02ld:%02ld", ui_data.total_time / 60, ui_data.total_time % 60);
        lv_bar_set_range(guider_ui.screen_slider_song, 0, ui_data.total_time);
      
      }

      if (flag & UI_DIRTY_PROGRESS) {
        lv_label_set_text_fmt(guider_ui.screen_text_now, "%02ld:%02ld", ui_data.cur_time / 60, ui_data.cur_time % 60);
        lv_bar_set_value(guider_ui.screen_slider_song, ui_data.cur_time, LV_ANIM_OFF);
      }

      if (flag & UI_DIRTY_TITLE) {
        lv_label_set_text(guider_ui.screen_label_title, ui_data.title);
        lv_label_set_text(guider_ui.screen_label_artist, ui_data.artist);
      }

      if (flag & UI_DIRTY_ENV) {
        lv_label_set_text_fmt(guider_ui.screen_label_env, "%dC %d%%", ui_data.temperature, ui_data.humidity);
        if (ui_data.battery >= 80) {
          lv_label_set_text(guider_ui.screen_label_battery, "" LV_SYMBOL_BATTERY_FULL " ");
        } else if (ui_data.battery >= 60) {
          lv_label_set_text(guider_ui.screen_label_battery, "" LV_SYMBOL_BATTERY_3 " ");
        } else if (ui_data.battery >= 40) {
          lv_label_set_text(guider_ui.screen_label_battery, "" LV_SYMBOL_BATTERY_1 " ");
        } else lv_label_set_text(guider_ui.screen_label_battery, "" LV_SYMBOL_BATTERY_EMPTY " ");
        
      }

      if (flag & UI_DIRTY_PLAYBTN) {
        lv_obj_add_state(guider_ui.screen_imgbtn_play, LV_STATE_CHECKED);
      }

      if (flag & UI_DIRTY_CLOCK) {
        lv_label_set_text_fmt(guider_ui.screen_label_clock, "%02d:%02d", ui_data.hour, ui_data.min);
      }

      if (flag & UI_DIRTY_LIST) {
        ui_music_list_update(&guider_ui);
        lv_label_set_text_fmt(guider_ui.screen_1_label_total, "共 %d 首 · 本地音乐", g_player.song_count);
      }
    }
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
  player_msg_t msg;
  /* Infinite loop */
  for (;;) {
    key = Key_Scan();
    if (key.event != KEY_EVENT_NONE) {
      switch (key.key_id) {
        case 1:
          if (key.event == KEY_EVENT_SHORT) {
            msg.cmd = CMD_VOL_UP;
          } else {
            msg.cmd = CMD_NEXT;
          }
          break;
        case 2:
          if (key.event == KEY_EVENT_SHORT) {
            msg.cmd = CMD_PAUSE;
          } else {
            msg.cmd = CMD_PLAY;
          }
          break;
        case 3:
          if (key.event == KEY_EVENT_SHORT) {
            msg.cmd = CMD_VOL_DOWN;
          } else {
            msg.cmd = CMD_PREV;
          }
          break;
        default: break;
      }
      msg.param = 0;
      osMessageQueuePut(PlayerCmdQueueHandle, &msg, 0, 100);
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
  float humidity, temperature, voltage;
  ADC_Calibration();
  /* Infinite loop */
  for (;;) {
    // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    osMutexAcquire(SensorMutexHandle, 0);
    if (!SHT30_Read(&humidity, &temperature)) {
      ui_data.temperature = temperature;
      ui_data.humidity = humidity;
    }
    voltage = ADC_ReadVoltage(&hadc1);
    osMutexRelease(SensorMutexHandle);
    ui_data.battery = voltage;
    u4_printf("%d\n", ui_data.battery);
    ui_data.dirty_flag |= UI_DIRTY_ENV;
    xTaskNotifyGive(GuiUpdateTaskHandle);
    osDelay(30000);
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
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  /* Infinite loop */
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    ui_data.hour = time.Hours;
    ui_data.min = time.Minutes;
    ui_data.dirty_flag |= UI_DIRTY_CLOCK;
    xTaskNotifyGive(GuiUpdateTaskHandle); // 通知lvgl任务刷新
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
  player_msg_t msg;
  /* Infinite loop */
  for (;;) {
    osMessageQueueGet(PlayerCmdQueueHandle, &msg, 0, osWaitForever);
    switch (msg.cmd) {
      case CMD_PLAY:
        g_player.state = PLAYER_PLAY;
        break;
      case CMD_PAUSE:
        g_player.state = PLAYER_PAUSE;
        break;
      case CMD_NEXT:
        Player_SwitchTo(Player_GetNextIndex());
        break;
      case CMD_PREV:
        Player_SwitchTo(Player_GetPrevIndex());
        break;
      case CMD_SET_INDEX:
        Player_SwitchTo((uint8_t)msg.param);
        break;
      case CMD_SET_MODE:
        g_player.mode = msg.param;
        break;
      case CMD_VOL_UP:
        if (g_player.volume < 20) {
          g_player.volume ++;
        }
        Player_SetVolume(g_player.volume);
        break;
      case CMD_VOL_DOWN:
        if (g_player.volume > 0) {
          g_player.volume --;
        }
        Player_SetVolume(g_player.volume);
        break;
      case CMD_SET_VOL:
        g_player.volume = msg.param;
        Player_SetVolume(g_player.volume);
        break;
      default: break;
    }

  }
  /* USER CODE END StartControlTask */
}

/* CallbackPlayerTimer function */
void CallbackPlayerTimer(void *argument)
{
  /* USER CODE BEGIN CallbackPlayerTimer */
  uint16_t bitrate_kbps = atk_mo1053_get_bitrate();
  ui_data.total_time = (song_list[g_player.current_index].size * 8) / (bitrate_kbps * 1000);
  ui_data.dirty_flag |= UI_DIRTY_TIME;
  xTaskNotifyGive(GuiUpdateTaskHandle); // 通知lvgl任务刷新
  /* USER CODE END CallbackPlayerTimer */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

