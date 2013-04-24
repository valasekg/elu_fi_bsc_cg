#include "MyOGLApp.h"
#include "GLUtils.hpp"

CMyOGLApp::CMyOGLApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;

	m_angleY = 0;
	
	m_mesh = 0;

	m_dist = 5;
}

CMyOGLApp::~CMyOGLApp(void)
{
}

GLuint CMyOGLApp::GenTexture()
{
    unsigned char tex[256][256][3];
 
    for (int i=0; i<256; ++i)
        for (int j=0; j<256; ++j)
        {
			tex[i][j][0] = rand()%256;
			tex[i][j][1] = rand()%256;
			tex[i][j][2] = rand()%256;
        }
 
	GLuint tmpID;

	// generate a texture resource name
    glGenTextures(1, &tmpID);
	// activate it
    glBindTexture(GL_TEXTURE_2D, tmpID);
	// load data into GPU memory
    gluBuild2DMipmaps(  GL_TEXTURE_2D,	// into the active texture
						GL_RGB8,		// 8 bits for the red, green, and blue channels each
						256, 256,		// texture size is 256x256 pixels
						GL_RGB,				// the texture source (=system memory array tex) layout is (red, green, blue)
						GL_UNSIGNED_BYTE,	// each component is stored on 8 bits (a char)
						tex);				// the source of the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// bilinear filter on min
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// bilinear filter on max
	glBindTexture(GL_TEXTURE_2D, 0); // deactivate the texture

	return tmpID;
}


void CMyOGLApp::Init()
{
	// clear color set to blue-ish
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);		// turn on back-face culling
	glEnable(GL_DEPTH_TEST);	// enable depth-test

	//
	// define the geometry
	//

	Vertex vert[] =
	{ 
		//           x,  y,  z             nx,ny,nz			 s, t
		{glm::vec3(-10,  0, -10), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3( 10,  0, -10), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(-10,  0,  10), glm::vec3(0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3( 10,  0,  10), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
	};

	// create index buffer
    GLushort indices[]=
    {
        1,0,2,
        1,2,3,
    };

	// create a VAO
	glGenVertexArrays(1, &m_vaoID);
	// activate the new VAO m_vaoID
	glBindVertexArray(m_vaoID);
	
	// create a VBO
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // activate the VBO m_vboID
	// load the data stored in array vert into the VBO (essentially: upload the data to the GPU)
	glBufferData( GL_ARRAY_BUFFER,	// allocate memory for the active VBO and set its data
				  sizeof(vert),		// size of the VBO allocation, in bytes
				  vert,				// load data into the VBO from this location of the system memory
				  GL_STATIC_DRAW);	// we only want to store data into the VBO once (STATIC), and we want to use the VBO as a source for drawing our scene at each frame (DRAW)
									// for other usage flags see http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
									// {STREAM, STATIC, DYNAMIC} and {DRAW, READ, COPY}
	

	// activate the first general attribute 'channel' in the VAO
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(
		0,				// set the attributes of VAO channel 0
		3,				// this channel has 3 componenets
		GL_FLOAT,		// each of those componenets are floats
		GL_FALSE,		// do not normalize
		sizeof(Vertex),	// stride
		0				// channel 0`s data begins at the beginning of the VBO, no offset
	); 

	// activate 'channel' idx 1
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// texture coordinates
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)) );

	// create index buffer
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

	//
	// shader initialization
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// create the shader container (program)
	m_programID = glCreateProgram();

	// attach the vertex and fragment (pixel) shaders to the program
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// make correspondances between the VAO channels and the shader 'in' variables
	// IMPORTANT: do this prior to linking the programs!
	glBindAttribLocation(	m_programID,	// ID of the shader program from which we want to map a variable to a channel
							0,				// the VAO channel number we want to bind the variable to
							"vs_in_pos");	// the name of the variable in the shader
	glBindAttribLocation( m_programID, 1, "vs_in_normal");
	glBindAttribLocation( m_programID, 2, "vs_in_tex0");

	// link the shaders
	glLinkProgram(m_programID);

	// check the linking
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

		MessageBoxA(0, aSzoveg, "Sáder Huba panasza", 0);

		delete aSzoveg;
	}

	// we can dispose of the vertex and fragment shaders
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// other initializations
	//

	// set the projection matrix
	m_matProj = glm::perspective( 45.0f, m_client_width/(float)m_client_height, 1.0f, 1000.0f );

	// query the IDs of the shader uniform variables
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_worldIT = glGetUniformLocation( m_programID, "worldIT");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );
	m_loc_texture = glGetUniformLocation( m_programID, "texture" );
	m_loc_eye_pos = glGetUniformLocation( m_programID, "eye_pos" );
	m_loc_light_pos = glGetUniformLocation( m_programID, "light_pos");
	m_loc_kd = glGetUniformLocation( m_programID, "Kd");
	// generate the texture
	m_textureID = TextureFromFile("texture.jpg");
	m_mesh_texture = GenTexture();

	// load mesh
	m_mesh = ObjParser::parse("Suzanne.obj");
	m_mesh->initBuffers();

	m_cow = ObjParser::parse("cow.obj");
	m_cow->initBuffers();

	m_cessna = ObjParser::parse("cessna.obj");
	m_cessna->initBuffers();
}


// cleanup the allocated resources
void CMyOGLApp::Cleanup()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteTextures(1, &m_textureID);

	glDeleteProgram( m_programID );

	delete m_mesh;
	delete m_cow;
	delete m_cessna;
}

void CMyOGLApp::UpdateScene()
{
	// view transform
	m_eye = glm::vec3( m_dist*cosf( m_angleY ) , m_dist, m_dist*sinf( m_angleY ) );

	m_matView = glm::lookAt(m_eye,						// camera position
							glm::vec3( 0,  0,  0),		// look at position
							glm::vec3( 0,  1,  0));		// vector pointing upwards

	// update light position
	float t = timeGetTime()/1000.0f;
	m_lpos = glm::vec3( 10*cosf(t), 5+sinf(10*t), 10*sinf(t) );
}

void CMyOGLApp::DrawGround()
{
	// switch on the shader
	glUseProgram( m_programID );

	// set shader uniforms
	/*

	GLM transformation examples:
		glm::rotate<float>( angle, axis_coord_x, axis_coord_y, axis_coord_z) <- rotation about axis_coord_{xyz} axis by angle degrees
		glm::translate<float>( d_x, d_y, d_z) <- translation by vector d
		glm::scale<float>( s_x, s_y, s_z ) <- scaling

	*/

	glm::mat4 matWorld = glm::mat4(1.0f);
	glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );

	// send the matrices to the GPU
	glUniformMatrix4fv( m_loc_world, 1, GL_FALSE, &( matWorld[0][0]) ); 
	glUniformMatrix4fv( m_loc_worldIT, 1, GL_FALSE, &( matWorldIT[0][0]) ); 
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	glUniform3f(m_loc_eye_pos, m_eye.x, m_eye.y, m_eye.z );

	glUniform3f(m_loc_light_pos, m_lpos.x, m_lpos.y, m_lpos.z );

	// activate the texture sampler GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	// activate our texture
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// link the texture sampler with our texture
	glUniform1i(	m_loc_texture,	
					0);				

	// switch on the VAO
	glBindVertexArray(m_vaoID);

	// draw the indexed primitive
	glDrawElements(	GL_TRIANGLES,		// primitive type
					6,					// the number of indices (vertices) used in this draw command
					GL_UNSIGNED_SHORT,	// index type
					0);					// index address

	// VAO off
	glBindVertexArray(0);

	// shader off
	glUseProgram( 0 );
}

void CMyOGLApp::DrawMesh( Mesh* _mesh, glm::mat4 _world, glm::vec4 _c = glm::vec4(1, 1, 1, 1) )
{
	// switch on the shader
	glUseProgram( m_programID );

	// set shader uniforms
	/*

	GLM transformation examples:
		glm::rotate<float>( angle, axis_coord_x, axis_coord_y, axis_coord_z) <- rotation about axis_coord_{xyz} axis by angle degrees
		glm::translate<float>( d_x, d_y, d_z) <- translation by vector d
		glm::scale<float>( s_x, s_y, s_z ) <- scaling

	*/

	glm::mat4 matWorld = _world;
	glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );

	// send the matrices to the GPU
	glUniformMatrix4fv( m_loc_world, 1, GL_FALSE, &( matWorld[0][0]) ); 
	glUniformMatrix4fv( m_loc_worldIT, 1, GL_FALSE, &( matWorldIT[0][0]) ); 
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	glUniform4f(m_loc_kd, _c.x, _c.y, _c.z, _c.w);
	glUniform3f(m_loc_eye_pos, m_eye.x, m_eye.y, m_eye.z );
	glUniform3f(m_loc_light_pos, m_lpos.x, m_lpos.y, m_lpos.z );

	// activate the texture sampler GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	// activate our texture
	glBindTexture(GL_TEXTURE_2D, m_mesh_texture);
	// link the texture sampler with our texture
	glUniform1i(	m_loc_texture,	
					0);				

	// draw mesh
	_mesh->draw();

	// shader off
	glUseProgram( 0 );
}

void CMyOGLApp::DrawCow()
{
	glm::mat4 world = 
		glm::translate<float>(0, 1, 0) *
		glm::rotate<float>(-90, 1, 0, 0)*
		glm::scale<float>(0.3f, 0.3f, 0.3f);
	DrawMesh( m_cow, world, glm::vec4(1, 0, 0, 1) );
}

void CMyOGLApp::DrawSuzannes()
{
	// switch on the shader
	glUseProgram( m_programID );

	// set shader uniforms
	/*

	GLM transformation examples:
		glm::rotate<float>( angle, axis_coord_x, axis_coord_y, axis_coord_z) <- rotation about axis_coord_{xyz} axis by angle degrees
		glm::translate<float>( d_x, d_y, d_z) <- translation by vector d
		glm::scale<float>( s_x, s_y, s_z ) <- scaling

	*/
	// send the matrices to the GPU
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	glUniform3f(m_loc_eye_pos, m_eye.x, m_eye.y, m_eye.z );

	glUniform3f(m_loc_light_pos, m_lpos.x, m_lpos.y, m_lpos.z );

	// activate the texture sampler GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	// activate our texture
	glBindTexture(GL_TEXTURE_2D, m_mesh_texture);
	// link the texture sampler with our texture
	glUniform1i(	m_loc_texture,	
					0);				

	for (int i=0; i<10; ++i)
	{
		float t = 360*timeGetTime()/5000.0f;
		float s = 0.3f*(3+(3*i % 5))/3;
		glm::mat4 matWorld = 
			glm::rotate<float>(-t + i*36, 0, 1, 0)*
			glm::translate<float>(5, 1 + ( (i+1)*3 % 4 + 1 )*abs(cosf( t/36 + (i%4) )), 0)*
			glm::scale<float>(s, s, s);
		glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );
		
		glUniformMatrix4fv( m_loc_world, 1, GL_FALSE, &( matWorld[0][0]) ); 
		glUniformMatrix4fv( m_loc_worldIT, 1, GL_FALSE, &( matWorldIT[0][0]) ); 

		glm::vec4 c = glm::vec4( i/10.0f, 1-i/16.0f, 1-0.025*i, 1 );

		glUniform4f(m_loc_kd, c.x, c.y, c.z, c.w);

		// draw mesh
		m_mesh->draw();
	}

	// shader off
	glUseProgram( 0 );
}

void CMyOGLApp::DrawCessna()
{
	float t = 360*timeGetTime()/5000.0f;
	glm::mat4 world = 
		glm::rotate<float>( t, 0, 1, 0) *
		glm::translate<float>(20, 10, 0) *
		glm::rotate<float>(180, 0, 1, 0) *
		glm::rotate<float>(-90, 1, 0, 0)*
		glm::scale<float>(0.3f, 0.3f, 0.3f);

	DrawMesh( m_cessna, world, glm::vec4(0, 1, 1, 1) );
}

void CMyOGLApp::DrawScene()
{
	// clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawGround();

	DrawCow();
	DrawCessna();
	DrawSuzannes();
}

void CMyOGLApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,			// 90 degrees field of view
									_w/(float)_h,	// aspect ratio matching that of the client area
									0.01f,			// near plane
									100.0f);		// far plane

	m_client_width = _w;
	m_client_height = _h;
}

void CMyOGLApp::HandleInput()
{
	if ( GetActiveWindow() != m_hWnd ) 
		return;

	// arrow keys: VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// do something
	}
	// alphanumeric keys
	if (GetKeyState('A') & 0x8000)
	{
		// do something
	}

	// variable to store the mouse position in
	static POINT act_pos;
	static POINT prev_pos;
	// query the mouse position and put it into act_pos
	GetCursorPos(&act_pos);
	m_angleY += (act_pos.x - prev_pos.x)/100.0f;
	m_dist += (act_pos.y - prev_pos.y)/25.0f;

	prev_pos = act_pos;
}
