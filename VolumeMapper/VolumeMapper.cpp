#include <iostream>
#include <Windows.h>
#include <shellapi.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vld.h> 

#include "HotkeyHelper.h"
#include "WASAPIWrapper.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define	WM_USER_SHELLICON WM_USER + 1

// global variables
HINSTANCE hInst;
NOTIFYICONDATA nidApp;
HMENU hPopMenu;
BOOL HotkeysEnabled;

// strings
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

// forward declaration
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL ShowMenu(HWND hWnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// supress warnings
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// COM
	CoInitialize(NULL);

	// define strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VOLUMEMAPPER, szWindowClass, MAX_LOADSTRING);

	// register window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_VOLUMEMAPPER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Failed to register window class!"), _T("VolumeMapper: Error"), NULL);
		return FALSE;
	}


	// init
	if (!InitInstance(hInstance, nShowCmd))
	{
		MessageBox(NULL, _T("Failed to create window."), _T("VolumeMapper: Error"), NULL);
		return FALSE;
	}

	// main loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// save instance
	hInst = hInstance;

	// try to create a window
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	// prepare icon resource
	HICON hMainIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_APP_ICON));

	nidApp.cbSize = sizeof(NOTIFYICONDATA);
	nidApp.hWnd = (HWND)hWnd;
	nidApp.uID = IDI_APP_ICON;
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nidApp.hIcon = hMainIcon;
	nidApp.uCallbackMessage = WM_USER_SHELLICON;
	LoadString(hInstance, IDS_APP_TITLE, nidApp.szTip, MAX_LOADSTRING);
	Shell_NotifyIcon(NIM_ADD, &nidApp);

	// register hotkeys
	HotkeyHelper::RegisterHotkeys(hWnd);
	HotkeysEnabled = TRUE;

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_USER_SHELLICON:
		// systray msg callback 
		switch (LOWORD(lParam))
		{
		case WM_RBUTTONDOWN:

			// build and show menu
			ShowMenu(hWnd);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// menu selection
		switch (wmId)
		{
		case ID_VOLUMEMAPPERMENU_TOGGLE:
			if (HotkeysEnabled)
			{
				HotkeyHelper::UnregisterHotkeys(hWnd);
				HotkeysEnabled = FALSE;
			}
			else
			{
				HotkeyHelper::RegisterHotkeys(hWnd);
				HotkeysEnabled = TRUE;
			}
			break;
		case ID_VOLUMEMAPPERMENU_EXIT:
			Shell_NotifyIcon(NIM_DELETE, &nidApp);
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_HOTKEY:
		// process hotkey actions
		if ((1 <= wParam) && (wParam <= 4)) {
			WASAPIWrapper::ChangeForegroundAppVolume(-0.03f);
		}
		if ((6 <= wParam) && (wParam <= 9)) {
			WASAPIWrapper::ChangeForegroundAppVolume(0.03f);
		}
		break;
	case WM_DESTROY:
		if (HotkeysEnabled)
		{
			// unregister hotkeys
			HotkeyHelper::UnregisterHotkeys(hWnd);
		}
		CoUninitialize();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

BOOL ShowMenu(HWND hWnd)
{
	POINT lpClickPoint;

	GetCursorPos(&lpClickPoint);
	hPopMenu = CreatePopupMenu();

	// enable/disable hotkey entry
	if (HotkeysEnabled)
	{
		InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, ID_VOLUMEMAPPERMENU_TOGGLE, _T("Disable"));
	}
	else
	{
		InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, ID_VOLUMEMAPPERMENU_TOGGLE, _T("Enable"));
	}

	// static entries
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, ID_VOLUMEMAPPERMENU_SEP, _T("SEP"));
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, ID_VOLUMEMAPPERMENU_EXIT, _T("Exit"));
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
	return TRUE;
}

