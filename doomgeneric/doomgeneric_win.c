#include "doomkeys.h"

#include "doomgeneric.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <Windows.h>

#define PICOCALC_SCREEN_W 320
#define PICOCALC_SCREEN_H 320
#define PICOCALC_DOOM_X   0
#define PICOCALC_DOOM_Y   60

static BITMAPINFO s_Bmi = { sizeof(BITMAPINFOHEADER), PICOCALC_SCREEN_W, -PICOCALC_SCREEN_H, 1, 32 };
static uint16_t s_PicoCalcFrame565[PICOCALC_SCREEN_W * PICOCALC_SCREEN_H];
static uint32_t s_Win32PreviewFrame[PICOCALC_SCREEN_W * PICOCALC_SCREEN_H];
static HWND s_Hwnd = 0;
static HDC s_Hdc = 0;


#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned char key)
{
	switch (key)
	{
	case VK_RETURN:
		key = KEY_ENTER;
		break;
	case VK_ESCAPE:
		key = KEY_ESCAPE;
		break;
	case VK_LEFT:
		key = KEY_LEFTARROW;
		break;
	case VK_RIGHT:
		key = KEY_RIGHTARROW;
		break;
	case VK_UP:
		key = KEY_UPARROW;
		break;
	case VK_DOWN:
		key = KEY_DOWNARROW;
		break;
	case VK_CONTROL:
		key = KEY_FIRE;
		break;
	case VK_SPACE:
		key = KEY_USE;
		break;
	case VK_SHIFT:
		key = KEY_RSHIFT;
		break;
	default:
		key = tolower(key);
		break;
	}

	return key;
}

static void addKeyToQueue(int pressed, unsigned char keyCode)
{
	unsigned char key = convertToDoomKey(keyCode);

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	case WM_KEYDOWN:
		addKeyToQueue(1, wParam);
		break;
	case WM_KEYUP:
		addKeyToQueue(0, wParam);
		break;
	default:
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
	return 0;
}

static uint16_t PicoCalc_Rgb888ToRgb565(uint32_t color)
{
	const uint8_t r = (uint8_t)((color >> 16) & 0xFF);
	const uint8_t g = (uint8_t)((color >> 8) & 0xFF);
	const uint8_t b = (uint8_t)(color & 0xFF);

	return (uint16_t)(((r & 0xF8) << 8) |
		((g & 0xFC) << 3) |
		(b >> 3));
}

static uint32_t Win32_Rgb565ToRgb888(uint16_t color)
{
	uint32_t r = (color >> 11) & 0x1F;
	uint32_t g = (color >> 5) & 0x3F;
	uint32_t b = color & 0x1F;

	r = (r << 3) | (r >> 2);
	g = (g << 2) | (g >> 4);
	b = (b << 3) | (b >> 2);

	return (r << 16) | (g << 8) | b;
}

static void PicoCalc_BuildPreviewFrame()
{
	memset(s_PicoCalcFrame565, 0, sizeof(s_PicoCalcFrame565));

	for (int y = 0; y < DOOMGENERIC_RESY; ++y)
	{
		const uint32_t* src = &DG_ScreenBuffer[y * DOOMGENERIC_RESX];
		uint16_t* dst = &s_PicoCalcFrame565[(PICOCALC_DOOM_Y + y) * PICOCALC_SCREEN_W + PICOCALC_DOOM_X];

		for (int x = 0; x < DOOMGENERIC_RESX; ++x)
		{
			dst[x] = PicoCalc_Rgb888ToRgb565(src[x]);
		}
	}
}

static void Win32_BuildPreviewFrame()
{
	for (int i = 0; i < PICOCALC_SCREEN_W * PICOCALC_SCREEN_H; ++i)
	{
		s_Win32PreviewFrame[i] = Win32_Rgb565ToRgb888(s_PicoCalcFrame565[i]);
	}
}

static void Win32_PresentPicoCalcFrame()
{
	StretchDIBits(
		s_Hdc,
		0, 0,
		PICOCALC_SCREEN_W, PICOCALC_SCREEN_H,
		0, 0,
		PICOCALC_SCREEN_W, PICOCALC_SCREEN_H,
		s_Win32PreviewFrame,
		&s_Bmi,
		DIB_RGB_COLORS,
		SRCCOPY);

	SwapBuffers(s_Hdc);
}

static void Win32_PumpEvents()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void DG_Init()
{
	// window creation
	const char windowClassName[] = "DoomWindowClass";
	const char windowTitle[] = "Doom";
	WNDCLASSEXA wc;

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = 0;
	wc.lpfnWndProc = wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = 0;

	if (!RegisterClassExA(&wc))
	{
		printf("Window Registration Failed!");

		exit(-1);
	}

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = PICOCALC_SCREEN_W;
	rect.bottom = PICOCALC_SCREEN_H;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowExA(0, windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0, 0);
	if (hwnd)
	{
		s_Hwnd = hwnd;

		s_Hdc = GetDC(hwnd);
		ShowWindow(hwnd, SW_SHOW);
	}
	else
	{
		printf("Window Creation Failed!");

		exit(-1);
	}

	memset(s_KeyQueue, 0, KEYQUEUE_SIZE * sizeof(unsigned short));
}

void DG_DrawFrame()
{
	Win32_PumpEvents();
	PicoCalc_BuildPreviewFrame();
	Win32_BuildPreviewFrame();
	Win32_PresentPicoCalcFrame();
}

void DG_SleepMs(uint32_t ms)
{
	Sleep(ms);
}

uint32_t DG_GetTicksMs()
{
	return GetTickCount();
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
	{
		//key queue is empty

		return 0;
	}
	else
	{
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

void DG_SetWindowTitle(const char * title)
{
	if (s_Hwnd)
	{
		SetWindowTextA(s_Hwnd, title);
	}
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
    }
    

    return 0;
}