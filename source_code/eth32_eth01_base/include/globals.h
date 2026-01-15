
// Pixel values
#define NUM_PIX_WIDTH  32
#define NUM_PIX_HEIGHT 32
#define NUM_PIX_TOTAL  (NUM_PIX_HEIGHT * NUM_PIX_WIDTH)
#define SCREENS_H      2
#define SCREENS_V      2
#define SCREENS_TOTAL  (SCREENS_H * SCREENS_V)
#define MEMORY_SIZE    ((SCREENS_TOTAL * NUM_PIX_TOTAL) * 3 / 4)
#define ONE_SECOND_CYCLES 240000000  // 600Mhz processor
#define ONE_MS_CYCLES     ONE_SECOND_CYCLES / 1000
#define FRAMERATE         60  // 50 fps
#define ONE_FRAME_CYCLES  (ONE_MS_CYCLES * 1000) / FRAMERATE // Weird notation to prevent floating numbers. It is equal to (1000/framerate)*one_ms_cycle ms per frame * amount of clock cycles

#define LEDS_PER_PIN   NUM_PIX_TOTAL * 2 // amount of pixels we can handle per pin