﻿#ifndef __MINIDUMP_H__
#define __MINIDUMP_H__

#ifndef _UNICODE
#define _UNICODE
#endif

#include <winsock2.h>
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>

class MiniDumper
{
private:
	static CRITICAL_SECTION cs_;
	MiniDumper();
	MiniDumper(const MiniDumper & ){}

	static LONG WINAPI  HandleException(LPEXCEPTION_POINTERS pExceptionPointers);
public:
	~MiniDumper();

	static MiniDumper & Instance();
	void Install();
};


#endif //__MINIDUMP_H__
