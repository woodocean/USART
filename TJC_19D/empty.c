#include "ti_msp_dl_config.h"
#include "board.h"
#include "My_USART.h"
#include "stdio.h"
#include <stdlib.h>

#define POINTS	301

volatile bool flag = 1;
uint8_t data[POINTS];

int main(void)
{
	SYSCFG_DL_init();
		
	int i = 0;
	float k;
	int y;

	float R_in,R_out,Gain;
	for(i=0;i<POINTS;i++){
		k = sin(0.15* i) ;
        data[i] = 70.0 * k+ 100;
	}
	i = 0;
	
	R_in = 23.44;
	R_out = 11.51;
	Gain = 3.42;
	delay_ms(500);
    while (1)
    {
		delay_ms(50);
			
		for(i=0;i<POINTS;i++){
					data[i] = rand()%256;
		}
		
		if(flag){
			v_Draw_Refresh(data,Curve_WIDTH);
			//v_Draw_LINES(data,Curve_WIDTH,POINTS);
			//v_Draw_Curve(data,Curve_WIDTH,POINTS);
		}
		//v_Send_float(R_in,0,2);
		//v_Send_float(R_out,1,2);
		//v_Send_float(Gain,2,1);
		
		//R_in = R_in + 0.15;
		//R_out = R_out + 0.33;
		//Gain = Gain - 1.5;
		//flag = 0;

		
		uart1_send_string("t0.txt=\"C1 open\"\xff\xff\xff");
    }
}

int cnt;

//滴答定时器的中断服务函数
void SysTick_Handler(void)
{
	if(cnt >= 5000){
		cnt = 0;
		flag = 1;
	} else {
		cnt ++;
	}
}




