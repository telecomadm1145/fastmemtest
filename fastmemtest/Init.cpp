#include "Array.h"
#include "CrtInit.h"
#include "CrtExit.h"
#include "App.h"

#include "Dll.h"

namespace nt::internal
{
	void Main()
	{
		InitGlobalVars();
		App* userApp;
		nt::App::Create(userApp);
		userApp->Startup();
		exit(0);
	}
}

long NtSDKStartup()
{
	using namespace nt::internal;
	Main();
	return 0;
}