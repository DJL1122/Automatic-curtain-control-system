/*******************************************************************
*��    �ߣ������˷��ȥ
*��    ����V1.0
*����ʱ�䣺2020��  4��  30��
********************************************************************/
#include "pid.h"
float	Position_KP=6,Position_KI=0.6,Position_KD=2;  //λ�ÿ���PID����
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ�����Encoder����������λ����Ϣ��TargetĿ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ��
e(k-1)������һ�ε�ƫ��
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
float Position_PID1 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}


