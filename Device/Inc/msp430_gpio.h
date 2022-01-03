//
// Created by 廖锦程 on 2021/11/25.
//

#ifndef MSP430_GPIO_H
#define MSP430_GPIO_H
#include <msp430.h>
#define GPIO1_OUT(PINx)        P1DIR |= (PINx)
#define GPIO1_IN(PINx)         P1DIR &=~(PINx)
#define GPIO1_READ(PINx)       P1IN  & (PINx)
#define GPIO1_SET(PINx)        P1OUT |= (PINx)
#define GPIO1_RESET(PINx)      P1OUT &=~(PINx)
#define GPIO1_Toggle(PINx)     P1OUT ^= (PINx)
#define GPIO1_PUSH(PINx)       P1REN |= (PINx);P1OUT |= (PINx)
#define GPIO1_PULL(PINx)       P1REN |= (PINx);P1OUT &= (PINx)

#define GPIO2_OUT(PINx)        P2DIR |= (PINx)
#define GPIO2_IN(PINx)         P2DIR &=~(PINx)
#define GPIO2_READ(PINx)       P2IN  & (PINx)
#define GPIO2_SET(PINx)        P2OUT |= (PINx)
#define GPIO2_RESET(PINx)      P2OUT &=~(PINx)
#define GPIO2_Toggle(PINx)     P2OUT ^= (PINx)
//void GPIO_Init(void);
void GPIO_IRQHandler(void);
void P13_OnclickCallback(void);
#endif //MSP430_GPIO_H

