#include <windows.h>

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR szCmdLine,
					int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("HelloWin"); // alkalmaz�s neve
	static TCHAR szWindowClassName[] = TEXT( "MyWndClassFTW" );

    HWND         hwnd;		// ablak handle
    MSG          msg;		// �zenet
	WNDCLASSEX   wndclass;  // ablak oszt�ly

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
		MessageBox(0, TEXT("Nem siker�lt l�trehozni az ablakot!"), TEXT("Hiba!"), MB_OK );
		return -1;
	}

	hwnd = CreateWindowEx(	WS_EX_OVERLAPPEDWINDOW,			// l�trehozand� ablak st�lusa, ld. http://msdn.microsoft.com/en-us/library/ff700543%28v=vs.85%29.aspx
							szWindowClassName,				// ablakoszt�ly neve
							szAppName,						// az ablak felirata
							WS_OVERLAPPEDWINDOW,			// ablak st�lusa, ld. http://msdn.microsoft.com/en-us/library/ms632600%28v=vs.85%29.aspx
							CW_USEDEFAULT, CW_USEDEFAULT,	// ablak kezdeti X �s Y koordin�t�i
							640, 480,						// ablak kezdeti sz�less�ge (640) �s magass�ga (480)
							NULL,							// ablak sz�l�j�nek le�r�ja
							NULL,							// ablak men�j�nek le�r�ja (HMENU)
							hInstance,						// programunk p�ld�ny�nak le�r�ja
							NULL );							// ablakhoz l�trehoz�sakor k�ldend� egy�b adatok

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
					TEXT("Hell�! Most indul a program!"), 
					TEXT("Hell� c�mke"), 
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
		 MessageBox(0, TEXT("Most �r v�get a program"), TEXT("�zenet"), MB_OK );
         PostQuitMessage (0) ;
         return 0 ;
    }

    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
