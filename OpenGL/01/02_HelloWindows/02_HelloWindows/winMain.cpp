#include <windows.h>

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR szCmdLine,
					int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("HelloWin"); // alkalmazás neve
	static TCHAR szWindowClassName[] = TEXT( "MyWndClassFTW" );

    HWND         hwnd;		// ablak handle
    MSG          msg;		// üzenet
	WNDCLASSEX   wndclass;  // ablak osztály

	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));
	wndclass.cbSize			= sizeof( WNDCLASSEX );
	wndclass.lpfnWndProc	= WndProc;
	wndclass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance		= GetModuleHandle(NULL);
	wndclass.lpszClassName	= szWindowClassName;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance		= hInstance;
	

	if (!RegisterClassEx(&wndclass)) 
	{
		MessageBox(0, TEXT("Nem sikerült létrehozni az ablakot!"), TEXT("Hiba!"), MB_OK );
		return -1;
	}

	hwnd = CreateWindowEx(	WS_EX_OVERLAPPEDWINDOW,			// létrehozandó ablak stílusa, ld. http://msdn.microsoft.com/en-us/library/ff700543%28v=vs.85%29.aspx
							szWindowClassName,				// ablakosztály neve
							szAppName,						// az ablak felirata
							WS_OVERLAPPEDWINDOW,			// ablak stílusa, ld. http://msdn.microsoft.com/en-us/library/ms632600%28v=vs.85%29.aspx
							CW_USEDEFAULT, CW_USEDEFAULT,	// ablak kezdeti X és Y koordinátái
							640, 480,						// ablak kezdeti szélessége (640) és magassága (480)
							NULL,							// ablak szülõjének leírója
							NULL,							// ablak menüjének leírója (HMENU)
							hInstance,						// programunk példányának leírója
							NULL );							// ablakhoz létrehozásakor küldendõ egyéb adatok

    ShowWindow (hwnd, iCmdShow) ;
    UpdateWindow (hwnd) ;
     
    while (GetMessage (&msg, NULL, 0, 0))
    {
         TranslateMessage (&msg) ;
         DispatchMessage (&msg) ;
    }

    return msg.wParam ;
}

LRESULT CALLBACK WndProc( HWND hwnd, 
						  UINT message, 
						  WPARAM wParam, 
						  LPARAM lParam )
{
    HDC         hdc ;
    PAINTSTRUCT ps ;
    RECT        rect ;
     
    switch (message)
    {
    case WM_CREATE:
		 MessageBox(0, 
					TEXT("Helló! Most indul a program!"), 
					TEXT("Helló címke"), 
					MB_OK);
         return 0;

    case WM_PAINT:
         hdc = BeginPaint (hwnd, &ps);
          
         GetClientRect (hwnd, &rect) ;
          
         DrawText (hdc, 
				   TEXT ("Hello Windows!"), 
				   -1, 
				   &rect,
                   DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
         EndPaint (hwnd, &ps) ;
         return 0 ;
          
    case WM_DESTROY:
		 MessageBox(0, TEXT("Most ér véget a program"), TEXT("Üzenet"), MB_OK );
         PostQuitMessage (0) ;
         return 0 ;
    }

    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
