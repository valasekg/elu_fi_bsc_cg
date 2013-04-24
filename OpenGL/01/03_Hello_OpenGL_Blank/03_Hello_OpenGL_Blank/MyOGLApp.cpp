#include "MyOGLApp.h"

CMyOGLApp::CMyOGLApp(void)
{
}

CMyOGLApp::~CMyOGLApp(void)
{
}

void CMyOGLApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
}


// er�forr�sok felszabad�t�sa
void CMyOGLApp::Cleanup()
{
}

// sz�nt�r elemeinek friss�t�se
void CMyOGLApp::UpdateScene()
{
}

// sz�nt�r kirajzol�sa
void CMyOGLApp::DrawScene()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CMyOGLApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_client_width = _w;
	m_client_height = _h;
}

void CMyOGLApp::HandleInput()
{
	// ir�nybillenty�k: VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		MessageBox(0, TEXT("Balra!"), TEXT("�zenet"), 0);
	}
	// az egyes billenty�ket char-k�nt is lek�rdezhetj�k - nagybet�s�tve
	if (GetKeyState('A') & 0x8000)
	{
		MessageBox(0, TEXT("Az 'A' vagy 'a' bet�!"), TEXT("�zenet"), 0);
	}

	// eg�rpoz�ci� az act_pt-be ker�l
	static POINT act_pos;
	// a k�perny�n bel�l elk�rj�k az eg�r koordin�t�it �s act_pos-ba belet�ltj�k
	GetCursorPos(&act_pos);
	// a k�perny�-koordin�t�kat az aktu�lis ablakunkon bel�li koordin�t�kk� alak�tjuk
	ScreenToClient(m_hWnd, &act_pos);
}
