#include "win_local.h"
#include "sys/sys_public.h"

LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

int MapKey (int key)
{
	if (key>=65 && key<=91)
		key = key + 'A' - 'a';

	return key;
}

LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	int key;
	switch( uMsg ) {
		case WM_WINDOWPOSCHANGED:
			break;

		case WM_CREATE:

			win32.hWnd = hWnd;

			//void GLW_WM_CREATE( HWND hWnd );
			//GLW_WM_CREATE( hWnd );

			break;

		case WM_DESTROY:
			break;

		case WM_CLOSE:
			Sys_Quit();
			break;

		case WM_ACTIVATE:
			// if we got here because of an alt-tab or maximize,
			// we should activate immediately.  If we are here because
			// the mouse was clicked on a title bar or drag control,
			// don't activate until the mouse button is released
			{
				int	fActive, fMinimized;

				fActive = LOWORD(wParam);
				fMinimized = (BOOL) HIWORD(wParam);

				//win32.activeApp = (fActive != WA_INACTIVE);
				//if ( win32.activeApp ) {
				//	idKeyInput::ClearStates();
				//	com_editorActive = false;
				//	Sys_GrabMouseCursor( true );
				//}

				//if ( fActive == WA_INACTIVE ) {
				//	win32.movingWindow = false;
				//}

				//// start playing the game sound world
				//session->SetPlayingSoundWorld();

				// we do not actually grab or release the mouse here,
				// that will be done next time through the main loop
			}
			break;

		case WM_MOVE: {
			int		xPos, yPos;
			RECT r;
			int		style;

			if (!win32.cdsFullscreen )
			{
				xPos = (short) LOWORD(lParam);    // horizontal position 
				yPos = (short) HIWORD(lParam);    // vertical position 

				r.left   = 0;
				r.top    = 0;
				r.right  = 1;
				r.bottom = 1;

				style = GetWindowLong( hWnd, GWL_STYLE );
				AdjustWindowRect( &r, style, FALSE );

				/*		win32.win_xpos.SetInteger( xPos + r.left );
				win32.win_ypos.SetInteger( yPos + r.top );
				win32.win_xpos.ClearModified();
				win32.win_ypos.ClearModified();*/
			}
			break;
		}
		case WM_TIMER: {
			//if ( win32.win_timerUpdate.GetBool() ) {
				//common->Frame();
			//}
			break;
		}
		case WM_SYSCOMMAND:
			if ( wParam == SC_SCREENSAVE || wParam == SC_KEYMENU ) {
				return 0;
			}
			break;

		case WM_SYSKEYDOWN:
			//if ( wParam == 13 ) {	// alt-enter toggles full-screen
			//	cvarSystem->SetCVarBool( "r_fullscreen", !renderSystem->IsFullScreen() );
			//	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "vid_restart\n" );
			//	return 0;
			//}
			// fall through for other keys
		case WM_KEYDOWN:
			key = MapKey(wParam);
			Sys_QueEvent( win32.sysMsgTime, SE_KEY, key, true, 0, NULL );
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			key = MapKey(wParam);
			Sys_QueEvent( win32.sysMsgTime, SE_KEY, key, false, 0, NULL );
			break;

		case WM_CHAR:
			//Sys_QueEvent( win32.sysMsgTime, SE_CHAR, wParam, 0, 0, NULL );
			break;

		case WM_NCLBUTTONDOWN:
//			win32.movingWindow = true;
			break;

		case WM_ENTERSIZEMOVE:
			win32.movingWindow = true;
			break;

		case WM_EXITSIZEMOVE:
			win32.movingWindow = false;
			break;

		case WM_SIZING:
			//WIN_Sizing(wParam, (RECT *)lParam);
			break;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		//case WM_MOUSEMOVE: 
		case WM_LBUTTONDOWN:{
			POINT p;
			GetCursorPos(&p);
			Sys_QueEvent( win32.sysMsgTime, SE_MOUSE, p.x, p.y, 0, NULL );
			break;
		}
		case WM_MOUSEWHEEL: {
			int delta = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
//			int key = delta < 0 ? K_MWHEELDOWN : K_MWHEELUP;
			delta = abs( delta );
			while( delta-- > 0 ) {
				//Sys_QueEvent( win32.sysMsgTime, SE_KEY, key, true, 0, NULL );
				//Sys_QueEvent( win32.sysMsgTime, SE_KEY, key, false, 0, NULL );
			}
			break;
		}
	}

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
