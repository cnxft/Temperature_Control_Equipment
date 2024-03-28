#include "USERCAN/usercan.h"

// 设置筛选器
HAL_StatusTypeDef CAN_SetFilters(void)
{
	CAN_FilterTypeDef canFilter;
	// 设置ID 和 掩码
	canFilter.FilterIdHigh = 0x0020;
	canFilter.FilterIdLow = 0x0000;
	canFilter.FilterMaskIdHigh = 0x0020;
	canFilter.FilterMaskIdLow = 0x0000;

	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterBank = 0;
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterScale = CAN_FILTERSCALE_32BIT;

	canFilter.FilterActivation = CAN_FILTER_ENABLE; // 启用筛选器
	canFilter.SlaveStartFilterBank = 14;			
	
	HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(&hcan, &canFilter);
	return result;
}

void CAN_RX_MESSAGE(uint8_t msgID)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];
	HAL_Delay(1);
	if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) != 1)
	{
		return;
	}

	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
	{
		printf("RxHeader.RTR%d\r\n",RxHeader.RTR);
		printf("RxData[0]%d\r\n",RxData[0]);
		printf("RxData[1]%d\r\n",RxData[1]);
	}
}

void CAN_TX_MESSAGE(uint8_t msgID, uint8_t frameType, uint8_t txDate)
{
	uint8_t TxData[8];
	TxData[0] = msgID;
	TxData[1] = txDate;

	CAN_TxHeaderTypeDef TxHeader; // 用来定义参数
	TxHeader.StdId = msgID;
	TxHeader.RTR = frameType;
	TxHeader.IDE = CAN_ID_STD;			   // 有宏定义
	TxHeader.DLC = 2;					   // 设置了只发两个字节
	TxHeader.TransmitGlobalTime = DISABLE; // 发送的全局时间？这里禁用时间戳

	// 小于1说明有可用的发送邮箱
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) < 1)
	{
		;
	}

	uint32_t TxMailbox;
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		return;
	}

	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3)
	{
		;
	}
}
