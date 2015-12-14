#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

#ifdef __cplusplus

//-----------------------------------------------------

#ifdef _WIN32

#pragma warning(disable : 4100)				// unreferenced formal parameter
#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
#pragma warning(disable : 4996)				// unsafe string operations

#include <malloc.h>							// no malloc.h on mac or unix

#endif /* _WIN32 */

//-----------------------------------------------------

#if !defined( _DEBUG ) && !defined( NDEBUG )
	// don't generate asserts
	#define NDEBUG
#endif

#ifdef _MSC_VER
	#include <stdio.h>
	#define Assert(x) { if(!(x)){printf("Assert "__FILE__ ":%u ("#x")\n", __LINE__);__debugbreak();	}}
#else//_MSC_VER
	#include <assert.h>
	#define Assert assert
#endif//_MSC_VER

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#ifdef _WIN32 
#define snprintf sprintf_s
#endif

typedef unsigned char byte;

#ifndef BIT
#define BIT( num )				( 1 << ( num ) )
#endif

//-----------------------------------------------------

#endif	/* __cplusplus */

#endif /* !__PRECOMPILED_H__ */
