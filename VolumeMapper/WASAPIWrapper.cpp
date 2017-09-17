#include "WASAPIWrapper.h"

const CLSID WASAPIWrapper::CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID WASAPIWrapper::IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

IMMDeviceEnumerator* WASAPIWrapper::pEnumerator = NULL;
IMMDevice* WASAPIWrapper::pDevice = NULL;
IAudioSessionManager2* WASAPIWrapper::pSessionManager = NULL;
IAudioSessionEnumerator* WASAPIWrapper::pSessionEnumerator = NULL;
int WASAPIWrapper::sessionCount = 0;
float WASAPIWrapper::PreviousVolumeLevel = 0.05f;

bool WASAPIWrapper::ChangeForegroundAppVolume(float value, bool addValue)
{
	HRESULT hr = S_OK;

	IAudioSessionControl* pSessionControl = NULL;
	IAudioSessionControl2* pSessionControl2 = NULL;
	AudioSessionState* pSessionState = NULL;
	ISimpleAudioVolume* pVolume = NULL;
	DWORD processId;
	float volumeLevel;

	WASAPIWrapper::Setup();

	// get active window and its process id
	DWORD foregroundAppProcessId;
	GetWindowThreadProcessId(GetForegroundWindow(), &foregroundAppProcessId);

	// iterate over sessions
	for (int i = 0; i < sessionCount; i++)
	{
		hr = pSessionEnumerator->GetSession(i, &pSessionControl);
		if (FAILED(hr)) {
			WASAPIWrapper::Reset();
			SAFE_RELEASE(pSessionControl);
			return false;
		}

		hr = pSessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pSessionControl2);
		if (FAILED(hr)) {
			WASAPIWrapper::Reset();
			SAFE_RELEASE(pSessionControl);
			SAFE_RELEASE(pSessionControl2);
			return false;
		}

		// don't check if it's system sounds
		if (pSessionControl2->IsSystemSoundsSession() == S_FALSE)
		{
			hr = pSessionControl2->GetProcessId(&processId);
			if (FAILED(hr)) {
				WASAPIWrapper::Reset();
				SAFE_RELEASE(pSessionControl);
				SAFE_RELEASE(pSessionControl2);
				return false;
			}

			if (processId == foregroundAppProcessId)
			{
				hr = pSessionControl2->QueryInterface(_uuidof(ISimpleAudioVolume), (void**)&pVolume);
				if (FAILED(hr)) {
					WASAPIWrapper::Reset();
					SAFE_RELEASE(pSessionControl);
					SAFE_RELEASE(pSessionControl2);
					SAFE_RELEASE(pVolume);
					return false;
				}

				// get volume level
				hr = pVolume->GetMasterVolume(&volumeLevel);
				if (FAILED(hr)) {
					WASAPIWrapper::Reset();
					SAFE_RELEASE(pSessionControl);
					SAFE_RELEASE(pSessionControl2);
					SAFE_RELEASE(pVolume);
					return false;
				}

				// if value is meant as absolute value reset the base
				if (addValue)
				{
					volumeLevel += value;
				}
				else
				{
					// save old value
					WASAPIWrapper::PreviousVolumeLevel = volumeLevel;

					volumeLevel = value;
				}

				// set volume
				hr = pVolume->SetMasterVolume(max(0.0f, volumeLevel), 0);

				// free loop resources
				SAFE_RELEASE(pVolume);
			}
		}

		SAFE_RELEASE(pSessionControl);
		SAFE_RELEASE(pSessionControl2);
	}

	WASAPIWrapper::Reset();

	return false;
}

float WASAPIWrapper::GetPreviousVolumeLevel()
{
	return WASAPIWrapper::PreviousVolumeLevel;
}

WASAPIWrapper::WASAPIWrapper()
{
}


WASAPIWrapper::~WASAPIWrapper()
{
}

bool WASAPIWrapper::Setup()
{
	HRESULT hr = S_OK;

	hr = CoCreateInstance(WASAPIWrapper::CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, WASAPIWrapper::IID_IMMDeviceEnumerator, (void**)&WASAPIWrapper::pEnumerator);
	if (FAILED(hr)) {
		WASAPIWrapper::Reset();
		return false;
	}

	// get default audio device
	hr = WASAPIWrapper::pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &WASAPIWrapper::pDevice);
	if (FAILED(hr)) {
		WASAPIWrapper::Reset();
		return false;
	}

	// get session manager
	hr = WASAPIWrapper::pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&WASAPIWrapper::pSessionManager);
	if (FAILED(hr)) {
		WASAPIWrapper::Reset();
		return false;
	}

	// get session enumerator
	hr = WASAPIWrapper::pSessionManager->GetSessionEnumerator(&WASAPIWrapper::pSessionEnumerator);
	if (FAILED(hr)) {
		WASAPIWrapper::Reset();
		return false;
	}

	// get session count
	hr = WASAPIWrapper::pSessionEnumerator->GetCount(&WASAPIWrapper::sessionCount);
	if (FAILED(hr)) {
		WASAPIWrapper::Reset();
		return false;
	}

	return true;
}

void WASAPIWrapper::Reset()
{
	SAFE_RELEASE(WASAPIWrapper::pEnumerator);
	SAFE_RELEASE(WASAPIWrapper::pDevice);
	SAFE_RELEASE(WASAPIWrapper::pSessionManager);
	SAFE_RELEASE(WASAPIWrapper::pSessionEnumerator);
	WASAPIWrapper::sessionCount = 0;
}
