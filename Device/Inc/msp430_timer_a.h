//
// Created by 廖锦程 on 2021/11/28.
//

#ifndef MSP430_TIMER_A_H
#define MSP430_TIMER_A_H
#define DEADTIME 20 //预设死区时间，以TA的clk为单位
/******* 设定TA输出IO口，目前设定为MSP430G2553 ，20Pin 封装无TA0.2********/

#define TA01_SET   \
    P1SEL |= BIT6; \
    P1DIR |= BIT6 //  1.6 P1.2
#define TA02_SET   \
    P3SEL |= BIT0; \
    P3DIR |= BIT0 // P3.0 无
#define TA11_SET   \
    P2SEL |= BIT1; \
    P2DIR |= BIT1 // P2.2
#define TA12_SET   \
    P2SEL |= BIT4; \
    P2DIR |= BIT4 // P2.4

#define TA01_OFF P1SEL &= ~BIT6 // P1.6
#define TA02_OFF P3SEL &= ~BIT0 // P3.0
#define TA11_OFF P2SEL &= ~BIT1 // P2.2
#define TA12_OFF P2SEL &= ~BIT4 // P2.4


extern char TA0_PWM_Init(char Clk, char Div, char Mode1, char Mode2);
extern char TA0_PWM_SetPeriod(unsigned int Period);
extern char TA0_PWM_SetPermill(char Channel, unsigned int Duty) ;

extern char TA1_PWM_Init(char Clk, char Div, char Mode1, char Mode2);
extern char TA1_PWM_SetPeriod(unsigned int Period);
extern char TA1_PWM_SetPermill(char Channel, unsigned int Duty);

char TA0_CallBack_Init(char Clk,char Div,unsigned int Period);
char TA1_CallBack_Init(char Clk,char Div,unsigned int Period);

#endif //MSP430_TIMER_A_H
