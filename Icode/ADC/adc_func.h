#ifndef __ADC_FUNC_H
#define __ADC_FUNC_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "ADC/ntc.h"
#include "ADC/vadc.h"

void NTCSave(float *ntcsave, float ntcsave0);
void NTC1save_init(float *ntcsave);

#endif
