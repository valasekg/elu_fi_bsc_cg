#include "MyOGLApp.h"
#include "GLUtils.hpp"

CMyOGLApp::CMyOGLApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_mesh = 0;
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
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		//          x,  y, z               nx,ny,nz			 s, t
		{glm::vec3(-10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(-10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3( 10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3( 10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(1, 1)},
	};

	// indexpuffer adatai
    GLushort indices[]=
    {
		// 1. h�romsz�g
        0,1,2,
		// 2. h�romsz�g
        2,1,3,
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
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	); 

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// text�rakoordin�t�k bekapcsol�sa a 2-es azonos�t�j� attrib�tom csatorn�n
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)) );

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_ibID);
	// a VAO �szreveszi, hogy bind-olunk egy index puffert �s feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
							0,				// a VAO-beli azonos�t� index
							"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation( m_programID, 1, "vs_in_col");
	glBindAttribLocation( m_programID, 2, "vs_in_tex0");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		MessageBoxA(0, aSzoveg, "S�der Huba panasza", 0);

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egy�b inicializ�l�s
	//

	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective( 45.0f, m_client_width/(float)m_client_height, 1.0f, 1000.0f );

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );

	m_loc_texture = glGetUniformLocation( m_programID, "texture" );

	//
	// egy�b er�forr�sok bet�lt�se
	//

	// text�ra bet�lt�se
	m_waterTextureID = TextureFromFile("texture.bmp");

	// mesh betoltese
	m_mesh = ObjParser::parse("../../02_SaveToObj/02_SaveToObj/cowcessna.obj");
	m_mesh->initBuffers();
}


// er�forr�sok felszabad�t�sa
void CMyOGLApp::Cleanup()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);

	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteTextures(1, &m_waterTextureID);

	glDeleteProgram( m_programID );

	delete m_mesh;
}

void CMyOGLApp::UpdateScene()
{
	// n�zeti transzform�ci� be�ll�t�sa
	m_matView = glm::lookAt(glm::vec3(10, 10, 10),		// honnan n�zz�k a sz�nteret
							glm::vec3( 0,  0,  0),		// a sz�nt�r melyik pontj�t n�zz�k
							glm::vec3( 0,  1,  0));		// felfel� mutat� ir�ny a vil�gban
}

void CMyOGLApp::DrawGround()
{
	// a talaj kirajzolasahoz szukseges shader beallitasa
	glUseProgram( m_programID );

	// shader parameterek be�ll�t�sa
	/*

	GLM transzform�ci�s m�trixokra p�ld�k:
		glm::rotate<float>( sz�g, tengely_x, tengely_y, tengely_z) <- tengely_{xyz} k�r�li elforgat�s
		glm::translate<float>( eltol_x, eltol_y, eltol_z) <- eltol�s
		glm::scale<float>( s_x, s_y, s_z ) <- l�pt�kez�s

	*/
	m_matWorld = glm::mat4(1.0f);

	// majd k�ldj�k �t a megfelel� m�trixokat!
	glUniformMatrix4fv( m_loc_world,// erre a helyre t�lts�nk �t adatot
						1,			// egy darab m�trixot
						GL_FALSE,	// NEM transzpon�lva
						&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );
	
	// aktiv�ljuk a 0-�s text�ra mintav�telez� egys�get
	glActiveTexture(GL_TEXTURE0);
	// aktiv�ljuk a gener�lt text�r�nkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// text�ra mintav�telez� �s shader-beli sampler2D �sszerendel�se
	glUniform1i(	m_loc_texture,	// ezen azonos�t�j� sampler 2D
					0);				// olvassa az ezen index� mintav�telez�t

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawElements(	GL_TRIANGLES,		// primit�v t�pus
					6,					// hany csucspontot hasznalunk a kirajzolashoz
					GL_UNSIGNED_SHORT,	// indexek tipusa
					0);					// indexek cime

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// text�ra kikapcsol�sa
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram( 0 );
}

void CMyOGLApp::DrawMesh()
{
	// a mesh kirajzolasahoz hasznalt shader bekapcsolasa
	glUseProgram( m_programID );

	m_matWorld = glm::translate<float>(0, 1, 0);

	// majd k�ldj�k �t a megfelel� m�trixokat!
	glUniformMatrix4fv( m_loc_world,// erre a helyre t�lts�nk �t adatot
						1,			// egy darab m�trixot
						GL_FALSE,	// NEM transzpon�lva
						&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	// aktiv�ljuk a 0-�s text�ra mintav�telez� egys�get
	glActiveTexture(GL_TEXTURE0);
	// aktiv�ljuk a gener�lt text�r�nkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// text�ra mintav�telez� �s shader-beli sampler2D �sszerendel�se
	glUniform1i(	m_loc_texture,	// ezen azonos�t�j� sampler 2D
					0);				// olvassa az ezen index� mintav�telez�t


	m_mesh->draw();

	glUseProgram( 0 );
}

void CMyOGLApp::DrawScene()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DrawGround();
	DrawMesh();
}

void CMyOGLApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,			// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									1.0f,			// kozeli vagosik
									1000.0f);		// tavoli vagosik

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
