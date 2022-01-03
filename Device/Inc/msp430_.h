//
// Created by 廖锦程 on 2021/11/25.
//

#ifndef MSP430__H
#define MSP430__H
#include <msp430g2553.h>
#include <stdint.h>
#include "msp430_gpio.h"
#include "msp430_timer_a.h"
#include "msp430_adc10.h"

#define __weak __attribute__((weak))
#define SET(X,Y) (X)|=(Y)
#define RESET(X,Y) (X)&=~(Y)
#endif //MSP430__H
