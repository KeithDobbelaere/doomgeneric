#include "doomkeys.h"
#include "doomgeneric.h"
#include "picocalc_present.h"

#include "pico/stdlib.h"
#include "pico/time.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint16_t s_PicoCalcSlab565[PICOCALC_SCREEN_W * PICOCALC_SLAB_ROWS];

static void PicoCalc_PlatformInit(void)
{
	stdio_init_all();

	// Give USB stdio a moment to enumerate when connected.
	sleep_ms(1500);

	printf("PicoCalc Doom backend init\n");

	// TODO:
	// - GPIO
	// - SPI display
	// - DMA
	// - keyboard/input
	// - storage/filesystem
}

static void PicoCalc_PresentBeginFrame(void)
{
	// TODO: Optional display frame setup.
}

static void PicoCalc_PresentSlab565(int screenY, const uint16_t* pixels, int rows)
{
	(void)screenY;
	(void)pixels;
	(void)rows;

	// TODO:
	// - set display address window
	// - byte-swap if needed
	// - DMA RGB565 slab to SPI
}

static void PicoCalc_PresentEndFrame(void)
{
	// TODO: Optional end-of-frame hook.
}

void DG_Init(void)
{
	PicoCalc_PlatformInit();
}

void DG_DrawFrame(void)
{
	PicoCalc_PresentBeginFrame();

	for (int y = 0; y < PICOCALC_SCREEN_H; y += PICOCALC_SLAB_ROWS)
	{
		int rows = PICOCALC_SLAB_ROWS;

		if (y + rows > PICOCALC_SCREEN_H)
		{
			rows = PICOCALC_SCREEN_H - y;
		}

		PicoCalc_BuildSlab565(
			s_PicoCalcSlab565,
			y,
			rows,
			DG_ScreenBuffer,
			DOOMGENERIC_RESX,
			DOOMGENERIC_RESY);

		PicoCalc_PresentSlab565(y, s_PicoCalcSlab565, rows);
	}

	PicoCalc_PresentEndFrame();
}

void DG_SleepMs(uint32_t ms)
{
	sleep_ms(ms);
}

uint32_t DG_GetTicksMs(void)
{
	return (uint32_t)to_ms_since_boot(get_absolute_time());
}

int DG_GetKey(int* pressed, unsigned char* key)
{
	if (pressed)
	{
		*pressed = 0;
	}

	if (key)
	{
		*key = 0;
	}

	return 0;
}

void DG_SetWindowTitle(const char* title)
{
	(void)title;
}

int main(void)
{
	static char* argv[] =
	{
		"doomgeneric",
		"-iwad",
		"doom.wad"
	};

	const int argc = (int)(sizeof(argv) / sizeof(argv[0]));

	doomgeneric_Create(argc, argv);

	while (1)
	{
		doomgeneric_Tick();
	}

	return 0;
}