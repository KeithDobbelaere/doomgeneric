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

void PicoCalc_BuildSlab565(
	uint16_t* dst,
	int dstY,
	int slabRows,
	const uint32_t* doomFrame,
	int doomW,
	int doomH)
{
	if (!dst || !doomFrame || doomW <= 0 || doomH <= 0 || slabRows <= 0)
	{
		return;
	}

	memset(dst, 0, (size_t)PICOCALC_SCREEN_W * (size_t)slabRows * sizeof(uint16_t));

	const int doomAreaTop = PICOCALC_DOOM_Y;
	const int doomAreaBottom = PICOCALC_DOOM_Y + doomH;

	const int slabTop = dstY;
	const int slabBottom = dstY + slabRows;

	if (slabBottom <= doomAreaTop || slabTop >= doomAreaBottom)
	{
		return;
	}

	const int copyW = doomW < (PICOCALC_SCREEN_W - PICOCALC_DOOM_X)
		? doomW
		: (PICOCALC_SCREEN_W - PICOCALC_DOOM_X);

	for (int localY = 0; localY < slabRows; ++localY)
	{
		const int screenY = dstY + localY;
		const int doomY = screenY - PICOCALC_DOOM_Y;

		if (screenY < 0 || screenY >= PICOCALC_SCREEN_H)
		{
			continue;
		}

		if (doomY < 0 || doomY >= doomH)
		{
			continue;
		}

		const uint32_t* src = &doomFrame[doomY * doomW];
		uint16_t* rowDst = &dst[localY * PICOCALC_SCREEN_W + PICOCALC_DOOM_X];

		for (int x = 0; x < copyW; ++x)
		{
			rowDst[x] = PicoCalc_Rgb888ToRgb565(src[x]);
		}
	}
}

void PicoCalc_BuildFrame565(const uint32_t* doomFrame, int doomW, int doomH)
{
	for (int y = 0; y < PICOCALC_SCREEN_H; y += PICOCALC_SLAB_ROWS)
	{
		int rows = PICOCALC_SLAB_ROWS;

		if (y + rows > PICOCALC_SCREEN_H)
		{
			rows = PICOCALC_SCREEN_H - y;
		}

		PicoCalc_BuildSlab565(
			&s_PicoCalcFrame565[y * PICOCALC_SCREEN_W],
			y,
			rows,
			doomFrame,
			doomW,
			doomH);
	}
}