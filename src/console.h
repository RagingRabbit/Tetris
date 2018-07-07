#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include <string>
#include <conio.h>
#include <chrono>

#ifndef _MSC_VER
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008

BOOL WINAPI GetCurrentConsoleFontEx(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
#endif

#define DLL_EXPORT extern "C" __declspec(dllexport)

#define ESC "\x1b"
#define CSI "\x1b["
#define OSC "\x1b]"
#define BEL "\x07"

#define USE_ALTERNATE_BUFFER CSI "?1049h"
#define USE_MAIN_BUFFER CSI "?1049l"
#define SOFT_RESET CSI "!p"

#define ENABLE_CURSOR_BLINK CSI "?12h"
#define DISABLE_CURSOR_BLINK CSI "?12l"
#define SHOW_CURSOR CSI "?25h"
#define HIDE_CURSOR CSI "?25l"
#define SAVE_CURSOR CSI "s"
#define RESTORE_CURSOR CSI "u"
#define CURSOR_UP ESC "A"
#define CURSOR_DOWN ESC "B"
#define CURSOR_RIGHT ESC "C"
#define CURSOR_LEFT ESC "D"
#define CURSOR_NEXT_LINE CSI "E"
#define CURSOR_PREVIOUS_LINE CSI "F"
#define CURSOR_UP_N(n) CSI #n "A"
#define CURSOR_DOWN_N(n) CSI #n "B"
#define CURSOR_RIGHT_N(n) CSI #n "C"
#define CURSOR_LEFT_N(n) CSI #n "D"
#define CURSOR_NEXT_LINE_N(n) CSI #n "E"
#define CURSOR_PREVIOUS_LINE_N(n) CSI #n "F"
#define CURSOR_SET_XY(x, y) CSI + std::to_string(y) + ";" + std::to_string(x) + "H"

#define FORMAT_NONE CSI "0m"
#define FORMAT_BRIGHT CSI "1m"
#define FORMAT_UNDERLINE CSI "4m"
#define FORMAT_NO_UNDERLINE CSI "24m"
#define FORMAT_NEGATIVE CSI "7m"
#define FORMAT_NO_NEGATIVE CSI "27m"

#define RESET_FOREGROUND CSI "39m"
#define RESET_BACKGROUND CSI "49m"

#define ENABLE_LINE_DRAWING_MODE ESC "(0"
#define DISABLE_LINE_DRAWING_MODE ESC "(B"
#define LINE_BOTTOM_RIGHT "j"
#define LINE_TOP_RIGHT "k"
#define LINE_TOP_LEFT "l"
#define LINE_BOTTOM_LEFT "m"
#define LINE_MIDDLE "n"
#define LINE_HORIZONTAL "q"
#define LINE_MID_LEFT "t"
#define LINE_MID_RIGHT "u"
#define LINE_MID_BOTTOM "v"
#define LINE_MID_TOP "w"
#define LINE_VERTICAL "x"

#define CLEAR_SCREEN CSI "2J"
#define CLEAR_LINE CSI "K"
#define NEXT_LINE CSI "E"
#define INSERT_LINES(n) CSI #n "L"
#define DELETE_LINES(n) CSI #n "M"
#define SCROLL_UP(n) CSI #n "S"
#define SCROLL_DOWN(n) CSI #n "T"

bool running = false;

CONSOLE_FONT_INFOEX defaultFont;
CONSOLE_SCREEN_BUFFER_INFO defaultScreen;
COORD defaultSize;

bool keys[0xFF];
float offtimers[0xFF];
float keytimers[0xFF];
long long last;

float keyrepeatrate = 0.06f;
float keyrepeatdelay = 0.2f;

extern "C" __declspec(dllexport)
inline void set_title(const char *c)
{
	printf(OSC "2;%s" BEL, c);
}

extern "C" __declspec(dllexport)
inline void set_cursor_pos(int x, int y)
{
	printf(CSI "%d;%dH", y + 1, x + 1);
}

extern "C" __declspec(dllexport)
inline void put_char(int x, int y, char c, int f, int b)
{
	printf(CSI "%d;%dH" CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%c",
		y + 1,
		x + 1,
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

inline void put_char(int x, int y, char c)
{
	printf(CSI "%d;%dH%c", y + 1, x + 1, c);
}

inline void put_char(char c, int f, int b)
{
	printf(CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%c",
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

inline void put_char(char c)
{
	printf("%c", c);
}

extern "C" __declspec(dllexport) inline void put_char_xy(int x, int y, char c) { put_char(x, y, c); }
extern "C" __declspec(dllexport) inline void put_char_c(char c) { put_char(c); }

extern "C" __declspec(dllexport)
inline void print_s(int x, int y, const char *c, int f, int b)
{
	printf(CSI "%d;%dH" CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%s",
		y + 1,
		x + 1,
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

inline void print_s(int x, int y, const char *c)
{
	printf(CSI "%d;%dH%s", y + 1, x + 1, c);
}

inline void print_s(const char *c)
{
	printf(c);
}

extern "C" __declspec(dllexport) inline void print_s_xy(int x, int y, const char *c) { print_s(x, y, c); }
extern "C" __declspec(dllexport) inline void print_s_c(const char *c) { print_s(c); }

extern "C" __declspec(dllexport)
inline void set_color(int f, int b)
{
	printf(CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm",
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff));
}

extern "C" __declspec(dllexport)
inline long long get_nanos()
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

extern "C" __declspec(dllexport)
inline void init_scr()
{
#ifdef _WIN32
	DWORD dwMode = 0;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &dwMode);

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	dwMode |= DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), dwMode);

	defaultFont.cbSize = sizeof(defaultFont);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &defaultFont);

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &defaultScreen);

	for (int i = 0; i < 0xFF; i++)
	{
		keys[i] = false;
		offtimers[i] = -1.0f;
		keytimers[i] = -1.0f;
	}
	last = get_nanos();
#endif
}

extern "C" __declspec(dllexport)
inline void end_scr()
{
#ifdef _WIN32
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &defaultFont);
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), defaultScreen.dwSize);
	SMALL_RECT tmpRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmpRect);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &defaultScreen.srWindow);
#endif
}

extern "C" __declspec(dllexport)
inline void set_font(int w, int h, const wchar_t *c)
{
#ifdef _WIN32
	CONSOLE_FONT_INFOEX font = defaultFont;
	font.dwFontSize.X = (short)w;
	font.dwFontSize.Y = (short)h;
	font.FontWeight = 400;
	font.FontFamily = 0;
	wcscpy(font.FaceName, c);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
#endif
}

extern "C" __declspec(dllexport)
inline void set_size(int w, int h)
{
#ifdef _WIN32
	SMALL_RECT rect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { (SHORT)w, (SHORT)h });

	SHORT ww = (short)w;
	SHORT hh = (short)h;
	rect.Right = ww - 1;
	rect.Bottom = hh - 1;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);
#endif
}

inline void flush_input()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

extern "C" __declspec(dllexport)
inline int get_width()
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO buffer = {};
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer);
	return buffer.dwSize.X;
#endif
}

extern "C" __declspec(dllexport)
inline int get_height()
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO buffer = {};
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer);
	return buffer.dwSize.Y;
#endif
}

extern "C" __declspec(dllexport)
void set_repeat_delay(float delay)
{
	keyrepeatdelay = delay;
}

extern "C" __declspec(dllexport)
void set_repeat_rate(float rate)
{
	keyrepeatrate = rate;
}

extern "C" __declspec(dllexport)
void poll_events(void(p_key)(int, bool))
{
#ifdef _WIN32
	INPUT_RECORD records[32];
	DWORD numevents = 0;
	GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numevents);
	if (numevents > 0)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), records, numevents, &numevents);

		for (DWORD i = 0; i < numevents; i++)
		{
			if (records[i].EventType == KEY_EVENT)
			{
				//p_key(records[i].Event.KeyEvent.wVirtualKeyCode, records[i].Event.KeyEvent.bKeyDown);

				WORD code = records[i].Event.KeyEvent.wVirtualKeyCode;
				BOOL down = records[i].Event.KeyEvent.bKeyDown;

				if (!keys[code] && down)
				{
					keys[code] = true;
					offtimers[code] = 0.0f;
				}
				else if (keys[code] && !down)
				{
					keys[code] = false;
					offtimers[code] = -1.0f;
					keytimers[code] = -1.0f;
					p_key(code, false);
				}
			}
		}
	}

	long long now = get_nanos();
	float delta = (now - last) / 1e9f;
	last = now;

	for (int i = 0; i < 0xFF; i++)
	{
		if (offtimers[i] >= 0.0f)
		{
			if (offtimers[i] == 0.0f)
			{
				p_key(i, true);
				offtimers[i] += delta;
			}
			else if (offtimers[i] > keyrepeatdelay)
			{
				keytimers[i] = 0.0f;
				offtimers[i] = -1.0f;
			}
			else
			{
				offtimers[i] += delta;
			}
		}
		if (keytimers[i] >= 0.0f)
		{
			keytimers[i] += delta;
			if (keytimers[i] >= keyrepeatrate)
			{
				keytimers[i] = 0.0f;
				p_key(i, true);
			}
		}
	}
#endif
}

extern "C" __declspec(dllexport)
inline void run_loop(const char *title, void(p_init)(void), void(p_update)(float), void(p_end)(void), void(p_key)(int, bool))
{
	long long lastsec = get_nanos();
	long long last = get_nanos();
	long long now = get_nanos();
	int frames = 0;

	running = true;
	p_init();

	while (running)
	{
		now = get_nanos();
		long long delta = now - last;
		last = now;
		if (now - lastsec > 1e9)
		{
			set_title((std::string(title) + "  -  " + std::to_string(frames)).c_str());
			frames = 0;
			lastsec = now;
		}

		poll_events(p_key);

		p_update(delta / 1e9f);
		frames++;
	}

	p_end();
}

extern "C" __declspec(dllexport)
inline void stop_loop()
{
	running = false;
}