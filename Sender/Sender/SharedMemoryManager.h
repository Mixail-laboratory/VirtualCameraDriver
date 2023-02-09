#pragma once
#include <Windows.h>
#include <string>
#include <unordered_set>
#include "json.hpp"


class SharedMemoryManager final
{
public:
	explicit SharedMemoryManager(const nlohmann::json& jsonData);

	inline int getInputNumbers() const {
		return m_jsonData["inputs"].size();
	}

	inline std::unordered_map<std::string, PBYTE> getMemoryMap() const {
		return memoryMap;
	}
private:
	nlohmann::json m_jsonData;
	std::unordered_map<std::string, PBYTE> memoryMap;
	static const ULONG buffSize = 1920 * 1080 * 4;

private:
	int createMemoryMap();


};

