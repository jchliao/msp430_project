//
// Created by 廖锦程 on 2021/11/24.
//

#include "msp430_it.h"
/*
__interrupt_vec(PORT1_VECTOR)
void Port_1 (void){
    GPIO_IRQHandler();
    P1IFG = 0 ;  //中断标志位清零
}
 */

//__interrupt_vec(TIMER0_A0_VECTOR)
//void Timer_A(void){
//    P1OUT^=BIT0;    //翻转P1.0电平
//    CCR0+=62500;    //时间加一个tx 如果溢出也在一个TAR之内；
//}
/*
 * 功能每62500个SCLK时钟翻转一次P1.0 生成一个周期为125000clk占空比为50%的PWM
 * SCLK 设置为1MHz
 * 8分频后  PWM周期为1s vector=TIMER0_A1_VECTOR
 */