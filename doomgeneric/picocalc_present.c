#include "picocalc_present.h"

#include <string.h>

static uint16_t s_PicoCalcFrame565[PICOCALC_SCREEN_W * PICOCALC_SCREEN_H];

static uint16_t PicoCalc_Rgb888ToRgb565(uint32_t color)
{
	const uint8_t r = (uint8_t)((color >> 16) & 0xFF);
	const uint8_t g = (uint8_t)((color >> 8) & 0xFF);
	const uint8_t b = (uint8_t)(color & 0xFF);

	return (uint16_t)(((r & 0xF8) << 8) |
		((g & 0xFC) << 3) |
		(b >> 3));
}

const uint16_t* PicoCalc_GetFrame565(void)
{
	return s_PicoCalcFrame565;
}

const uint16_t* PicoCalc_GetFrame565Row(int y)
{
	if (y < 0)
	{
		y = 0;
	}
	else if (y >= PICOCALC_SCREEN_H)
	{
		y = PICOCALC_SCREEN_H - 1;
	}

	return &s_PicoCalcFrame565[y * PICOCALC_SCREEN_W];
}

void PicoCalc_BuildFrame565(const uint32_t* doomFrame, int doomW, int doomH)
{
	memset(s_PicoCalcFrame565, 0, sizeof(s_PicoCalcFrame565));

	const int maxCopyW = PICOCALC_SCREEN_W - PICOCALC_DOOM_X;
	const int maxCopyH = PICOCALC_SCREEN_H - PICOCALC_DOOM_Y;

	const int copyW = doomW < maxCopyW ? doomW : maxCopyW;
	const int copyH = doomH < maxCopyH ? doomH : maxCopyH;

	for (int y = 0; y < copyH; ++y)
	{
		const uint32_t* src = &doomFrame[y * doomW];
		uint16_t* dst = &s_PicoCalcFrame565[(PICOCALC_DOOM_Y + y) * PICOCALC_SCREEN_W + PICOCALC_DOOM_X];

		for (int x = 0; x < copyW; ++x)
		{
			dst[x] = PicoCalc_Rgb888ToRgb565(src[x]);
		}
	}
}