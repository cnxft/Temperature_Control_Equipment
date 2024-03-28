#ifndef __NTC_H
#define __NTC_H

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h"
#include "math.h"
#include "stdio.h"
// 公式法
//  #define NTC_B 3950
//  #define NTC_T25 298.15
//  #define NTC_R25 3950

// 定义一个电阻的结构体，保存其参数
typedef struct NTCPARAMETER
{
    uint16_t ntcB;   // B值
    float ntcT25;    // 25摄氏度的开尔文温度
    uint16_t ntcADC; // 检测到的adc的值S
    float ntcMADC;   // adc检测的最大adc值 f103 4096
    float ntcR1;     // 分压电阻的阻值
    float ntcR2;     // 热敏电阻的阻值
    float ntcR;      // 热敏电阻当前的阻值
    float ntcT;      // 当前的温度
} ntcpar;

// 公式法
float ntc_count_value(ntcpar ntcx);

// 查表

#endif /*__NTC_H*/
