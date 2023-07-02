/***************STM32F103C8T6**********************
 * 文件名  ：usart3
 * 硬件连接：------------------------
 *          | PB11  - USART3(Tx)      |
 *          | PB10  - USART3(Rx)      |
 *           ------------------------
********************LIGEN*************************/
#include "usart3.h"
#include "led.h"
#include "delay.h"
#include "motor.h"
#include "timer3.h"

#include "FreeRTOS.h"	 //FreeRTOS配置头文件
#include "semphr.h" 	 //信号量
#include "queue.h"		 //队列
#include "event_groups.h"//事件标志组

u16 USART3_RX_STA=0;
void usart3_init()
{
  NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口2时钟使能
 
 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	USART_InitStructure.USART_BaudRate = 9600;//波特率一般设置为115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	2
  
 
	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
//	USART3_RX_STA=0;		//清零
}

u8 res;
void USART3_IRQHandler(void)
{
	xQueueHandle xQueueRx;
	BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)
	{
		res=USART_ReceiveData(USART3);
		xQueueSendToBackFromISR (xQueueRx,&res,&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    if(res=='1'){
			led1_on();
			pwm_motor(TIM1,50);
			
			
		}
		else if(res=='2'){
			led1_off();
			pwm_motor(TIM1,-50);
     
			
		}
		else if(res=='3'){
			pwm_motor(TIM1,0);
			
			
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}

}
