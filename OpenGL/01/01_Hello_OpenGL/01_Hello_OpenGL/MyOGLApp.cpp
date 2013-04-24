#include "MyOGLApp.h"

CMyOGLApp::CMyOGLApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
}

CMyOGLApp::~CMyOGLApp(void)
{
}

void CMyOGLApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0)},
		{glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0)},
		{glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(0, 0, 1)},
		{glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3(1, 1, 1)},
	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  sizeof(vert),		// ennyi b�jt nagys�gban
				  vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
				  GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat
	

	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		(GLuint)0,		// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	); 

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(3); // ez lesz majd a sz�n - mi�rt pont a 3. attrib�tum? http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
	glVertexAttribPointer(
		(GLuint)3,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
}


// er�forr�sok felszabad�t�sa
void CMyOGLApp::Cleanup()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
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

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// VAO kikapcsolasa
	glBindVertexArray(0);

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
