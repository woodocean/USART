#ifndef	__MY_USART_H__
#define __MY_USART_H__

#include "ti_msp_dl_config.h"


//曲线控件宽度、高度、网格宽度
#define Curve_WIDTH	301
#define Curve_HIGHT	201
#define GDW					30
#define BAUD	38400	//波特率

//定义控件名 如s0.id，可复用，易读便于维护
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
//插值函数
void v_LineInsert(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int POINTS);
void v_InsertZeros(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int gdw,int POINTS);
//绘图刷屏函数
void v_Draw_Refresh(uint8_t pos_y[],int WIDTH);
//绘图平推函数
void v_Draw_Usart_addpoint(int pos_y);
void v_Draw_Push(uint8_t pos_y[],int WIDTH);
//控件赋值函数--打印浮点数
void v_Send_float(float data,char ID[],int digi_place);
//控件赋值函数--打印文字
void v_Send_Txt(char txt[], char ID[]);



#endif
