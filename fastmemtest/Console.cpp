#include "Console.h"
#include "NtPeb.h"
#include "peb.h"
#include "ConApi.h"

System::IO::Stream* System::Console::pStdIn = 0;
System::IO::Stream* System::Console::pStdErr = 0;
System::IO::Stream* System::Console::pStdOut = 0;


void System::Console::Load()
{
	auto peb = (nt::native::Peb*)nt::native::NtCurrentPeb();
	if (pStdIn == 0)
		pStdIn = new IO::FileStream(peb->ProcessParameters->StdIn,IO::FileAccess::Read);
	if (pStdOut == 0)
		pStdOut = new IO::FileStream(peb->ProcessParameters->StdOut, IO::FileAccess::Write);
	if (pStdErr == 0)
		pStdErr = new IO::FileStream(peb->ProcessParameters->StdErr, IO::FileAccess::Write);
}

System::ConsoleKeyEvent System::Console::ReadKey()
{
	using namespace nt::native;
	auto peb = (Peb*)NtCurrentPeb();
	InputRecord rec;
	while (true) {
		ReadConsoleInput(peb->ProcessParameters->StdIn, &rec, 1, 0);
		switch (rec.EventType)
		{
		case InputRecord::Event::KeyEvent:
		{
			if (rec.KeyEvent.bKeyDown)
			{
				ConsoleKeyEvent cke;
				cke.VirtualKey = rec.KeyEvent.wVirtualKeyCode;
				cke.ControlKeyState = rec.KeyEvent.dwControlKeyState;
				cke.RepeatCount = rec.KeyEvent.wRepeatCount;
				cke.UnicodeChar = rec.KeyEvent.UnicodeChar;
				return cke;
			}
			break;
		}
		default:
			break;
		}
	}
}

void System::Console::SetTitle(const char* title)
{
	SetConsoleTitleA(title);
}
