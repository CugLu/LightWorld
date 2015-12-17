#include "win_local.h"
#include "glutils.h"
#include "sys/sys_public.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

const char* CLASS_NAME = "glimp";
static bool GL_SetPixelFormat( ) {
    PIXELFORMATDESCRIPTOR src = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd
		1,								// version number
		PFD_DRAW_TO_WINDOW |			// support window
		PFD_SUPPORT_OPENGL |			// support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,					// RGBA type
		32,								// 32-bit color depth
		0, 0, 0, 0, 0, 0,				// color bits ignored
		8,								// 8 bit destination alpha
		0,								// shift bit ignored
		0,								// no accumulation buffer
		0, 0, 0, 0, 					// accum bits ignored
		24,								// 24-bit z-buffer	
		8,								// 8-bit stencil buffer
		0,								// no auxiliary buffer
		PFD_MAIN_PLANE,					// main layer
		0,								// reserved
		0, 0, 0							// layer masks ignored
    };

	Sys_Printf( "Initializing OpenGL driver\n" );

	if ( win32.hDC == NULL ) {
		if ( ( win32.hDC = GetDC( win32.hWnd ) ) == NULL ) {
			Sys_Printf( "...GetDC failed\n" );
			return false;
		}
	}

	if ( ( win32.pixelformat = ChoosePixelFormat( win32.hDC, &src ) ) == 0 ) {
		Sys_Printf( "...ChoosePixelFormat failed^0\n");
		return false;
	}
	Sys_Printf( "...PIXELFORMAT %d selected\n", win32.pixelformat );

	if ( SetPixelFormat( win32.hDC, win32.pixelformat, &win32.pfd ) == FALSE ) {
		Sys_Printf( "...^3SetPixelFormat failed^0\n", win32.hDC );
		return false;
	}

	Sys_Printf( "...creating GL context: " );
	if ( ( win32.hGLRC = wglCreateContext( win32.hDC ) ) == 0 ) {
		Sys_Printf( "wgl create contect failed\n" );
		return false;
	}
	Sys_Printf( "succeeded\n" );

	Sys_Printf( "...making context current: " );
	if ( !wglMakeCurrent( win32.hDC, win32.hGLRC ) ) {
		wglDeleteContext( win32.hGLRC );
		win32.hGLRC = NULL;
		Sys_Printf( "wgl create contec failed\n" );
		return false;
	}
	Sys_Printf( "succeeded\n" );

	return true;
}

static void GL_RegisterWindowClasses( void ) {
	WNDCLASS wc;
	if ( win32.windowClassRegistered ) {
		return;
	}

	memset( &wc, 0, sizeof( wc ) );

	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC) MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = win32.hInstance;
	// wc.hIcon         = LoadIcon( win32.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wc.hbrBackground = (struct HBRUSH__ *)COLOR_GRAYTEXT;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = CLASS_NAME;

	if ( !RegisterClass( &wc ) ) {
		Sys_Error( "GLW_CreateWindow: could not register window class" );
	}
	Sys_Printf( "...registered window class\n" );
}

static bool GL_CreateWindow(int w, int h) {
	int				stylebits;
	int				x, y;
	int				exstyle;

	RECT	r;
	r.bottom = w;
	r.left = 0;
	r.top = 0;
	r.right = h;

	exstyle = 0;
	stylebits = WINDOW_STYLE|WS_SYSMENU;
	AdjustWindowRect (&r, stylebits, FALSE);

	x = win32.win_xpos;
	y = win32.win_ypos;

	win32.hWnd = CreateWindowEx (
		 exstyle, 
		 CLASS_NAME,
		 "null",
		 stylebits,
		 x, y, w, h,
		 NULL,
		 NULL,
		 win32.hInstance,
		 NULL);

	if ( !win32.hWnd ) {
		Sys_Printf( "^3GLW_CreateWindow() - Couldn't create window^0\n" );
		return false;
	}

	SetForegroundWindow( win32.hWnd );
	SetFocus( win32.hWnd );

	return true;
}

void GLimp_Shutdown( void ) {
	const char *success[] = { "failed", "success" };
	int retVal;

	Sys_Printf( "Shutting down OpenGL subsystem\n" );

	// set current context to NULL
	if ( wglMakeCurrent(NULL, NULL)) {
		retVal = wglMakeCurrent( NULL, NULL ) != 0;
		Sys_Printf( "...wglMakeCurrent( NULL, NULL ): %s\n", success[retVal] );
	}

	// delete HGLRC
	if ( win32.hGLRC ) {
		retVal = wglDeleteContext( win32.hGLRC ) != 0;
		Sys_Printf( "...deleting GL context: %s\n", success[retVal] );
		win32.hGLRC = NULL;
	}

	// release DC
	if ( win32.hDC ) {
		retVal = ReleaseDC( win32.hWnd, win32.hDC ) != 0;
		Sys_Printf( "...releasing DC: %s\n", success[retVal] );
		win32.hDC   = NULL;
	}

	// destroy window
	if ( win32.hWnd ) {
		Sys_Printf( "...destroying window\n" );
		ShowWindow( win32.hWnd, SW_HIDE );
		DestroyWindow( win32.hWnd );
		win32.hWnd = NULL;
	}

	// reset display settings
	if ( win32.cdsFullscreen ) {
		Sys_Printf( "...resetting display\n" );
		ChangeDisplaySettings( 0, 0 );
		win32.cdsFullscreen = false;
	}

	// close the thread so the handle doesn't dangle
	if ( win32.renderThreadHandle ) {
		Sys_Printf( "...closing smp thread\n" );
		CloseHandle( win32.renderThreadHandle );
		win32.renderThreadHandle = NULL;
	}
}

static int GL_InitGL()										
{
	glewInit();

	char *GL_version	=	(char *)glGetString(GL_VERSION);
    char *GL_vendor	=	(char *)glGetString(GL_VENDOR);
    char *GL_renderer	=	(char *)glGetString(GL_RENDERER);
	//char *GL_extension =	(char *)glGetString(GL_EXTENSIONS);
	Sys_Printf("gl version: %s\n", GL_version);
	Sys_Printf("gl vendor: %s\n", GL_vendor);
	Sys_Printf("gl renderer: %s\n", GL_renderer);
	//Sys_Printf("gl extension: %s\n", GL_extension);

	//Setting up swap intervals
	//PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
	//PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;
	//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	//wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	//if (wglGetSwapIntervalEXT && wglSwapIntervalEXT)
	//{
	//	Sys_Printf("wglGetSwapIntervalEXT %d\n", wglGetSwapIntervalEXT());
	//	wglSwapIntervalEXT(0);
	//}

	return false;										
}

void GL_SwapBuffers( void ) {
	SwapBuffers(win32.hDC);	
}

bool GL_CreateDevice(int width, int height){
	Sys_Printf( "Initializing OpenGL Window\n" );

	GL_RegisterWindowClasses();
	GL_CreateWindow(width, height);
	GL_SetPixelFormat();
	GL_InitGL();

	ShowWindow( win32.hWnd, SW_SHOW );
	UpdateWindow( win32.hWnd );

	return true;
}

