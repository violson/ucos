/*实验                                          

 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "ucos_ii.h"

#define	TASK_STK_SIZE	 64	      //定义堆栈长度


OS_STK LED0_TASK_STK[TASK_STK_SIZE];
OS_STK LED1_TASK_STK[TASK_STK_SIZE];  /*定义两个任务的堆栈数组*/

#define ON 0
#define OFF 1

#define LED0(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_1)  /*宏定义两个选择函数*/


void GPIO_configuration(void)						      //配置I/O口
{		
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*开启GPIOA的外设时钟*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  /*选择要控制的GPIOA引脚*/	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      /*设置引脚模式为通用推挽输出*/  		
														  
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);                /*调用库函数，初始化GPIOA*/	 

	/* 关闭led0灯	*/
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	 
}

void BSP_Init(void)	       //硬件配置
{  
    GPIO_configuration();  /* GPIO端口初始化 */
}



void SysTick_init(void)	       /*  SysTick_init   配置SysTick定时器  */
{
    SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);   //初始化并使能SysTick定时器
}


void Task_LED0(void *p_arg)
{
    (void)p_arg;					 	// 'p_arg' 并没有用到，防止编译器提示警告

    SysTick_init();                    	//在第一个任务中开启系统时钟
              	
    while (1)
    {
        LED0( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED0( OFF);
		OSTimeDlyHMSM(0, 0,0,100);       
    }
}
void Task_LED1(void *p_arg)
{
	(void)p_arg;					 	// 'p_arg' 并没有用到，防止编译器提示警告

	while (1)
	{
		LED1( ON );
		OSTimeDlyHMSM(0, 0,0,500);
		LED1( OFF);
		OSTimeDlyHMSM(0, 0,0,500);       
	}
}



int main(void)
{
	BSP_Init();

	OSInit();
	OSTaskCreate(Task_LED0,(void *)0, &LED0_TASK_STK[TASK_STK_SIZE-1], 4);
	OSTaskCreate(Task_LED1,(void *)0, &LED1_TASK_STK[TASK_STK_SIZE-1], 5);
	OSStart();
	return 0;
}













