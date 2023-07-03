/*------------------------------------------------------*/
/*                                                      */
/*            ����main��������ں���Դ�ļ�               */
/*                                                      */
/*------------------------------------------------------*/

#include <stdlib.h>
#include "sys.h"
#include "delay.h"	     //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "usart3.h"
#include "timer.h"
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�

#include "FreeRTOS.h"	 //FreeRTOS����ͷ�ļ�
#include "semphr.h" 	 //�ź���
#include "queue.h"		 //����
#include "event_groups.h"//�¼���־��

#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "control.h"     //������Ҫ��ͷ�ļ� ����ģ��������ݷ��͸�������
#include "led.h"	     //������Ҫ��ͷ�ļ� LED
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "one.h"
/*-------------------------------------------------------------*/
/*          	WIFI������ONENET���ã����ã�			      	   */
/*-------------------------------------------------------------*/
const char SSID[] 			 = "redmi";           //·��������
const char PASS[] 			 = "123456789";     //·��������

const char PRODUCTID[] 	     = "425635";  	    //��ƷID
const char DEVICEID []	     = "717066523";     //�豸ID  
const char AUTHENTICATION[]  = "123456";        //��Ȩ��Ϣ  
const char SERVER_IP[]	     = "183.230.40.39"; //��ŷ�����IP�����������øģ�
const int  SERVER_PORT 		 = 6002;		    //��ŷ������Ķ˿ںţ����øģ�

/*-------------------------------------------------------------*/
/*          			topic���⣨���ã�			      	   */
/*-------------------------------------------------------------*/
const char APP_TOPIC[]    = "control";   //Ҫ����app������tpoic
const char LOCAL_TOPIC[]  = "state"; //�������ؿ���ģ��״̬topic
/*-------------------------------------------------------------*/
/*          ���������Լ�����ģ���ʼ״̬���ã����ã�		   	   */
/*-------------------------------------------------------------*/

/* state[0]:LED1״̬��O(������)Ϊ������XΪ�ر�

 */
extern char state[5] = "XOXXX";			  		//�����͵��豸״̬����
const char *CMD_APP_GET_STATE  = "STATUS";  //APP���͵��豸״̬��ȡ����

const char *CMD_LED1ON  = "ON";   		
const char *CMD_LED1OFF = "OFF";  		
const char *CMD_one  = "25";   		
const char *CMD_two= "50";  
const char *CMD_three  = "75";   		

/*-------------------------------------------------------------*/
/*               freerto����ͨ�ſ��ƣ��̶���			      	   */
/*-------------------------------------------------------------*/

/*	��ֵ�ź������                         
 *	���ã����ڿ���MQTT����崦��������MQTT���ݽ��շ��ͻ��崦�������з���
 *		  ����������յ���������ʱ�������ź���		 
 */
SemaphoreHandle_t BinarySemaphore;
	
/*	�¼���־��                         
 *	���ã���־WIFI���ӣ�PING����������ģʽ����wifi�Ƿ��������ӣ��Ƿ������ݣ��������Ƿ����� 
 *  ���壺1.�¼���־��λ1Ϊ0��λ0Ϊ1ʱ����0x03��0000 0001����wifi������������ʱλ0��λ1����ʱconnect���Ļ�δ���͡� 
 *		  2.�¼���־��λ1Ϊ1��λ0Ϊ1ʱ����0x03��0000 0011����connect���ķ��ͣ��������ӳɹ�����ʱλ1��λ1��PING��
 *			��������30s����ģʽ�������������������ݿ�ʼ�ϴ����豸Զ�̿��ƣ�LED���ƣ����ܿ����� 
 */
EventGroupHandle_t Event_Handle = NULL;      //�¼���־�飨λ0��WIFI����״̬ λ1��PING������2S���ٷ���ģʽ��
const int WIFI_CONECT = (0x01 << 0);         //�����¼������λ 0������������ģʽ��ֵ1��ʾ�Ѿ����ӣ�0��ʾδ����
const int PING_MODE   = (0x01 << 1);         //�����¼������λ 1��PING����������ģʽ��1��ʾ����30S����ģʽ��0��ʾδ�������ͻ���2S���ٷ���ģʽ

/*	���������ݷ�����Ϣ����                         
 *	���ã��������������ݷ��͵���������Ϣ����  
 */
QueueHandle_t Message_Queue;		 		 //��Ϣ���о��  
const UBaseType_t MESSAGE_DATA_TX_NUM = 10;	 //��Ϣ���������Ϣ��Ŀ  
const UBaseType_t MESSAGE_DATA_TX_LEN = 30;  //��Ϣ���е�Ԫ��С����λΪ�ֽ�  

/*-------------------------------------------------------------*/
/*               ������������������1�����ã�		      	   */
/*-------------------------------------------------------------*/
//��ʼ����
TaskHandle_t StartTask_Handler;
void my_start_task(void *pvParameters);
//MQTT����崦������
TaskHandle_t MQTT_Cmd_Task_Handler;
void my_mqtt_buffer_cmd_task(void *pvParameters);

//�����ջ��С��������
//TaskHandle_t STACK_Task_Handler;
//void stack_task(void *pvParameters);

/*-------------------------------------------------------------*/
/*               ������������������2���̶���		      	   */
/*-------------------------------------------------------------*/
//WIFI����
TaskHandle_t WIFI_Task_Handler;
void wifi_task(void *pvParameters);
//MQTT���ݽ��շ��ͻ��崦������
TaskHandle_t MQTT_RxTx_Task_Handler;
void mqtt_buffer_rx_tx_task(void *pvParameters);

TaskHandle_t vPC_Communication_Task_Handler;
void vPC_Communication_Task(void *pvParameters);


/*---------------------------------------------------------------*/
/*��������int main()                                             */
/*��  �ܣ�������							                         */
/*		  1.��ʼ��������ģ��  				     				 */
/*		  2.������ʼ�����ڿ�ʼ�����ﴴ��������������           */
/*		  3.�����������				       			 		     */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*---------------------------------------------------------------*/
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init();	       //��ʱ������ʼ��
	usart1_init(115200);   //����1���ܳ�ʼ����������115200���봮������ͨ��		
	usart2_init(115200);   //����2���ܳ�ʼ����������115200��wifiͨ��	
	usart3_init();         ////����3���ܳ�ʼ����������9600������ͨ��	
	tim4_init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms	
	led_init();		  	   //��ʼ��LED
	MiniBalance_PWM_Init(99,71);
  MiniBalance_Motor_Init();
	Encoder_Init_TIM2();
	TIM1_Int_Init(99,3599);
 
	
	wifi_reset_io_init();  //��ʼ��esp8266
	IoT_parameter_init();  //��ʼ��OneNETƽ̨MQTT�������Ĳ���	
	

	
	//������ʼ����
	xTaskCreate((TaskFunction_t	) my_start_task,		//������
			  (const char* 	)"my_start_task",		//��������
				(uint16_t 		) 128,				  	//�����ջ��С
				(void* 		  	) NULL,				 	//���ݸ��������Ĳ���
				(UBaseType_t 	) 1, 				  	//�������ȼ�
				(TaskHandle_t*  ) &StartTask_Handler);	//������ƿ� 
			
	vTaskStartScheduler();  							//�����������
}

/*---------------------------------------------------------------*/
/*��������void my_start_task(void *pvParameters)                 */
/*��  �ܣ���ʼ�������ã�							             */
/*		  1.�����ź�������Ϣ���е�����ͨ�ŷ�ʽ   				     */
/*		  2.������������       			 						 */
/*		  3.ɾ������       			 		    				 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*---------------------------------------------------------------*/
void my_start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); //�����ٽ���
	
	//������ֵ�ź���
	BinarySemaphore = xSemaphoreCreateBinary();	
	//�¼���־�飬���ڱ�־wifi����״̬�Լ�ping����״̬
	Event_Handle = xEventGroupCreate(); 
	//�����豸��Ϣ����
	Message_Queue = xQueueCreate(MESSAGE_DATA_TX_NUM, MESSAGE_DATA_TX_LEN); 
	
	//���񴴽�����������1.������ 2.�������� 3.�����ջ��С 3.���ݸ��������Ĳ��� 4.�������ȼ�����ֵԽ�����ȼ�Խ�� 8>2�� 5.������ƿ飨�����
	//����WIFI����
    xTaskCreate(wifi_task, 				"wifi_task", 				128, NULL, 8, &WIFI_Task_Handler); 			
	//����MQTT����崦������
    xTaskCreate(my_mqtt_buffer_cmd_task,"my_mqtt_buffer_cmd_task",  128, NULL, 7, &MQTT_Cmd_Task_Handler); 			
	//����MQTT���ݽ��շ��ͻ��崦������
    xTaskCreate(mqtt_buffer_rx_tx_task, "mqtt_buffer_rx_tx_task", 	200, NULL, 6, &MQTT_RxTx_Task_Handler); 
  //��������ͨ��
    xTaskCreate(vPC_Communication_Task,"vPC_Communication_Task",500,NULL,5, &vPC_Communication_Task_Handler);
	
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

xQueueHandle xQueueRx;
void vPC_Communication_Task(void *pvParameters)
	{
		u8 rx=0;
		BaseType_t xHigherPriorityTaskWoken;
		
		xHigherPriorityTaskWoken = pdFALSE;
		
		xQueueRx=xQueueCreate(20,sizeof(u8));
	
		while(1)
		{
			if(xQueueReceiveFromISR(xQueueRx,&rx,&xHigherPriorityTaskWoken))
			{
					printf("%c\r\n",rx);
			}
			vTaskDelay(1000/portTICK_RATE_MS );
		}
 
}

/*---------------------------------------------------------------*/
/*��������void my_mqtt_buffer_cmd_task(void *pvParameters)       */
/*��  �ܣ�MQTT����崦���������ã�							 */
/*		  1.MQTT�������������ִ����Ӧ����     				 */
/*		  2.������ִ�н�����͸�������       			 		 */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ������ȡ����ֵ�ź���ʱִ�У��յ�APP��������ִ�У�    		 */
/*---------------------------------------------------------------*/
void my_mqtt_buffer_cmd_task(void *pvParameters)	
{
	while(1)
	{
		xSemaphoreTake(BinarySemaphore, portMAX_DELAY);	//��ȡ�ź�������ȡ���ź���������ִ�У������������̬���ȴ�ִ��
		while (MQTT_CMDOutPtr != MQTT_CMDInPtr)			//ѭ����������ָ��	
		{                            		       
			printf("����:%s\r\n", &MQTT_CMDOutPtr[2]);              	   
			
//			if(!memcmp(&MQTT_CMDOutPtr[2], CMD_APP_GET_STATE, strlen(CMD_APP_GET_STATE))) //�豸״̬�ж�
//			{    
//				if (led1_state())   //led1״̬
//					state[0] = 'O'; //����
//				else
//					state[0] = 'X';	//�ر�
//			}
			 if(!memcmp(&MQTT_CMDOutPtr[2], CMD_LED1ON, strlen(CMD_LED1ON)))
			{                                              
				led1_on();  	    //LED1����
				state[0]  = 'O';
				CON();
			}
			else if(!memcmp(&MQTT_CMDOutPtr[2], CMD_LED1OFF, strlen(CMD_LED1OFF)))
			{                                           
				led1_off(); 	   //LED1�ر�
				state[1]  = 'O';
				COFF();
			}
			else if(!memcmp(&MQTT_CMDOutPtr[2], CMD_one, strlen(CMD_one)))
			{                                           
				led1_off(); 	   //LED1�ر�
				state[2]  = 'O';
				CONE();
			}
			else if(!memcmp(&MQTT_CMDOutPtr[2], CMD_two, strlen(CMD_two)))
			{                                           
				led1_off(); 	   //LED1�ر�
				state[3]  = 'O';
				CTWO();
				
			}
			else if(!memcmp(&MQTT_CMDOutPtr[2], CMD_three, strlen(CMD_three)))
			{                                           
				led1_off(); 	   //LED1�ر�
				state[4]  = 'O';
				CTHREE();
			}
		
			else printf("δָ֪��\r\n");	
					
			send_device_state(LOCAL_TOPIC, state);  //���͸������������豸״̬��APP���գ�
			printf("devsta:%s\r\n", state);
			
			MQTT_CMDOutPtr += CBUFF_UNIT;			//ָ������
			if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)    //���ָ�뵽������β����
			MQTT_CMDOutPtr = MQTT_CMDBuf[0];        //ָ���λ����������ͷ	
			delay_ms(10); 
		}
		delay_ms(100);	  
	}
}


/*---------------------------------------------------------------*/
/*��������void wifi_task(void *pvParameters)                     */
/*��  �ܣ�WIFI���񣨹̶���										 */
/*		  1.����wifi�Լ��Ʒ�����       							 */
/*		  2.��������        									     */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����1.����������ǰ�رշ���ping���Ķ�ʱ��3������¼���־λ	 */
/*		  2.�����������ӣ��׳��¼���־�������Լ����������̬		 */
/*		  3.����������wifi�ѶϿ�������¼���־������ִ�б��������� */
/*			����	 											 */
/*---------------------------------------------------------------*/
void wifi_task(void *pvParameters)
{
	while(1)
	{ 
		printf("��Ҫ���ӷ�����\r\n");                 
		TIM_Cmd(TIM4, DISABLE);                       //�ر�TIM4 
		TIM_Cmd(TIM3, DISABLE);                       //�ر�TIM3
		xEventGroupClearBits(Event_Handle, PING_MODE);//�رշ���PING���Ķ�ʱ��3������¼���־λ
		WiFi_RxCounter = 0;                           //WiFi������������������                        
		memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);     //���WiFi���ջ����� 
		if(WiFi_Connect_IoTServer() == 0)			  //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
		{   			     
			printf("����TCP���ӳɹ�\r\n");            
			WiFi_RxCounter = 0;                       //WiFi������������������                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE); //���WiFi���ջ����� 
			MQTT_Buff_Init();                         //��ʼ�����ͻ�����  
			
			xEventGroupSetBits(Event_Handle, WIFI_CONECT);  //�����������ӣ��׳��¼���־
			vTaskSuspend(NULL);	    						//�����������ӣ������Լ����������̬�������ɹ���תΪ����̬ʱ�������ִ����ȥ��
			xEventGroupClearBits(Event_Handle, WIFI_CONECT);//����������wifi�ѶϿ�������¼���־������ִ�б������������� 
			xEventGroupClearBits(Event_Handle, PING_MODE);  //�رշ���PING���Ķ�ʱ��3������¼���־λ
		}
		delay_ms(10);	    //��ʱ10ms
	}
}


/*---------------------------------------------------------------*/
/*��������void mqtt_buffer_rx_tx_task(void *pvParameters)        */
/*��  �ܣ�MQTT���շ��ʹ������񣨹̶���							 */
/*		  1.�����ͻ���������       							 */
/*		  2.������ջ��������ݣ������Ը�����ס�ֽ��յ����ݣ������� */
/*		    �������з���������������������				     */
/*��  ������                          			   				 */
/*����ֵ����                                       			     */
/*��  ����1.�����������¼�����ִ�д����񣬷������				 */
/*		  2.���յ�����������ʱ������ֵ�ź���				 		 */
/*		  3.CONNECT���ĳɹ�������30s��PING��ʱ���������¼���־λ	 */
/*		  4.PING���Ŀ��ٷ���ģʽ��2s���յ��ظ�������30s��ping��ʱ	 */
/*			���������¼���־λ			 						 */
/*		  5.CONNECT����ʧ�ܣ�WIFI���ӷ����������ɹ���̬תΪ����̬��*/
/*			��������												 */
/*---------------------------------------------------------------*/
void mqtt_buffer_rx_tx_task(void *pvParameters)
{
	while(1)
	{
		//�����������¼�����ִ�д����񣬷������
		xEventGroupWaitBits((EventGroupHandle_t	)Event_Handle,		
							(EventBits_t		)WIFI_CONECT,
							(BaseType_t			)pdFALSE,				
							(BaseType_t			)pdTRUE,
							(TickType_t			)portMAX_DELAY);
		/*-------------------------------------------------------------*/
		/*                     �����ͻ���������					   */
		/*-------------------------------------------------------------*/
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ�������������
		{                
			//�������ݻ���
			if(MQTT_TxDataOutPtr[2] == 0x30) 
			{	
				printf("��������:0x30\r\n");
			}
			else
			{  
				printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]);
			}
			
			MQTT_TxData(MQTT_TxDataOutPtr);					
			MQTT_TxDataOutPtr += TBUFF_UNIT;				
			if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)		
			{ 
				MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];	
			}			
		}					
		/*-------------------------------------------------------------*/
		/*                     ������ջ���������                       */
		/*-------------------------------------------------------------*/
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
		{		
			printf("���յ�����:");

			//�����һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
			//��ȡ��������
			if((MQTT_RxDataOutPtr[2] == 0x30))
			{ 
				printf("�������ȼ�0����\r\n"); 		   	 //���������Ϣ
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);//����ȼ�0��������
				xSemaphoreGive(BinarySemaphore);	     //������ֵ�ź���������MQTT����崦������ִ��
			}			
			//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
			//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
			else if(MQTT_RxDataOutPtr[2] == 0x20)
			{             			
				switch(MQTT_RxDataOutPtr[5])
				{					   
					case 0x00: printf("CONNECT���ĳɹ�\r\n");				//CONNECT���ĳɹ�					   
							   TIM3_ENABLE_30S();				 			//����30s��PING��ʱ��	
							   xEventGroupSetBits(Event_Handle, PING_MODE); //����30s��PING��ʱ���������¼���־λ
							   break;													                                         
					case 0x01: printf("�����Ѿܾ�����֧�ֵ�Э��汾��w	׼������\r\n");       
							   vTaskResume(WIFI_Task_Handler);				//WIFI���ӷ����������ɹ���̬תΪ����̬����������
							   break;														
					case 0x02: printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");   
							   vTaskResume(WIFI_Task_Handler);              //WIFI���ӷ����������ɹ���̬תΪ����̬����������
							   break; 														
					case 0x03: printf("�����Ѿܾ�������˲����ã�׼������\r\n");	    
							   vTaskResume(WIFI_Task_Handler);				//WIFI���ӷ����������ɹ���̬תΪ����̬����������
							   break;														
					case 0x04: printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");	   
							   vTaskResume(WIFI_Task_Handler);				//WIFI���ӷ����������ɹ���̬תΪ����̬����������					
							   break;														
					case 0x05: printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");				   
							   vTaskResume(WIFI_Task_Handler);				//WIFI���ӷ����������ɹ���̬תΪ����̬����������					
							   break;																
					default  : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");		     
							   vTaskResume(WIFI_Task_Handler);				//WIFI���ӷ����������ɹ���̬תΪ����̬����������			
							   break;																						
				}				
			}
			//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
			else if(MQTT_RxDataOutPtr[2] == 0xD0)
			{ 
				printf("PING���Ļظ�\r\n");                       
				if(pingFlag == 1)
				{                   						     //���pingFlag=1����ʾ��һ�η���
					pingFlag = 0;    				       		 //Ҫ���pingFlag��־
				}
				else if(pingFlag > 1)	
				{ 				 								 //���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
					pingFlag = 0;     				      		 //Ҫ���pingFlag��־
					TIM3_ENABLE_30S(); 				      		 //PING��ʱ���ػ�30s��ʱ��
					xEventGroupSetBits(Event_Handle, PING_MODE); //30s��PING��ʱ���������¼���־λ
				}				
			}	
			//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
			//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
			if(MQTT_RxDataOutPtr[2]==0x90)
			{ 
				switch(MQTT_RxDataOutPtr[6])
				{					
					case 0x00 :
					case 0x01 : printf("���Ŀ��Ʊ��ĳɹ�\r\n");					
								break;                                                                         
					default   : printf("����ʧ��\r\n");   
								break;                                								
				}					
			}			
			
			MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������	
			if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
			{
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
			}		          
		}			
		delay_ms(100);//��ʱ10ms
	}
}

