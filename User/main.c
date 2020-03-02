/*ʵ��                                          

 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "ucos_ii.h"

#define	TASK_STK_SIZE	 64	      //�����ջ����


OS_STK LED0_TASK_STK[TASK_STK_SIZE];
OS_STK LED1_TASK_STK[TASK_STK_SIZE];  /*������������Ķ�ջ����*/

#define ON 0
#define OFF 1

#define LED0(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_1)  /*�궨������ѡ����*/


void GPIO_configuration(void)						      //����I/O��
{		
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*����GPIOA������ʱ��*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  /*ѡ��Ҫ���Ƶ�GPIOA����*/	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      /*��������ģʽΪͨ���������*/  		
														  
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);                /*���ÿ⺯������ʼ��GPIOA*/	 

	/* �ر�led0��	*/
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	 
}

void BSP_Init(void)	       //Ӳ������
{  
    GPIO_configuration();  /* GPIO�˿ڳ�ʼ�� */
}



void SysTick_init(void)	       /*  SysTick_init   ����SysTick��ʱ��  */
{
    SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);   //��ʼ����ʹ��SysTick��ʱ��
}


void Task_LED0(void *p_arg)
{
    (void)p_arg;					 	// 'p_arg' ��û���õ�����ֹ��������ʾ����

    SysTick_init();                    	//�ڵ�һ�������п���ϵͳʱ��
              	
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
	(void)p_arg;					 	// 'p_arg' ��û���õ�����ֹ��������ʾ����

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













