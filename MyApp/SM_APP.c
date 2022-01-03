//
// Created by 廖锦程 on 2021/12/30.
//

#include "SM_APP.h"
#include "Player_APP.h"
#include "TCA6416A.h"
#include "Record_APP.h"
#include "TouchIN.h"
#include "softFIFO.h"

unsigned char State=HELLO;
unsigned char WDT_Counter = 0; /*用于对按键按下时间进行计数*/

/******************************************************************************************************
 * 名       称：LongClick_Dect()
 * 功       能：对WDT中断计时，计满清零并返回”长键“信息
 * 入口参数：无
 * 出口参数：无
 * 说       明：
 * 范       例：无
 ******************************************************************************************************/
unsigned char LongClick_Dect()
{
    WDT_Counter++;
    if (WDT_Counter == COUNTER_THRESHOLD)
    {
        WDT_Counter = 0;
        return (1);
    }
    else
        return (0);
}

/******************************************************************************************************
 * 名       称： I2C_IODect()
 * 功       能：TCA6416A的扩展IO事件检测函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：检测具体哪个扩展IO被按下
 * 范       例：无
 ******************************************************************************************************/
unsigned char I2C_IODect()			                 //检测事件确实发生了
{
    PinIN();
    static unsigned char Key_State;
    static unsigned char KEY_Now=0;
    unsigned char KEY_Past;
    unsigned char Key_Dect = 0;	  //按键状态值
    unsigned char Key3_Dect = 0;	  //按键状态值
    KEY_Past=KEY_Now;
    //----判断I2C_IO10所连的KEY1按键是否被按下------
    if((TCA6416A_InputBuffer&BIT8) == BIT8)
        KEY_Now |=BIT0;
    else
        KEY_Now &=~BIT0;
    if(((KEY_Past&BIT0)==BIT0)&&(KEY_Now&BIT0) !=BIT0)
        Key_Dect |= BIT0;
    if(((KEY_Past&BIT0)!=BIT0)&&(KEY_Now&BIT0) ==BIT0)
        Key_Dect &=~BIT0;

    //----判断I2C_IO11所连的KEY2按键是否被按下------
    if((TCA6416A_InputBuffer&BIT9)== BIT9)
        KEY_Now |= BIT1;
    else
        KEY_Now &=~BIT1;
    if(((KEY_Past&BIT1)==BIT1)&&(KEY_Now&BIT1)!=BIT1)
        Key_Dect |= BIT1;
    if(((KEY_Past&BIT1)!=BIT1)&&(KEY_Now&BIT1)==BIT1)
        Key_Dect &=~BIT1;

    //----判断I2C_IO12所连的KEY3按键是否被按下------
    if((TCA6416A_InputBuffer&BITA) == BITA)
        KEY_Now |=BIT2;
    else
        KEY_Now &=~BIT2;
    if(((KEY_Past&BIT2)==BIT2)&&(KEY_Now&BIT2) !=BIT2)
        Key3_Dect=1;
    if(((KEY_Past&BIT2)!=BIT2)&&(KEY_Now&BIT2) ==BIT2)
        Key3_Dect=2;

    //----判断I2C_IO13所连的KEY4按键是否被按下------
    if((TCA6416A_InputBuffer&BITB) ==  BITB)
        KEY_Now |=BIT3;
    else
        KEY_Now &=~BIT3;
    if(((KEY_Past&BIT3) == BIT3)&& (KEY_Now&BIT3) != BIT3)
        Key_Dect |= BIT3;
    if(((KEY_Past&BIT3) != BIT3)&& (KEY_Now&BIT3) == BIT3)
        Key_Dect &=~BIT3;
    switch (Key_State) //该状态机靠扫描的按键值Key_Dect跳转状态
    {
        case IDLE:
            WDT_Counter = 0; //空闲状态对计数清零
            if (Key3_Dect == 1){
                Key_State = SHORT;
            }
            break; //路径1
        case SHORT:
            if (Key3_Dect == 2) //路径2
            {
                Key_State = IDLE;
                Key_Dect |= BIT2;//短按事件处理函数
            }
            if (LongClick_Dect()) //路径3
            {
                Key_State = LONG;
                Key_Dect |=(BIT2<<4);; //长按事件处理函数
            }
            break;
        case LONG:
            WDT_Counter = 0; //长按状态对计数清零
            if (Key3_Dect == 2)
                Key_State = IDLE;
            break; //路径4
        default:
            Key_State = IDLE;
            break;
    }
    return Key_Dect;
}



void SM_Mealy()
{
    static unsigned char TouchIN_last;	            //状态机的状态变量

    unsigned char Key_Dect = I2C_IODect();  //按键状态值

    if((State==HELLO)||(State==SELECT)){
        TouchIN_last = TouchIN;
        TouchIN_Dect();
    }

    switch (State)                          //该状态机靠扫描的按键值Key_Dect跳转状态
    {
        case HELLO:
            if (Key_Dect == UP_KEY){
                State = SELECT;
                SelectWavMode();
                //PLAYEREINT;
            }
            if((TouchIN_last&BIT0)!=BIT0&&(TouchIN&BIT0)==BIT0){
                SubVol();
                DisVol();
            }
            if((TouchIN_last&BIT1)!=BIT1&&(TouchIN&BIT1)==BIT1){
                AddVol();
                DisVol();
            }
            if (Key_Dect == DOWN_KEY)
            {
                State = RECORD;
                PLAYERDINT;
                RecordMode();
            }
            if (Key_Dect == OK_KEY) //路径2
            {
                //AddVol();
                //DisVol();
            }
            if (Key_Dect == BACK_KEY) //路径2
            {
                //SubVol();
                //DisVol();
            }
            break; //路径1

        case SELECT:
            if (Key_Dect == UP_KEY)
            {
                PreWav();
            }
            if (Key_Dect == DOWN_KEY)
            {
                NextWav();
            }
            if (Key_Dect == OK_KEY) //路径2
            {
                State = PLAYING;
                PlayWav();
                Display_Progress(&WavList[player.index],&player);
                //PLAYEREINT;
            }
            if (Key_Dect == BACK_KEY) //路径2
            {

                State = HELLO;
                Hello();
                //Clear_FIFO();
            }
            break;
        case PLAYING:
            if (Key_Dect == UP_KEY)
            {
                PLAYERDINT;
                PreWav();
                PLAYEREINT;
            }
            if (Key_Dect == DOWN_KEY)
            {
                PLAYERDINT;
                NextWav();
                PLAYEREINT;
            }

            if (Key_Dect == OK_KEY) //路径2
            {
                State = PAUSE;
                PauseWav();
                //PLAYERDINT;
                //Clear_FIFO();
            }
            if (Key_Dect>>4 == OK_KEY) //路径2
            {
                PLAYERDINT;
                ChangPlayMode();
                PlayWav();
                PLAYEREINT;
            }
            if (Key_Dect == BACK_KEY) //路径2
            {
                State = SELECT;
                for(int i=0;i<=7;i++){
                    PinOUT(i,1);
                }
                player.last_p=10;
                SelectWavMode();
            }
            break;
        case PAUSE:
            if (Key_Dect == UP_KEY)
            {
                PreWav();
            }
            if (Key_Dect == DOWN_KEY)
            {
                NextWav();
            }
            if (Key_Dect == OK_KEY) //路径2
            {
                State = PLAYING;
                PlayWav();
                //PLAYEREINT;
            }
            if (Key_Dect>>4 == OK_KEY) //路径2
            {
                ChangPlayMode();
                PauseWav();
            }
            if (Key_Dect == BACK_KEY) //路径2
            {
                State = SELECT;
                for(int i=0;i<=7;i++){
                    PinOUT(i,1);
                }
                SelectWavMode();
                //PLAYEREINT;
            }
            break;
        case RECORD:
            if (Key_Dect == UP_KEY){
                State = RECORD;
            }
            if (Key_Dect == DOWN_KEY){
                State = RECORD;
            }
            if (Key_Dect == OK_KEY){
                State = RECORDING;
                RecInit();
                StartRec();
                PinOUT(0,0);
            }
            if (Key_Dect == BACK_KEY){
                State = HELLO;
                ADC10DTC0=0;
                Hello();
                PLAYEREINT;
            }
            break; //路径1
        case RECORDING:
            if (Key_Dect == OK_KEY) //路径2
            {
                State = RECORD_P;
                PauseRec();
            }
            if (Key_Dect == BACK_KEY) //路径2
            {
                State = RECORD;
                StopRec();
                PinOUT(0,1);
                //RecordMode();
                //StopWav();
            }
            break; //路径1
        case RECORD_P:
            if (Key_Dect == OK_KEY) //路径2
            {
                State = RECORDING;
                ResumeRec();
            }
            if (Key_Dect == BACK_KEY) //路径2
            {
                State = RECORD;
                StopRec();
                //Hello();
                //StopWav();
            }
            break;
        default:
            State = HELLO;
            break;
    }
}