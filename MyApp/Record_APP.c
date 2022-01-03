//
// Created by 廖锦程 on 2021/12/30.
//

#include "Record_APP.h"
#include "Player_APP.h"
#include "msp430.h"
#include "LCD_128.h"
#include <stdint.h>
#include "msp430_adc10.h"
#include "SD_SPI.h"
#include "SM_APP.h"

unsigned long pRec = 0;

struct rec_data_adc_t{
    uint16_t data0[40];
    uint16_t data1[40];
};
struct rec_data_sd_t{
    unsigned char data0[80];
    unsigned char data1[80];
};
union rec_data_u{
    struct rec_data_adc_t adc;
    struct rec_data_sd_t sd;
} rec_data;



void RecInit(){
    TA1CCTL0 &= ~CCIE;
    TA0_PWM_Init('s',1,'b','0');
    TA01_OFF;
    TA0_PWM_SetPeriod(SYSCLK/8000);

    TA0_PWM_SetPermill(1,500);
    ADC10_DTC_Init(rec_data.adc.data0, 40, 'c');
    _EINT();
}

void PauseRec(){
    ADC10CTL0 &=~ENC;
};
void StartRec(){
    ADC10CTL0 |= ENC + ADC10SC;
};
void StopRec(){
    ADC10CTL0 &= ~ENC;
    ADC10DTC0 &=~ ADC10CT;
    pRec=0;
};
void ResumeRec(){
    _NOP();
};

void RecordMode(){
    LCD_Clear();
    LCD_DisplaySeg(_LCD_1E);
    LCD_DisplaySeg(_LCD_1G);

    LCD_DisplayDigit(2,2);
    LCD_ClearSeg(_LCD_2A);
    LCD_ClearSeg(_LCD_2B);

    LCD_DisplayDigit(6,3);
    LCD_ClearSeg(_LCD_3A);
    LCD_ClearSeg(_LCD_3F);

    LCD_DisplaySeg(_LCD_4E);
    LCD_DisplaySeg(_LCD_4G);

    LCD_DisplayDigit(8,5);
    LCD_ClearSeg(_LCD_5A);
    LCD_ClearSeg(_LCD_5F);

    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}


/******************************************************************************************************
 * 名       称：ADC10_ISR_HOOK
 * 功       能：ADC转换完成后唤醒CPU
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接从CCS的Example中移植过来
 * 范       例：无
 ******************************************************************************************************/
__interrupt_vec(ADC10_VECTOR)
void ADC10_ISR_HOOK(void)
{
    //ADC10SA = &rec_data.adc.data0[0];  //打开
    //ACD10_ENABLE;
    //__bis_SR_register(GIE);
    //_BIC_SR_IRQ(LPM4_bits);
    //LCD_DisplayNum(1111);
    //GPIO1_Toggle(BIT0);
if(State==RECORDING){
    if(ADC10DTC0&ADC10B1){
        static unsigned char turn = 1;
        turn ^= BIT0;
        //PinOUT(2,turn);         //指定0号管脚输出为0&1
        PinOUT(3,turn);         //指定1号管脚输出为0&1
        //__delay_cycles(3000000);
        //SD_Write_Sector(WavList[6].start_p + pRec,rec_data.sd.data1,0,100);
        pRec++;
    }
    else
    {
        static unsigned char turn2 = 1;
        turn2 ^= BIT0;
        //PinOUT(0,turn);         //指定0号管脚输出为0&1
        PinOUT(2,turn2);         //指定1号管脚输出为0&1
        //__delay_cycles(3000000);
        //SD_Write_Sector(WavList[6].start_p + pRec,rec_data.sd.data0,0,100);
        pRec++;
    }
}
    _NOP();
}


