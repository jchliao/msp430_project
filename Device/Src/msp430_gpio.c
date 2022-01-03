//
// Created by 廖锦程 on 2021/11/25.
//

#include "msp430_gpio.h"
//__attribute__((weak)) void GPIO_Init(void);
/*
void GPIO_IRQHandler(void){
    unsigned int Push_Key = 0;
//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
    Push_Key = P1IFG & (~P1DIR);
//-----延时一段时间，避开机械抖动区域-----
    __delay_cycles(1000); //消抖延时
//----判断按键状态是否与延时前一致-----
    if ((P1IN & Push_Key) == 0) //如果该次按键确实有效
    {
//----判断具体哪个IO被按下，调用该IO的事件处理函数-----
        switch (Push_Key) {
// case BIT0: P10_Onclick(); break;
// case BIT1: P11_Onclick(); break;
// case BIT2: P12_Onclick(); break;
            case BIT3:
                P13_OnclickCallback();
                break;
// case BIT4: P14_Onclick(); break;
// case BIT5: P15_Onclick(); break;
// case BIT6: P16_Onclick(); break;
// case BIT7: P17_Onclick(); break;
            default:
                break; //任何情况下均加上default
        }
    }
};

 */