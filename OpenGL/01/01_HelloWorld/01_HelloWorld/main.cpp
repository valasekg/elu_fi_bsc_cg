#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance,	// aktu�lis alk. p�ld�ny
				    HINSTANCE hPrevInstance,// el�z� p�ld�ny
					LPSTR cmdLine,			// parancssor
					int nShow)				// megjelen�t�si param
{
	MessageBox(0,						// sz�l� ablak handle-je
			   TEXT("Hello vil�g!"),	// az �zenet sz�vege
			   TEXT("�zenet"),			// az ablak felirata
			   MB_OK);					// a gombok

	return 0;
}