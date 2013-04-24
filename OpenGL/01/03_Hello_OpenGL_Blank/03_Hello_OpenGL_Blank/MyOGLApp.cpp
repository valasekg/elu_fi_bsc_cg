#include "MyOGLApp.h"

CMyOGLApp::CMyOGLApp(void)
{
}

CMyOGLApp::~CMyOGLApp(void)
{
}

void CMyOGLApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
}


// erõforrások felszabadítása
void CMyOGLApp::Cleanup()
{
}

// színtér elemeinek frissítése
void CMyOGLApp::UpdateScene()
{
}

// színtér kirajzolása
void CMyOGLApp::DrawScene()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) 
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
	// iránybillentyûk: VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		MessageBox(0, TEXT("Balra!"), TEXT("Üzenet"), 0);
	}
	// az egyes billentyûket char-ként is lekérdezhetjük - nagybetûsítve
	if (GetKeyState('A') & 0x8000)
	{
		MessageBox(0, TEXT("Az 'A' vagy 'a' betû!"), TEXT("Üzenet"), 0);
	}

	// egérpozíció az act_pt-be kerül
	static POINT act_pos;
	// a képernyõn belül elkérjük az egér koordinátáit és act_pos-ba beletöltjük
	GetCursorPos(&act_pos);
	// a képernyõ-koordinátákat az aktuális ablakunkon belüli koordinátákká alakítjuk
	ScreenToClient(m_hWnd, &act_pos);
}
