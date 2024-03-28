#include "KEY/Key_Driver.h"

struct Button KEY1, KEY2, KEY3, KEY4, KEYWKUP;

uint8_t READ_KEY1_GPIO()
{
    return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
}

uint8_t READ_KEY2_GPIO()
{
    return HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
}

uint8_t READ_KEY3_GPIO()
{
    return HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin);
}

uint8_t READ_KEY4_GPIO()
{
    return HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin);
}

uint8_t READ_KEYWKUP_GPIO()
{
    return HAL_GPIO_ReadPin(KEY_MIDDLE_GPIO_Port, KEY_MIDDLE_Pin);
}

PressEvent Get_Key(uint8_t Key_Number)
{
    switch (Key_Number)
    {
    case 1:
        return get_button_event(&KEY1);
    case 2:
        return get_button_event(&KEY2);
    case 3:
        return get_button_event(&KEY3);
    case 4:
        return get_button_event(&KEY4);
    case 5:
        return get_button_event(&KEYWKUP);
    default:
        return NONE_PRESS;
    }
}

uint8_t Get_Key_Pressed(void)
{
    if (Get_Key(Key1) == PRESS_UP)
        return MENU_UP;
    else if (Get_Key(Key2) == PRESS_UP)
        return MENU_DOWN;
    else if (Get_Key(Key3) == PRESS_UP)
        return MENU_LEFT;
    else if (Get_Key(Key4) == PRESS_UP)
        return MENU_RIGHT;
    else if (Get_Key(KeyWkUp) == PRESS_UP)
        return MENU_ENTER;
    else
        return 0;
}

void KEY1_PRESS_UP_Handler(void *btn) {}
void KEY1_LONG_PRESS_HOLD_Handler(void *btn) {}
void KEY1_SINGLE_CLICK_Handler(void *btn) {}
void KEY2_PRESS_UP_Handler(void *btn) {}
void KEY2_LONG_PRESS_HOLD_Handler(void *btn) {}
void KEY2_SINGLE_CLICK_Handler(void *btn) {}

void KEY3_PRESS_UP_Handler(void *btn) {}
void KEY4_PRESS_UP_Handler(void *btn) {}

void KEYWKUP_PRESS_UP_Handler(void *btn) {}

void Key_Loading(void)
{
    button_init(&KEY1, READ_KEY1_GPIO, 0);
    button_init(&KEY2, READ_KEY2_GPIO, 0);
    button_init(&KEY3, READ_KEY3_GPIO, 0);
    button_init(&KEY4, READ_KEY4_GPIO, 0);
    button_init(&KEYWKUP, READ_KEYWKUP_GPIO, 0);

    button_attach(&KEY1, PRESS_UP, KEY1_PRESS_UP_Handler);
    // button_attach(&KEY1,LONG_PRESS_HOLD,KEY1_LONG_PRESS_HOLD_Handler);
    // button_attach(&KEY1,SINGLE_CLICK,KEY1_SINGLE_CLICK_Handler);

    button_attach(&KEY2, PRESS_UP, KEY2_PRESS_UP_Handler);
    // button_attach(&KEY2,LONG_PRESS_HOLD,KEY2_LONG_PRESS_HOLD_Handler);
    // button_attach(&KEY2,SINGLE_CLICK,KEY2_SINGLE_CLICK_Handler);

    button_attach(&KEY3, PRESS_UP, KEY3_PRESS_UP_Handler);
    button_attach(&KEY4, PRESS_UP, KEY4_PRESS_UP_Handler);

    button_attach(&KEYWKUP, PRESS_UP, KEYWKUP_PRESS_UP_Handler);

    button_start(&KEY1);
    button_start(&KEY2);
    button_start(&KEY3);
    button_start(&KEY4);
    button_start(&KEYWKUP);
}

/*
TaskHandle_t KeyScan_TaskHandle;

void KeyScan_Task(void *parameter)
{
    while (1)
    {
        button_ticks();
        vTaskDelay(5);
    }
}

void KeyScan_Task_Create(void)
{
    xTaskCreate((TaskFunction_t)KeyScan_Task,"KeyScan_Task",128,NULL,7,KeyScan_TaskHandle);
}

*/
