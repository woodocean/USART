# 串口屏函数库

该工程用19D题目的显示模块为例子，包含曲线绘图、整型数浮点数显示刷新、文字显示刷新等函数。

## 函数接口

1. 波形绘图——用add指令，波形往前推进

   ```
   //输入：纵轴位置数组首地址，串口屏曲线宽度
   //功能：平推式绘制一次曲线
   //不足：绘制时占用CPU，可以考虑用DMA改进
   void v_Draw_Push(uint8_t pos_y[],int WIDTH);
   ```

2. 波形绘图——用addt透传指令，波形会直接刷新(类似于OLED刷屏)

   ```
   //输入：纵轴位置数组首地址，串口屏曲线宽度（数组大小）
   //功能：透传数组，实现刷屏显示
   //说明：传进来的数组应先事先插值，与串口屏宽度匹配
   void v_Draw_Refresh(uint8_t pos_y[],int WIDTH)
   ```

3. 数据、文字控件赋值

   ```
   //控件赋值函数--打印浮点数
   void v_Send_float(float data,char ID[],int digi_place);
   //控件赋值函数--打印文字
   void v_Send_Txt(char txt[], char ID[]);
   
   //定义控件名 如s0.id，可复用，易读便于维护
   #define Curve_1_ID		"s0.id"
   #define R_in_ID			"x0"
   #define R_out_ID		"x1"
   #define Gain_ID			"x2"
   #define Situation_ID	"t0"
   ```

4. 插值函数——曲线匹配宽度、频谱插零

   ```
   //线性插值函数
   //输入：待插值数组指针，插值后数组（待作图）指针，控件宽度，控件网格宽度，待插值数组大小
   //效果：线性插值
   //注意：需要控件宽度为数据数组大小的整数倍为佳
   void v_LineInsert(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int POINTS);
   
   //插零函数
   //输入：待插值数组指针，插值后数组（待作图）指针，控件宽度，控件网格宽度，待插值数组大小
   //效果：按网格插零,为显示频谱曲线服务
   //注意：需要控件宽度为网格宽度整数倍最佳
   void v_InsertZeros(uint8_t pos_y[],uint8_t pos_y_inserted [],int WIDTH,int gdw,int POINTS)
   ```

   

## 使用指南

1. 需配合串口屏使用
   1. 波特率需一致 38400
   2. 控件号需一致
   3. 浮点数控件的小数点位数要提前设置，且保持一致
   4. 绘制频谱图需要插零，其横轴节点数N应该与需要绘制的频谱数组大小M相等

2. 单片机配置
   1. 延时函数使用了滴答定时器，需要配合滴答定时器中断
   2. 使用串口1，引脚为PA17(TX）和PA18（RX）

3. 注意

   1. 数据透传点数过多，串口屏需要时间处理，所以当刷屏完后，应当间隔5毫秒让串口屏处理，否则后续指令无法正常执行

      > 函数中已加延时




