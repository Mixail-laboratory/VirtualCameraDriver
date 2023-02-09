#include "SharedMemoryManager.h"
#include <stdexcept>

SharedMemoryManager::SharedMemoryManager(const nlohmann::json& jsonData): m_jsonData(jsonData)
{
	auto result = createMemoryMap();

    if (result) {
        throw std::invalid_argument("invalid json data");
    }
}

int SharedMemoryManager::createMemoryMap()
{
    for (auto& item : m_jsonData["inputs"]) {
        HANDLE hMapFile;
        BYTE* pBuf;

        std::string s = item["dest"];
        std::wstring stemp = std::wstring(s.begin(), s.end());
        LPCWSTR sw = stemp.c_str();
        hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, sw);



        if (hMapFile == NULL) {
            return 1;
        }

        pBuf = (BYTE*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, buffSize);

        if (pBuf == NULL) {
            return 2;
        }

        memoryMap.insert({ item["dest"], pBuf });
    }
    return 0;
}
