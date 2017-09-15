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

#define EXIT_ON_ERROR(hres) if (FAILED(hres)) { printf("ERROR: 0x%08x\n", hres); goto Exit; }
#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

class WASAPIWrapper
{
public:
	static bool ChangeForegroundAppVolume(float delta);

	static const CLSID CLSID_MMDeviceEnumerator;
	static const IID IID_IMMDeviceEnumerator;
private:
	WASAPIWrapper();
	~WASAPIWrapper();
};

