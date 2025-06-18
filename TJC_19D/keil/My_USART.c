#include "My_USART.h"


//输入：纵轴位置
//功能：在串口屏中加入位置pos_y的点，效果就是波形逐渐向右平移
//说明：需要将串口屏与串口1相连
void v_Draw_Usart_addpoint(int pos_y){

		char str1[10];
		itoa(pos_y,str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(1);
}

//输入：纵轴位置数组首地址，串口屏曲线宽度
//功能：平推式绘制曲线
//不足：绘制时占用CPU，可以考虑用DMA改进
void v_Draw_Push(uint8_t pos_y[],int WIDTH){
	int i,j;
	char str1[10];
	for(i=0;i<WIDTH;i++){
		itoa(pos_y[i],str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(5);																			//控制平推速度
	}
}


//输入：纵轴位置数组首地址，串口屏曲线宽度（数组大小）
//功能：透传数组，实现刷屏显示
//说明：传进来的数组应先事先插值，与串口屏宽度匹配
void v_Draw_Refresh(uint8_t pos_y[],int WIDTH){
	char str1[10];
	int i;
	
	//发送透传请求指令
	itoa(WIDTH,str1);
	uart1_send_string("addt s0.id,0,");
	uart1_send_string(str1);
	uart1_send_string("\xff\xff\xff");
	
	//等待适量时间给串口屏处理透传请求
	delay_ms(10);

	//透传
	for(i=0;i<WIDTH;i++){
		uart1_send_char(pos_y[i]);
	}
	
	//确保结束，以免影响下一个指令
	uart1_send_string("\x01\xff\xff\xff");
	//延时5ms给串口屏处理刷屏，以免影响下一个指令
	delay_ms(5);
}

//线性插值函数
//输入：待插值数组指针，插值后数组（待作图）指针，控件宽度，控件网格宽度，待插值数组大小
//效果：线性插值
//注意：需要控件宽度为数据数组大小的整数倍为佳
void v_LineInsert(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int POINTS){
	int i,j;
	char str1[10];
	char dis;
	float unit;
	int gdw = WIDTH /POINTS;
	for(i=0;i<POINTS -1;i++){
		pos_y_inserted[i*gdw] = pos_y[i];
		
		dis =	pos_y[i+1] - pos_y[i];
		unit = (float)dis / (float)(gdw - 1);
		for(j=1;j<gdw;j++){
			pos_y_inserted[i*gdw + j] = (uint8_t)(pos_y[i] + unit * j);
		}
	}
	
	pos_y_inserted[(POINTS -1)*gdw] = pos_y[POINTS -1];
}

//插零函数
//输入：待插值数组指针，插值后数组（待作图）指针，控件宽度，控件网格宽度，待插值数组大小
//效果：按网格插零,为显示频谱曲线服务
//注意：需要控件宽度为网格宽度整数倍最佳
void v_InsertZeros(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int gdw,int POINTS){
	int i,j;
	char str1[10];
	for(i=0;i<POINTS -1;i++){
		pos_y_inserted[i*gdw] = pos_y[i];
		
		for(j=1;j<gdw;j++){
			pos_y_inserted[i*gdw + j] = 0;
		}
	}
	
	pos_y_inserted[(POINTS -1)*gdw] = pos_y[POINTS -1];
}

//输入浮点数和期待显示的控件ID、期望输出的文本
//注：目前只能写英文，uart1_send_string识别不了中文，可能要printf重定向？还是什么别的
void v_Send_Txt(char txt[], char ID[]){
		uart1_send_string(ID);
		uart1_send_string(".txt=\"");
		uart1_send_string(txt);
		uart1_send_string(" \"\xff\xff\xff");
}

//输入浮点数和期待显示的控件ID、小数位数
//注意:此处的小数位数需配合串口屏提前设置
void v_Send_float(float data,char ID[],int digi_place){
	char i;
	int vir_float;
	char str[10];
	for(i=0;i<digi_place;i++){
		data = data * 10;
	}
	
	vir_float = (int)data;
	itoa(vir_float,str);
	
	uart1_send_string(ID);
	uart1_send_string(".val=");
	uart1_send_string(str);
	uart1_send_string("\xff\xff\xff");
	
}


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


//搭配滴答定时器实现的精确us延时
void delay_us(unsigned long __us) 
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;

    // 计算需要的时钟数 = 延迟微秒数 * 每微秒的时钟数
    ticks = __us * (32000000 / 1000000);

    // 获取当前的SysTick值
    told = SysTick->VAL;

    while (1)
    {
        // 重复刷新获取当前的SysTick值
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // 如果达到了需要的时钟数，就退出循环
            if (tcnt >= ticks)
                break;
        }
    }
}
//搭配滴答定时器实现的精确ms延时
void delay_ms(unsigned long ms) 
{
	delay_us( ms * 1000 );
}

