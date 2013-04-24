#pragma once

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

// lib files to be linked to our executable
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#pragma comment(lib, "glew32.lib")

class COGLAppBase
{
public:
	COGLAppBase(void);
	virtual ~COGLAppBase(void);

	BOOL	Initialize(HINSTANCE hInst, int WindowWidth, int WindowHeight);
	VOID	Run();

	virtual void DrawScene() {}
	virtual void HandleInput() {}
	virtual void Init() {}
	virtual void Cleanup() {}
	virtual void UpdateScene() {}
	virtual void Resize(int, int) {}
protected:
	HWND		m_hWnd;		// our application's window's handle
	HINSTANCE	m_hInstance;// handle to our process
	HDC			m_hDC;		// device context handle
	HGLRC		m_hRC;	// rendering context handle
	// virtual message processor, called from the static StaticMsgProc
	virtual LRESULT CALLBACK MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// time elapsed since last frame
	float	m_timeElapsed;

	// the window's client area's width and height
	int	m_client_width, m_client_height;

private:
	WNDCLASSEX	m_WndCls;

	// static message processor
	static LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	InitWindow(int Width, int Height);
	HGLRC	InitOpenGL();
	void	DrawBase();

};
