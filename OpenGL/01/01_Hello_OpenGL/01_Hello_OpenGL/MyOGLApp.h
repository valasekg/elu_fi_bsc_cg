#pragma once
#include "oglappbase.h"

// OpenGL include-ok
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// egy�b include-ok
#include <mmsystem.h>			// timeGetTime() ebben van benne
#define _USE_MATH_DEFINES		// matematikai konstansok a math.h-bol hasznalhatoak legyenek (M_...)
#include <math.h>				// sin, cos stb. ebben van

class CMyOGLApp :
	public COGLAppBase
{
public:
	CMyOGLApp(void);
	~CMyOGLApp(void);

	void Init();
	void Cleanup();

	void Resize(int _w, int _h);
	void UpdateScene();

	void DrawScene();
	void HandleInput();

private:
	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�

	// egy-egy cs�cspontban elt�rolt adatok struct-ja
	struct Vertex
	{
		glm::vec3 p;	// poz�ci�
		glm::vec3 c;	// sz�n
	};
};
