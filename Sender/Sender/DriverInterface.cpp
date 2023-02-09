#include "Common.h"
#include "DeviceEnumeration.h"
#include "Device.h"
#include "DriverInterface.h"


int Init()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (!SUCCEEDED(hr))
	{
		return 0;
	}

	numDevices = EnumerateDevicePaths(cachedPaths, NUM_MAX_PATHS);
	if (numDevices < 0) {
		return 0;
	}

	temporaryBuffer = malloc(TEMPORARY_BUFFER_SIZE);
	if (temporaryBuffer == NULL)
	{
		return 0;
	}

	return 1;
}

int Free()
{
	if (activeDevice != NULL) 
	{
		delete activeDevice;
	}

	free(temporaryBuffer);

	CoUninitialize(); 

	return 1;
}

int GetNumDevices()
{
	return numDevices;
}

int GetDevicePath(int index, char* str, int maxLen)
{
	if (index < 0 || index >= numDevices) {
		return -1;
	}

	strcpy_s(str, maxLen, cachedPaths[index].c_str());

	return 1;
}

void DestroyDevice()
{
	if (activeDevice != NULL)
	{
		delete activeDevice;
		activeDevice = NULL;
	}
}

int SetDevice(char* str, int strLen) 
{
	DestroyDevice();

	IBaseFilter* filter = NULL;
	if (!GetFilter(string(str), &filter) || filter == NULL)
	{
		return 0;
	}

	activeDevice = new Device(filter);
	if (!activeDevice->Init()) 
	{
		delete activeDevice;
		activeDevice = NULL;

		return 0;
	}

	return 1;
}

int SetBuffer(PVOID data, DWORD stride, DWORD width, DWORD height)
{
	if (activeDevice == NULL) 
	{
		return -1;
	}

	if (width != WIDTH || height != HEIGHT) 
	{
		return -1;
	}


	PUCHAR inputData = (PUCHAR) data;
	PUCHAR buffer = (PUCHAR)temporaryBuffer;
	
	for (ULONG y = 0; y < height; y++)
	{
		PUCHAR sourceLine = inputData + (4 * WIDTH) * (y);
		PUCHAR targetLine = buffer + ((WIDTH * 4) * y);
		memcpy(targetLine, sourceLine, WIDTH * 4);
	}

	activeDevice->SetData(temporaryBuffer, TEMPORARY_BUFFER_SIZE);

	return 1;
}