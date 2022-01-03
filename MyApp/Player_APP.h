//
// Created by 廖锦程 on 2021/12/30.
//

#ifndef PLAYER_APP_H
#define PLAYER_APP_H

#define PLAYEREINT TA1CCTL0 = CCIE
#define PLAYERDINT TA1CCTL0 &=~CCIE

#define  SYSCLK 12000000				    //系统时钟
#define  SAMPLE_FREQ 11025				    //音频文件的采样率

typedef struct
{
    char name[4];
    unsigned long start_p;
    unsigned long size;  // 单位512B
} Wav_t;

typedef struct
{
    unsigned char num_wav;
    unsigned char index;
    unsigned long last_p;
    char mode;
    unsigned char vol_reduce;
} play_t;

extern Wav_t WavList[8];
extern play_t player;

void Hello();
void SelectWavMode();
void PreWav();
void NextWav();
void PlayWav();
void PauseWav();
void StopWav();
void ChangPlayMode();
void AddVol();
void SubVol();
void DisVol();

void Display_Progress(Wav_t * Wav_p,play_t * play_p);

#endif //PLAYER_APP_H
