/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "USERAPP/UserApp.h"
#include "shell_port.h"
#include "timers.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for TaskApp */
osThreadId_t TaskAppHandle;
const osThreadAttr_t TaskApp_attributes = {
  .name = "TaskApp",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for TaskOutPut */
osThreadId_t TaskOutPutHandle;
const osThreadAttr_t TaskOutPut_attributes = {
  .name = "TaskOutPut",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskMessSend */
osThreadId_t TaskMessSendHandle;
const osThreadAttr_t TaskMessSend_attributes = {
  .name = "TaskMessSend",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskSerCom */
osThreadId_t TaskSerComHandle;
const osThreadAttr_t TaskSerCom_attributes = {
  .name = "TaskSerCom",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskCanCom */
osThreadId_t TaskCanComHandle;
const osThreadAttr_t TaskCanCom_attributes = {
  .name = "TaskCanCom",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskKey */
osThreadId_t TaskKeyHandle;
const osThreadAttr_t TaskKey_attributes = {
  .name = "TaskKey",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for TaskMenu */
osThreadId_t TaskMenuHandle;
const osThreadAttr_t TaskMenu_attributes = {
  .name = "TaskMenu",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Queue_OLEDKeys */
osMessageQueueId_t Queue_OLEDKeysHandle;
const osMessageQueueAttr_t Queue_OLEDKeys_attributes = {
  .name = "Queue_OLEDKeys"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_App(void *argument);
void AppTask_OutPut(void *argument);
void AppTask_MessSend(void *argument);
void AppTask_SerCom(void *argument);
void AppTask_CanCom(void *argument);
void AppTask_Key(void *argument);
void AppTask_Menu(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_OLEDKeys */
  Queue_OLEDKeysHandle = osMessageQueueNew (10, sizeof(uint8_t), &Queue_OLEDKeys_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskApp */
  TaskAppHandle = osThreadNew(AppTask_App, NULL, &TaskApp_attributes);

  /* creation of TaskOutPut */
  TaskOutPutHandle = osThreadNew(AppTask_OutPut, NULL, &TaskOutPut_attributes);

  /* creation of TaskMessSend */
  TaskMessSendHandle = osThreadNew(AppTask_MessSend, NULL, &TaskMessSend_attributes);

  /* creation of TaskSerCom */
  TaskSerComHandle = osThreadNew(AppTask_SerCom, (void*) &shell, &TaskSerCom_attributes);

  /* creation of TaskCanCom */
  TaskCanComHandle = osThreadNew(AppTask_CanCom, NULL, &TaskCanCom_attributes);

  /* creation of TaskKey */
  TaskKeyHandle = osThreadNew(AppTask_Key, NULL, &TaskKey_attributes);

  /* creation of TaskMenu */
  TaskMenuHandle = osThreadNew(AppTask_Menu, NULL, &TaskMenu_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_App */
/**
  * @brief  Function implementing the TaskApp thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_App */
__weak void AppTask_App(void *argument)
{
  /* USER CODE BEGIN AppTask_App */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_App */
}

/* USER CODE BEGIN Header_AppTask_OutPut */
/**
* @brief Function implementing the TaskOutPut thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_OutPut */
__weak void AppTask_OutPut(void *argument)
{
  /* USER CODE BEGIN AppTask_OutPut */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_OutPut */
}

/* USER CODE BEGIN Header_AppTask_MessSend */
/**
* @brief Function implementing the TaskMessSend thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_MessSend */
__weak void AppTask_MessSend(void *argument)
{
  /* USER CODE BEGIN AppTask_MessSend */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_MessSend */
}

/* USER CODE BEGIN Header_AppTask_SerCom */
/**
* @brief Function implementing the TaskSerCom thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_SerCom */
__weak void AppTask_SerCom(void *argument)
{
  /* USER CODE BEGIN AppTask_SerCom */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_SerCom */
}

/* USER CODE BEGIN Header_AppTask_CanCom */
/**
* @brief Function implementing the TaskCanCom thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_CanCom */
__weak void AppTask_CanCom(void *argument)
{
  /* USER CODE BEGIN AppTask_CanCom */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_CanCom */
}

/* USER CODE BEGIN Header_AppTask_Key */
/**
* @brief Function implementing the TaskKey thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Key */
__weak void AppTask_Key(void *argument)
{
  /* USER CODE BEGIN AppTask_Key */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_Key */
}

/* USER CODE BEGIN Header_AppTask_Menu */
/**
* @brief Function implementing the TaskMenu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Menu */
__weak void AppTask_Menu(void *argument)
{
  /* USER CODE BEGIN AppTask_Menu */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_Menu */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

