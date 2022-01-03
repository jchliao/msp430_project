//
// Created by 廖锦程 on 2021/11/24.
//

#include "bsp_GPIO.h"

void bsp_GPIO_Init(void){
//    P1DIR |= BIT6|BIT4|BIT0;  //设置为输出
//    P1OUT |= BIT6;            //LED亮
//    P1REN |= BIT3;  //P13上下拉电阻启用
//    P1OUT |= BIT3;  //P13默认为输入 此时为上拉
//    P1IE  |= BIT3;  //开启P13中断
//    P1IES |= BIT3;  //下沿中断
//    P1IFG &=~BIT3;  //中断标志位清零
//    P1SEL |= BIT4|BIT0;
//    P1SEL2&=~BIT4|BIT0;
    GPIO1_OUT(BIT0);  //设置为输出
    GPIO1_RESET(BIT0);       //LED亮
    //GPIO1_RESET(BIT2);       //LED亮
    //SET(P1SEL,BIT4);
    //RESET(P1SEL2,BIT4);
}

void GPIO_IRQHandler(void);