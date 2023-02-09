#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <vector>
#include "DriverInterface.h"
#include <thread>
#include <fstream>

#include "SharedMemoryManager.h"
#include "CameraManager.h"

#include <mutex>

CameraManager camMgr;
std::mutex display_mutex;


void sendFrame(BYTE* buffer, std::string name) {
	for (;;) {

		camMgr.setBuffer(buffer, name);

		std::this_thread::sleep_for(std::chrono::milliseconds(15));

	}
}

using json = nlohmann::json;


int main() {

	std::ifstream file("Config.json");
	json data = json::parse(file);

	
	SharedMemoryManager mgr(data);
	

	auto map = mgr.getMemoryMap();
   
	std::vector<std::thread> vectorOfThreads;

	for (auto item : map) {
		
		vectorOfThreads.emplace_back(sendFrame, item.second, item.first);
	}



	std::this_thread::sleep_for(std::chrono::hours(1));

	
	return 0;
} 