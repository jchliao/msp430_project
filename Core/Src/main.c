#include "msp430g2553.h"
#include <string.h>
#include "SD_HardWare.h"
#include "SD_SPI.h"
#include "SPI.h"
#include "DAC7311.h"
#include "softFIFO.h"
#include "LCD_128.h"
#include "SM_APP.h"
#include "Player_APP.h"
#include "Record_APP.h"
#include "MSP430G2553.h"
#include "TouchIN.h"


//-----在main函数前提前申明函数-----
void WDT_init();
void Time1_A_Init();
char SD_Read_InFIFO(Wav_t * Wav_p,play_t * play_p);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;	//关狗
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
    __delay_cycles(100000);
    //-----初始化SPI-----
    SPI_init();
    //-----初始化SD-----
    SD_Init();
    //-----初始化DAC-----
    DAC7311_Init();
    //-----初始化LCD-----
    LCD_Init();
    // -----初始化采样率定时器-----
    Time1_A_Init();
    WDT_init();
    unsigned int flash_flag = 0;
    _EINT();		 //开总中断
    //显示Hello界面
    Hello();
    while(1)
    {
        //播放模式程序
        if(State==PLAYING){
            //闪灯
            if(flash_flag%75==0){
                Display_Progress(&WavList[player.index],&player);
            }
            flash_flag++;
            SD_Read_InFIFO(&WavList[player.index],&player);// 读取指定歌曲 读不完512字节不退出；
            //播放完一曲要干什么
            if(player.last_p==WavList[player.index].size){
                if (player.mode=='S')PreWav(); //在次播放本曲
                if (player.mode=='L')NextWav();//下一曲
            }
        }
    }
}

/******************************************************************************************************
 * 名       称：WDT_init()
 * 功       能：初始化WDT定时中断为16ms
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void WDT_init()
{
    WDTCTL=WDT_ADLY_16;
    IE1 |= WDTIE;           //WDT中断使能
}

/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：响应WDT定时中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：不能直接判断事件，需启用状态机
 * 范       例：无
 ******************************************************************************************************/
__interrupt_vec(WDT_VECTOR)
void WDT_ISR(void)
{
    _EINT();
    //-----启用状态机-----
    SM_Mealy();
}
/******************************************************************************************************
 * 名       称：SD_Read_InFIFO(unsigned long sector)
 * 功       能：不停的读取SD卡数据
 * 入口参数：头扇区地址
 * 出口参数：无
 * 说       明：与SD卡库函数不同的是，本函数读取的数据是往FIFO里写的
 * 范       例：无
 ******************************************************************************************************/
char SD_Read_InFIFO(Wav_t * Wav_p,play_t * play_p)
{
    unsigned char CMD[6]={0};
    unsigned char retry = 0;
    unsigned char temp = 0;
    unsigned int i = 0;
    unsigned long sector = (Wav_p->start_p + play_p->last_p)<<9;					     //转成逻辑地址
    //=====以下部分与SD卡库函数一致=====
    CMD[0]=0x51;	   									//SD_CMD17
    //-----转换32位扇区地址-----
    CMD[1]= ((sector & 0xff000000) >>24);
    CMD[2]= ((sector & 0x00ff0000) >>16);
    CMD[3]= ((sector & 0x0000ff00) >>8);
    CMD[4]= sector & 0x000000ff;
    CMD[5]=0xFF;										    //SD_SPI_CRC
    play_p->last_p ++;				//扇区递增
    //-----将命令写入SD卡最多100遍直到应答-----
    do
    {
        temp = Write_Command_SD(CMD);
        retry++;
        if(retry == 100)
        {
            return(0);
        }
    } while(temp!=0);
    while (SD_Read_Byte() != 0xfe) ;	//应答正确
    //=====以上部分和SD卡库函数一致，以下部分改为写数据到FIFO=====
    for(i=0;i<512;i++)
    {
        while(FIFO_Count>= FIFOBUFF_SIZE)
            if(State==PLAYING||State==PAUSE)
            _NOP();
            else break;
        Write_FIFO(SD_Read_Byte());

    }
    //-----空收循环冗余校验位-----
    SD_Read_Byte();													//CRC - Byte
    SD_Read_Byte();													//CRC - Byte
    SD_CS_High();														//CS片选禁能
    return(1);
}

//设置Time1 用于DAC转换
void Time1_A_Init()
{
    TA1CCR0 = SYSCLK/SAMPLE_FREQ;		//设定TA定时周期
    TA1CTL = TASSEL_2 + MC_1;           //TA0设为增计数模式，时钟=SMCLK
    TA1CCTL0 = CCIE;
    _enable_interrupts();                       //开全局总中断，
}
/******************************************************************************************************
 * 名       称：Timer0_A0
 * 功       能：TACCR0中断服务函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：此中断源独占中断向量入口，无需判断
 * 范       例：无
 ******************************************************************************************************/
__interrupt_vec(TIMER1_A0_VECTOR) void Timer1_A0 (void)
{
    TA1CCTL0 &= ~CCIE;                       // 关闭比较/捕获模块0的中断
    unsigned int temp =0,tempL=0,tempH=0;
    if(FIFO_Count!=0)
    {
        tempL =  Read_FIFO();
        tempH =  Read_FIFO();
        temp = ((tempH<<8) &0xff00)|(tempL &0x00ff);
        temp = temp+0x8000;	      //有符号数变无符号数
        if(State==PLAYING){
            write2DAC7311(temp>>player.vol_reduce);
        }
    }
    TA1CCTL0 = CCIE;                         // 允许比较/捕获模块0的中断
}
