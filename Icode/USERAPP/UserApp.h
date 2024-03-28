#ifndef __USERAPP_H
#define __USERAPP_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stm32f1xx_hal_def.h"
#include "spi.h"

#include "math.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include "cmsis_os.h"
#include "timers.h"
#include "tim.h"
#include "queue.h"
#include "semphr.h"

#include "U8G2/u8g2.h"
#include "Oled/oled.h"
#include "MENU/menu.h"
#include "MENU/menu_func.h"

#include "LED/led.h"
#include "PWMOUT/pid.h"
#include "PWMOUT/pwmout_func.h"

#include "ADC/adc_func.h"

#include "KEY/Key_Driver.h"

#include "LETTERSHELL/shell_port.h"
#include "USERCAN/usercan.h"

#include "AT24Cxx/at24cxx.h"
#include "AT24CXX/SaveAndRead_func.h"

extern uint8_t out_state;

extern u8g2_t u8g2;
extern uint8_t Page_State;
extern uint8_t BgColor;
extern uint8_t Options_Time;
extern uint8_t Dialog_Time;

extern PID_Calibration heat_pid_calibration;
extern PID_State heat_pid_state;

extern PWMOUT h1out;
extern PWMOUT h2out;

extern ntcpar ntc1;
extern vadcpar vadc1;
extern uint16_t dmaadc[3];  // ADC DMA接收adc值的缓存区
extern float ntc1save[111]; // 保存温度

extern osMessageQueueId_t Queue_OLEDKeysHandle;
extern osThreadId_t TaskMenuHandle;

#define SWITCH_FOR_TESTING_CODE 0 // 关闭测试代码

// 裸机还是FreeRTOS
#define HAL_BARE_MACHINE 0
#define HAL_FREERTOS 1

// 留着以后延时用的
#define SYSTEM_TIME HAL_FREERTOS

// 启用串口发送数据
#define SERIAL_ENABLE_or_DISABL 0
// 启用can发送数据
#define CAN_ENABLE_or_DISABLE 0

/****************在AT24C64保存数据的相对地址*******************/
#define DATE_FIRST_ADD 352  // 保存数据的第一个位置
#define Date_OUTSTATE_SUM 0 // 相对初始位置的差值
#define Date_NTCB_SUM 1     // 相对差值1
#define Date_NTCT25_SUM 3
#define Date_NTCMAXADC_SUM 7
#define Date_NTCR1_SUM 11
#define Date_NTCR2_SUM 15
#define Date_VADCMADC_SUM 19
#define Date_VDDA_SUM 21
#define Date_RESH_SUM 25
#define Date_RESL_SUM 29
#define Date_KP_SUM 33
#define Date_KI_SUM 41
#define Date_KD_SUM 49
#define Date_TARGET_SUM 57
#define Date_TIMEDELTA_SUM 65
#define Date_TOTAL_NUM 15

void AppTask_App(void *argument);
void AppTask_OutPut(void *argument);
void AppTask_MessSend(void *argument);
void AppTask_SerCom(void *argument);
void AppTask_CanCom(void *argument);
void AppTask_Key(void *argument);
void AppTask_Menu(void *argument);

#endif
