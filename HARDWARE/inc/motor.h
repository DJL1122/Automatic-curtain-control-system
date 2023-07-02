#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define PWMA   TIM1->CCR1  //PA8

#define BIN1   PBout(13)
#define BIN2   PBout(12)

void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
void pwm_motor(TIM_TypeDef* TIMx,int Compare1);
void on(void);
void off(void);
void stop(void);
#endif
