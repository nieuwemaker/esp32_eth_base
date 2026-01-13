/*
 * PerformanceData.h
 *
 *  Created on: December 27 2023
 *      Author: wvanvliet
 */
#include "Arduino.h"
#include <map>
#ifndef PERFORMANCEDATA_H_
#define PERFORMANCEDATA_H_

// Variables needed to check how much RAM2 is still available
extern unsigned long _heap_start;
extern unsigned long _heap_end;
extern char *__brkval;
extern char _ebss[];

class PerformanceData{
    protected:
        PerformanceData();
        static PerformanceData* performanceData_;
    public:
        static PerformanceData* GetInstance();
        void
            ResetCounters(),
            CountFPS(),
            CountLoop(),
            CalcFreeMem(),
            CountTPM2();
        std::map<String,int> Export();
        int FpsCounter; // Counter for measuring current flowing FPS
        int LastFps;    // After 1 second has passed the FPS value is saved for 1 second
        int LoopCounter;// Counter for measuring the performance of the CPU. More cycles per second is more free space for processor
        int LastLoop;   // After 1 second the value to remember
        int Tpm2Counter;// Counter for measuring the amount of incoming TPM messages
        int FreeMem;    
        int MEM1;
        int MEM2;
    private:

};
#endif