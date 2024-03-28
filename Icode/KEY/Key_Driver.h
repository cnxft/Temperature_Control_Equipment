#ifndef _Key_Driver_H_
#define _Key_Driver_H_

#include "stm32f1xx_hal.h" //HAL¿âÎÄ¼şÉùÃ÷
#include "main.h"
#include "MENU/menu.h"
#include "KEY/multi_button.h"

#define Key1    1
#define Key2    2
#define Key3    3
#define Key4    4
#define KeyWkUp 5

// #define UP 3
// #define DOWN 2


// void KEY_GPIO_Config(void);
void Key_Loading(void);
PressEvent Get_Key(uint8_t Key_Number);
uint8_t Get_Key_Pressed(void);
// void KeyScan_Task_Create(void);

#endif 
