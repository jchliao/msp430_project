//
// Created by 廖锦程 on 2021/12/20.
//

#ifndef SOFTFIFO_H
#define SOFTFIFO_H

#define FIFOBUFF_SIZE 50				/*缓冲区大小,根据程序需要自行调整*/
//#define FIFOBUFF_SIZE  				/*缓冲区大小,根据程序需要自行调整*/
extern unsigned char FIFOBuff[FIFOBUFF_SIZE]; 	//定义缓冲队列数组(FIFO)
extern unsigned int FIFO_Count;           					//FIFO中数据个数

extern unsigned char Read_FIFO();
extern void Write_FIFO(unsigned char value);
void Clear_FIFO();

#endif //SOFTFIFO_H
