//
// Created by 廖锦程 on 2021/12/8.
//

#include "msp430_adc10.h"

/******************************************************************************************************
 * 名       称：ADC10_init(void)
 * 功       能：初始化ADC10采集内部温度传感器，单次手动采样
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接从CCS的Example中移植过来
 * 范       例：无
 ******************************************************************************************************/
void ADC10_init(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10;
    __delay_cycles(30000);
    ADC10CTL0 |= ENC;
}
/******************************************************************************************************
 * 名       称：ADC10_ISR_HOOK
 * 功       能：ADC转换完成后唤醒CPU
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接从CCS的Example中移植过来
 * 范       例：无
 ******************************************************************************************************/
//
//__interrupt_vec(ADC10_VECTOR) void ADC10_ISR_HOOK(void)
//{
//    _bic_SR_register_on_exit(LPM4_bits);
//}

void ADC10_DTC_Init(const uint16_t *p_data, uint16_t data_long, char DTCMode) {
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + ADC10ON + REFON + ADC10IE;
    ADC10CTL1 = INCH_5 + SHS_1 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_2; //ADC10BUSY
    ADC10AE0 = BIT5;
    //ADC10DTC0 = ADC10TB;
    switch (DTCMode) {
        case 'C':
        case 'c':
            //ADC10DTC0 = ADC10TB + ADC10CT;
            ADC10DTC0 = ADC10TB + ADC10B1 + ADC10CT;

            break;
        case 'S':
        case 's':
            //ADC10DTC0 = ADC10TB;
            ADC10DTC0 = ADC10B1;
            break;
        default:
            break;
    }
    //ADC10DTC0 = ADC10B1 + ADC10CT;
    ADC10DTC1 = data_long;
    ADC10SA = p_data;
    __delay_cycles(30000);
    ADC10CTL0 |= ENC;
}

