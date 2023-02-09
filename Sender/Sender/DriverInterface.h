#include "Common.h"
#include "DeviceEnumeration.h"
#include "Device.h"



#define NUM_MAX_PATHS 16
static string cachedPaths[NUM_MAX_PATHS];
static int numDevices;

static Device* activeDevice = NULL;

#define TEMPORARY_BUFFER_SIZE (WIDTH * HEIGHT * 4)
static PVOID temporaryBuffer = NULL;

int Init();
int Free();
int GetNumDevices();
int GetDevicePath(int index, char* str, int maxLen);
void DestroyDevice();

int SetDevice(char* str, int strLen);
int SetBuffer(PVOID data, DWORD stride, DWORD width, DWORD height);




