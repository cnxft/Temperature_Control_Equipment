#include "OLED/oled.h"

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{

	switch (msg)
	{
	case U8X8_MSG_BYTE_SEND: /*通过SPI发送arg_int个字节数据*/
		HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)arg_ptr, arg_int);
		while (hspi1.TxXferCount)
			;
		// HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 200);
		break;
	case U8X8_MSG_BYTE_INIT: /*初始化函数*/
		OLED_RES_Clr();
		HAL_Delay(200);
		OLED_RES_Set();
		break;
	case U8X8_MSG_BYTE_SET_DC: /*设置DC引脚,表明发送的是数据还是命令*/
		switch (arg_int)
		{
		case 0:
			HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
			printf("oled.c other arg_int");
			break;
		}
		break;
	case U8X8_MSG_BYTE_START_TRANSFER:
		OLED_CS_Clr();
		break;
	case U8X8_MSG_BYTE_END_TRANSFER:
		OLED_CS_Set();
		break;
	default:
		return 0;
	}
	return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	return 1;
}
/**
 * u8g2 初始化
 */
void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
	u8g2_InitDisplay(u8g2);		// 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
	u8g2_SetPowerSave(u8g2, 0); // 打开显示器
	u8g2_ClearBuffer(u8g2);

	u8g2_SendBuffer(u8g2);
}
