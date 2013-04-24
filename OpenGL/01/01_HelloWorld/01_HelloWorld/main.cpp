#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance,	// aktuális alk. példány
				    HINSTANCE hPrevInstance,// elõzõ példány
					LPSTR cmdLine,			// parancssor
					int nShow)				// megjelenítési param
{
	MessageBox(0,						// szülõ ablak handle-je
			   TEXT("Hello világ!"),	// az üzenet szövege
			   TEXT("Üzenet"),			// az ablak felirata
			   MB_OK);					// a gombok

	return 0;
}