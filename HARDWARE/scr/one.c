#include "one.h"
#include "motor.h"
#include "led.h"
#include "delay.h"
void CON(){
   if(state[4]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
		state[4]='X';
	}
	if(state[3]=='O'){
		led1_on(); 		//LED1开启
		pwm_motor(TIM1,50);
		delay_ms(6000);
		pwm_motor(TIM1,0);
		state[3]='X';
	}
	if(state[2]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(9000);
		pwm_motor(TIM1,0);
		state[2]='X';
	}
	if (state[1]=='O'){
	  led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(13000);
		pwm_motor(TIM1,0);
		state[1]='X';
		
	}
}

void COFF(){
	if(state[2]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
		state[2]='X';
	}
	if(state[3]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		delay_ms(6000);
		pwm_motor(TIM1,0);
		state[3]='X';
	}
	if(state[4]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		delay_ms(9000);
		pwm_motor(TIM1,0);
		state[4]='X';
	}
	if(state[0]=='O')
		pwm_motor(TIM1,-50);
		delay_ms(13000);
		pwm_motor(TIM1,0);
	  state[0]='X';
		
	}

void CONE(){
	
	if(state[4]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		delay_ms(6000);
		pwm_motor(TIM1,0);
		state[4]='X';
	}
	if(state[3]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
		state[3]='X';
	}
	if(state[1]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
		state[1]='X';
	}
	if(state[0]=='O')
		pwm_motor(TIM1,-50);
		delay_ms(9000);
		pwm_motor(TIM1,0);
	  state[0]='X';
		

}
void CTWO(){
	if(state[0]=='O'){
		  led1_on();  	    //LED1开启
		  pwm_motor(TIM1,-50);
		  delay_ms(6200);
		  pwm_motor(TIM1,0);
		  state[0]='X';
	}
	if(state[1]=='O'){
		  led1_on();  	    //LED1开启
		  pwm_motor(TIM1,50);
		  delay_ms(6200);
		  pwm_motor(TIM1,0);
		  state[1]='X';
	}
	if(state[2]=='O'){
		  led1_on();  	    //LED1开启
		  pwm_motor(TIM1,50);
		  delay_ms(3000);
		  pwm_motor(TIM1,0);
		  state[2]='X';
	}
	if(state[4]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,-50);
		  delay_ms(3000);
		  pwm_motor(TIM1,0);
		  state[4]='X';
		
	}
	
}
void CTHREE(){

	if(state[3]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
		state[3]='X';
	}
	if(state[2]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(6000);
		pwm_motor(TIM1,0);
		state[4]='X';
	}
	if(state[1]=='O'){
		led1_on();  	    //LED1开启
		pwm_motor(TIM1,50);
		delay_ms(9000);
		pwm_motor(TIM1,0);
		state[1]='X';
	}
	if(state[0]=='O')
		pwm_motor(TIM1,-50);
		delay_ms(3000);
		pwm_motor(TIM1,0);
	  state[0]='X';
		
}

	

