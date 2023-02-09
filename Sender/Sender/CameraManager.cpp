#include "CameraManager.h"
#include <iostream>
#include <stdexcept>

CameraManager::CameraManager()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (!SUCCEEDED(hr))
	{
		throw std::invalid_argument("can't init driver");
	}

	numOfDevices = EnumerateDevicePaths(cachedPaths, maxPaths);
	if (numOfDevices < 0) {
		throw std::invalid_argument("can't init driver");
	}

	initTempBuffers();

	
	
}

void CameraManager::initTempBuffers()
{
	for (int i = 0; i < numOfDevices; i++) {
		char name[256];
		GetDevicePath(i, name, 256);
		std::string strName = name;
		if (strName.find("root#unknown#") == std::string::npos) {
			continue;
		}
		int index = strName[32] - '0' + 1;
		
		auto newStr = "Camera" + std::to_string(index);
		auto buffer = new UCHAR[tempBufSize];
		mapOfBuffers.insert({ newStr, {nullptr, buffer} });

		initDevices(strName.data(), strlen(strName.data()), buffer);
	}
}

void CameraManager::initDevices(const char* str, int strLen, PVOID tempBuffer)
{
	IBaseFilter* filter = NULL;
	if (!GetFilter(string(str), &filter) || filter == NULL)
	{
		throw std::invalid_argument("can't get filter");
	}
	
	auto activeDevice = new Device(filter);
	if (!activeDevice->Init())
	{
		delete activeDevice;
		activeDevice = NULL;

		throw std::invalid_argument("can't get filter");

	}

	int index = str[32] - '0' + 1;
	char cIndex = index + '0';
	auto newStr = "Camera" + std::to_string(index);
	mapOfBuffers[newStr] = { activeDevice, tempBuffer };

}

CameraManager::~CameraManager()
{
	for (auto& item : mapOfBuffers) {
		delete[] item.second.second;
	}

	CoUninitialize();

}

int CameraManager::setBuffer(PVOID data, const std::string& name)
{
	if (mapOfBuffers[name].first == NULL)
	{
		return -1;
	}

	

	PUCHAR inputData = (PUCHAR)data;
	PUCHAR buffer = (PUCHAR)mapOfBuffers[name].second;

	for (ULONG y = 0; y < 1080; y++)
	{
		PUCHAR sourceLine = inputData + (4 * WIDTH) * (y);
		PUCHAR targetLine = buffer + ((WIDTH * 4) * y);
		memcpy(targetLine, sourceLine, WIDTH * 4);
	}

	mapOfBuffers[name].first->SetData(mapOfBuffers[name].second, tempBufSize);

	return 1;
}

int CameraManager::GetDevicePath(int index, char* str, int maxLen)
{
	if (index < 0 || index >= numOfDevices) {
		return -1;
	}

	strcpy_s(str, maxLen, cachedPaths[index].c_str());

	return 1;
}
