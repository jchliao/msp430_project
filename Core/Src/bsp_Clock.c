//
// Created by 廖锦程 on 2021/11/24.
//

#include "bsp_Clock.h"
void bsp_Clock_Init(void) {

    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0; //复位默认值，此行代码可省略
    if (CALBC1_12MHZ != 0xFF) {
        DCOCTL = 0x00;
        DCOCTL = CALDCO_1MHZ;
        BCSCTL1 = CALBC1_1MHZ;          //Set DCO to 1MHz
    }
    //BCSCTL1 |= XT2OFF + DIVA_0;         // 复位默认值，此行代码可省略
    BCSCTL2 = DIVM_0 | DIVS_0;
    RESET(BCSCTL2,SELS);
    BCSCTL3 = XT2S_0 + LFXT1S_2 + XCAP_1; // 设为内部低频振荡器 默认为外部时钟
}
