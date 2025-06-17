#include "My_USART.h"

//���룺int�������֣�char[]�ַ����׵�ַ
//���ܣ�������תΪ�ַ���
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

//���룺����λ��
//���ܣ�ÿ���10ms�ڴ������м���λ��pos_y�ĵ㣬Ч�����ǲ���������ƽ��
//˵������Ҫ���������봮��1�������Լ���϶�ʱ��tic��10ms��ʱ
void v_Draw_Usart_addpoint(int pos_y){

		char str1[10];
		itoa(pos_y,str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(1);
}

//���룺����λ�������׵�ַ�����������߿�ȣ������С��
//���ܣ�����Ƶ��ͼ�����ݵ�ǰ��������Ƶĸ���POINTS����ÿ�����������Ƶ����
//˵�������ϴ�������������WIDTH�Լ�������POINTS��ͬʱ�ⲿ���������СҲӦΪ������
//��ע��M0�ڴ����ޣ������С���ܹ��󣬷���������ܻ��ƣ����������СΪ479�����У�
//		���뻭����ͼ��������ͼ�����ϲ�ֵ��ʽ
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
	
	//����͸������ָ��
	itoa(WIDTH,str1);
	uart1_send_string("addt s0.id,0,");
	uart1_send_string(str1);
	uart1_send_string("\xff\xff\xff");
	
	//�ȴ�����ʱ��
	delay_ms(10);

	//͸��
	for(i=0;i<WIDTH;i++){
		uart1_send_char(pos_y[i]);
	}
	
	//ȷ������������Ӱ����һ��ָ��
	uart1_send_string("\x01\xff\xff\xff");
}

//���Բ�ֵ
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


//���븡�������ڴ���ʾ�Ŀؼ�ID��С��λ��
//ע��:�˴���С��λ������ϴ�������ǰ����
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

//����1���͵����ַ�
void uart1_send_char(char ch)
{
    //������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
    while( DL_UART_isBusy(UART_1_INST) == true );
    //���͵����ַ�
    DL_UART_Main_transmitData(UART_1_INST, ch);
}

//���ڷ����ַ���
void uart1_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart1_send_char(*str++);
    }
}

