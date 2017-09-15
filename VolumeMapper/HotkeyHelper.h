#pragma once

#include <Windows.h>
#include <tchar.h>

class HotkeyHelper
{
public:
	static bool RegisterHotkeys(HWND hWnd);
	static bool UnregisterHotkeys(HWND hWnd);
private:
	HotkeyHelper();
	~HotkeyHelper();
};


