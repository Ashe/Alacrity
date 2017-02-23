// by Mark Featherstone (C) 2015 All Rights Reserved.
#include <stdio.h>
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "Error.h"
#include "UtilsDefines.h"

using namespace UDEF;


namespace Assert
{


bool defaultHandler( const char* condition, const char* msg, const char* file, int line, void *pContext )
{
	MessageBoxA( NULL, msg, "An error has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	return true;
}

HandlerFnPtr g_pfnHandler = defaultHandler;
void *g_pHandlerContext = NULL;


HandlerFnPtr getHandler()
{
	return g_pfnHandler;
}

void *getHandlerContext()
{
	return g_pHandlerContext;	
}

void setHandler( HandlerFnPtr newHandler, void *pContext )
{
	g_pfnHandler = newHandler;
	g_pHandlerContext = pContext;
}

bool report( const char* condition, const char* msg, const char* file, int line )
{
	sprintf_s( g_txt, MAX_TXT, "Condition:   %s\nFile:   %s\nLine:   %i\nInfo:   %s", condition, file, line, msg );
	return g_pfnHandler( condition, g_txt, file, line, g_pHandlerContext );
}

bool reportFormatted(const char* condition, const char* file, int line, const char* msg, ...)
{
	va_list varlist;
	va_start( varlist, msg );
	sprintf_s( g_txt, MAX_TXT, "Condition:   %s\nFile:   %s\nLine:   %i\nInfo:   %s", condition, file, line, msg );
	vsprintf_s( g_txt2, MAX_TXT, g_txt, varlist );
	va_end(varlist);
	return g_pfnHandler( condition, g_txt2, file, line, g_pHandlerContext );
}

void test()
{
	bool fail=false;
	MPOD_ASSERT(fail);
}

}