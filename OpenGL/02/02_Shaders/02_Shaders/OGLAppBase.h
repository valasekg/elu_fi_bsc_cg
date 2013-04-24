#pragma once

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

// exe-hez hozz�szerkesztend� lib f�jlok megad�sa
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
	// bels� adattagok
	HWND		m_hWnd;		// alkalmaz�sablakunk le�r�ja
	HINSTANCE	m_hInstance;// process le�r�ja
	HDC			m_hDC;		// device context le�r�ja
	HGLRC		m_hRC;	// rendering context le�r�ja
	// az �zenetfeldolgoz� f�ggv�ny fel�l�rhat� ig�ny szerint
	virtual LRESULT CALLBACK MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// el�z� frame �ta eltelt id�
	float	m_timeElapsed;

	// az ablak klienster�let�nek nagys�ga pixelben
	int	m_client_width, m_client_height;

private:
	WNDCLASSEX	m_WndCls;

	// bels� elj�r�sok
	static LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	InitWindow(int Width, int Height);
	HGLRC	InitOpenGL();
	void	DrawBase();

};
