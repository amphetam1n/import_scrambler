#pragma once

#include <string>
#include <Windows.h>
#undef UNICODE
#include <TlHelp32.h>
#include <map>
#include <chrono>

#define AMPHETAM1N_REFRESH_TIME std::chrono::milliseconds( 10000 )
#define AMPHETAM1N_IMPORT( fn ) \
amphetam1n::get< fn >( #fn )

namespace amphetam1n
{
	static std::map< std::string, uintptr_t > m_functions{ };
	static std::map< std::string, HMODULE > m_modules{ };
	static std::chrono::milliseconds m_last_refresh{ };
	static DWORD m_pid{ };

	static auto get_loaded_modules( ) -> void
	{
		m_last_refresh = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) );

		if ( !m_modules.empty( ) )
			m_modules.clear( );

		if ( m_pid == 0 )
			m_pid = GetCurrentProcessId( );

		auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, m_pid );
		auto me32 = MODULEENTRY32{ };

		if ( snapshot == INVALID_HANDLE_VALUE )
			throw std::exception( "CreateToolhelp32Snapshot failed" );

		std::memset( &me32, 0, sizeof( MODULEENTRY32 ) );
		me32.dwSize = sizeof( MODULEENTRY32 );

		if ( !Module32First( snapshot, &me32 ) )
		{
			CloseHandle( snapshot );
			throw std::exception( "Module32First failed" );
		}

		while ( Module32Next( snapshot, &me32 ) )
		{
			auto name = me32.szModule;
			auto handle = GetModuleHandleA( name );
			m_modules.insert( std::make_pair( name, handle ) );
		}

		CloseHandle( snapshot );
	}

	static auto find_function( const char* function ) ->uintptr_t
	{
		auto current = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) );

		if ( m_modules.empty( ) || m_last_refresh + AMPHETAM1N_REFRESH_TIME < current )
			get_loaded_modules( );

		if ( m_functions.find( function ) != m_functions.end( ) )
			return m_functions[ function ];

		for ( auto& module : m_modules )
		{
			auto func = GetProcAddress( module.second, function );

			if ( func == NULL )
				continue;

			m_functions.insert( std::make_pair( function, reinterpret_cast< uintptr_t >( func ) ) );
			break;
		}

		return m_functions[ function ];
	}

	template < typename fn >
	static auto get( const char* function ) -> fn
	{
		return reinterpret_cast< fn >( find_function( function ) );
	}
}