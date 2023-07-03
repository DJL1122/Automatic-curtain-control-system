/***************STM32F103C8T6**********************
 * �ļ���  ��usart2
 * Ӳ�����ӣ�------------------------
 *          | PA2  - USART2(Tx)      |
 *          | PA3  - USART2(Rx)      |
 *           ------------------------
********************LIGEN*************************/
#include "usart2.h" 
#include "usart.h"
#include "usart3.h"
#include "string.h"
#include "stdarg.h"	 	  
#include "delay.h"

u8 USART2_RX_BUF[USART2_REC_LEN]; 				//���ջ���,���USART2_REC_LEN���ֽ�.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	  //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
 
u16 USART2_RX_STA=0;   	

void usart2_init(u32 bound)
{
	//GPIO�˿�����
  	GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure;   
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
    USART_DeInit(USART2);  
    /* Configure USART Tx as alternate function push-pull */  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
      
    /* Configure USART Rx as input floating */  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
    USART_InitStructure.USART_BaudRate = bound;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
	USART_Init(USART2, &USART_InitStructure );   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	
//	USART_ITConfig(USART2, USART_IT_PE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_ERR, ENABLE);
	
	USART_Cmd(USART2, ENABLE); 
}

int i;//int ast=0;
void HexToStr(u8 *pbDest, u8 *pbSrc, int nLen)
{
	char	ddl,ddh;
	int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	pbDest[nLen*2] = '\0';
}
u8 sss[23]={0};
u8 saa[24]={0};
void USART2_IRQHandler(void)
{	
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		if(USART2_RX_STA<23)
		{
			res =USART_ReceiveData(USART2);	//��ȡ���յ�������
			USART2_RX_BUF[USART2_RX_STA]=res ;
			//u3_printf("%d",USART2_RX_STA);
			//delay_ms(1);
			USART2_RX_STA++;
			return;			
		}
		//u3_printf("ok=%d\r\n",USART2_RX_STA);
		//delay_ms(100);
		//u3_printf("%s",USART2_RX_BUF);
		//delay_ms(100);
		USART2_RX_STA=0;
//		if(USART2_RX_STA==22)
//		{
//			
//			printf("AT+CIPSEND=0,%d\r\n",23);delay_ms(500);//
		for(i =0;i<23;i++){
			saa[i]=USART2_RX_BUF[i+1];
		}saa[23]=USART2_RX_BUF[0];
		HexToStr(sss, saa, 23);
//			for(i = 1;i<23;i++)
//			{
//				USART_SendData(USART3,USART2_RX_BUF[i]+0x30);
//				delay_ms(1);
//			}USART_SendData(USART3,USART2_RX_BUF[0]+0x30);
		printf("AT+CIPSEND=0,%d\r\n",45);delay_ms(500);//
		printf("%s",sss);

////			u3_printf("kzq:%s\r\n",USART2_RX_BUF);
////			delay_ms(100);
//			//		
			memset(USART2_RX_BUF,0,USART2_REC_LEN);
//			USART2_RX_STA=0;
//		}
		//USART2_RX_STA++;
  }
}
//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...) //...��ʾ�ɱ����������ɱ�������һ���б�������ר�ŵ�ָ��ָ�����������޶����������� 
{  
	u16 i,j; 
	va_list ap; //��ʼ��ָ��ɱ�����б��ָ��
	va_start(ap,fmt); //����һ���ɱ�����ĵ�ַ����ap����apָ��ɱ�����б�Ŀ�ʼ
	vsprintf((char*)USART2_TX_BUF,fmt,ap); //������fmt��apָ��Ŀɱ����һ��ת���ɸ�ʽ���ַ�������(char*)USART2_TX_BUF�����У�������ͬsprintf������ֻ�ǲ������Ͳ�ͬ
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(USART2,USART2_TX_BUF[j]);  //�Ѹ�ʽ���ַ����ӿ����崮���ͳ�ȥ 
	}
}

