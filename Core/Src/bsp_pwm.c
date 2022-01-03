//
// Created by 廖锦程 on 2021/11/29.
//

#include "bsp_pwm.h"
void bsp_measure_freq(void){
    /*设置时钟源为SMCLK*/
    TA1CTL |= TASSEL_2;
/*设置工作模式为Up Mode*/
    TA1CTL |= MC_1;
/*设置定时间隔*/
    TA1CCR0 = 49999;//50ms
/*开启TAIFG中断*/
    TA1CTL |= TAIE;

/*TA1,CCR2用于捕捉功能*/
    TA1CCTL2 |= CAP;
/*上升沿捕捉*/
    TA1CCTL2 |= CM_1;
/*P2.5作为捕捉输入(CCI2B)*/
    TA1CCTL2 |= CCIS_1;
    P2SEL |= BIT5;
/*允许捕捉比较中断*/
    TA1CCTL2 |= CCIE;

    /*打开全局中断*/
    //__bis_SR_register(GIE);
    _EINT();        //开启总中断
}

void bsp_pwm(void) {
/*设置时钟源为SMCLK*/
    TA0CTL |= TASSEL1;
    /*设置工作模式为Up&Down*/
    TA0CTL |= MC0|MC1;
    /*设置TA0CCR0为0x00FF*/
    TA0CCR0 = 0x00FF;
    /*设置TA0CCR1为0x00FF*/
    TA0CCR1 = 0x00AA;//占空比(TACCR0 - TACCR1) / TACCR0,频率=SMCLK/(TACCR0)/2
    /*设置为比较模式*/
    RESET(TA0CCTL0,CAP);
    RESET(TA0CCTL1,CAP);
    /*设置比较输出模式*/
    TA0CCTL1 |= OUTMOD_6;
    /*设置IO复用*/
    TA01_SET;
}

void bsp_pwm_teacher(void) {
    //P1DIR = BIT2;    //P1.0 设为输出
    //GPIO1_OUT(BIT0);
    //CCTL0=CCIE;      //中断使能
    CCR0=6250;      //50000个时钟
    TACTL=TASSEL_2+MC_1+ID_0;            //SMCLK+连续模式
    TACCTL1 = OUTMOD_7;
    TA0CCTL2 = OUTMOD_7;
    GPIO1_OUT(BIT2);
    SET(P1SEL,BIT2);
    RESET(P1SEL2,BIT2);

    GPIO1_OUT(BIT1);
    SET(P1SEL,BIT1);
    RESET(P1SEL2,BIT1);

    CCR0 = 6250;
    CCR1 = 4000;
    CCR2 = 1000;
    //_BIS_SR(LPM0_bits|GIE);      //关cpu 开中断
}
void bsp_pwm_teacher2(void) {
    //P1DIR = BIT2;    //P1.0 设为输出
    //GPIO1_OUT(BIT0);
    //CCTL0=CCIE;      //中断使能
    CCR0=6250;      //50000个时钟
    TACTL=TASSEL_2+MC_1+ID_0;            //SMCLK+连续模式
    TACCTL1 = OUTMOD_7;
    GPIO1_OUT(BIT2);
    SET(P1SEL,BIT2);
    RESET(P1SEL2,BIT2);
    TACCR0 = 0x2;
    TA0CCR1 = 0x1;
    //_BIS_SR(LPM0_bits|GIE);      //关cpu 开中断
}

