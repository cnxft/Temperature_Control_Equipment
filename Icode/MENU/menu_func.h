#ifndef __MENU_FUNC_H
#define __MENU_FUNC_H

#include "USERAPP/UserApp.h"


#define false 0
#define true 1

/**
 * 定义一个用来保存坐标系参数的结构体类型
 */
typedef struct COORDINATE_SYS_PARAMETER
{
    uint8_t parsetsign;  // 坐标系设置，标记位
    int Benchmark_value; // 基准线的值
    uint8_t Scale_value; // 最小刻度
} COORSYSPAR;

/***********************************************************************************************
 * Detection界面
 ***********************************************************************************************/
enum DETECTIONOUT_ENUM
{
    OUTSTATE,         // 输出 1  不输出 0
    TEMPTARGET,       // 输出目标温度
    H2PWVUL,          // 风扇pwm输出
    DASHBOARDOUT_LAST // 这个枚举的最后一个
};

enum DETECTIONOUT_SET
{
    DETECTIONOUT_SET_CLOSE, // 设置的开关 关
    DETECTIONOUT_SET_OPEN,  // 设值的开关 开
};
/**************************************************
 * 仪表盘
 **************************************************/
enum DASHBOARD_MENU
{
    DASHBOARD_NTC,
    DASHBOARD_OUT,
    DASHBOARD_LAST
};

// void DialogScaleShow(u8g2_t *u8g2,uint16_t x,uint16_t y,uint16_t Targrt_w,uint16_t Targrt_h);
void DetectionNTC(void);
void DetectionOUT(void);

void KPSET(void);
void KISET(void);
void KDSET(void);

void NTCBSET(void);
void NTCR1SET(void);
void NTCR2SET(void);

void TIME_DELTA_SET(void);

void Dashboard(void);
void Dashboard_PID(void);
void Dashboard_OUT(void);
#endif
