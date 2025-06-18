#include "My_USART.h"


//���룺����λ��
//���ܣ��ڴ������м���λ��pos_y�ĵ㣬Ч�����ǲ���������ƽ��
//˵������Ҫ���������봮��1����
void v_Draw_Usart_addpoint(int pos_y){

		char str1[10];
		itoa(pos_y,str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(1);
}

//���룺����λ�������׵�ַ�����������߿��
//���ܣ�ƽ��ʽ��������
//���㣺����ʱռ��CPU�����Կ�����DMA�Ľ�
void v_Draw_Push(uint8_t pos_y[],int WIDTH){
	int i,j;
	char str1[10];
	for(i=0;i<WIDTH;i++){
		itoa(pos_y[i],str1);
		uart1_send_string("add s0.id,0,");
		uart1_send_string(str1);
		uart1_send_string("\xff\xff\xff");
		delay_ms(5);																			//����ƽ���ٶ�
	}
}


//���룺����λ�������׵�ַ�����������߿�ȣ������С��
//���ܣ�͸�����飬ʵ��ˢ����ʾ
//˵����������������Ӧ�����Ȳ�ֵ���봮�������ƥ��
void v_Draw_Refresh(uint8_t pos_y[],int WIDTH){
	char str1[10];
	int i;
	
	//����͸������ָ��
	itoa(WIDTH,str1);
	uart1_send_string("addt s0.id,0,");
	uart1_send_string(str1);
	uart1_send_string("\xff\xff\xff");
	
	//�ȴ�����ʱ�������������͸������
	delay_ms(10);

	//͸��
	for(i=0;i<WIDTH;i++){
		uart1_send_char(pos_y[i]);
	}
	
	//ȷ������������Ӱ����һ��ָ��
	uart1_send_string("\x01\xff\xff\xff");
	//��ʱ5ms������������ˢ��������Ӱ����һ��ָ��
	delay_ms(5);
}

//���Բ�ֵ����
//���룺����ֵ����ָ�룬��ֵ�����飨����ͼ��ָ�룬�ؼ���ȣ��ؼ������ȣ�����ֵ�����С
//Ч�������Բ�ֵ
//ע�⣺��Ҫ�ؼ����Ϊ���������С��������Ϊ��
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

//���㺯��
//���룺����ֵ����ָ�룬��ֵ�����飨����ͼ��ָ�룬�ؼ���ȣ��ؼ������ȣ�����ֵ�����С
//Ч�������������,Ϊ��ʾƵ�����߷���
//ע�⣺��Ҫ�ؼ����Ϊ���������������
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

//���븡�������ڴ���ʾ�Ŀؼ�ID������������ı�
//ע��Ŀǰֻ��дӢ�ģ�uart1_send_stringʶ�������ģ�����Ҫprintf�ض��򣿻���ʲô���
void v_Send_Txt(char txt[], char ID[]){
		uart1_send_string(ID);
		uart1_send_string(".txt=\"");
		uart1_send_string(txt);
		uart1_send_string(" \"\xff\xff\xff");
}

//���븡�������ڴ���ʾ�Ŀؼ�ID��С��λ��
//ע��:�˴���С��λ������ϴ�������ǰ����
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


//����δ�ʱ��ʵ�ֵľ�ȷus��ʱ
void delay_us(unsigned long __us) 
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;

    // ������Ҫ��ʱ���� = �ӳ�΢���� * ÿ΢���ʱ����
    ticks = __us * (32000000 / 1000000);

    // ��ȡ��ǰ��SysTickֵ
    told = SysTick->VAL;

    while (1)
    {
        // �ظ�ˢ�»�ȡ��ǰ��SysTickֵ
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // ����ﵽ����Ҫ��ʱ���������˳�ѭ��
            if (tcnt >= ticks)
                break;
        }
    }
}
//����δ�ʱ��ʵ�ֵľ�ȷms��ʱ
void delay_ms(unsigned long ms) 
{
	delay_us( ms * 1000 );
}

