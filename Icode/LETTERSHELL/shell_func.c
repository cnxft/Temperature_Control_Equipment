#include "LETTERSHELL/shell_func.h"

extern uint8_t out_state;
HAL_StatusTypeDef Shell_OUT(int state)
{
    if (state == 0)
    {
        out_state = 0;
    }
    else
    {
        out_state = 1;
    }
    return HAL_OK;
}

HAL_StatusTypeDef Shell_Target(float tar)
{
    heat_pid_state.target = tar;
    return Date_Save(Date_TARGET_SUM);
}
HAL_StatusTypeDef Shell_Parameter_Show(void)
{
    printf("kp:%.1f ki:%.1f kd:%.1f\r\n", heat_pid_calibration.kp, heat_pid_calibration.ki, heat_pid_calibration.kd);
    printf("ntcB:%d ntcT25:%.1f ntcMADC:%.1f\r\n", ntc1.ntcB, ntc1.ntcT25, ntc1.ntcMADC);
    printf("ntcR1:%.1f ntcR2:%.1f\r\n", ntc1.ntcR1, ntc1.ntcR2);
    printf("vadcMADC:%d vdda:%.1f resh:%.1f resl:%.1f\r\n", vadc1.vadcMADC, vadc1.vdda, vadc1.resh, vadc1.resl);
    printf("target:%.1f time_delta:%.1f\r\n", heat_pid_state.target, heat_pid_state.time_delta);
    return HAL_OK;
}

HAL_StatusTypeDef Shell_PID_Reset(void)
{
    heat_pid_state.previous_error = 0;
    heat_pid_state.integral = 0;
    return HAL_OK;
}

HAL_StatusTypeDef Shell_Kp(float kp)
{
    heat_pid_calibration.kp = kp;
    Date_Save(Date_KP_SUM);
    return HAL_OK;
}

HAL_StatusTypeDef Shell_Ki(float ki)
{
    heat_pid_calibration.ki = ki;
    Date_Save(Date_KI_SUM);
    return HAL_OK;
}

HAL_StatusTypeDef Shell_Kd(float kd)
{
    heat_pid_calibration.kd = kd;
    Date_Save(Date_KD_SUM);
    return HAL_OK;
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), out_state, Shell_OUT, Enable output 1 or 0);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), out_target, Shell_Target, Set target temp double);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), parameter_show, Shell_Parameter_Show, show pid wushen);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), pid_reset, Shell_PID_Reset, pid reset wushen);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kp_set, Shell_Kp, Set Kp float);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ki_set, Shell_Ki, Set Ki float);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kd_set, Shell_Kd, Set Kd float);
