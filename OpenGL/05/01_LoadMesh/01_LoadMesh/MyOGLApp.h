#pragma once
#include "oglappbase.h"

// OpenGL include-ok
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// egyéb include-ok
#include <mmsystem.h>			// timeGetTime() ebben van benne
#define _USE_MATH_DEFINES		// matematikai konstansok a math.h-bol hasznalhatoak legyenek (M_...)
#include <math.h>				// sin, cos stb. ebben van

// mesh
#include "ObjParser_OGL3.h"

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
	// belsõ eljárások
	void DrawGround();
	void DrawMesh();

	// shaderekhez szükséges változók
	GLuint m_programID; // shaderek programja

	// transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// mátrixok helye a shaderekben
	GLuint	m_loc_world;
	GLuint	m_loc_view;
	GLuint	m_loc_proj;
	GLuint	m_loc_texture;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object erõforrás azonosító
	GLuint m_vboID; // vertex buffer object erõforrás azonosító
	GLuint m_ibID;  // index buffer object erõforrás azonosító
	GLuint m_waterTextureID; // fájlból betöltött textúra azonosítója

	struct Vertex
	{
		glm::vec3 p; // pozíció
		glm::vec3 n; // normális
		glm::vec2 t; // textúra koordináták
	};

	// mesh adatok
	Mesh *m_mesh;
};
