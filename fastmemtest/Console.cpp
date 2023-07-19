#include "Console.h"
#include "NtPeb.h"
#include "peb.h"

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