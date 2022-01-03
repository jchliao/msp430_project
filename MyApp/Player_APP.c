//
// Created by 廖锦程 on 2021/12/30.
//

#include "Player_APP.h"
#include "LCD_128.h"
#include <msp430.h>
#include "SM_APP.h"

Wav_t WavList[8] = {
        {"XKC",76367,15109},
        {"YKZ",33163,11003},
        {"LNH",811,12923},
        {"BS",66267,9984},
        {"ZTX",45195,9428},
        {"QT",54635,11617},
        {"REC",300000,301000},
        {"REC",600000,601000},
};


play_t player = {sizeof(WavList)/sizeof(Wav_t),0,10,'L',4};

void Hello(){
    LCD_Clear();
    LCD_DisplayDigit(3,1);
    LCD_DisplayDigit(8,2);
    LCD_DisplayDigit(8,3);
    LCD_DisplayDigit(6,4);
    LCD_DisplayDigit(6,5);
    LCD_DisplayDigit(8,6);
    LCD_ClearSeg(_LCD_1B);
    LCD_ClearSeg(_LCD_1C);
    LCD_ClearSeg(_LCD_2A);
    LCD_ClearSeg(_LCD_2D);
    LCD_ClearSeg(_LCD_3C);
    LCD_ClearSeg(_LCD_3B);
    LCD_ClearSeg(_LCD_4G);
    LCD_ClearSeg(_LCD_4A);
    LCD_ClearSeg(_LCD_5A);
    LCD_ClearSeg(_LCD_4C);
    LCD_ClearSeg(_LCD_5G);
    LCD_ClearSeg(_LCD_5C);
    LCD_ClearSeg(_LCD_6G);
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();

    LCD_DisplaySeg(_LCD_10C);
    LCD_DisplaySeg(_LCD_10D);
    LCD_DisplaySeg(_LCD_10E);
    LCD_DisplayDigit(6,9);
    LCD_ClearSeg(_LCD_9A);
    LCD_ClearSeg(_LCD_9F);
    LCD_DisplaySeg(_LCD_8E);
    LCD_DisplaySeg(_LCD_8F);

    _DINT();
    HT1621_Reflash_Digit(10);
    HT1621_Reflash_Digit(9);
    HT1621_Reflash_Digit(8);
    _EINT();

    DisVol();
}
void SelectWavMode()
{
    LCD_Clear();
    LCD_DisplayDigit(5,10);
    LCD_DisplayDigit(6,9);
    LCD_DisplayDigit(6,8);
    LCD_DisplayDigit(6,7);
    LCD_ClearSeg(_LCD_9C);
    LCD_ClearSeg(_LCD_8A);
    LCD_ClearSeg(_LCD_8G);
    LCD_ClearSeg(_LCD_8C);
    LCD_ClearSeg(_LCD_7C);
    LCD_DisplayNum(player.index+1);
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}
void NextWav(){
    player.index = player.index==player.num_wav-1 ? 0 : player.index+1;
    player.last_p=10;
    LCD_DisplayNum(player.index+1);
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}
void PreWav(){
    if((player.last_p>30)&(State==PLAYING))
        player.last_p=10;
    else
        player.index = player.index== 0 ? player.num_wav-1 : player.index-1;
    LCD_DisplayNum(player.index+1);
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}
void PlayWav(){
    LCD_Clear();
    LCD_ClearSeg(32);
    LCD_DisplaySeg(31);
    LCD_DisplayNum(player.index+1);
    if(player.mode=='L'){
        LCD_DisplayDigit(6,10);
        LCD_ClearSeg(_LCD_10A);
        LCD_ClearSeg(_LCD_10C);
        LCD_ClearSeg(_LCD_10G);
    }
    else{
        LCD_DisplayDigit(5,10);
    }
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}
void PauseWav(){

    //TA0CCTL0 &= ~CCIE;
    LCD_ClearSeg(31);
    LCD_DisplaySeg(32);
    if(player.mode=='L'){
        LCD_DisplayDigit(6,10);
        LCD_ClearSeg(_LCD_10A);
        LCD_ClearSeg(_LCD_10C);
        LCD_ClearSeg(_LCD_10G);
    }
    else{
        LCD_DisplayDigit(5,10);
    }
    _DINT();
    HT1621_Reflash(LCD_Buffer);
    _EINT();
}
void ChangPlayMode(){
    if(player.mode=='S')
        player.mode='L';
    else
        player.mode='S';
}
void AddVol(){
    if (player.vol_reduce==0){

    }
    else
    {
        player.vol_reduce--;
    }
}
void SubVol(){
    if (player.vol_reduce==7){

    }
    else
    {
        player.vol_reduce++;
    }
}
void DisVol(){
//    LCD_DisplaySeg(_LCD_10C);
//    LCD_DisplaySeg(_LCD_10D);
//    LCD_DisplaySeg(_LCD_10E);
//    LCD_DisplayDigit(6,9);
//    LCD_ClearSeg(_LCD_9A);
//    LCD_ClearSeg(_LCD_9F);
//    LCD_DisplaySeg(_LCD_8E);
//    LCD_DisplaySeg(_LCD_8F);
    LCD_DisplayDigit(8-player.vol_reduce,7);
//    HT1621_Reflash_Digit(10);
//    HT1621_Reflash_Digit(9);
//    HT1621_Reflash_Digit(8);
    _DINT();
    HT1621_Reflash_Digit(7);
    _EINT();
}
/******************************************************************************************************
 * 名       称：Display_Progress(Wav_t * Wav_p,play_t * play_p)
 * 功       能：进度条
 * 入口参数：
 * 出口参数：无
 * 说       明：与SD卡库函数不同的是，本函数读取的数据是往FIFO里写的
 * 范       例：无
 ******************************************************************************************************/

void Display_Progress(Wav_t * Wav_p,play_t * play_p){
    static unsigned char pinW0;
    switch ((Wav_p->size-play_p->last_p)*17/(Wav_p->size-10)) {
        case 16:
            pinW0=0b00000000;
            break;
        case 15:
            pinW0 |= 0b00000000 ;   //灭
            pinW0^=BIT7;           //闪
            pinW0 &=~ 0b01111111; //亮
            break;
        case 14:
            pinW0=0b10000000;
            break;
        case 13:
            pinW0|= 0b10000000 ;//灭
            pinW0^=BIT6;        //闪
            pinW0&=~ 0b00111111; //亮
            break;
        case 12:
            pinW0=0b11000000;
            break;
        case 11:
            pinW0|= 0b11000000 ;//灭
            pinW0^=BIT5;        //闪
            pinW0&=~ 0b00011111; //亮
            break;
        case 10:
            pinW0=0b11100000;
            break;
        case 9:
            pinW0|= 0b11100000 ;//灭
            pinW0^=BIT4;        //闪
            pinW0&=~ 0b00001111; //亮
            break;
        case 8:
            pinW0=0b11110000;
            break;
        case 7:
            pinW0|= 0b11110000 ;//灭
            pinW0^=BIT3;        //闪
            pinW0&=~0b00000111; //亮
            break;
        case 6:
            pinW0=0b11111000;
            break;
        case 5:
            pinW0|= 0b11111000 ;//灭
            pinW0^=BIT2;        //闪
            pinW0&=~0b00000011; //亮
            break;
        case 4:
            pinW0=0b11111100;
            break;
        case 3:
            pinW0|= 0b11111100 ;//灭
            pinW0^=BIT1;        //闪
            pinW0&=~ 0b00000001; //亮
            break;
        case 2:
            pinW0=0b11111110;
            break;
        case 1:
            pinW0|= 0b11111110 ;//灭
            pinW0^=BIT0;        //闪
            pinW0&=~0b00000000; //亮
            break;
        case 0:
            pinW0=0b11111111;
            break;
        default:
            for(int i=0;i<=7;i++){
                PinOUT(i,0);
            }
            break;
    }
    PinOUTx(pinW0);
}

