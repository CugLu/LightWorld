#ifndef __SYS_PUBLIC__
#define __SYS_PUBLIC__

#include <stdarg.h>
#include "common/Array.h"

typedef struct
{
	int w;
	int h;
	unsigned char pData[1024*1024*4];
}sysTextContent_t;

typedef struct sysMemoryStats_s {
	int memoryLoad;
	int totalPhysical;
	int availPhysical;
	int totalPageFile;
	int availPageFile;
	int totalVirtual;
	int availVirtual;
	int availExtendedVirtual;
} sysMemoryStats_t;

typedef enum {
	K_TAB = 9,
	K_ENTER = 13,
	K_ESCAPE = 27,
	K_SPACE = 32,

	K_BACKSPACE = 127,

	K_COMMAND = 128,
	K_CAPSLOCK,
	K_SCROLL,
	K_POWER,
	K_PAUSE,

	K_UPARROW = 133,
	K_DOWNARROW,
	K_LEFTARROW,
	K_RIGHTARROW,

	// The 3 windows keys
	K_LWIN = 137,
	K_RWIN,
	K_MENU,

	K_ALT = 140,
	K_CTRL,
	K_SHIFT,
	K_INS,
	K_DEL,
	K_PGDN,
	K_PGUP,
	K_HOME,
	K_END,

	K_F1 = 149,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_INVERTED_EXCLAMATION = 161,	// upside down !
	K_F13,
	K_F14,
	K_F15,

	K_KP_HOME = 165,
	K_KP_UPARROW,
	K_KP_PGUP,
	K_KP_LEFTARROW,
	K_KP_5,
	K_KP_RIGHTARROW,
	K_KP_END,
	K_KP_DOWNARROW,
	K_KP_PGDN,
	K_KP_ENTER,
	K_KP_INS,
	K_KP_DEL,
	K_KP_SLASH,
	K_SUPERSCRIPT_TWO = 178,		// superscript 2
	K_KP_MINUS,
	K_ACUTE_ACCENT = 180,			// accute accent
	K_KP_PLUS,
	K_KP_NUMLOCK,
	K_KP_STAR,
	K_KP_EQUALS,

	K_MASCULINE_ORDINATOR = 186,	
	// K_MOUSE enums must be contiguous (no char codes in the middle)
	K_MOUSE1 = 187,
	K_MOUSE2,
	K_MOUSE3,
	K_MOUSE4,
	K_MOUSE5,
	K_MOUSE6,
	K_MOUSE7,
	K_MOUSE8,

	K_MWHEELDOWN = 195,
	K_MWHEELUP,

	K_JOY1 = 197,
	K_JOY2,
	K_JOY3,
	K_JOY4,
	K_JOY5,
	K_JOY6,
	K_JOY7,
	K_JOY8,
	K_JOY9,
	K_JOY10,
	K_JOY11,
	K_JOY12,
	K_JOY13,
	K_JOY14,
	K_JOY15,
	K_JOY16,
	K_JOY17,
	K_JOY18,
	K_JOY19,
	K_JOY20,
	K_JOY21,
	K_JOY22,
	K_JOY23,
	K_JOY24,
	K_JOY25,
	K_JOY26,
	K_JOY27,
	K_GRAVE_A = 224,	// lowercase a with grave accent
	K_JOY28,
	K_JOY29,
	K_JOY30,
	K_JOY31,
	K_JOY32,

	K_AUX1 = 230,
	K_CEDILLA_C = 231,	// lowercase c with Cedilla
	K_GRAVE_E = 232,	// lowercase e with grave accent
	K_AUX2,
	K_AUX3,
	K_AUX4,
	K_GRAVE_I = 236,	// lowercase i with grave accent
	K_AUX5,
	K_AUX6,
	K_AUX7,
	K_AUX8,
	K_TILDE_N = 241,	// lowercase n with tilde
	K_GRAVE_O = 242,	// lowercase o with grave accent
	K_AUX9,
	K_AUX10,
	K_AUX11,
	K_AUX12,
	K_AUX13,
	K_AUX14,
	K_GRAVE_U = 249,	// lowercase u with grave accent
	K_AUX15,
	K_AUX16,

	K_PRINT_SCR	= 252,	// SysRq / PrintScr
	K_RIGHT_ALT = 253,	// used by some languages as "Alt-Gr"
	K_LAST_KEY  = 254	// this better be < 256!
} keyNum_t;

typedef enum {
	SE_NONE,				// evTime is still valid
	SE_KEY,					// evValue is a key code, evValue2 is the down flag
	SE_CHAR,				// evValue is an ascii char
	SE_MOUSE,				// evValue and evValue2 are reletive signed x / y moves
	SE_JOYSTICK_AXIS,		// evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE				// evPtr is a char*, from typing something at a non-game console
} sysEventType_t;

typedef struct sysEvent_s {
	sysEventType_t	evType;
	int				evValue;
	int				evValue2;
	int				evPtrLength;		// bytes of data pointed to by evPtr, for journaling
	void *			evPtr;				// this must be manually freed if not NULL
} sysEvent_t;

typedef unsigned long address_t;

template<class type> class idList;		// for Sys_ListFiles

void			Sys_Error( const char *error, ...);
void			Sys_Quit( void );
void			Sys_Warning( const char *msg, ...);
// will go to the various text consoles
// NOT thread safe - never use in the async paths
void			Sys_Printf( const char *msg, ... );

// allow game to yield CPU time
// NOTE: due to SYS_MINSLEEP this is very bad portability karma, and should be completely removed
void			Sys_Sleep( int msec );

// Sys_Milliseconds should only be used for profiling purposes,
// any game related timing information should come from event timestamps
int			Sys_Milliseconds( void );

// for accurate performance testing
double		Sys_GetClockTicks( void );
double		Sys_ClockTicksPerSecond( void );

// returns amount of system ram
int			Sys_GetSystemRam( void );

// returns amount of video ram
int			Sys_GetVideoRam( void );

// returns amount of drive space in path
int			Sys_GetDriveFreeSpace( const char *path );

// returns memory stats
void			Sys_GetCurrentMemoryStatus( sysMemoryStats_t &stats );
void			Sys_GetExeLaunchMemoryStatus( sysMemoryStats_t &stats );

// lock and unlock memory
bool			Sys_LockMemory( void *ptr, int bytes );
bool			Sys_UnlockMemory( void *ptr, int bytes );

// set amount of physical work memory
void			Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes );

// DLL loading, the path should be a fully qualified OS path to the DLL file to be loaded
int			Sys_DLL_Load( const char *dllName );
void *		Sys_DLL_GetProcAddress( int dllHandle, const char *procName );
void		Sys_DLL_Unload( int dllHandle );

// event generation
void		Sys_GenerateEvents( void );
sysEvent_t	Sys_GetEvent( void );  
void		Sys_ClearEvents( void );

// input is tied to windows, so it needs to be started up and shut down whenever 
// the main window is recreated
void			Sys_InitInput( void );
void			Sys_ShutdownInput( void );
void			Sys_InitScanTable( void );
const unsigned char *Sys_GetScanTable( void );
unsigned char	Sys_GetConsoleKey( bool shifted );
// map a scancode key to a char
// does nothing on win32, as SE_KEY == SE_CHAR there
// on other OSes, consider the keyboard mapping
unsigned char	Sys_MapCharForKey( int key );

// keyboard input polling
int			Sys_PollKeyboardInputEvents( void );
int			Sys_ReturnKeyboardInputEvent( const int n, int &ch, bool &state );
void			Sys_EndKeyboardInputEvents( void );

// mouse input polling
int			Sys_PollMouseInputEvents( void );
int			Sys_ReturnMouseInputEvent( const int n, int &action, int &value );
void			Sys_EndMouseInputEvents( void );

// when the console is down, or the game is about to perform a lengthy
// operation like map loading, the system can release the mouse cursor
// when in windowed mode
void			Sys_GrabMouseCursor( bool grabIt );

void			Sys_ShowWindow( bool show );
bool			Sys_IsWindowVisible( void );
void			Sys_ShowConsole( int visLevel, bool quitOnClose );


void			Sys_Mkdir( const char *path );
//ID_TIME_T			Sys_FileTimeStamp( FILE *fp );
// NOTE: do we need to guarantee the same output on all platforms?
//const char *	Sys_TimeStampToStr( ID_TIME_T timeStamp );
const char *	Sys_DefaultCDPath( void );
const char *	Sys_DefaultBasePath( void );
const char *	Sys_DefaultSavePath( void );
const char *	Sys_EXEPath( void );

// use fs_debug to verbose Sys_ListFiles
// returns -1 if directory was not found (the list is cleared)
int			Sys_ListFiles( const char *directory, const char *extension, idList<class lfStr> &list );

// know early if we are performing a fatal error shutdown so the error message doesn't get lost
void			Sys_SetFatalError( const char *error );

// display perference dialog
void			Sys_DoPreferences( void );

char* Sys_ReadFileData(const char* filename);
/*
==============================================================

	Networking

==============================================================
*/

typedef enum {
	NA_BAD,					// an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP
} netadrtype_t;

typedef struct {
	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned short	port;
} netadr_t;

#define	PORT_ANY			-1

class idPort {
public:
				idPort();				// this just zeros netSocket and port
	virtual		~idPort();

	// if the InitForPort fails, the idPort.port field will remain 0
	bool		InitForPort( int portNumber );
	int		GetPort( void ) const { return bound_to.port; }
	netadr_t	GetAdr( void ) const { return bound_to; }
	void		Close();

	bool		GetPacket( netadr_t &from, void *data, int &size, int maxSize );
	bool		GetPacketBlocking( netadr_t &from, void *data, int &size, int maxSize, int timeout );
	void		SendPacket( const netadr_t to, const void *data, int size );

	int			packetsRead;
	int			bytesRead;

	int			packetsWritten;
	int			bytesWritten;

private:
	netadr_t	bound_to;		// interface and port
	int			netSocket;		// OS specific socket
};

class idTCP {
public:
				idTCP();
	virtual		~idTCP();

	// if host is host:port, the value of port is ignored
	bool		Init( const char *host, short port );
	void		Close();

	// returns -1 on failure (and closes socket)
	// those are non blocking, can be used for polling
	// there is no buffering, you are not guaranteed to Read or Write everything in a single call
	// (specially on win32, see recv and send documentation)
	int			Read( void *data, int size );
	int			Write( void *data, int size );

private:
	netadr_t	address;		// remote address
	int			fd;				// OS specific socket
};



				// parses the port number
				// can also do DNS resolve if you ask for it.
				// NOTE: DNS resolve is a slow/blocking call, think before you use
				// ( could be exploited for server DoS )
bool			Sys_StringToNetAdr( const char *s, netadr_t *a, bool doDNSResolve );
const char *	Sys_NetAdrToString( const netadr_t a );
bool			Sys_IsLANAddress( const netadr_t a );
bool			Sys_CompareNetAdrBase( const netadr_t a, const netadr_t b );

void			Sys_InitNetworking( void );
void			Sys_ShutdownNetworking( void );

double Sys_GetClockTicks( void );
double Sys_ClockTicksPerSecond( void );


bool        Sys_DrawText(const char* text, sysTextContent_t* img);

int Sys_ListAllFile( const char *directory, const char *extension, Array<lfStr>& fileList );
#endif /* !__SYS_PUBLIC__ */
