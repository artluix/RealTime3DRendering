#include "DemoApplication.h"

#include <library/Utils.h>

#include <memory>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto app = std::make_unique<DemoApplication>(
		instance,
		L"RenderingClass",
		L"RealTime3DRendering",
		showCmd
	);

	app->Run();

	return 0;
}
