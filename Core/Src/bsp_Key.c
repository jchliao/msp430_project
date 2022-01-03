//
// Created by 廖锦程 on 2021/11/25.
//

#include "bsp_Key.h"
void bsp_Key_Init(void){
    GPIO1_PUSH(BIT3);                 //上拉
    SET(P1IE,BIT3);                   //开启P13中断
    SET(P1IES,BIT3);                  //下沿中断
    RESET(P1IFG,BIT3);
    _EINT();        //开启总中断
}