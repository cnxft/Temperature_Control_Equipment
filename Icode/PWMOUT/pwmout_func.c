#include "PWMOUT/pwmout_func.h"

PID_Calibration heat_pid_calibration;
// PID_Calibration heat_pid_calibration =
//     {
//         .kp = 200.0, // Proportional gain
//         .ki = 0.0, // Integral gain
//         .kd = 0.0  // Derivative gain
// };

PID_State heat_pid_state;
// PID_State heat_pid_state =
//     {
//         .actual = 0,
//         .target = 100,
//         .time_delta = PID_TIME_DELTA / 1000,
//         .previous_error = 0,
//         .integral = 0,
//         .output = 0 // 最后的输出
// };

PWMOUT h1out;
PWMOUT h2out;
/**
 * 计算一个pwm的值
 * p i d 参数
 * 迭代
 * 传进来的温度
 */
uint16_t pid_pwm_count(PID_Calibration calibration, PID_State *state, float res_ntc_temp)
{
    uint16_t pid_pwm_vlaue;
    state->actual = res_ntc_temp;
    pid_iterate(calibration, state);

    if (state->output < 0)
    {
        pid_pwm_vlaue = 0;
    }
    else if (state->output >= 500)
    {
        pid_pwm_vlaue = 500;
    }
    else
        pid_pwm_vlaue = state->output;

    pid_pwm_vlaue = (uint16_t)(pid_pwm_vlaue *h1out.hpwm_max / 500);
    h1out.hpwm = pid_pwm_vlaue;
    // printf("output:%.1f,pid_pwm_value:%d,temp.1:%f\r\n", state->output, pid_pwm_vlaue, res_ntc_temp);
    return pid_pwm_vlaue;
}
