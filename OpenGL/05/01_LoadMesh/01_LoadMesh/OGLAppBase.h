#pragma once

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

// exe-hez hozzászerkesztendõ lib fájlok megadása
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
	// belsõ adattagok
	HWND		m_hWnd;		// alkalmazásablakunk leírója
	HINSTANCE	m_hInstance;// process leírója
	HDC			m_hDC;		// device context leírója
	HGLRC		m_hRC;	// rendering context leírója
	// az üzenetfeldolgozó függvény felülírható igény szerint
	virtual LRESULT CALLBACK MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// elõzõ frame óta eltelt idõ
	float	m_timeElapsed;

	// az ablak kliensterületének nagysága pixelben
	int	m_client_width, m_client_height;

private:
	WNDCLASSEX	m_WndCls;

	// belsõ eljárások
	static LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	InitWindow(int Width, int Height);
	HGLRC	InitOpenGL();
	void	DrawBase();

};
