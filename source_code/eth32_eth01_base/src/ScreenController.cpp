#include "ScreenController.h"

ScreenController* ScreenController::screenController_ = nullptr;

ScreenController* ScreenController::GetInstance(){
    if(screenController_ == nullptr){
        screenController_ = new ScreenController();
    }
    return screenController_;
}


/////////////////////////////////////////////////////////////////////////////////
// CURTAINCONTROLLER Methods
/////////////////////////////////////////////////////////////////////////////////

ScreenController::ScreenController() {
    this->CreateLedMatrix();
    this->frameUpdateRequested = true;
}
/**
 * Default the Leds array contains the Led data Per Curtain, so every curtain has 400 pixels and is in TDLR orientation
 * It always assumes the screens are next to eachother, which creates space for only 1 row of curtains.
*/
void ScreenController::CreateLedMatrix(){
    this->IndexTable.clear();
    for(uint8_t y=0;y<NUM_PIX_HEIGHT*SCREENS_V;y++){
        std::vector<int> indexRow;
        uint8_t curtY = y / 20;
        for(uint8_t x=0;x<NUM_PIX_WIDTH*SCREENS_H;x++){
            uint8_t curtX = x / 20;
            int index = 0;
            /*
            if(orientation == ORIENTATION_TDLR){
                index = (x * 20) + (curtX * ((this->CurtData.VertCurtainCount-1) * 400)) + (curtY * 400) + (y%20);
            } else if(orientation == ORIENTATION_TDRL){
                //index = 0;
                uint8_t inCurtX = x % 20; // X coordinate within a curtain
                index = (LEDS_PER_CURTAIN - CURTAIN_LEDS_LENGTH - (inCurtX * 20)) + (curtX * LEDS_PER_CURTAIN) +y; // simple version
                //index = (LEDS_PER_CURTAIN - CURTAIN_LEDS_LENGTH - (x * 20)) + (curtX * ((this->CurtData.VertCurtainCount-1) * 400)) + (curtY * 400) + (y%20);
            }
                */
            // TO DO: Support all other orientations ... Bu not NOW! 
            indexRow.push_back(index);
        }
        this->IndexTable.push_back(indexRow);
    }
}

void ScreenController::Begin(){
    //for(uint8_t i=0;i<NUMBER_OF_PINS;i++){
        //FastLED.addLeds<WS2812B, this->ledPinList[i]>(this->Leds[i], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    //}
    FastLED.addLeds<WS2812Controller800Khz, GPIO_NUM_32,GBR>(this->Leds[0], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_33>(this->Leds[1], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_5>(this->Leds[2], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_2>(this->Leds[3], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_4>(this->Leds[4], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_12>(this->Leds[5], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_14>(this->Leds[6], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812, GPIO_NUM_15>(this->Leds[7], LEDS_PER_PIN).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(200);
    FastLED.clear();
    this->Update();
}

void ScreenController::Update(){
    if(this->frameUpdateRequested){
        this->Show();
    }
}

void ScreenController::Show(){
    FastLED.show(); // Very time consuming operation
    this->frameUpdateRequested = false;
}

void ScreenController::Clear(){
    FastLED.clear();
    this->RequestFrameUpdate();
}

void ScreenController::Stop(){

}

void ScreenController::Test(){
    FastLED.clear();
    for(uint8_t i=0;i<NUMBER_OF_PINS;i++){
        for(uint8_t j=0;j<30;j++){
            int randomIndex = random(LEDS_PER_PIN);
            this->Leds[i][randomIndex] = CRGB::White;
        }
    }
    this->RequestFrameUpdate();
}

/**
 * Directly set the value of a led in a 2d space, based on the current resolution
 */
void ScreenController::SetLed(int x, int y, CRGB color){
    //this->Leds[ this->GetXY(x,y)] = color;
}

int ScreenController::GetXY(int x, int y){
    return this->IndexTable[y][x];
}

CRGB ScreenController::GetXYLed(int x, int y){
    return CRGB::AliceBlue;
    //return this->Leds[this->IndexTable[y][x]];
}

void ScreenController::RequestFrameUpdate(){
    this->frameUpdateRequested = true;
}