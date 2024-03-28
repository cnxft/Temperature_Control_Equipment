#include "ADC/vadc.h"

float vadc_count_value(vadcpar vadc)
{
    vadc.vcc = (float)vadc.vadcADC / vadc.vadcMADC;
    vadc.vcc *= vadc.vdda;
    vadc.vcc = vadc.vcc * (vadc.resh + vadc.resl) / vadc.resl;
    return vadc.vcc;
}
