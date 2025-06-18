#include "ti_msp_dl_config.h"
#include "My_USART.h"
#include "stdio.h"
#include <stdlib.h>


#define DATA_POINT 11					//����ڵ���

volatile bool refresh = true;	//����ˢ����Ļʱ��
volatile uint16_t cnt_refresh;
uint8_t data[Curve_WIDTH];		//��������
uint8_t Freq[Curve_WIDTH];		//ģ����ɢƵ������

int main(void)
{
	SYSCFG_DL_init();
		
	int i = 0;
	double k;
	float R_in,R_out,Gain;
	
	//����������ʼ��
	R_in = 23.44;
	R_out = 11.51;
	Gain = 3.42;
	
	for(i=0;i<Curve_WIDTH;i++){
		k = sin(0.15* i) ;
    data[i] = 70.0 * k + 100;
		Freq[i] = 50.0 * k + 60;
	}
	
    while (1)
    {
					
				if(true == refresh){
					

				
					v_Draw_Refresh(data,Curve_WIDTH);											//ˢ����������
					
					//v_Draw_Push(data,Curve_WIDTH);												//ƽ�ƻ�������
					
					/*v_InsertZeros(Freq,data,Curve_WIDTH,GDW,DATA_POINT);		//��ֵ��Ƶ��ͼ
					v_Draw_Refresh(data,Curve_WIDTH);				*/						

					
					v_Send_float(R_in,R_in_ID,2);											//�������ؼ���ֵ
					v_Send_float(R_out,R_out_ID,2);
					v_Send_float(Gain,Gain_ID,1);
					
					v_Send_Txt("C1 open",Situation_ID);		//�ı��ؼ���ֵ
					
					R_in 	= R_in	+ 3.0;
					R_out = R_out	+ 2.0;
					Gain 	= Gain 	- 1.0;
					
					uint8_t temp = rand()%256;
					for(i=0;i<Curve_WIDTH;i++){
						k = sin(0.15* i) ;
						data[i] = 70.0 * k + temp;
						Freq[i] = 50.0 * k + temp;
					}
					
					refresh = false;
				}
				
		}
			
}


//�δ�ʱ�����жϷ�����
void SysTick_Handler(void)
{
	if(cnt_refresh >= 1000){	//ˢ��ʱ��-��λms
		cnt_refresh = 0;
		refresh = true;
	} else {
		cnt_refresh ++;
	}
}

