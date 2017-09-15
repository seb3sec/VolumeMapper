#include "HotkeyHelper.h"

HotkeyHelper::HotkeyHelper()
{
}


HotkeyHelper::~HotkeyHelper()
{
}

bool HotkeyHelper::RegisterHotkeys(HWND hWnd)
{
	// volume down
	if (!RegisterHotKey(hWnd, 1, MOD_NOREPEAT, 0xAE))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_UP (0xAE)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 2, MOD_NOREPEAT | MOD_ALT, 0xAE))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_UP (0xAE)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 3, MOD_NOREPEAT | MOD_SHIFT, 0xAE))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_UP (0xAE)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 4, MOD_NOREPEAT | MOD_ALT | MOD_SHIFT, 0xAE))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_UP (0xAE)!"), _T("VolumeMapper"), MB_OK);
	}
	// volume up
	if (!RegisterHotKey(hWnd, 6, MOD_NOREPEAT, 0xAF))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_DOWN (0xAF)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 7, MOD_NOREPEAT | MOD_ALT, 0xAF))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_DOWN (0xAF)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 8, MOD_NOREPEAT | MOD_SHIFT, 0xAF))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_DOWN (0xAF)!"), _T("VolumeMapper"), MB_OK);
	}
	if (!RegisterHotKey(hWnd, 9, MOD_NOREPEAT | MOD_ALT | MOD_SHIFT, 0xAF))
	{
		MessageBox(NULL, _T("Failed to register hotkey VOLUME_DOWN (0xAF)!"), _T("VolumeMapper"), MB_OK);
	}

	return TRUE;
}

bool HotkeyHelper::UnregisterHotkeys(HWND hWnd)
{
	// volume down
	UnregisterHotKey(hWnd, 1);
	UnregisterHotKey(hWnd, 2);
	UnregisterHotKey(hWnd, 3);
	UnregisterHotKey(hWnd, 4);
	// volume up
	UnregisterHotKey(hWnd, 6);
	UnregisterHotKey(hWnd, 7);
	UnregisterHotKey(hWnd, 8);
	UnregisterHotKey(hWnd, 9);

	return TRUE;
}