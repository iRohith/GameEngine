#pragma once

#include <windows.h>
#include <windowsx.h>

struct Win32HNDL {
	HWND hWnd;
	HINSTANCE hInstance;
	WINDOWPLACEMENT wpc;
	LONG HWNDStyle;
	LONG HWNDStyleEx;
	HDC hdc;
	int index;
};

constexpr inline Win32HNDL* GetWin32HNDL(void* ptr) { return (Win32HNDL*)ptr; }
