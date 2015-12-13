
#ifndef __WIN_LOCAL_H__
#define __WIN_LOCAL_H__

#include <windows.h>
#include "../sys_public.h"


#define	MAX_OSPATH			256

#define	WINDOW_STYLE	(WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE | WS_THICKFRAME)

//void	Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr );

void	Sys_CreateConsole( void );
void	Sys_DestroyConsole( void );

char	*Sys_ConsoleInput (void);
char	*Sys_GetCurrentUser( void );

//cpuid_t	Sys_GetCPUId( void );

void Sys_Init();

void Sys_PumpEvents( void );
int		MapKey (int key);

void	Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr );
// Input subsystem

void	IN_Init (void);
void	IN_Shutdown (void);
// add additional non keyboard / non mouse movement on top of the keyboard move cmd

void	IN_DeactivateMouseIfWindowed( void );
void	IN_DeactivateMouse( void );
void	IN_ActivateMouse( void );

void	IN_Frame( void );

int		IN_DIMapKey( int key );

void	DisableTaskKeys( BOOL bDisable, BOOL bBeep, BOOL bTaskMgr );


// window procedure
LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void Conbuf_AppendText( const char *msg );

typedef struct {
	HWND			hWnd;
	HINSTANCE		hInstance;

	bool			activeApp;			// changed with WM_ACTIVATE messages
	bool			mouseReleased;		// when the game has the console down or is doing a long operation
	bool			movingWindow;		// inhibit mouse grab when dragging the window
	bool			mouseGrabbed;		// current state of grab and hide

	OSVERSIONINFOEX	osversion;

	//cpuid_t			cpuid;

	// when we get a windows message, we store the time off so keyboard processing
	// can know the exact time of an event (not really needed now that we use async direct input)
	int				sysMsgTime;

	bool			windowClassRegistered;

	WNDPROC			wndproc;

	HDC				hDC;							// handle to device context
	HGLRC			hGLRC;						// handle to GL rendering context
	PIXELFORMATDESCRIPTOR pfd;		
	int				pixelformat;

	HINSTANCE		hinstOpenGL;	// HINSTANCE for the OpenGL library

	int				desktopBitsPixel;
	int				desktopWidth, desktopHeight;

	bool			cdsFullscreen;

	//FILE			*log_fp;

	unsigned short	oldHardwareGamma[3][256];
	// desktop gamma is saved here for restoration at exit

	int win_xpos;
	int win_ypos;
	bool win_outputDebugString;
	bool win_outputEditString;
	//static idCVar	sys_arch;
	//static idCVar	sys_cpustring;
	//static idCVar	in_mouse;
	//static idCVar	win_allowAltTab;
	//static idCVar	win_notaskkeys;
	//static idCVar	win_username;
	//static idCVar	win_xpos;			// archived X coordinate of window position
	//static idCVar	win_ypos;			// archived Y coordinate of window position
	//static idCVar	win_outputDebugString;
	//static idCVar	win_outputEditString;
	//static idCVar	win_viewlog;
	//static idCVar	win_timerUpdate;
	//static idCVar	win_allowMultipleInstances;

	//CRITICAL_SECTION criticalSections[MAX_CRITICAL_SECTIONS];
	HANDLE			backgroundDownloadSemaphore;

	HINSTANCE		hInstDI;			// direct input

	HANDLE			renderCommandsEvent;
	HANDLE			renderCompletedEvent;
	HANDLE			renderActiveEvent;
	HANDLE			renderThreadHandle;
	unsigned long	renderThreadId;
	void				(*glimpRenderThread)( void );
	void				*smpData;
	int				wglErrors;
	// SMP acceleration vars
	
	HFONT			defaultFont;
	HBITMAP          hBitmap;
} Win32Vars_t;

extern Win32Vars_t win32;

void Sys_ShowConsole( int visLevel, bool quitOnClose );

LONG WINAPI InputLineWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif /* !__WIN_LOCAL_H__ */
