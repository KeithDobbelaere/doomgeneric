#ifndef PICOCALC_PRESENT_H
#define PICOCALC_PRESENT_H

#include <stdint.h>

#define PICOCALC_SCREEN_W 320
#define PICOCALC_SCREEN_H 320
#define PICOCALC_DOOM_X   0
#define PICOCALC_DOOM_Y   60

const uint16_t* PicoCalc_GetFrame565(void);
const uint16_t* PicoCalc_GetFrame565Row(int y);

void PicoCalc_BuildFrame565(const uint32_t* doomFrame, int doomW, int doomH);

#endif  // PICOCALC_PRESENT_H