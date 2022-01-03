//
// Created by 廖锦程 on 2021/12/30.
//

#ifndef SM_APP_H
#define SM_APP_H
#include <msp430.h>
//-----对状态进行宏定义-----
#define HELLO 0
#define SELECT 1
#define PLAYING 2
#define PAUSE 3

#define RECORD 4
#define RECORDING 5
#define RECORD_P 6

#define IDLE 0
#define SHORT 1
#define LONG 2

#define UP_KEY BIT0
#define DOWN_KEY BIT1
#define OK_KEY BIT2
#define BACK_KEY BIT3

#define  COUNTER_THRESHOLD 30 /*长键判别门限*/
extern unsigned char State;


void SM_Mealy();
#endif //SM_APP_H
