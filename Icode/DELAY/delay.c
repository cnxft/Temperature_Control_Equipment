#include "DELAY/delay.h"

#define DELAY_SYS FreeRTOS_72MHZ

#if DELAY_SYS == BARE_MACHINE
/*
操作步骤：
a.根据延时时间和定时器所选时钟频率，计算出定时器要计数的时间数值；
b.将该数值加载到重装载寄存器中；
c.将当前值寄存器清零，打开定时器开始计数；
d.等待控制及状态寄存器的位16变为1；
e.关闭定时器，退出。
*/

/**
 * @brief  微秒级延时
 * @param  xus 延时时长，范围：0~233015
 * @retval 无
 */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 72 * xus;   // 设置定时器重装值    （系统滴答定时器SysTick：当加入操作系统时可利用其提供定时以完成任务的切换）
    SysTick->VAL = 0x00;        // 清空当前计数值
    SysTick->CTRL = 0x00000005; // 设置时钟源为HCLK，启动定时器
    while (!(SysTick->CTRL & 0x00010000))
        ;                       // 等待计数到0
    SysTick->CTRL = 0x00000004; // 关闭定时器
}
#endif

#if DELAY_SYS == FreeRTOS_72MHZ
/*
操作步骤：
a.根据延时时间和定时器所选时钟频率，计算出定时器要计数的时间数值；
b.获取当前数值寄存器的数值；
c.以当前数值为基准开始计数；
d.当所计数值等于（大于）需要延时的时间数值时退出。
注：计数时间值的计算，我们以延时10us，时钟频率为72MHZ的STM32F103C8T6来计算，
        计数值 = 延时时间/1S × 时钟频率 = 0.000 01/1 *72 000 000 =  720

*/

// void delay_us(uint32_t nus)
// {
//     uint32_t ticks;
//     uint32_t told, tnow, reload, tcnt = 0;
//     if ((0x0001 & (SysTick->CTRL)) == 0) // 定时器未工作
//         vPortSetupTimerInterrupt();      // 初始化定时器

//     reload = SysTick->LOAD;                    // 获取重装载寄存器值
//     ticks = nus * (SystemCoreClock / 1000000); // 计数时间值

//     vTaskSuspendAll();   // 阻止OS调度，防止打断us延时
//     told = SysTick->VAL; // 获取当前数值寄存器值（开始时数值）
//     while (1)
//     {
//         tnow = SysTick->VAL; // 获取当前数值寄存器值
//         if (tnow != told)    // 当前值不等于开始值说明已在计数
//         {
//             if (tnow < told)         // 当前值小于开始数值，说明未计到0
//                 tcnt += told - tnow; // 计数值=开始值-当前值

//             else                              // 当前值大于开始数值，说明已计到0并重新计数
//                 tcnt += reload - tnow + told; // 计数值=重装载值-当前值+开始值  （
//                                               // 已从开始值计到0）

//             told = tnow; // 更新开始值
//             if (tcnt >= ticks)
//                 break; // 时间超过/等于要延迟的时间,则退出.
//         }
//     }
//     xTaskResumeAll(); // 恢复OS调度
// }
//nus为要延时的us数.		    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;						//LOAD的值	    	 
	ticks=nus*(SystemCoreClock/1000000); 	//需要的节拍数	  		 
	tcnt=0;
	vTaskSuspendAll();									//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;						//时间超过/等于要延迟的时间,则退出.
		}  
	};
	xTaskResumeAll();										//恢复OS调度									    
}

// SystemCoreClock为系统时钟(system_stmf4xx.c中)，通常选择该时钟作为
// systick定时器时钟，根据具体情况更改
#endif
