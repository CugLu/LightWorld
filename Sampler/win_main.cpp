#include "sys/win32/win_local.h"
#include "sys/sys_public.h"

#include "Material.h"
#include "ShaderSource.h"
#include "common/File.h"
#include "sampler.h"

Win32Vars_t win32;

int main()
{
	GL_CreateDevice(800, 600);
	ShadowSampler game;
	game.Init();

	//	::SetFocus( win32.hWnd );

	// main game loop
	while( 1 ) 
	{
		Sys_PumpEvents();
		game.Frame();
	}

}

int WINAPI WinMain1( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

	//Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );
	//Sys_GetCurrentMemoryStatus( exeLaunchMemoryStats );
//
	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();
	Sys_ShowConsole( 1, true );

	Sys_Init();

	GL_CreateDevice(800, 600);
	ShadowSampler game;
	game.Init();

//	::SetFocus( win32.hWnd );

    // main game loop
	while( 1 ) 
	{
		Sys_PumpEvents();
		game.Frame();
	}

	return 0;
}
