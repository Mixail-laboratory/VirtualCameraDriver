#pragma once
#include "Common.h"
#include "DeviceEnumeration.h"
#include "Device.h"
#include "json.hpp"


#include <unordered_map>
class CameraManager final
{
public:
	CameraManager();



	~CameraManager();

	int setBuffer(PVOID data, const std::string& name);
private:
	int numOfDevices{};
	static const int maxPaths = 20;

	static const int tempBufSize = 1920 * 1080 * 4;
	std::string cachedPaths[maxPaths];

	std::unordered_map<std::string, std::pair<Device*, PVOID>> mapOfBuffers{};

	

private:
	int GetDevicePath(int index, char* str, int maxLen);

	void initTempBuffers();

	void initDevices(const char* str, int strLen, PVOID tempBuffer);
};

