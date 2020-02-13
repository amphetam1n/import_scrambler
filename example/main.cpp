#include "../include/import.hpp"

int main( int argc, char** argv )
{
	LoadLibraryA( "user32.dll" );
	typedef int( __stdcall* MessageBoxA )( HWND, LPCSTR, LPCSTR, UINT );
	AMPHETAM1N_IMPORT( MessageBoxA )( NULL, "Hello, world!", "amphetam1n/import_scrambler", MB_OK );
	return 0;
}