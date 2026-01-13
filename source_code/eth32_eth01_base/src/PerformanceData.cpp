#include "PerformanceData.h"

PerformanceData* PerformanceData::performanceData_ = nullptr;

PerformanceData* PerformanceData::GetInstance(){
    if(performanceData_ == nullptr){
        performanceData_ = new PerformanceData();
    }
    return performanceData_;
}

PerformanceData::PerformanceData(){
    this->FpsCounter  = 0;
    this->LoopCounter = 0;
    this->LastFps     = 1;
}

void PerformanceData::CountFPS(){
    this->FpsCounter++;
}

void PerformanceData::CountLoop(){
    this->LoopCounter++;
}

void PerformanceData::CountTPM2(){
    this->Tpm2Counter++;
}

void PerformanceData::ResetCounters(){
    this->LastFps     = this->FpsCounter; // save the value for the animations
    this->FpsCounter  = 0;
    this->LastLoop    = this->LoopCounter;
    this->LoopCounter = 0;
    this->Tpm2Counter = 0;
    this->CalcFreeMem(); // update value
}

// Source of code is: https://forum.pjrc.com/index.php?threads/how-to-display-free-ram.33443/
// and: https://forum.pjrc.com/index.php?threads/how-to-display-free-ram.33443/
void PerformanceData::CalcFreeMem(){
    /* THIS IS BASED ON TEENSY. NEED TO FIND ESP32 VERSION
    char* sp = __builtin_frame_address(0);
    char* p = (char*) malloc(10000); // size should be quite big, to avoid allocating fragment!
    free(p);
    this->MEM1 = sp - _ebss;
    this->MEM2 = (char *)&_heap_end - p;//__brkval;*/
    //this->FreeMem = (char *)&_heap_end - __brkval;
}

std::map<String,int> PerformanceData::Export(){
    std::map<String,int> values;
    values.insert({"FPS",this->LastFps});
    values.insert({"Cycles",this->LastLoop});
    return values;
    /*
  if(Serial){
    Serial.printf("LOOP Cycles: %d\tCURT FPS: %d\tRAM1: %8d\tRAM2: %8d\tTPMPACK: %d\n",this->LastLoop,this->LastFps, this->MEM1, this->MEM2, this->Tpm2Counter);
  }
    */
}