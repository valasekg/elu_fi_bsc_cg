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
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

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
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,		// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(3); // ez lesz majd a szín - miért pont a 3. attribútum? http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
	glVertexAttribPointer(
		(GLuint)3,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
}


// erõforrások felszabadítása
void CMyOGLApp::Cleanup()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
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

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
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
