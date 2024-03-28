#ifndef __PWMOUT_FUNC_H
#define __PWMOUT_FUNC_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "PWMOUT/pid.h"
#include "USERAPP/UserApp.h"

#define PID_TIME_DELTA 1000 // PID周期 单位ms 程序中用s




extern PID_Calibration heat_pid_calibration;
extern PID_State heat_pid_state;

uint16_t pid_pwm_count(PID_Calibration calibration, PID_State *state, float res_ntc_temp);

#endif /*__PWMOUT_FUNC_H*/
