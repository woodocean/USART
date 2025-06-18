#ifndef	__MY_USART_H__
#define __MY_USART_H__

#include "ti_msp_dl_config.h"


//���߿ؼ���ȡ��߶ȡ�������
#define Curve_WIDTH	301
#define Curve_HIGHT	201
#define GDW					30
#define BAUD	38400	//������

//����ؼ��� ��s0.id���ɸ��ã��׶�����ά��
#define Curve_1_ID		"s0.id"
#define R_in_ID				"x0"
#define R_out_ID			"x1"
#define Gain_ID				"x2"
#define Situation_ID	"t0"



void uart1_send_char(char ch);
void uart1_send_string(char* str);
void delay_us(unsigned long __us);
void delay_ms(unsigned long ms);
void delay_1us(unsigned long __us);
void delay_1ms(unsigned long ms);

void itoa(int num,char str[] );
//��ֵ����
void v_LineInsert(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int POINTS);
void v_InsertZeros(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int gdw,int POINTS);
//��ͼˢ������
void v_Draw_Refresh(uint8_t pos_y[],int WIDTH);
//��ͼƽ�ƺ���
void v_Draw_Usart_addpoint(int pos_y);
void v_Draw_Push(uint8_t pos_y[],int WIDTH);
//�ؼ���ֵ����--��ӡ������
void v_Send_float(float data,char ID[],int digi_place);
//�ؼ���ֵ����--��ӡ����
void v_Send_Txt(char txt[], char ID[]);



#endif
