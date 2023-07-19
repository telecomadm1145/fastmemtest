#include "Console.h"
struct InitConsole {
	InitConsole()
	{
		System::Console::Load();
	}
} ___InitConsole;