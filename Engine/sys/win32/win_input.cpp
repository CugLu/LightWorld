/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../../common/precompiled.h"
#include "../../framework/KeyInput.h"
#include "win_local.h"


#define DINPUT_BUFFERSIZE           256

#define CHAR_FIRSTREPEAT 200
#define CHAR_REPEAT 100

typedef struct MYDATA {
	LONG  lX;                   // X axis goes here
	LONG  lY;                   // Y axis goes here
	LONG  lZ;                   // Z axis goes here
	BYTE  bButtonA;             // One button goes here
	BYTE  bButtonB;             // Another button goes here
	BYTE  bButtonC;             // Another button goes here
	BYTE  bButtonD;             // Another button goes here
} MYDATA;

//static DIOBJECTDATAFORMAT rgodf[] = {
//  { &GUID_XAxis,    FIELD_OFFSET(MYDATA, lX),       DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
//  { &GUID_YAxis,    FIELD_OFFSET(MYDATA, lY),       DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
//  { &GUID_ZAxis,    FIELD_OFFSET(MYDATA, lZ),       0x80000000 | DIDFT_AXIS | DIDFT_ANYINSTANCE,   0,},
//  { 0,              FIELD_OFFSET(MYDATA, bButtonA), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
//  { 0,              FIELD_OFFSET(MYDATA, bButtonB), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
//  { 0,              FIELD_OFFSET(MYDATA, bButtonC), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
//  { 0,              FIELD_OFFSET(MYDATA, bButtonD), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
//};

//==========================================================================

static const unsigned char s_scantokey[256] = { 
//  0            1       2          3          4       5            6         7
//  8            9       A          B          C       D            E         F
	0,           27,    '1',       '2',        '3',    '4',         '5',      '6', 
	'7',        '8',    '9',       '0',        '-',    '=',          K_BACKSPACE, 9, // 0
	'q',        'w',    'e',       'r',        't',    'y',         'u',      'i', 
	'o',        'p',    '[',       ']',        K_ENTER,K_CTRL,      'a',      's',   // 1
	'd',        'f',    'g',       'h',        'j',    'k',         'l',      ';', 
	'\'',       '`',    K_SHIFT,   '\\',       'z',    'x',         'c',      'v',   // 2
	'b',        'n',    'm',       ',',        '.',    '/',         K_SHIFT,  K_KP_STAR, 
	K_ALT,      ' ',    K_CAPSLOCK,K_F1,       K_F2,   K_F3,        K_F4,     K_F5,  // 3
	K_F6,       K_F7,   K_F8,      K_F9,       K_F10,  K_PAUSE,     K_SCROLL, K_HOME, 
	K_UPARROW,  K_PGUP, K_KP_MINUS,K_LEFTARROW,K_KP_5, K_RIGHTARROW,K_KP_PLUS,K_END, // 4
	K_DOWNARROW,K_PGDN, K_INS,     K_DEL,      0,      0,           0,        K_F11, 
	K_F12,      0,      0,         K_LWIN,     K_RWIN, K_MENU,      0,        0,     // 5
	0,          0,      0,         0,          0,      0,           0,        0, 
	0,          0,      0,         0,          0,      0,           0,        0,     // 6
	0,          0,      0,         0,          0,      0,           0,        0, 
	0,          0,      0,         0,          0,      0,           0,        0,      // 7
// shifted
	0,           27,    '!',       '@',        '#',    '$',         '%',      '^', 
	'&',        '*',    '(',       ')',        '_',    '+',          K_BACKSPACE, 9, // 0
	'q',        'w',    'e',       'r',        't',    'y',         'u',      'i', 
	'o',        'p',    '[',       ']',        K_ENTER,K_CTRL,      'a',      's',   // 1
	'd',        'f',    'g',       'h',        'j',    'k',         'l',      ';', 
	'\'',       '~',    K_SHIFT,   '\\',       'z',    'x',         'c',      'v',   // 2
	'b',        'n',    'm',       ',',        '.',    '/',         K_SHIFT,  K_KP_STAR, 
	K_ALT,      ' ',    K_CAPSLOCK,K_F1,       K_F2,   K_F3,        K_F4,     K_F5,  // 3
	K_F6,       K_F7,   K_F8,      K_F9,       K_F10,  K_PAUSE,     K_SCROLL, K_HOME, 
	K_UPARROW,  K_PGUP, K_KP_MINUS,K_LEFTARROW,K_KP_5, K_RIGHTARROW,K_KP_PLUS,K_END, // 4
	K_DOWNARROW,K_PGDN, K_INS,     K_DEL,      0,      0,           0,        K_F11, 
	K_F12,      0,      0,         K_LWIN,     K_RWIN, K_MENU,      0,        0,     // 5
	0,          0,      0,         0,          0,      0,           0,        0, 
	0,          0,      0,         0,          0,      0,           0,        0,     // 6
	0,          0,      0,         0,          0,      0,           0,        0, 
	0,          0,      0,         0,          0,      0,           0,        0      // 7
}; 

static const unsigned char *keyScanTable = s_scantokey;	

// this should be part of the scantables and the scan tables should be 512 bytes
// (256 scan codes, shifted and unshifted).  Changing everything to use 512 byte
// scan tables now might introduce bugs in tested code.  Since we only need to fix
// the right-alt case for non-US keyboards, we're just using a special-case table
// for it.  Eventually, the tables above should be fixed to handle all possible
// scan codes instead of just the first 128.
static unsigned char	rightAltKey = K_ALT;

#define NUM_OBJECTS (sizeof(rgodf) / sizeof(rgodf[0]))

//static DIDATAFORMAT	df = {
//	sizeof(DIDATAFORMAT),       // this structure
//	sizeof(DIOBJECTDATAFORMAT), // size of object data format
//	DIDF_RELAXIS,               // absolute axis coordinates
//	sizeof(MYDATA),             // device data size
//	NUM_OBJECTS,                // number of objects
//	rgodf,                      // and here they are
//};

/*
=======
MapKey

Map from windows to quake keynums

FIXME: scan code tables should include the upper 128 scan codes instead
	   of having to special-case them here.  The current code makes it difficult
	   to special-case conversions for non-US keyboards.  Currently the only
	   special-case is for right alt.
=======
*/
int IN_DIMapKey (int key) {
	if ( key>=128 ) {
		switch ( key ) {
			case DIK_HOME:
				return K_HOME;
			case DIK_UPARROW:
				return K_UPARROW;
			case DIK_PGUP:
				return K_PGUP;
			case DIK_LEFTARROW:
				return K_LEFTARROW;
			case DIK_RIGHTARROW:
				return K_RIGHTARROW;
			case DIK_END:
				return K_END;
			case DIK_DOWNARROW:
				return K_DOWNARROW;
			case DIK_PGDN:
				return K_PGDN;
			case DIK_INSERT:
				return K_INS;
			case DIK_DELETE:
				return K_DEL;
			case DIK_RMENU:
				return rightAltKey;
			case DIK_RCONTROL:
				return K_CTRL;
			case DIK_NUMPADENTER:
				return K_KP_ENTER;
			case DIK_NUMPADEQUALS:
				return K_KP_EQUALS;
			case DIK_PAUSE:
				return K_PAUSE;
			case DIK_DIVIDE:
				return K_KP_SLASH;
			case DIK_LWIN:
				return K_LWIN;
			case DIK_RWIN:
				return K_RWIN;
			case DIK_APPS:
				return K_MENU;
			case DIK_SYSRQ:
				return K_PRINT_SCR;
			default:
				return 0;
		}
	} else {
		switch (key) {
			case DIK_NUMPAD7:
				return K_KP_HOME;
			case DIK_NUMPAD8:
				return K_KP_UPARROW;
			case DIK_NUMPAD9:
				return K_KP_PGUP;
			case DIK_NUMPAD4:
				return K_KP_LEFTARROW;
			case DIK_NUMPAD5:
				return K_KP_5;
			case DIK_NUMPAD6:
				return K_KP_RIGHTARROW;
			case DIK_NUMPAD1:
				return K_KP_END;
			case DIK_NUMPAD2:
				return K_KP_DOWNARROW;
			case DIK_NUMPAD3:
				return K_KP_PGDN;
			case DIK_NUMPAD0:
				return K_KP_INS;
			case DIK_DECIMAL:
				return K_KP_DEL;
			case DIK_SUBTRACT:
				return K_KP_MINUS;
			case DIK_ADD:
				return K_KP_PLUS;
			case DIK_NUMLOCK:
				return K_KP_NUMLOCK;
			case DIK_MULTIPLY:
				return K_KP_STAR;
			default:
				return keyScanTable[key];
		}
	}
}




/*
============================================================

  MOUSE CONTROL

============================================================
*/

==================
Sys_GetScanTable
==================
*/
const unsigned char *Sys_GetScanTable( void ) {
	return keyScanTable;
}

/*
===============
Sys_GetConsoleKey
===============
*/
unsigned char Sys_GetConsoleKey( bool shifted ) {
	return keyScanTable[41 + ( shifted ? 128 : 0 )];
}


//=====================================================================================

static DIDEVICEOBJECTDATA polled_didod[ DINPUT_BUFFERSIZE ];  // Receives buffered data 

static int diFetch;
static byte toggleFetch[2][ 256 ];


#if 1
// I tried doing the full-state get to address a keyboard problem on one system,
// but it didn't make any difference

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_PollKeyboardInputEvents( void ) {
    DWORD              dwElements;
    HRESULT            hr;

    if( win32.g_pKeyboard == NULL ) {
        return 0;
	}
    
    dwElements = DINPUT_BUFFERSIZE;
    hr = win32.g_pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     polled_didod, &dwElements, 0 );
    if( hr != DI_OK ) 
    {
        // We got an error or we got DI_BUFFEROVERFLOW.
        //
        // Either way, it means that continuous contact with the
        // device has been lost, either due to an external
        // interruption, or because the buffer overflowed
        // and some events were lost.
        hr = win32.g_pKeyboard->Acquire();

		

		// nuke the garbage
		if (!FAILED(hr)) {
			//Bug 951: The following command really clears the garbage input.
			//The original will still process keys in the buffer and was causing
			//some problems.
			win32.g_pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), NULL, &dwElements, 0 );
			dwElements = 0;
		}
        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
    }

    if( FAILED(hr) ) {
        return 0;
	}

	return dwElements;
}

#else

/*
====================
Sys_PollKeyboardInputEvents

Fake events by getting the entire device state
and checking transitions
====================
*/
int Sys_PollKeyboardInputEvents( void ) {
    HRESULT            hr;

    if( win32.g_pKeyboard == NULL ) {
        return 0;
	}
    
	hr = win32.g_pKeyboard->GetDeviceState( sizeof( toggleFetch[ diFetch ] ), toggleFetch[ diFetch ] );
    if( hr != DI_OK ) 
    {
        // We got an error or we got DI_BUFFEROVERFLOW.
        //
        // Either way, it means that continuous contact with the
        // device has been lost, either due to an external
        // interruption, or because the buffer overflowed
        // and some events were lost.
        hr = win32.g_pKeyboard->Acquire();

		// nuke the garbage
		if (!FAILED(hr)) {
			hr = win32.g_pKeyboard->GetDeviceState( sizeof( toggleFetch[ diFetch ] ), toggleFetch[ diFetch ] );
		}
        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
    }

    if( FAILED(hr) ) {
        return 0;
	}

	// build faked events
	int		numChanges = 0;

	for ( int i = 0 ; i < 256 ; i++ ) {
		if ( toggleFetch[0][i] != toggleFetch[1][i] ) {
			polled_didod[ numChanges ].dwOfs = i;
			polled_didod[ numChanges ].dwData = toggleFetch[ diFetch ][i] ? 0x80 : 0;
			numChanges++;
		}
	}

	diFetch ^= 1;

	return numChanges;
}

#endif

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_ReturnKeyboardInputEvent( const int n, int &ch, bool &state ) {
	ch = IN_DIMapKey( polled_didod[ n ].dwOfs );
	state = (polled_didod[ n ].dwData & 0x80) == 0x80;
	if ( ch == K_PRINT_SCR || ch == K_CTRL || ch == K_ALT || ch == K_RIGHT_ALT ) {
		// for windows, add a keydown event for print screen here, since
		// windows doesn't send keydown events to the WndProc for this key.
		// ctrl and alt are handled here to get around windows sending ctrl and
		// alt messages when the right-alt is pressed on non-US 102 keyboards.
		Sys_QueEvent( GetTickCount(), SE_KEY, ch, state, 0, NULL );
	}
	return ch;
}


void Sys_EndKeyboardInputEvents( void ) {
}

void Sys_QueMouseEvents( int dwElements ) {
	int i, value;

	for( i = 0; i < dwElements; i++ ) {
		if ( polled_didod[i].dwOfs >= DIMOFS_BUTTON0 && polled_didod[i].dwOfs <= DIMOFS_BUTTON7 ) {
			value = (polled_didod[i].dwData & 0x80) == 0x80;
			Sys_QueEvent( polled_didod[i].dwTimeStamp, SE_KEY, K_MOUSE1 + ( polled_didod[i].dwOfs - DIMOFS_BUTTON0 ), value, 0, NULL );
		} else {
			switch (polled_didod[i].dwOfs) {
			case DIMOFS_X:
				value = polled_didod[i].dwData;
				Sys_QueEvent( polled_didod[i].dwTimeStamp, SE_MOUSE, value, 0, 0, NULL );
				break;
			case DIMOFS_Y:
				value = polled_didod[i].dwData;
				Sys_QueEvent( polled_didod[i].dwTimeStamp, SE_MOUSE, 0, value, 0, NULL );
				break;
			case DIMOFS_Z:
				value = ( (int) polled_didod[i].dwData ) / WHEEL_DELTA;
				int key = value < 0 ? K_MWHEELDOWN : K_MWHEELUP;
				value = abs( value );
				while( value-- > 0 ) {
					Sys_QueEvent( polled_didod[i].dwTimeStamp, SE_KEY, key, true, 0, NULL );
					Sys_QueEvent( polled_didod[i].dwTimeStamp, SE_KEY, key, false, 0, NULL );
				}
				break;
			}
		}
	}
}

//=====================================================================================

int Sys_PollMouseInputEvents( void ) {
	DWORD				dwElements;
	HRESULT				hr;

	if ( !win32.g_pMouse || !win32.mouseGrabbed ) {
		return 0;
	}

    dwElements = DINPUT_BUFFERSIZE;
    hr = win32.g_pMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), polled_didod, &dwElements, 0 );

    if( hr != DI_OK ) {
        hr = win32.g_pMouse->Acquire();
		// clear the garbage
		if (!FAILED(hr)) {
			win32.g_pMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), polled_didod, &dwElements, 0 );
		}
    }

    if( FAILED(hr) ) {
        return 0;
	}

	Sys_QueMouseEvents( dwElements );

	return dwElements;
}

int Sys_ReturnMouseInputEvent( const int n, int &action, int &value ) {
	int diaction = polled_didod[n].dwOfs;

	if ( diaction >= DIMOFS_BUTTON0 && diaction <= DIMOFS_BUTTON7 ) {
		value = (polled_didod[n].dwData & 0x80) == 0x80;
		action = M_ACTION1 + ( diaction - DIMOFS_BUTTON0 );
		return 1;
	}

	switch( diaction ) {
		case DIMOFS_X:
			value = polled_didod[n].dwData;
			action = M_DELTAX;
			return 1;
		case DIMOFS_Y:
			value = polled_didod[n].dwData;
			action = M_DELTAY;
			return 1;
		case DIMOFS_Z:
			// mouse wheel actions are impulses, without a specific up / down
			value = ( (int) polled_didod[n].dwData ) / WHEEL_DELTA;
			action = M_DELTAZ;
			// a value of zero here should never happen
			if ( value == 0 ) {
				return 0;
			}
			return 1;
	}
	return 0;
}

void Sys_EndMouseInputEvents( void ) { }

unsigned char Sys_MapCharForKey( int key ) {
	return (unsigned char)key;
}
