#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"


enum DELAY_SYS_CHOOSE
{
    BARE_MACHINE,
    FreeRTOS_72MHZ,
};

void Delay_us(uint32_t xus);

#endif
