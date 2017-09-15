#include "WASAPIWrapper.h"

const CLSID WASAPIWrapper::CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID WASAPIWrapper::IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

bool WASAPIWrapper::ChangeForegroundAppVolume(float delta)
{
	HRESULT hr = S_OK;

	IMMDevice* pDevice = NULL;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IAudioSessionManager2* pSessionManager = NULL;
	IAudioSessionEnumerator* pSessionEnumerator = NULL;
	IAudioSessionControl* pSessionControl = NULL;
	IAudioSessionControl2* pSessionControl2 = NULL;
	AudioSessionState* pSessionState = NULL;
	ISimpleAudioVolume* pVolume = NULL;
	DWORD processId;
	float volumeLevel;
	int sessionCount = 0;

	// get active window
	DWORD foregroundAppProcessId;
	GetWindowThreadProcessId(GetForegroundWindow(), &foregroundAppProcessId);

	hr = CoCreateInstance(WASAPIWrapper::CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, WASAPIWrapper::IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	// get default audio device
	hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr);

	// get session manager
	hr = pDevice->Activate( __uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
	EXIT_ON_ERROR(hr);

	// get session enumerator
	hr = pSessionManager->GetSessionEnumerator(&pSessionEnumerator);
	EXIT_ON_ERROR(hr);
	
	hr = pSessionEnumerator->GetCount(&sessionCount);
	EXIT_ON_ERROR(hr);
	for (int i = 0; i < sessionCount; i++)
	{
		hr = pSessionEnumerator->GetSession(i, &pSessionControl);
		EXIT_ON_ERROR(hr);

		hr = pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);
		EXIT_ON_ERROR(hr);

		// don't check if it's system sounds
		if (pSessionControl2->IsSystemSoundsSession() == S_FALSE)
		{
			hr = pSessionControl2->GetProcessId(&processId);
			EXIT_ON_ERROR(hr);

			if (processId == foregroundAppProcessId)
			{
				// debug
				//std::wostringstream ss;
				//ss << "Session PID: " << processId << "\nForeground PID: " << foregroundAppProcessId << "\n";
				//MessageBox(NULL, ss.str().c_str(), _T("VolumeMapper"), NULL);

				hr = pSessionControl2->QueryInterface(_uuidof(ISimpleAudioVolume), (void**)&pVolume);
				EXIT_ON_ERROR(hr);

				// get and adjust volume level
				hr = pVolume->GetMasterVolume(&volumeLevel);
				EXIT_ON_ERROR(hr);
				hr = pVolume->SetMasterVolume(max(0.0f, volumeLevel + delta), 0);
			}
		}
	}

Exit:
	SAFE_RELEASE(pSessionManager);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pSessionEnumerator);
	SAFE_RELEASE(pSessionControl);
	SAFE_RELEASE(pSessionControl2);
	SAFE_RELEASE(pVolume);

	return FALSE;
}

WASAPIWrapper::WASAPIWrapper()
{
}


WASAPIWrapper::~WASAPIWrapper()
{
}
