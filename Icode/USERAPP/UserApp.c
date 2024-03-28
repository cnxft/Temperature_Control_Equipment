#include "USERAPP/UserApp.h"

void AppTask_App(void *argument)
{
  h1out.hpwm_max = htim2.Init.Period + 1;
  h1out.hpwm = 0;
  h2out.hpwm_max = htim2.Init.Period + 1;
  h2out.hpwm = htim2.Init.Period + 1;
  osDelay(2000);
  for (size_t i = 0; i < Date_TOTAL_NUM; i++)
  {
    // 这里写读取数据的函数
    if (HAL_OK == Date_Init(i))
    {
      xTaskNotifyGive(TaskMenuHandle);
    }
#if SWITCH_FOR_TESTING_CODE
    BaseType_t xReturn;
    xReturn = xTaskNotifyGive(TaskMenuHandle);
    if (xReturn == pdTRUE)
    {
      printf("give %d success\r\n", i);
    }
#endif
    osDelay(pdMS_TO_TICKS(100));
  }

  for (;;)
  {
    ntc1.ntcADC = dmaadc[0];
    ntc1.ntcT = ntc_count_value(ntc1);

    vadc1.vadcADC = dmaadc[2];
    vadc1.vcc = vadc_count_value(vadc1);

    // num++;
    // 设置风扇的转速

    osDelay(1);
  }
}

void AppTask_OutPut(void *argument)
{
  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, h2out.hpwm_max);
  for (;;)
  {
    if (out_state == 1)
    {
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,
                           pid_pwm_count(heat_pid_calibration, &heat_pid_state, ntc1.ntcT));
    }
    else
    {
      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
      // __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    }
    osDelay(pdMS_TO_TICKS(PID_TIME_DELTA));
  }
}

// // 要写入到24c16的字符串数组
// const uint8_t TEXT_Buffer[] = {"C++ is the best language!"}; // 要写入的内容
// #define SIZE sizeof(TEXT_Buffer)                             // 写入内容的大小
// #define ADDRESS 2021                                         // 读写地址
// uint8_t datatemp[SIZE];
#if SWITCH_FOR_TESTING_CODE
extern osThreadId_t TaskAppHandle;
extern osThreadId_t TaskOutPutHandle;
extern osThreadId_t TaskMessSendHandle;
extern osThreadId_t TaskSerComHandle;
extern osThreadId_t TaskCanComHandle;
extern osThreadId_t TaskKeyHandle;
extern osThreadId_t TaskMenuHandle;
#endif

void AppTask_MessSend(void *argument)
{

#if CAN_ENABLE_or_DISABLE
  uint8_t msgID = 1;
  uint8_t txDate = 67;
#endif
  for (;;)
  {
#if SWITCH_FOR_TESTING_CODE
    // TaskHandle_t taskHandle = xTaskGetIdleTaskHandle();
    printf("\r\n");
    printf("TaskAppHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskAppHandle));
    printf("TaskOutPutHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskOutPutHandle));
    printf("TaskMessSendHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskMessSendHandle));
    printf("TaskSerComHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskSerComHandle));
    printf("TaskCanComHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskCanComHandle));
    printf("TaskKeyHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskKeyHandle));
    printf("TaskMenuHandle = %d\r\n", uxTaskGetStackHighWaterMark(TaskMenuHandle));
    printf("\r\n");
#endif

#if SERIAL_ENABLE_or_DISABL
    if (out_state == 1)
    {
      printf("T%.1fP%.1fI%.1fD%.1fS%d\r\n", ntc1.ntcT, heat_pid_calibration.kp, heat_pid_calibration.ki, heat_pid_calibration.kd, out_state);
      // shellWriteEndLine(&shell, shellBuffer1, 512);
    }
#endif

#if CAN_ENABLE_or_DISABLE
    CAN_TX_MESSAGE(msgID, CAN_RTR_DATA, txDate);
#endif
    osDelay(3000);
  }
}

void AppTask_SerCom(void *argument)
{
  Shell *shell = (Shell *)argument;
  char data;
#if SHELL_TASK_WHILE == 1
  while (1)
  {
#endif
    if (shell->read && shell->read(&data, 1) == 1)
    {
      shellHandler(shell, data);
    }
#if SHELL_TASK_WHILE == 1
  }
#endif
}

void AppTask_CanCom(void *argument)
{
  for (;;)
  {
    CAN_RX_MESSAGE(1);

    osDelay(100);
  }
}

void AppTask_Key(void *argument)
{
  uint8_t key_first = 0;
  uint8_t key_second = 0;
  for (;;)
  {
    // 产生一个滴答
    button_ticks();
    // 判断
    key_first = Get_Key_Pressed();
    if (key_first == 0)
    {
      key_second = 0;
    }
    if ((key_first != 0) && (key_second != key_first))
    {
      key_second = key_first;
      // 写入队列中
      BaseType_t err = xQueueSendToBack(Queue_OLEDKeysHandle, &key_first, pdMS_TO_TICKS(0));
      // 如果写满了就清空
      if (err == errQUEUE_FULL)
      {
        xQueueReset(Queue_OLEDKeysHandle);
      }
      // osDelay(pdMS_TO_TICKS(300));
    }
    osDelay(pdMS_TO_TICKS(TICKS_INTERVAL));
  }
}

/**
 * 如果读到按键就会立刻执行
 * 没读到就等300ms
 */
void AppTask_Menu(void *argument)
{
  for (size_t i = 0; i < Date_TOTAL_NUM; i++)
  {
    uint32_t ulReturn;
    BaseType_t xClearCountOnExit = pdTRUE; // pdTRUE，二值信号量，false是计数信号量
    ulReturn = ulTaskNotifyTake(xClearCountOnExit, portMAX_DELAY);
    if (ulReturn == pdTRUE)
    {
      Draw_Process(i, Date_TOTAL_NUM - 1);
    }
  }
  // 菜单初始化
  Menu_Init();

  for (;;)
  {
    uint8_t Dir = 0;
    if (MENU_STATE != APP_RUN)
    {
      if (xQueueReceive(Queue_OLEDKeysHandle, &Dir, pdMS_TO_TICKS(200)) != pdTRUE)
      {
        Dir = 0;
      }
    }
    NTCSave(ntc1save, ntc1.ntcT); // 把温度保存到数组中
    // if (sign_temp_ntcT == 1)
    // {
    //   NTCSave(ntc1save, ntc1.ntcT); // 把温度保存到数组中
    //   sign_temp_ntcT = 0;
    // }

    switch (MENU_STATE)
    {
    case MENU_RUN:
      Process_Menu_Run(Dir);
      break;

    case APP_RUN:
      Process_App_Run();
      break;

    case APP_BREAK:
      Process_Menu_Run(Dir);
      break;

    default:
      break;
    }
    // osDelay(pdMS_TO_TICKS(MENU_INTERVAL_TIME));
  }
}
