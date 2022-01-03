//
// Created by 廖锦程 on 2021/12/30.
//

#ifndef RECORD_APP_H
#define RECORD_APP_H

#define SARTREC ADC10CTL0 |= ENC + ADC10SC   // Sampling and conversion start

void RecInit();
void PauseRec();
void StartRec();
void StopRec();
void ResumeRec();

void RecordMode();

#endif //RECORD_APP_H
