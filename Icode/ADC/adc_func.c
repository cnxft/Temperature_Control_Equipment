#include "ADC/adc_func.h"

uint16_t dmaadc[3];
float ntc1save[111];
// float ntc2save[111]={0};

ntcpar ntc1;
// ntcpar ntc1 =
//     {
//         .ntcB = 3950,
//         .ntcT25 = 298.15, // 25摄氏度的开尔文温度
//         .ntcMADC = 4095,
//         .ntcR1 = 10000, // 分压电阻的阻值Ω
//         .ntcR2 = 100000};

vadcpar vadc1;
// vadcpar vadc1 =
//     {
//         .vadcADC = 0,
//         .vadcMADC = 4095,
//         .vdda = 3.3,
//         .resh = 10,
//         .resl = 1,
//         .vcc = 0};

void NTCSave(float *ntcsave, float ntcsave0)
{
    for (size_t i = 110; i > 0; i--)
    {
        ntcsave[i] = ntcsave[i - 1];
    }
    ntcsave[0] = ntcsave0;
}

void NTC1save_init(float *ntcsave)
{
    ntc1.ntcADC = dmaadc[0];
    ntc1.ntcT = ntc_count_value(ntc1);
    for (size_t i = 0; i < 111; i++)
    {
        ntcsave[i] = ntc1.ntcT;
    }
}
