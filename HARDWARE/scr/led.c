 
/*-------------------------------------------------*/
/*         	                                       */
/*           		 LED控制           	 		   */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PD2  LED1
// PA8  LED2
#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "delay.h"

/*-------------------------------------------------*/
/*函数名：初始化LED1和LED2函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void led_init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); 				//PD2 输出高 
	 
}

/*-------------------------------------------------*/
/*函数名：LED1开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void led1_on(void)
{			
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); 			    //PD2 输出低
} 

/*-------------------------------------------------*/
/*函数名：LED1关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void led1_off(void)
{		
	GPIO_SetBits(GPIOC, GPIO_Pin_13); 				//PD2 输出高
}

/*-------------------------------------------------*/
/*函数名：LED1状态                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
int led1_state(void)
{		
	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)) 
	{
		return 0;
	} else {
		return 1;
	}
}


