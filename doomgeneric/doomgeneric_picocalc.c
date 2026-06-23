#include "doomkeys.h"
#include "doomgeneric.h"
#include "picocalc_present.h"

#include <stdint.h>
#include <string.h>

static uint16_t s_PicoCalcSlab565[PICOCALC_SCREEN_W * PICOCALC_SLAB_ROWS];

static void PicoCalc_PlatformInit(void)
{
	// TODO: Pico SDK init:
	// - stdio / clocks
	// - GPIO
	// - SPI display
	// - DMA
	// - keyboard / input
	// - storage / filesystem
}

static void PicoCalc_PresentBeginFrame(void)
{
	// TODO: Optional hardware frame setup.
	// For ILI9488-style display, this may eventually set a full-screen
	// address window once per frame, or per slab depending on the driver.
}

static void PicoCalc_PresentSlab565(int screenY, const uint16_t* pixels, int rows)
{
	(void)screenY;
	(void)pixels;
	(void)rows;

	// TODO: Hardware present path:
	// - set address window: x=0, y=screenY, w=320, h=rows
	// - byte-swap RGB565 if required by display/SPI path
	// - start DMA transfer
	// - wait or ping-pong slab buffers
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
	(void)ms;

	// TODO: Pico SDK:
	// sleep_ms(ms);
}

uint32_t DG_GetTicksMs(void)
{
	// TODO: Pico SDK:
	// return to_ms_since_boot(get_absolute_time());

	return 0;
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

	// Return 0 when no key event is available.
	// Return 1 when an event has been written to *pressed and *key.
	return 0;
}

void DG_SetWindowTitle(const char* title)
{
	(void)title;

	// No-op on PicoCalc.
}