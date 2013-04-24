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
	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// transzform�ci�s m�trixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// m�trixok helye a shaderekben
	GLuint	m_loc_world;
	GLuint	m_loc_view;
	GLuint	m_loc_proj;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 c;
	};
};
