//
// Created by 廖锦程 on 2021/12/8.
//

#ifndef MSP430_ADC10_H
#define MSP430_ADC10_H
#include "msp430_.h"

#define ACD10_ENABLE ADC10CTL0|=ENC
#define ACD10_SC ADC10CTL0|=ADC10SC
void ADC10_Init(uint16_t CHx,uint16_t DIVx,uint16_t CONSEQx, uint16_t SHS_x);

void ADC10_DTC_Init(const uint16_t *p_data, uint16_t data_long, char DTCMode);

#endif //MSP430_ADC10_H
