#pragma once
#include "oglappbase.h"

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// other headers
#include <mmsystem.h>			// Windows multimedia functions, such as timeGetTime()
#define _USE_MATH_DEFINES		// we want math.h to define math constants, e.g. M_PI
#include <math.h>				// sin, cos, etc.

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
	// private functions
	GLuint GenTexture();

	// shader related variables
	GLuint m_programID; // shaderek program ID

	// transformation matrices
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// IDs of shader variables
	GLuint	m_loc_world;
	GLuint	m_loc_worldIT;
	GLuint	m_loc_view;
	GLuint	m_loc_proj;
	GLuint	m_loc_texture;

	float	m_angleY;

	glm::vec3 m_eye;

	// OpenGL resource identifiers
	GLuint m_vaoID; // vertex array object resource name
	GLuint m_vboID; // vertex buffer object resource name
	GLuint m_ibID;  // index buffer object resource name
	GLuint m_textureID; // textúra resource name

	// data to be stored at each vertex
	struct Vertex
	{
		glm::vec3 p;	// vertex position
		glm::vec3 n;	// vertex normal
		glm::vec2 t;	// texture coordinates
	};
};
