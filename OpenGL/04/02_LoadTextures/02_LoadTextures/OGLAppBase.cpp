#include "OGLAppBase.h"

COGLAppBase::COGLAppBase(void)
{
	m_hRC = NULL;
	m_hWnd = NULL;
	m_hDC = NULL;
}

COGLAppBase::~COGLAppBase(void)
{
	if (m_hDC) 
	{
		wglMakeCurrent(m_hDC, NULL);
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	if (m_hRC) 
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}
}

BOOL COGLAppBase::InitWindow(int Width, int Height)
{
	m_client_width = Width;
	m_client_height = Height;

	const TCHAR* const APP_TITLE = TEXT("OpenGL program");
	const TCHAR* const WND_CLASSNAME = TEXT("OGLWndCls1213p1");

	if (IsWindow(m_hWnd)) return TRUE;
	m_hWnd = NULL;

	ZeroMemory(&m_WndCls, sizeof(WNDCLASSEX));
	m_WndCls.cbSize			= sizeof( WNDCLASSEX );
	m_WndCls.lpfnWndProc	= StaticMsgProc;
	m_WndCls.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	m_WndCls.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	m_WndCls.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_WndCls.hInstance		= GetModuleHandle(NULL);
	m_WndCls.lpszClassName	= WND_CLASSNAME;
	m_WndCls.style			= CS_HREDRAW | CS_VREDRAW;
	m_WndCls.hInstance		= m_hInstance;
	

	if (!RegisterClassEx(&m_WndCls)) return FALSE;

	m_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, 
							m_WndCls.lpszClassName, 
							APP_TITLE, 
							WS_OVERLAPPEDWINDOW, 
							CW_USEDEFAULT, CW_USEDEFAULT, 
							Width, Height, 
							NULL, 
							NULL, 
							m_hInstance, 
							(LPVOID)this );

	if ( m_hWnd != NULL )
		SetWindowLong( m_hWnd, GWL_USERDATA, (LONG)this);

	return ( m_hWnd != NULL );
}

BOOL COGLAppBase::Initialize(HINSTANCE hInst, int WindowWidth, int WindowHeight)
{
	m_hInstance = hInst;
	if (!InitWindow(WindowWidth, WindowHeight))
	{
		return false;
	}

	m_hRC = InitOpenGL();

	if (!m_hRC)
		return FALSE;

	if (glewInit() != GLEW_OK) 
	{
		return FALSE;
	}

	Init();

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );


	return true;
}

VOID COGLAppBase::Run()
{
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );

	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timerFrequency;
	QueryPerformanceFrequency(&timerFrequency);
	double fi = 1000.0/(double)timerFrequency.QuadPart;

    while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
		{
			QueryPerformanceCounter(&timeStart);

			UpdateScene();
			DrawBase();
			HandleInput();

			QueryPerformanceCounter(&timeEnd);

			// az elõzõ frame óta eltelt idõ (ms-ben) kerül az m_timeElapsed-be
			m_timeElapsed = (float)((timeEnd.QuadPart - timeStart.QuadPart)*fi);
		}
    }

	Cleanup();
}

void COGLAppBase::DrawBase()
{
	DrawScene();

	SwapBuffers( m_hDC );
}

HGLRC COGLAppBase::InitOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;

	m_hDC = GetDC(m_hWnd);

	ZeroMemory( &pfd, sizeof(pfd) );

    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 24; // 8 bit R, G, B csatornánként
	pfd.cDepthBits	 = 16; // 16 bites Z-puffer
	pfd.cRedBits	 = 8;  // 8 bit legyen a vörös
	pfd.cGreenBits	 = 8;  // 8 bit legyen a zöld
	pfd.cBlueBits	 = 8;  // és 8 bit legyen a kék komponensek tárolására
	pfd.cRedShift	 = 0;  // az elsõ 8 biten legyen az R
	pfd.cGreenShift	 = 8;  // a második 8 biten  G
	pfd.cBlueShift	 = 16; // a harmadik 8 biten a B

	int pixel_format = ChoosePixelFormat(m_hDC, &pfd);

	// ha  nincs az igényeinkhez hasonló, akkor sikertelenek vagyunk
	if ( pixel_format == 0 )
	{
		MessageBox( m_hWnd, TEXT("Nem sikerült az OpenGL inicializálása!"), TEXT("Hiba"), 0);
		return NULL;
	}

	if ( !SetPixelFormat( m_hDC, pixel_format, &pfd ) )
	{
		MessageBox( m_hWnd, TEXT("Nem sikerült a pixelformat beállítása!"), TEXT("Hiba"), 0);
		return NULL;
	}

	HGLRC GLRC = wglCreateContext(m_hDC);

	if (!GLRC) 
		return NULL;

	wglMakeCurrent(m_hDC, GLRC);

	return GLRC;
}

LRESULT WINAPI COGLAppBase::StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	COGLAppBase* pInst = 0;

	if ( msg == WM_NCCREATE )
	{
		pInst = (COGLAppBase*) ((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLong( hWnd, GWL_USERDATA, (LONG)pInst );
	}
	else
	{
		pInst = (COGLAppBase*)GetWindowLong( hWnd, GWL_USERDATA );
	}

	if (pInst)
		return pInst->MsgProc( hWnd, msg, wParam, lParam );
	else
		return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT WINAPI COGLAppBase::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
		case WM_SIZE:
			Resize( LOWORD( lParam ), HIWORD( lParam ) );
			break;
		case WM_KEYDOWN:
			{
				switch (wParam)
				{
				case VK_ESCAPE:
					{
						DestroyWindow( hWnd );
						return 0;
					}
				}
				return 0;
			}
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}