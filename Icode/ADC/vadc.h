#ifndef __VADC_H
#define __VADC_H

#include "stm32f1xx_hal.h"
#include "main.h"

// 定义一个电阻的结构体，保存其参数
typedef struct VADCPARAMETER
{
    uint16_t vadcADC; // 当前adc的值
    uint16_t vadcMADC;// 最大adc的值
    float vdda; //基准电压
    float resh; //测压点vcc侧的电阻
    float resl;//测压点gnd侧的电阻
    float vcc;
} vadcpar;


float vadc_count_value(vadcpar vadc);

#endif
