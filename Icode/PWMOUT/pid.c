#include "PWMOUT/pid.h"

void pid_iterate(PID_Calibration calibration, PID_State *state)
{
    // 计算期望值和实际值之间的差值（误差）
    double error = state->target - state->actual;
    // 计算和更新积分
    state->integral += (error * state->time_delta);
    // 计算导数
    double derivative = (error - state->previous_error) / state->time_delta;
    // 根据算法计算输出值
    state->output = ((calibration.kp * error) + (calibration.ki * state->integral) + (calibration.kd * derivative));
    // 将state.previous_error更新为此迭代计算的错误值
    state->previous_error = error;
    // 返回反映计算的状态结构
    // return state;
}
