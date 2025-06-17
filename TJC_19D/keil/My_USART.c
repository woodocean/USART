#include "My_USART.h"

//输入：int输入数字，char[]字符串首地址
//功能：将数字转为字符串
void itoa(int num,char str[] )
	{
	int sign = num,i = 0,j = 0;
	char temp[11];
		
	if(sign == 0){
		str[j] = '0';
		str[j+1] = '\0';
	}
	else{
		if(sign<0){
		num = -num;
	}
	do
	{
	temp[i] = num%10+'0';       
	num/=10;
	i++;
	}while(num>0);
	if(sign<0)
	{
	temp[i++] = '-';
	}
	temp[i] = '\0';
	i--;
	while(i>=0)
	{
	str[j] = temp[i];
	j++;
	i--;
	}
	str[j] = '\0';
	}		
}

//输入：纵轴位置
//功能：每间隔10ms在串口屏中加入位置pos_y的点，效果就是波形逐渐向右平移
//说明：需要将串口屏与串口1相连，以及结合定时器tic的10ms延时
void v_Draw_Usart_addpoint(int pos_y){

		char str1[10];
		itoa(pos_y,str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(1);
}

//输入：纵轴位置数组首地址，串口屏曲线宽度（数组大小）
//功能：绘制频谱图，根据当前串口屏设计的格数POINTS，对每个横坐标绘制频谱线
//说明：需结合串口屏横轴像素WIDTH以及格子数POINTS，同时外部传入数组大小也应为格子数
//备注：M0内存有限，数组大小不能过大，否则溢出不能绘制，试了数组大小为479，不行；
//		若想画曲线图而非谱线图，需结合插值公式
void v_Draw_LINES(uint8_t pos_y[],int WIDTH,int POINTS){
	int i,j;
	char str1[10];
	char gap = WIDTH / (POINTS-1) ;
	for(i=0;i<POINTS;i++){
		itoa(pos_y[i],str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		for(j=0;j<gap-1;j++){
			itoa(0,str1);
			uart1_send_string("add s0.id,0,");
			uart1_send_string(str1);
			uart1_send_string("\xff\xff\xff");
		}
	}
}


void v_Draw_Refresh(uint8_t pos_y[],int WIDTH){
	char str1[10];
	int i;
	
	//发送透传请求指令
	itoa(WIDTH,str1);
	uart1_send_string("addt s0.id,0,");
	uart1_send_string(str1);
	uart1_send_string("\xff\xff\xff");
	
	//等待适量时间
	delay_ms(10);

	//透传
	for(i=0;i<WIDTH;i++){
		uart1_send_char(pos_y[i]);
	}
	
	//确保结束，以免影响下一个指令
	uart1_send_string("\x01\xff\xff\xff");
}

//线性插值
void v_Draw_Curve(uint8_t pos_y[],int WIDTH,int POINTS){
	int i,j;
	char str1[10];
	char gap = WIDTH / (POINTS-1) ;
	char dis;
	float unit;
	for(i=0;i<POINTS;i++){
		itoa(pos_y[i],str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		
		if(i < POINTS-1){
			dis = pos_y[i+1] - pos_y[i];
		}else if(i == POINTS -1){
			dis = pos_y[i]-pos_y[i-1];
		}
		
		
		for(j=0;j<gap-1;j++){
			unit = (float)(dis)/(float)(gap) * j;
			itoa((char)(unit)+pos_y[i],str1);
			uart1_send_string("add s0.id,0,");
			uart1_send_string(str1);
			uart1_send_string("\xff\xff\xff");
		}
	}
}


//输入浮点数和期待显示的控件ID、小数位数
//注意:此处的小数位数需配合串口屏提前设置
void v_Send_float(float data,int ID,int digi_place){
	char i;
	int vir_float;
	char str[10];
	char str_id[10];
	for(i=0;i<digi_place;i++){
		data = data * 10;
	}
	
	vir_float = (int)data;
	itoa(vir_float,str);
	itoa(ID,str_id);
	
	uart1_send_string("x");
	uart1_send_string(str_id);
	uart1_send_string(".val=");
	uart1_send_string(str);
	uart1_send_string("\xff\xff\xff");
	
}

//串口1发送单个字符
void uart1_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while( DL_UART_isBusy(UART_1_INST) == true );
    //发送单个字符
    DL_UART_Main_transmitData(UART_1_INST, ch);
}

//串口发送字符串
void uart1_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        uart1_send_char(*str++);
    }
}

