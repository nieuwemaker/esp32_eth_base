/*
 * ScreenController.h
 *
 *  Created on: January 15 2026
 *      Author: wvanvliet
 */
#define FASTLED_ALLOW_INTERRUPTS 0
#include "Arduino.h"
#include "globals.h" // contains vector and map header
#include <FastLED.h>
#include <algorithm>

#ifndef SCREENCONTROLLER_H_
#define SCREENCONTROLLER_H_

#define NUMBER_OF_PINS 8

class ScreenController{
    protected:
         static ScreenController* screenController_;
         ScreenController();
    public:
        static ScreenController* GetInstance();
        void
            Begin(),
            Stop(),
            Show(),
            Clear(),
            Update(),
            Test(),
            RequestFrameUpdate(),
            CreateLedMatrix(),
            SetLed(int x, int y, CRGB color);
        CRGB GetXYLed(int x, int y); // Shorthand for getting CRGB led on actual Curtain orientation
        int GetXY(int x, int y); // Shorthand for getting arrayindex of CRGB led on actual Curtain orientation
        
        CRGB Leds[8][LEDS_PER_PIN];
        std::vector<std::vector<int>> IndexTable; // 2D index lookup table for the CRGB
        
    private:
        // NUM 2 & 12 & 15 must be low when booting
        int ledPinList[NUMBER_OF_PINS] = {GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_5, GPIO_NUM_2, GPIO_NUM_4, GPIO_NUM_12,GPIO_NUM_14,GPIO_NUM_15}; 
        bool frameUpdateRequested;
};
#endif