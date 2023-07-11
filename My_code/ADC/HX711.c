
#include "HX711.h"
#include "kalman.h"
uint32_t weight_zheng;
uint8_t weight_xiao;
uint32_t maopi;

uint32_t hx711;
uint16_t a;
//按克重为430个数为准
#define weight_canshu 86

KFP KFP_height={0.02,0,0,0,0.001,0.543};

void maopi_get()
{
    maopi = Read_Weigh();
}

void weight_get()
{
//	//观察窗口
//	OLED_ShowNum(1,1,maopi_zheng,9);
//	static uint8_t i = 0 ;
    hx711 = Read_Weigh();
    if(hx711 >= maopi)
    {
        //观察窗口
//		OLED_ShowNum(3,1,hx711,9);
        hx711 =  hx711 - maopi;

        hx711 = ((uint32_t)((float)hx711/weight_canshu));

        if(((hx711-KFP_height.out)>3)||((hx711-KFP_height.out)<(-3))){KFP_height.out =hx711;}//跳变快速赋值
        hx711 = (uint32_t)(kalmanFilter(&KFP_height, (float)hx711));//一阶滤波+++++

        weight_zheng = hx711;
        weight_xiao = weight_zheng*2%10;
        weight_zheng=weight_zheng*2/10;
    }
}
void user_delaynus_tim(uint32_t nus)
{

    uint16_t  differ = 0xffff-nus-5;
    //设置定时器2的技术初始值
    __HAL_TIM_SetCounter(&htim2,differ);
    //开启定时器
    HAL_TIM_Base_Start(&htim2);

    while( differ<0xffff-5)
    {
        differ = __HAL_TIM_GetCounter(&htim2);
    };
    //关闭定时器
    HAL_TIM_Base_Stop(&htim2);
}
/*
    普通定时器实现ms延时，可直接使用HAL库函数HAL_delay（）
*/
void delay_ms_tim(uint16_t nms)
{
    uint32_t i;
    for(i=0;i<nms;i++) user_delaynus_tim(1000);
}
uint32_t Read_Weigh(void)
{
    uint8_t i;
    uint32_t value = 0;

    /**
    数据手册写到，当数据输出管脚 DOUT 为高电平时，表明A/D 转换器还未准备好输出数据，此时串口时
    钟输入信号 PD_SCK 应为低电平，所以下面设置引脚状态。
    **/
    HAL_GPIO_WritePin(DT_GPIO_Port,DT_Pin,SET) ;//初始状态DT引脚为高电平
    HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,RESET); //初始状态SCK引脚为低电平

    /**
    等待DT引脚变为低电平
    **/
    while(HAL_GPIO_ReadPin(DT_GPIO_Port,DT_Pin)==GPIO_PIN_SET);
    user_delaynus_tim(1);

    /**
    当 DOUT 从高电平变低电平后，PD_SCK 应输入 25 至 27 个不等的时钟脉冲
    25个时钟脉冲 ---> 通道A 增益128
    26个时钟脉冲 ---> 通道B 增益32
    27个时钟脉冲 ---> 通道A 增益64
    **/
    for(i=0; i<24; i++) //24位输出数据从最高位至最低位逐位输出完成
    {
//		//方法一：
//		GPIO_SetBits(GPIOB, GPIO_Pin_6); //时钟高电平
//		value = value << 1; //如果DT位为低左移一位
//		delay_us(1);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_6); //时钟低电平
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
//			value++; //如果DT位为高，值+1
//		delay_us(1);


        //方法二：
        HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin,SET);//sck
        user_delaynus_tim(1);
        HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin,RESET);//sck
        if(HAL_GPIO_ReadPin(DT_GPIO_Port, DT_Pin) ==GPIO_PIN_RESET )
        {
            value = value << 1;
            value |= 0x00;
        }
        if(HAL_GPIO_ReadPin(DT_GPIO_Port, DT_Pin)== GPIO_PIN_SET)
        {
            value = value << 1;
            value |= 0x01;
        }
        user_delaynus_tim(1);
    }

    //第 25至 27 个时钟脉冲用来选择下一次 A/D 转换的输入通道和增益
    HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin,SET);
    value = value^0x800000;
    user_delaynus_tim(1);
    HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin,RESET);
    user_delaynus_tim(1);
    return value;
}
