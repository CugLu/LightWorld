#include "win_local.h"
#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>
#include <conio.h>

#include <windows.h>
#ifndef	ID_DEDICATED
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>

#pragma comment (lib, "wbemuuid.lib")
#endif

#pragma comment( lib, "Winmm.lib")

#include "common/String.h"
#include "sys/sys_public.h"
#include "common/Array.h"

#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1)
sysEvent_t	eventQue[MAX_QUED_EVENTS];
int			eventHead = 0;
int			eventTail = 0;

/*
================
Sys_Milliseconds
================
*/
int Sys_Milliseconds( void ) {
	int sys_curtime;
	static int sys_timeBase;
	static bool	initialized = false;

	if ( !initialized ) {
		sys_timeBase = ::timeGetTime();
		initialized = true;
	}
	sys_curtime = timeGetTime() - sys_timeBase;

	return sys_curtime;
}

/*
================
Sys_GetSystemRam

	returns amount of physical memory in MB
================
*/
int Sys_GetSystemRam( void ) {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof ( statex );
	GlobalMemoryStatusEx (&statex);
	int physRam = (int)statex.ullTotalPhys / ( 1024 * 1024 );
	// HACK: For some reason, ullTotalPhys is sometimes off by a meg or two, so we round up to the nearest 16 megs
	physRam = ( physRam + 8 ) & ~15;
	return physRam;
}


/*
================
Sys_GetDriveFreeSpace
returns in megabytes
================
*/
int Sys_GetDriveFreeSpace( const char *path ) {
	DWORDLONG lpFreeBytesAvailable;
	DWORDLONG lpTotalNumberOfBytes;
	DWORDLONG lpTotalNumberOfFreeBytes;
	int ret = 26;
	//FIXME: see why this is failing on some machines
	if ( ::GetDiskFreeSpaceEx( path, (PULARGE_INTEGER)&lpFreeBytesAvailable, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes ) ) {
		ret = ( int )( lpFreeBytesAvailable ) / ( 1024.0 * 1024.0 );
	}
	return ret;
}


/*
================
Sys_GetVideoRam
returns in megabytes
================
*/
//int Sys_GetVideoRam( void ) {
//#ifdef	ID_DEDICATED
//	return 0;
//#else
//	unsigned int retSize = 64;
//
//	CComPtr<IWbemLocator> spLoc = NULL;
//	HRESULT hr = CoCreateInstance( CLSID_WbemLocator, 0, CLSCTX_SERVER, IID_IWbemLocator, ( LPVOID * ) &spLoc );
//	if ( hr != S_OK || spLoc == NULL ) {
//		return retSize;
//	}
//
//	CComBSTR bstrNamespace( _T( "\\\\.\\root\\CIMV2" ) );
//	CComPtr<IWbemServices> spServices;
//
//	// Connect to CIM
//	hr = spLoc->ConnectServer( bstrNamespace, NULL, NULL, 0, NULL, 0, 0, &spServices );
//	if ( hr != WBEM_S_NO_ERROR ) {
//		return retSize;
//	}
//
//	// Switch the security level to IMPERSONATE so that provider will grant access to system-level objects.  
//	hr = CoSetProxyBlanket( spServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );
//	if ( hr != S_OK ) {
//		return retSize;
//	}
//
//	// Get the vid controller
//	CComPtr<IEnumWbemClassObject> spEnumInst = NULL;
//	hr = spServices->CreateInstanceEnum( CComBSTR( "Win32_VideoController" ), WBEM_FLAG_SHALLOW, NULL, &spEnumInst ); 
//	if ( hr != WBEM_S_NO_ERROR || spEnumInst == NULL ) {
//		return retSize;
//	}
//
//	ULONG uNumOfInstances = 0;
//	CComPtr<IWbemClassObject> spInstance = NULL;
//	hr = spEnumInst->Next( 10000, 1, &spInstance, &uNumOfInstances );
//
//	if ( hr == S_OK && spInstance ) {
//		// Get properties from the object
//		CComVariant varSize;
//		hr = spInstance->Get( CComBSTR( _T( "AdapterRAM" ) ), 0, &varSize, 0, 0 );
//		if ( hr == S_OK ) {
//			retSize = varSize.intVal / ( 1024 * 1024 );
//			if ( retSize == 0 ) {
//				retSize = 64;
//			}
//		}
//	}
//	return retSize;
//#endif
//}

/*
================
Sys_GetCurrentMemoryStatus

	returns OS mem info
	all values are in kB except the memoryload
================
*/
void Sys_GetCurrentMemoryStatus( sysMemoryStats_t &stats ) {
	MEMORYSTATUSEX statex;
	unsigned __int64 work;

	memset( &statex, sizeof( statex ), 0 );
	statex.dwLength = sizeof( statex );
	GlobalMemoryStatusEx( &statex );

	memset( &stats, 0, sizeof( stats ) );

	stats.memoryLoad = statex.dwMemoryLoad;

	work = statex.ullTotalPhys >> 20;
	stats.totalPhysical = *(int*)&work;

	work = statex.ullAvailPhys >> 20;
	stats.availPhysical = *(int*)&work;

	work = statex.ullAvailPageFile >> 20;
	stats.availPageFile = *(int*)&work;

	work = statex.ullTotalPageFile >> 20;
	stats.totalPageFile = *(int*)&work;

	work = statex.ullTotalVirtual >> 20;
	stats.totalVirtual = *(int*)&work;

	work = statex.ullAvailVirtual >> 20;
	stats.availVirtual = *(int*)&work;

	work = statex.ullAvailExtendedVirtual >> 20;
	stats.availExtendedVirtual = *(int*)&work;
}

/*
================
Sys_LockMemory
================
*/
bool Sys_LockMemory( void *ptr, int bytes ) {
	return ( VirtualLock( ptr, (SIZE_T)bytes ) != FALSE );
}

/*
================
Sys_UnlockMemory
================
*/
bool Sys_UnlockMemory( void *ptr, int bytes ) {
	return ( VirtualUnlock( ptr, (SIZE_T)bytes ) != FALSE );
}

/*
================
Sys_SetPhysicalWorkMemory
================
*/
void Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes ) {
	::SetProcessWorkingSetSize( GetCurrentProcess(), minBytes, maxBytes );
}

/*
================
Sys_GetCurrentUser
================
*/
char *Sys_GetCurrentUser( void ) {
	static char s_userName[1024];
	unsigned long size = sizeof( s_userName );


	if ( !GetUserName( s_userName, &size ) ) {
		strcpy( s_userName, "player" );
	}

	if ( !s_userName[0] ) {
		strcpy( s_userName, "player" );
	}

	return s_userName;
}	


/*
===============================================================================

	Call stack

===============================================================================
*/


#define PROLOGUE_SIGNATURE 0x00EC8B55

#include <dbghelp.h>

const int UNDECORATE_FLAGS =	UNDNAME_NO_MS_KEYWORDS |
								UNDNAME_NO_ACCESS_SPECIFIERS |
								UNDNAME_NO_FUNCTION_RETURNS |
								UNDNAME_NO_ALLOCATION_MODEL |
								UNDNAME_NO_ALLOCATION_LANGUAGE |
								UNDNAME_NO_MEMBER_TYPE;

#if defined(_DEBUG) && 1

typedef struct symbol_s {
	int					address;
	char *				name;
	struct symbol_s *	next;
} symbol_t;

typedef struct module_s {
	int					address;
	char *				name;
	symbol_t *			symbols;
	struct module_s *	next;
} module_t;

module_t *modules;

/*
==================
SkipRestOfLine
==================
*/
void SkipRestOfLine( const char **ptr ) {
	while( (**ptr) != '\0' && (**ptr) != '\n' && (**ptr) != '\r' ) {
		(*ptr)++;
	}
	while( (**ptr) == '\n' || (**ptr) == '\r' ) {
		(*ptr)++;
	}
}

/*
==================
SkipWhiteSpace
==================
*/
void SkipWhiteSpace( const char **ptr ) {
	while( (**ptr) == ' ' ) {
		(*ptr)++;
	}
}

/*
==================
ParseHexNumber
==================
*/
int ParseHexNumber( const char **ptr ) {
	int n = 0;
	while( (**ptr) >= '0' && (**ptr) <= '9' || (**ptr) >= 'a' && (**ptr) <= 'f' ) {
		n <<= 4;
		if ( **ptr >= '0' && **ptr <= '9' ) {
			n |= ( (**ptr) - '0' );
		} else {
			n |= 10 + ( (**ptr) - 'a' );
		}
		(*ptr)++;
	}
	return n;
}

/*
==================
Sym_Init
==================
*/
//void Sym_Init( long addr ) {
//	TCHAR moduleName[MAX_STRING_CHARS];
//	MEMORY_BASIC_INFORMATION mbi;
//
//	VirtualQuery( (void*)addr, &mbi, sizeof(mbi) );
//
//	GetModuleFileName( (HMODULE)mbi.AllocationBase, moduleName, sizeof( moduleName ) );
//
//	char *ext = moduleName + strlen( moduleName );
//	while( ext > moduleName && *ext != '.' ) {
//		ext--;
//	}
//	if ( ext == moduleName ) {
//		strcat( moduleName, ".map" );
//	} else {
//		strcpy( ext, ".map" );
//	}
//
//	module_t *module = (module_t *) malloc( sizeof( module_t ) );
//	module->name = (char *) malloc( strlen( moduleName ) + 1 );
//	strcpy( module->name, moduleName );
//	module->address = (int)mbi.AllocationBase;
//	module->symbols = NULL;
//	module->next = modules;
//	modules = module;
//
//	FILE *fp = fopen( moduleName, "rb" );
//	if ( fp == NULL ) {
//		return;
//	}
//
//	int pos = ftell( fp );
//	fseek( fp, 0, SEEK_END );
//	int length = ftell( fp );
//	fseek( fp, pos, SEEK_SET );
//
//	char *text = (char *) malloc( length+1 );
//	fread( text, 1, length, fp );
//	text[length] = '\0';
//	fclose( fp );
//
//	const char *ptr = text;
//
//	// skip up to " Address" on a new line
//	while( *ptr != '\0' ) {
//		SkipWhiteSpace( &ptr );
//		if ( idStr::Cmpn( ptr, "Address", 7 ) == 0 ) {
//			SkipRestOfLine( &ptr );
//			break;
//		}
//		SkipRestOfLine( &ptr );
//	}
//
//	int symbolAddress;
//	int symbolLength;
//	char symbolName[MAX_STRING_CHARS];
//	symbol_t *symbol;
//
//	// parse symbols
//	while( *ptr != '\0' ) {
//
//		SkipWhiteSpace( &ptr );
//
//		ParseHexNumber( &ptr );
//		if ( *ptr == ':' ) {
//			ptr++;
//		} else {
//			break;
//		}
//		ParseHexNumber( &ptr );
//
//		SkipWhiteSpace( &ptr );
//
//		// parse symbol name
//		symbolLength = 0;
//		while( *ptr != '\0' && *ptr != ' ' ) {
//			symbolName[symbolLength++] = *ptr++;
//			if ( symbolLength >= sizeof( symbolName ) - 1 ) {
//				break;
//			}
//		}
//		symbolName[symbolLength++] = '\0';
//
//		SkipWhiteSpace( &ptr );
//
//		// parse symbol address
//		symbolAddress = ParseHexNumber( &ptr );
//
//		SkipRestOfLine( &ptr );
//
//		symbol = (symbol_t *) malloc( sizeof( symbol_t ) );
//		symbol->name = (char *) malloc( symbolLength );
//		strcpy( symbol->name, symbolName );
//		symbol->address = symbolAddress;
//		symbol->next = module->symbols;
//		module->symbols = symbol;
//	}
//
//	free( text );
//}

/*
==================
Sym_Shutdown
==================
*/
void Sym_Shutdown( void ) {
	module_t *m;
	symbol_t *s;

	for ( m = modules; m != NULL; m = modules ) {
		modules = m->next;
		for ( s = m->symbols; s != NULL; s = m->symbols ) {
			m->symbols = s->next;
			free( s->name );
			free( s );
		}
		free( m->name );
		free( m );
	}
	modules = NULL;
}

/*
==================
Sym_GetFuncInfo
==================
*/
//void Sym_GetFuncInfo( long addr, idStr &module, idStr &funcName ) {
//	MEMORY_BASIC_INFORMATION mbi;
//	module_t *m;
//	symbol_t *s;
//
//	VirtualQuery( (void*)addr, &mbi, sizeof(mbi) );
//
//	for ( m = modules; m != NULL; m = m->next ) {
//		if ( m->address == (int) mbi.AllocationBase ) {
//			break;
//		}
//	}
//	if ( !m ) {
//		Sym_Init( addr );
//		m = modules;
//	}
//
//	for ( s = m->symbols; s != NULL; s = s->next ) {
//		if ( s->address == addr ) {
//
//			char undName[MAX_STRING_CHARS];
//			if ( UnDecorateSymbolName( s->name, undName, sizeof(undName), UNDECORATE_FLAGS ) ) {
//				funcName = undName;
//			} else {
//				funcName = s->name;
//			}
//			for ( int i = 0; i < funcName.Length(); i++ ) {
//				if ( funcName[i] == '(' ) {
//					funcName.CapLength( i );
//					break;
//				}
//			}
//			module = m->name;
//			return;
//		}
//	}
//
//	sprintf( funcName, "0x%08x", addr );
//	module = "";
//}

#elif defined(_DEBUG)

DWORD lastAllocationBase = -1;
HANDLE processHandle;
lfStr lastModule;

/*
==================
Sym_Init
==================
*/
void Sym_Init( long addr ) {
	TCHAR moduleName[MAX_STRING_CHARS];
	TCHAR modShortNameBuf[MAX_STRING_CHARS];
	MEMORY_BASIC_INFORMATION mbi;

	if ( lastAllocationBase != -1 ) {
		Sym_Shutdown();
	}

	VirtualQuery( (void*)addr, &mbi, sizeof(mbi) );

	GetModuleFileName( (HMODULE)mbi.AllocationBase, moduleName, sizeof( moduleName ) );
	_splitpath( moduleName, NULL, NULL, modShortNameBuf, NULL );
	lastModule = modShortNameBuf;

	processHandle = GetCurrentProcess();
	if ( !SymInitialize( processHandle, NULL, FALSE ) ) {
		return;
	}
	if ( !SymLoadModule( processHandle, NULL, moduleName, NULL, (DWORD)mbi.AllocationBase, 0 ) ) {
		SymCleanup( processHandle );
		return;
	}

	SymSetOptions( SymGetOptions() & ~SYMOPT_UNDNAME );

	lastAllocationBase = (DWORD) mbi.AllocationBase;
}

/*
==================
Sym_Shutdown
==================
*/
void Sym_Shutdown( void ) {
	SymUnloadModule( GetCurrentProcess(), lastAllocationBase );
	SymCleanup( GetCurrentProcess() );
	lastAllocationBase = -1;
}

/*
==================
Sym_GetFuncInfo
==================
*/
void Sym_GetFuncInfo( long addr, lfStr &module, lfStr &funcName ) {
	MEMORY_BASIC_INFORMATION mbi;

	VirtualQuery( (void*)addr, &mbi, sizeof(mbi) );

	if ( (DWORD) mbi.AllocationBase != lastAllocationBase ) {
		Sym_Init( addr );
	}

	BYTE symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + MAX_STRING_CHARS ];
	PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)&symbolBuffer[0];
	pSymbol->SizeOfStruct = sizeof(symbolBuffer);
	pSymbol->MaxNameLength = 1023;
	pSymbol->Address = 0;
	pSymbol->Flags = 0;
	pSymbol->Size =0;

	DWORD symDisplacement = 0;
	if ( SymGetSymFromAddr( processHandle, addr, &symDisplacement, pSymbol ) ) {
		// clean up name, throwing away decorations that don't affect uniqueness
	    char undName[MAX_STRING_CHARS];
		if ( UnDecorateSymbolName( pSymbol->Name, undName, sizeof(undName), UNDECORATE_FLAGS ) ) {
			funcName = undName;
		} else {
			funcName = pSymbol->Name;
		}
		module = lastModule;
	}
	else {
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
						);
		LocalFree( lpMsgBuf );

		// Couldn't retrieve symbol (no debug info?, can't load dbghelp.dll?)
		sprintf( funcName, "0x%08x", addr );
		module = "";
    }
}

#else

/*
==================
Sym_Init
==================
*/
void Sym_Init( long addr ) {
}

/*
==================
Sym_Shutdown
==================
*/
void Sym_Shutdown( void ) {
}

/*
==================
Sym_GetFuncInfo
==================
*/
void Sym_GetFuncInfo( long addr, lfStr &module, lfStr &funcName ) {
	module = "";
	sprintf( funcName, "0x%08x", addr );
}

#endif

/*
==================
GetFuncAddr
==================
*/
//address_t GetFuncAddr( address_t midPtPtr ) {
//	long temp;
//	do {
//		temp = (long)(*(long*)midPtPtr);
//		if ( (temp&0x00FFFFFF) == PROLOGUE_SIGNATURE ) {
//			break;
//		}
//		midPtPtr--;
//	} while(true);
//
//	return midPtPtr;
//}

/*
==================
GetCallerAddr
==================
*/
//address_t GetCallerAddr( long _ebp ) {
//	long midPtPtr;
//	long res = 0;
//
//	__asm {
//		mov		eax, _ebp
//		mov		ecx, [eax]		// check for end of stack frames list
//		test	ecx, ecx		// check for zero stack frame
//		jz		label
//		mov		eax, [eax+4]	// get the ret address
//		test	eax, eax		// check for zero return address
//		jz		label
//		mov		midPtPtr, eax
//	}
//	res = GetFuncAddr( midPtPtr );
//label:
//	return res;
//}

/*
==================
Sys_GetCallStack

 use /Oy option
==================
*/
//void Sys_GetCallStack( address_t *callStack, const int callStackSize ) {
//#if 1 //def _DEBUG
//	int i;
//	long m_ebp;
//
//	__asm {
//		mov eax, ebp
//		mov m_ebp, eax
//	}
//	// skip last two functions
//	m_ebp = *((long*)m_ebp);
//	m_ebp = *((long*)m_ebp);
//	// list functions
//	for ( i = 0; i < callStackSize; i++ ) {
//		callStack[i] = GetCallerAddr( m_ebp );
//		if ( callStack[i] == 0 ) {
//			break;
//		}
//		m_ebp = *((long*)m_ebp);
//	}
//#else
//	int i = 0;
//#endif
//	while( i < callStackSize ) {
//		callStack[i++] = 0;
//	}
//}

/*
==================
Sys_GetCallStackStr
==================
*/
//const char *Sys_GetCallStackStr( const address_t *callStack, const int callStackSize ) {
//	static char string[MAX_STRING_CHARS*2];
//	int index, i;
//	idStr module, funcName;
//
//	index = 0;
//	for ( i = callStackSize-1; i >= 0; i-- ) {
//		Sym_GetFuncInfo( callStack[i], module, funcName );
//		index += sprintf( string+index, " -> %s", funcName.c_str() );
//	}
//	return string;
//}

/*
==================
Sys_GetCallStackCurStr
==================
//*/
//const char *Sys_GetCallStackCurStr( int depth ) {
//	address_t *callStack;
//
//	callStack = (address_t *) _alloca( depth * sizeof( address_t ) );
//	Sys_GetCallStack( callStack, depth );
//	return Sys_GetCallStackStr( callStack, depth );
//}

/*
==================
Sys_GetCallStackCurAddressStr
==================
*/
//const char *Sys_GetCallStackCurAddressStr( int depth ) {
//	static char string[MAX_STRING_CHARS*2];
//	address_t *callStack;
//	int index, i;
//
//	callStack = (address_t *) _alloca( depth * sizeof( address_t ) );
//	Sys_GetCallStack( callStack, depth );
//
//	index = 0;
//	for ( i = depth-1; i >= 0; i-- ) {
//		index += sprintf( string+index, " -> 0x%08x", callStack[i] );
//	}
//	return string;
//}


BOOL  SaveBmp(HBITMAP hBitmap, const char* FileName) {         
    HDC     hDC;         
    //当前分辨率下每象素所占字节数         
    int     iBits;         
    //位图中每象素所占字节数         
    WORD     wBitCount;         
    //定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数             
    DWORD     dwPaletteSize=0,   dwBmBitsSize=0,   dwDIBSize=0,   dwWritten=0;             
    //位图属性结构             
    BITMAP     Bitmap;                 
    //位图文件头结构         
    BITMAPFILEHEADER     bmfHdr;                 
    //位图信息头结构             
    BITMAPINFOHEADER     bi;                 
    //指向位图信息头结构                 
    LPBITMAPINFOHEADER     lpbi;                 
    //定义文件，分配内存句柄，调色板句柄             
    HANDLE     fh,   hDib,   hPal,hOldPal=NULL;             
  
    //计算位图文件每个像素所占字节数             
    hDC  = CreateDC("DISPLAY",   NULL,   NULL,   NULL);         
    iBits  = GetDeviceCaps(hDC,   BITSPIXEL)     *     GetDeviceCaps(hDC,   PLANES);             
    DeleteDC(hDC);             
    if(iBits <=  1)                                                   
        wBitCount = 1;             
    else  if(iBits <=  4)                               
        wBitCount  = 4;             
    else if(iBits <=  8)                               
        wBitCount  = 8;             
    else                                                                                                                               
        wBitCount  = 24;             
  
    GetObject(hBitmap,   sizeof(Bitmap),   (LPSTR)&Bitmap);         
    bi.biSize= sizeof(BITMAPINFOHEADER);         
    bi.biWidth = Bitmap.bmWidth;         
    bi.biHeight =  Bitmap.bmHeight;         
    bi.biPlanes =  1;         
    bi.biBitCount = wBitCount;         
    bi.biCompression= BI_RGB;         
    bi.biSizeImage=0;         
    bi.biXPelsPerMeter = 0;         
    bi.biYPelsPerMeter = 0;         
    bi.biClrImportant = 0;         
    bi.biClrUsed =  0;         
  
    dwBmBitsSize  = ((Bitmap.bmWidth *wBitCount+31) / 32)*4* Bitmap.bmHeight;         
  
    //为位图内容分配内存             
    hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));             
    lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);             
    *lpbi  = bi;             
  
    //     处理调色板                 
    hPal  = GetStockObject(DEFAULT_PALETTE);             
    if (hPal) {             
        hDC  = ::GetDC(NULL);             
        hOldPal = ::SelectPalette(hDC,(HPALETTE)hPal, FALSE);             
        RealizePalette(hDC);             
    }         
  
    //     获取该调色板下新的像素值             
    GetDIBits(hDC,hBitmap, 0,(UINT)Bitmap.bmHeight,  
        (LPSTR)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize,   
        (BITMAPINFO *)lpbi, DIB_RGB_COLORS);             
  
    //恢复调色板                 
    if (hOldPal){             
        ::SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);             
        RealizePalette(hDC);             
        ::ReleaseDC(NULL,   hDC);             
    }             
  
    //创建位图文件                 
    fh  = CreateFile(FileName,   GENERIC_WRITE,0,   NULL,   CREATE_ALWAYS,           
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,   NULL);             
  
    if (fh     ==  INVALID_HANDLE_VALUE)         return     FALSE;             
  
    //     设置位图文件头             
    bmfHdr.bfType  = 0x4D42;     //     "BM"             
    dwDIBSize  = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;                 
    bmfHdr.bfSize  = dwDIBSize;             
    bmfHdr.bfReserved1  = 0;             
    bmfHdr.bfReserved2  = 0;             
    bmfHdr.bfOffBits  = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;             
    //     写入位图文件头             
    WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);             
    //     写入位图文件其余内容             
    WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);             
    //清除                 
    GlobalUnlock(hDib);             
    GlobalFree(hDib);             
    CloseHandle(fh);             
  
    return     TRUE;         
}  

bool Sys_DrawText(const char* text, sysTextContent_t* img) {
	HDC _hdc = CreateCompatibleDC(win32.hDC);
	RECT rc;
	rc.top = 0;
	rc.left = 0;

	// calculate the text texture size
	int len = strlen(text);
    SelectObject(_hdc, win32.defaultFont);
	DrawText(_hdc, text, len, &rc, DT_LEFT|DT_NOCLIP|DT_CALCRECT);
    DeleteObject(win32.defaultFont);

	// create a bitmap
    if (win32.hBitmap)
        DeleteObject(win32.hBitmap);
	win32.hBitmap = CreateBitmap(rc.right, rc.bottom, 1, 32, NULL);
	
	if (win32.hBitmap == nullptr)
	{
		Sys_Error("create bitmap failed ");
		return false;
	}

	// draw the text on the _hdc
    SelectObject(_hdc, win32.defaultFont);
	HGDIOBJ hOldBmp  = SelectObject(_hdc, win32.hBitmap);
	DrawText(_hdc, text, -1, &rc, DT_LEFT|DT_NOCLIP);
    DeleteObject(win32.defaultFont);
	SelectObject(_hdc, hOldBmp);

	// get pixel
    //unsigned char* pData = new unsigned char[rc.right * rc.bottom * 4];

	if (rc.right * rc.bottom > 1024 * 1024) {
		Sys_Error("size is too large ");
		return false;
	}

    struct {
		BITMAPINFOHEADER bmiHeader;
		int mask[4];
    } bi = {0};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if(0 == GetDIBits(_hdc, win32.hBitmap, 0, 0, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS)) {
		Sys_Error("GetDIBits(_hdc, hBitmap, 0, 0, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS)");
		return false;
	}

	bi.bmiHeader.biHeight = abs(bi.bmiHeader.biHeight);
	//GetDIBits(_hdc, win32.hBitmap, 0, bi.bmiHeader.biHeight, &img->pData[0], (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
	GetDIBits(_hdc, win32.hBitmap, 0, bi.bmiHeader.biHeight, &img->pData[0], (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

	// change pixel's alpha value to 255, when it's RGB != 0
	COLORREF * pPixel = NULL;
	for (int y = 0; y < rc.bottom; ++y) {
		pPixel = (COLORREF *)img->pData + y * rc.right;
		for (int x = 0; x < rc.right; ++x) {
			COLORREF& clr = *pPixel;

			clr |= (0xffffff | (GetRValue(clr) << 24));
			clr = ~clr;
			++pPixel;
		}
	}

	//texture->init(rc.right, rc.bottom, pData);

	img->w = rc.right;
	img->h = rc.bottom;

	DeleteDC(_hdc);     // #bug 释放dc
	//SaveBmp(win32.hBitmap, "d:/1.bmp");
	return true;
}

/* ============== Sys_Quit ============== */
void Sys_Quit( void ) {
	//timeEndPeriod( 1 );
	//Sys_ShutdownInput();
	Sys_DestroyConsole();
	ExitProcess( 0 );
}


#define MAXPRINTMSG 4096
void Sys_Printf( const char *fmt, ... ) {
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start(argptr, fmt);
	lfStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';

	if ( win32.win_outputDebugString ) {
		OutputDebugString( msg );
	}
	if ( win32.win_outputEditString ) {
		Conbuf_AppendText( msg );
	}

	printf(msg);
}

#define MAXPRINTMSG 4096
void Sys_Warning( const char *fmt, ... ) {
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start(argptr, fmt);
	lfStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';
	
	Sys_Printf(msg);
}

void Sys_Error( const char *fmt, ... ) {
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start(argptr, fmt);
	lfStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';

	if ( win32.win_outputDebugString ) {
		OutputDebugString( msg );
	}
	if ( win32.win_outputEditString ) {
		Conbuf_AppendText( "error: " );
		Conbuf_AppendText( msg );
	}

	printf(msg);
}

#define MAXPRINTMSG 4096
void Sys_DebugPrintf( const char *fmt, ... ) {
	char msg[MAXPRINTMSG];

	va_list argptr;
	va_start( argptr, fmt );
	lfStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	msg[ sizeof(msg)-1 ] = '\0';
	va_end( argptr );

	OutputDebugString( msg );
}


//void Sys_ShowWindow( bool show ) {
//	::ShowWindow( win32.hWnd, show ? SW_SHOW : SW_HIDE );
//}

void Sys_PumpEvents() {
    MSG msg;

	// pump the message loop
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		if ( !GetMessage( &msg, NULL, 0, 0 ) ) {
			Sys_Quit();
		}

		// save the msg time, because wndprocs don't have access to the timestamp
		if ( win32.sysMsgTime && win32.sysMsgTime > (int)msg.time ) {
			// don't ever let the event times run backwards	
			Sys_Printf( "Sys_PumpEvents: win32.sysMsgTime (%i) > msg.time (%i)\n", win32.sysMsgTime, msg.time );
		} else {
			win32.sysMsgTime = msg.time;
		}

		TranslateMessage (&msg);
      	DispatchMessage (&msg);
	}
}

/*
================
Sys_GetClockTicks
================
*/
double Sys_GetClockTicks( void ) {
	LARGE_INTEGER li;

	QueryPerformanceCounter( &li );
	return (double ) li.LowPart + (double) 0xFFFFFFFF * li.HighPart;
}

/*
================
Sys_ClockTicksPerSecond
================
*/
double Sys_ClockTicksPerSecond( void ) {
	static double ticks = 0;

	if ( !ticks ) {
		LARGE_INTEGER li;
		QueryPerformanceFrequency( &li );
		ticks = li.QuadPart;
	}

	return ticks;
}

void Sys_Init() {
	win32.defaultFont = CreateFont(20, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_THIN, // nWeight 
        FALSE, // bItalic 倾斜
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        ("Arial")); // lpszFac
}

void Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr ) {
	sysEvent_t	*ev;

	ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

	if ( eventHead - eventTail >= MAX_QUED_EVENTS ) 
	{
		Sys_Printf("Sys_QueEvent: overflow\n");
		eventTail++;
	}

	eventHead++;

	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
}

/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents( void ) {
	eventHead = eventTail = 0;
}

/*
================
Sys_GetEvent
================
*/
sysEvent_t Sys_GetEvent( void ) {
	sysEvent_t	ev;

	// return if we have data
	if ( eventHead > eventTail ) {
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// return the empty event 
	memset( &ev, 0, sizeof( ev ) );

	return ev;
}

int Sys_ListAllFile( const char *directory, const char *extension, Array<lfStr>& fileList ) {
	char search[256];
	char path[256];
	struct _finddata_t findinfo;
	int	 findhandle;
	int	 flag;

	if ( !extension) {
		extension = "";
	}

	// passing a slash as extension will find directories
	if ( extension[0] == '/' && extension[1] == 0 ) {
		extension = "";
		flag = 0;
	} else {
		flag = _A_SUBDIR;
	}

	//sprintf( search, "%s\\*%s", directory, extension );
	sprintf( search, "%s\\*.*", directory );

	findhandle = _findfirst( search, &findinfo );
	if ( findhandle == -1 ) {
		return -1;
	}

	do {
		if ( findinfo.attrib & _A_SUBDIR ) {
			//rc4(path);
			if(strcmp(findinfo.name, ".") != 0 && strcmp(findinfo.name, "..") != 0)
			{
				sprintf(path, "%s/%s", directory, findinfo.name);
				fileList.push_back(lfStr(path));
			}
		}
		else{
			printf("File: %s\n", findinfo.name);
			sprintf(path, "%s/%s", directory, findinfo.name);
			fileList.push_back(lfStr(path));
		}

	} while ( _findnext( findhandle, &findinfo ) != -1 );

	_findclose( findhandle );

	return 0;
}