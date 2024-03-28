#include "MENU/menu_func.h"

COORSYSPAR CoorSysPar =
    {
        .parsetsign = 0,
        .Benchmark_value = 0,
        .Scale_value = 1};

uint8_t IntoState = false;
void DialogScaleShow(u8g2_t *u8g2, uint16_t x, uint16_t y, uint16_t Targrt_w, uint16_t Targrt_h)
{
    if (IntoState == false)
    {
        DialogScale_Show(u8g2, x, y, Targrt_w, Targrt_h);
        IntoState = true;
    }
}

// /**
//  * 绘制三角光标
//  */
// void DrawTriangleCursor(uint8_t x, uint8_t y)
// {
//     u8g2_DrawHLine(&u8g2, x, y - 5, 10);
//     u8g2_DrawVLine(&u8g2, x, y - 10, 10);
// }

// /**
//  * 绘制上下翻页三角光标
//  */
// void DrawUDTriangleCursor(uint8_t x, uint8_t y)
// {
//     u8g2_DrawHLine(&u8g2, x, y - 5, 10);
//     u8g2_DrawVLine(&u8g2, x, y - 10, 10);
// }

/**
 * 绘制坐标系
 */
void DrawXOY()
{
    // 绘制y轴
    u8g2_DrawVLine(&u8g2, 118, 4, 45);
    // 绘制x轴
    u8g2_DrawHLine(&u8g2, 8, 49, 111);

    // 绘制刻度
    for (size_t j = 0; j < 9; j++)
    {
        for (size_t i = 0; i < 110; i += 5)
        {
            u8g2_DrawPixel(&u8g2, (8 + i), (4 + j * 5));
        }
    }
}

/**
 * 计算温度在屏幕上的坐标:
 * 由基准值和最小刻度可知最大值和最小值
 * 用等差法计算温度值在屏幕上的坐标
 * 超出显示范围的不显示
 * yy_min=CoorSysPar.Benchmark_value;
 * yy_max=CoorSysPar.Benchmark_value+CoorSysPar.Scale_value*9;
 */
int Y_count(float y_count)
{
    int ry_count;
    ry_count = (int)(49 - (y_count - CoorSysPar.Benchmark_value) / (CoorSysPar.Scale_value * 9.0 / 45));
    if (ry_count <= 4)
    {
        ry_count = -1;
    }
    if (ry_count >= 50)
    {
        ry_count = 64;
    }
    return ry_count;
}

extern PID_State heat_pid_state;
extern osMessageQueueId_t Queue_OLEDKeysHandle;
/**
 * DetectionNTC
 * 用来显示温度曲线的函数
 */
void DetectionNTC(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        if (CoorSysPar.parsetsign == 1)
        {
            CoorSysPar.Benchmark_value += 5;
        }
        else if (CoorSysPar.parsetsign == 2)
        {
            CoorSysPar.Scale_value += 1;
        }
        break;

    case MENU_DOWN:
        if (CoorSysPar.parsetsign == 1)
        {
            CoorSysPar.Benchmark_value -= 5;
        }
        else if (CoorSysPar.parsetsign == 2)
        {
            CoorSysPar.Scale_value -= 1;
        }
        break;

    case MENU_LEFT:
        CoorSysPar.parsetsign = 0;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        break;

    case MENU_RIGHT:
        if (CoorSysPar.parsetsign != 0)
        {
            CoorSysPar.parsetsign = 0;
        }
        break;

    case MENU_ENTER:
        if (CoorSysPar.parsetsign == 0)
        {
            CoorSysPar.parsetsign = 1;
        }
        else if (CoorSysPar.parsetsign == 1)
        {
            CoorSysPar.parsetsign = 2;
        }
        else if (CoorSysPar.parsetsign == 2)
        {
            CoorSysPar.parsetsign = 0;
        }
        break;
    default:
        u8g2_FirstPage(&u8g2);
        do
        {
            // 画实心框
            // DialogScale_Show(&u8g2, 0, 0, 127, 64);
            // 绘制坐标系
            DrawXOY();
            // 显示温度
            char show[32];
            sprintf(show, " %-4.1f C", ntc1.ntcT);
            u8g2_DrawStr(&u8g2, 80, 60, show);
            // u8g2_DrawCircle(&u8g2, 112, 56, 3, U8G2_DRAW_ALL); // 打印℃
            u8g2_DrawFrame(&u8g2, 111, 52, 3, 3);

            sprintf(show, "|%3d|%3d|", CoorSysPar.Benchmark_value, CoorSysPar.Scale_value);
            u8g2_DrawStr(&u8g2, 10, 60, show);

            if (CoorSysPar.parsetsign == 0)
            {
            }
            else if (CoorSysPar.parsetsign == 1)
            {
                // u8g2_DrawHLine(&u8g2, 10, 63, 15);
                u8g2_DrawBox(&u8g2, 14, 51, 21, 10);
            }
            else if (CoorSysPar.parsetsign == 2)
            {
                // u8g2_DrawHLine(&u8g2, 35, 63, 15);
                u8g2_DrawBox(&u8g2, 38, 51, 21, 10);
            }

            for (size_t i = 0; i < 111; i++)
            {
                // yy = (int)(49 * (30 - ntc1save[i]) / 10);
                u8g2_DrawPixel(&u8g2, (119 - i), Y_count(ntc1save[i]));
            }
            // osDelay(MENU_INTERVAL_TIME);

        } while (u8g2_NextPage(&u8g2));
        break;
    }
}

uint8_t DetectionOUT_enum = 0; //
uint8_t Detection_set = 0;     // 设置开关的标记

/**
 * DetectionoOUT
 * 用来设置输出的
 * 内容包括输出开启的开关
 * 目标温度
 * 风扇pwm百分比
 */
void DetectionOUT(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }

    switch (Dir)
    {
    case MENU_UP:
        if (Detection_set == DETECTIONOUT_SET_CLOSE)
        {
            DetectionOUT_enum--;
            if (DetectionOUT_enum >= DASHBOARDOUT_LAST)
            {
                DetectionOUT_enum = DASHBOARDOUT_LAST - 1;
            }
        }
        else if (Detection_set == DETECTIONOUT_SET_OPEN)
        {
            switch (DetectionOUT_enum)
            {
            case OUTSTATE:
                if (out_state == 1)
                {
                    out_state = 0;
                }
                else if (out_state == 0)
                {
                    out_state = 1;
                }
                break;

            case TEMPTARGET:
                heat_pid_state.target += 1;
                break;

            case H2PWVUL:
                if (h2out.hpwm < 100)
                {
                    h2out.hpwm += 10;
                }
                break;
            default:
                break;
            }
        }
        break;

    case MENU_DOWN:
        if (Detection_set == DETECTIONOUT_SET_CLOSE)
        {
            DetectionOUT_enum++;
            if (DetectionOUT_enum >= DASHBOARDOUT_LAST)
            {
                DetectionOUT_enum = 0; // 这里的0指代枚举的第一个
            }
        }
        else if (Detection_set == DETECTIONOUT_SET_OPEN)
        {
            switch (DetectionOUT_enum)
            {
            case OUTSTATE:
                if (out_state == 1)
                {
                    out_state = 0;
                }
                else if (out_state == 0)
                {
                    out_state = 1;
                }
                break;

            case TEMPTARGET:
                heat_pid_state.target -= 1;
                break;

            case H2PWVUL:
                if (h2out.hpwm > 0)
                {
                    h2out.hpwm -= 10;
                }
                break;
            default:
                break;
            }
        }
        break;

    case MENU_LEFT:
        CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        if (Detection_set == 1)
        {
            Detection_set = 0;
        }
        else if (Detection_set == 0)
        {
            Detection_set = 1;
        }
        break;
    default:
        u8g2_FirstPage(&u8g2);
        do
        {
            // 画框
            // DialogScaleShow(&u8g2, 0, 0, 127, 64);
            // DialogScale_Show(&u8g2, 0, 0, 127, 64);
            u8g2_DrawHLine(&u8g2, 0, 10, 127);
            u8g2_DrawHLine(&u8g2, 0, 50, 127);

            u8g2_DrawVLine(&u8g2, 0, 10, 40);
            u8g2_DrawVLine(&u8g2, 127, 10, 40);

            char show[32];

            sprintf(show, "Out_state : %3d", out_state);
            u8g2_DrawStr(&u8g2, 10, 25, show);

            sprintf(show, "Target    : %3.1f", heat_pid_state.target);
            u8g2_DrawStr(&u8g2, 10, 35, show);

            sprintf(show, "Heart PWM : %3d", h2out.hpwm);
            u8g2_DrawStr(&u8g2, 10, 45, show);

            if (Detection_set == DETECTIONOUT_SET_CLOSE)
            {
                // 绘制选项光标
                // DrawTriangleCursor(10, DetectionOUT_enum * 10 + 25);
                u8g2_DrawTriangle(&u8g2,
                                  115, DetectionOUT_enum * 10 + 25 - 5,
                                  120, DetectionOUT_enum * 10 + 25 - 10,
                                  120, DetectionOUT_enum * 10 + 25 - 0);
            }
            else
            {
                // 绘制数值增减光标
                // u8g2_DrawVLine(&u8g2, 120, DetectionOUT_enum * 10 + 25 - 10, 10);
                // u8g2_DrawTriangle(&u8g2,
                //                   120, DetectionOUT_enum * 10 + 25 - 10,
                //                   116, DetectionOUT_enum * 10 + 25 - 5,
                //                   125, DetectionOUT_enum * 10 + 25 - 5);
                // u8g2_DrawTriangle(&u8g2,
                //                   120, DetectionOUT_enum * 10 + 25 - 0,
                //                   117, DetectionOUT_enum * 10 + 25 - 4,
                //                   124, DetectionOUT_enum * 10 + 25 - 4);
                u8g2_DrawBox(&u8g2, 6, DetectionOUT_enum * 10 + 25 - 9, 103, 10);
            }
        } while (u8g2_NextPage(&u8g2));
        break;
    }
}

/**
 * kp设置选项卡
 * 位于pid设置菜单下
 */
void KPSET(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        heat_pid_calibration.kp += 0.1;
        break;

    case MENU_DOWN:
        heat_pid_calibration.kp -= 0.1;
        break;

    case MENU_LEFT:
        // CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " Kp:%-4.1f", heat_pid_calibration.kp);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

/**
 * ki设置选项卡
 * 位于pid设置菜单下
 */
void KISET(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        heat_pid_calibration.ki += 0.1;
        break;

    case MENU_DOWN:
        heat_pid_calibration.ki -= 0.1;
        break;

    case MENU_LEFT:
        // CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " Ki:%-4.1f", heat_pid_calibration.ki);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

/**
 * kd设置选项卡
 * 位于pid设置菜单下
 */
void KDSET(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        heat_pid_calibration.kd += 0.1;
        break;

    case MENU_DOWN:
        heat_pid_calibration.kd -= 0.1;
        break;

    case MENU_LEFT:
        CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " Kd:%-4.1f", heat_pid_calibration.kd);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

/**
 * 感温电阻设置选项卡
 * 位于ntc设置菜单下
 */
void NTCBSET()
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        ntc1.ntcB += 10;
        break;

    case MENU_DOWN:
        ntc1.ntcB -= 10;
        break;

    case MENU_LEFT:
        // CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " ntcB:%d", ntc1.ntcB);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

void NTCR1SET()
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        ntc1.ntcR1 += 10;
        break;

    case MENU_DOWN:
        ntc1.ntcR1 -= 10;
        break;

    case MENU_LEFT:
        // CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " ntcR1:%.f", ntc1.ntcR1);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

void NTCR2SET()
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        ntc1.ntcR2 += 10;
        break;

    case MENU_DOWN:
        ntc1.ntcR2 -= 10;
        break;

    case MENU_LEFT:
        // CoorSysPar.parsetsign = 0;
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, " ntcR2:%.f", ntc1.ntcR2);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

/**
 * out设置选项卡下
 * 用来选择加热的目标
 */
int temp_time_delta = 0;
void TIME_DELTA_SET(void)
{
    uint8_t Dir = 0;

    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        temp_time_delta += 1;
        break;

    case MENU_DOWN:
        temp_time_delta -= 1;
        break;

    case MENU_LEFT:
        IntoState = false;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        // end_sign = 0;
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;
    default:
        // 画框
        DialogScaleShow(&u8g2, 18, 15, 70, 22);

        char show[32];
        sprintf(show, "DELTA:%d  ", temp_time_delta);
        u8g2_DrawStr(&u8g2, 30, 30, show);

        u8g2_SendBuffer(&u8g2);
        break;
    }
}

/**
 * Dashboard
 * 仪表盘
 * 只显示，无设置
 */
int Dashboard_menu = 0;
void Dashboard(void)
{
    uint8_t Dir = 0;
    if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(300)) != pdTRUE)
    {
        Dir = 0;
    }
    switch (Dir)
    {
    case MENU_UP:
        Dashboard_menu++;
        if (Dashboard_menu == DASHBOARD_LAST)
        {
            Dashboard_menu = 0;
        }
        break;

    case MENU_DOWN:
        Dashboard_menu--;
        if (Dashboard_menu < 0)
        {
            Dashboard_menu = DASHBOARD_LAST - 1;
        }
        break;

    case MENU_LEFT:
        CoorSysPar.parsetsign = 0;
        // 改变菜单状态
        Switch_Menu_State(APP_BREAK);
        // 最后再跑一下菜单的程序显示出来
        Process_Menu_Run(MENU_LEFT);
        break;

    case MENU_RIGHT:
        break;

    case MENU_ENTER:
        break;

    default:
        if (Dashboard_menu == DASHBOARD_NTC)
        {
            Dashboard_PID();
        }
        else if (Dashboard_menu == DASHBOARD_OUT)
        {
            Dashboard_OUT();
        }
        break;
    }
}

/**
 * Dashboard_PID
 */
void Dashboard_PID(void)
{
    u8g2_FirstPage(&u8g2);
    do
    {
        char show[32];
        u8g2_SetFont(&u8g2, u8g2_font_profont15_mf);
        sprintf(show, "Dashboard_PID");
        u8g2_DrawStr(&u8g2, 1, 18, show);

        u8g2_SetFont(&u8g2, u8g2_font_profont10_mf);

        sprintf(show, " Kp:%-4.1f", heat_pid_calibration.kp);
        u8g2_DrawStr(&u8g2, 5, 2 + 3 * 10, show);

        sprintf(show, " Ki:%-4.1f", heat_pid_calibration.ki);
        u8g2_DrawStr(&u8g2, 5, 2 + 4 * 10, show);

        sprintf(show, " Kd:%-4.1f", heat_pid_calibration.kd);
        u8g2_DrawStr(&u8g2, 5, 2 + 5 * 10, show);

        sprintf(show, "ntcT:%-4.1f", ntc1.ntcT);
        u8g2_DrawStr(&u8g2, 50, 2 + 3 * 10, show);

        sprintf(show, "pid:%.1f", heat_pid_state.output);
        u8g2_DrawStr(&u8g2, 50, 2 + 4 * 10, show);

        sprintf(show, "pwm:%d", h1out.hpwm);
        u8g2_DrawStr(&u8g2, 50, 2 + 5 * 10, show);
    } while (u8g2_NextPage(&u8g2));
}

/**
 * Dashboard_OUT
 */
void Dashboard_OUT(void)
{
    u8g2_FirstPage(&u8g2);
    do
    {
        char show[32];
        u8g2_SetFont(&u8g2, u8g2_font_profont15_mf);
        sprintf(show, "Dashboard_OUT");
        u8g2_DrawStr(&u8g2, 1, 18, show);

        u8g2_SetFont(&u8g2, u8g2_font_profont10_mf);

        sprintf(show, " VCC:%-4.1f", vadc1.vcc);
        u8g2_DrawStr(&u8g2, 5, 2 + 3 * 10, show);

        sprintf(show, " ntcT:%-4.1f", ntc1.ntcT);
        u8g2_DrawStr(&u8g2, 5, 2 + 4 * 10, show);

        sprintf(show, " ntcB:%d", ntc1.ntcB);
        u8g2_DrawStr(&u8g2, 5, 2 + 5 * 10, show);

        sprintf(show, " target:%-4.1f", heat_pid_state.target);
        u8g2_DrawStr(&u8g2, 5, 2 + 6 * 10, show);

    } while (u8g2_NextPage(&u8g2));
}
