#include "AT24CXX/SaveAndRead_func.h"

HAL_StatusTypeDef Date_Init(uint8_t which_one)
{
    // 读数据共用体
    union date_union
    {
        uint8_t uint8_t_read_dat;   // 浮点数占1个字节
        uint16_t uint16_t_read_dat; // 浮点数占2个字节
        float float_read_dat;       // 浮点数占4个字节
        double double_read_dat;     // 双精度浮点数占8个字节
        uint8_t buf[8];             // 定义 8个字节 的空间
    };
    union date_union rev_data; // 用来接收

    switch (which_one)
    {
    case 0:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_OUTSTATE_SUM), rev_data.buf, sizeof(uint8_t)))
        {
            out_state = rev_data.uint8_t_read_dat;
            // printf("rev_data.uint8_t_read_dat:%d\r\n",rev_data.uint8_t_read_dat);
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 1:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_NTCB_SUM), rev_data.buf, sizeof(uint16_t)))
        {
            ntc1.ntcB = rev_data.uint16_t_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 2:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_NTCT25_SUM), rev_data.buf, sizeof(float)))
        {
            ntc1.ntcT25 = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 3:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_NTCMAXADC_SUM), rev_data.buf, sizeof(float)))
        {
            ntc1.ntcMADC = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 4:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_NTCR1_SUM), rev_data.buf, sizeof(float)))
        {
            ntc1.ntcR1 = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 5:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_NTCR2_SUM), rev_data.buf, sizeof(float)))
        {
            ntc1.ntcR2 = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 6:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_VADCMADC_SUM), rev_data.buf, sizeof(uint16_t)))
        {
            vadc1.vadcMADC = rev_data.uint16_t_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 7:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_VDDA_SUM), rev_data.buf, sizeof(float)))
        {
            vadc1.vdda = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 8:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_RESH_SUM), rev_data.buf, sizeof(float)))
        {
            vadc1.resh = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 9:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_RESL_SUM), rev_data.buf, sizeof(float)))
        {
            vadc1.resl = rev_data.float_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 10:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_KP_SUM), rev_data.buf, sizeof(double)))
        {
            heat_pid_calibration.kp = rev_data.double_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 11:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_KI_SUM), rev_data.buf, sizeof(double)))
        {
            heat_pid_calibration.ki = rev_data.double_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 12:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_KD_SUM), rev_data.buf, sizeof(double)))
        {
            heat_pid_calibration.kd = rev_data.double_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 13:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_TARGET_SUM), rev_data.buf, sizeof(double)))
        {
            heat_pid_state.target = rev_data.double_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case 14:
        if (HAL_OK == At24Cxx_Read_Amount_Byte((DATE_FIRST_ADD + Date_TIMEDELTA_SUM), rev_data.buf, sizeof(double)))
        {
            heat_pid_state.time_delta = rev_data.double_read_dat;
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    default:
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef Date_Save(uint8_t which_one)
{
    // 读数据共用体
    union date_union
    {
        uint8_t uint8_t_read_dat;   // 浮点数占1个字节
        uint16_t uint16_t_read_dat; // 浮点数占2个字节
        float float_read_dat;       // 浮点数占4个字节
        double double_read_dat;     // 双精度浮点数占8个字节
        uint8_t buf[8];             // 定义 8个字节 的空间
    };
    union date_union sav_data; // 用来接收

    switch (which_one)
    {
    case Date_OUTSTATE_SUM:
        sav_data.uint8_t_read_dat = out_state;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_OUTSTATE_SUM), sav_data.buf, sizeof(uint8_t)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_NTCB_SUM:
        sav_data.uint16_t_read_dat = ntc1.ntcB;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_NTCB_SUM), sav_data.buf, sizeof(uint16_t)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_NTCT25_SUM:
        sav_data.float_read_dat = ntc1.ntcT25;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_NTCT25_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_NTCMAXADC_SUM:
        sav_data.float_read_dat = ntc1.ntcMADC;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_NTCMAXADC_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_NTCR1_SUM:
        sav_data.float_read_dat = ntc1.ntcR1;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_NTCR1_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_NTCR2_SUM:
        sav_data.float_read_dat = ntc1.ntcR2;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_NTCR2_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_VADCMADC_SUM:
        sav_data.uint16_t_read_dat = vadc1.vadcMADC;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_VADCMADC_SUM), sav_data.buf, sizeof(uint16_t)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_VDDA_SUM:
        sav_data.float_read_dat = vadc1.vdda;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_VDDA_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_RESH_SUM:
        sav_data.float_read_dat = vadc1.resh;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_RESH_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_RESL_SUM:
        sav_data.float_read_dat = vadc1.resl;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_RESL_SUM), sav_data.buf, sizeof(float)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_KP_SUM:
        sav_data.double_read_dat = heat_pid_calibration.kp;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_KP_SUM), sav_data.buf, sizeof(double)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_KI_SUM:
        sav_data.double_read_dat = heat_pid_calibration.ki;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_KI_SUM), sav_data.buf, sizeof(double)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_KD_SUM:
        sav_data.double_read_dat = heat_pid_calibration.kd;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_KD_SUM), sav_data.buf, sizeof(double)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_TARGET_SUM:
        sav_data.double_read_dat = heat_pid_state.target;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_TARGET_SUM), sav_data.buf, sizeof(double)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    case Date_TIMEDELTA_SUM:
        sav_data.double_read_dat = heat_pid_state.time_delta;
        if (HAL_OK == At24Cxx_Write_Amount_Byte((DATE_FIRST_ADD + Date_TIMEDELTA_SUM), sav_data.buf, sizeof(double)))
        {
            return HAL_OK;
        }
        else
        {
            return HAL_ERROR;
        }
    default:
        return HAL_ERROR;
    }
}
