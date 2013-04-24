#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <iostream>
#include "MyOGLApp.h"


CMyOGLApp cApp;

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
	#if _DEBUG
		AllocConsole();
		#pragma warning(disable : 4996)
		freopen("CONOUT$", "wt", stdout);
		#pragma warning(default: 4996)
	#endif
	// 640x480-as ablakot próbálunk létrehozni
	if (!cApp.Initialize(hInstance, 640, 480))
	{
		MessageBox(0, TEXT("Hiba az inicializálás során!"), TEXT("Hiba"), 0);
		return -1;
	}
	cApp.Run();

    return 0;
}