#ifndef __USERCAN_H__
#define __USERCAN_H__

#include "stm32f1xx_hal.h"
#include "main.h"
#include "can.h"
#include "U8G2/u8g2.h"
#include "OLED/oled.h"
#include "stdio.h"

HAL_StatusTypeDef CAN_SetFilters(void);

void CAN_RX_MESSAGE(uint8_t msgID);
void CAN_TX_MESSAGE(uint8_t msgID, uint8_t frameType, uint8_t txDate);

#endif
