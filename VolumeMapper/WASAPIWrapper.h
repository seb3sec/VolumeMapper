#pragma once

#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Audiopolicy.h>
#include <Winerror.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <comdef.h>
#include <psapi.h>
#include <tchar.h>
#include <sstream>

#define SAFE_RELEASE(p) if ((p) != NULL) { (p)->Release(); (p) = NULL; }

class WASAPIWrapper
{
public:
	static bool ChangeForegroundAppVolume(float value, bool addValue);
	static float GetPreviousVolumeLevel();

	static const CLSID CLSID_MMDeviceEnumerator;
	static const IID IID_IMMDeviceEnumerator;
private:
	WASAPIWrapper();
	~WASAPIWrapper();

	static bool Setup();
	static void Reset();

	// WASAPI pointers
	static IMMDeviceEnumerator* pEnumerator;
	static IMMDevice* pDevice;
	static IAudioSessionManager2* pSessionManager;
	static IAudioSessionEnumerator* pSessionEnumerator;
	static int sessionCount;

	static float PreviousVolumeLevel;
};

