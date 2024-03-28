#include "ADC/ntc.h"

/*************************************************           
[函数名称]fGet_NTC_Temp                                                  
[函数功能]计算ntc温度                                                  
[参    数]Rntc 当前的NTC电阻值                                                  
[全局变量]                                                  
[返 回 值]当前的温度值                                                  
[备    注]                                                  
*************************************************/             
// float fGet_NTC_Temp(float Rntc)
// {
// 	float temp;
// 	temp = (log(Rntc)-log(NTC_R25))/NTC_B;
// 	temp = 1/NTC_T25 + temp;
// 	temp = 1/temp;
// 	return (temp-274.5);
// }

//公式法
float ntc_count_value(ntcpar ntcx)
{
    // 计算当前阻值
    ntcx.ntcR = ntcx.ntcR1 * ntcx.ntcADC / (ntcx.ntcMADC - ntcx.ntcADC);
    // 计算温度
    //RT=R0*EXP(B*(1/T-1/(273.15+25))  T是要解的温度
    ntcx.ntcT = ntcx.ntcR/ntcx.ntcR2;
	ntcx.ntcT = log(ntcx.ntcT);//ln(ntcx.ntcR/Rp)
	ntcx.ntcT/=ntcx.ntcB;//ln(ntcx.ntcR/Rp)/B
	ntcx.ntcT+=(1/ntcx.ntcT25);
	ntcx.ntcT = 1/(ntcx.ntcT);
	ntcx.ntcT-=273.15;

    return ntcx.ntcT;
}

//查表法


