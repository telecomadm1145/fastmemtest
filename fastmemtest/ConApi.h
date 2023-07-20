#pragma once
struct Coord
{
	short x;
	short y;
};
struct InputRecord
{
	enum class Event : ushort {
		KeyEvent = 0x1,
		MouseEvent = 0x2,
		BufferSizeEvent = 0x4,
		MenuEvent = 0x8,
		FocusEvent = 0x10,
	} EventType;
	union {
		struct {
			int bSetFocus;
		} FocusEvent;
		struct {
			int bKeyDown;
			ushort wRepeatCount;
			ushort wVirtualKeyCode;
			ushort wVirtualScanCode;
			wchar_t UnicodeChar;
			uint dwControlKeyState;
		} KeyEvent;
		struct {
			Coord dwMousePosition;
			uint dwButtonState;
			uint dwControlKeyState;
			uint dwEventFlags;
		} MouseEvent;
		struct {
			uint dwCommandId;
		} MenuEvent;
		struct {
			Coord dwSize;
		} BufferSizeEvent;
	};
};
bool SetConsoleCursorPosition(void* hConsoleOutput, Coord dwCursorPosition);

bool ReadConsoleInput(void* hConsoleInput, InputRecord* lpBuffer, int nLength, uint* lpNumberOfEventsRead);

bool WriteConsoleA(void* hConsoleOutput, const char* lpBuffer, ulong nNumberOfCharsToWrite, ulong* lpNumberOfCharsWritten);

bool WriteConsoleW(void* hConsoleOutput, const wchar_t* lpBuffer, ulong nNumberOfCharsToWrite, ulong* lpNumberOfCharsWritten);

bool SetConsoleTitleA(const char* lpConsoleTitle);

bool SetConsoleTitleW(const wchar_t* lpConsoleTitle);

bool SetConsoleTextAttribute(void* hConsoleOutput, ushort wAttributes);

bool SetConsoleMode(void* hConsoleHandle, ulong dwMode);

bool SetConsoleOutputCP(uint wCodePageID);

bool SetConsoleCP(uint wCodePageID);

bool SetConsoleFont(void* hConsoleOutput, bool bMaximumWindow, uint nFont, Coord dwFontSize, uint FontFamily, uint FontWeight, const wchar_t facename[38]);

bool SetConsoleScreenBufferSize(void* hConsoleOutput, Coord dwSize);

bool ReadConsoleInput(void* hConsoleInput, InputRecord* lpBuffer, int nLength, uint* lpNumberOfEventsRead);
