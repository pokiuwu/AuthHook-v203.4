#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "MapleType.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "detours.lib")

//---------------------------------------------------------------------------------------------

#define OPT_APPNAME			"v203.4"
#define OPT_APPVERSION		"1.0"
#define OPT_ADDR_HOSTNAME	"127.0.0.1"

#define OPT_ADDR_SEARCH		"8.31.99."
#define OPT_ADDR_NEXON		"8.31.99.141"
#define OPT_PORT_LOW		8484
#define OPT_PORT_HIGH		8989


//---------------------------------------------------------------------------------------------

void FuckMaple();

bool HookNMCO();
bool HookWinApi();
bool HookSockApi();
//bool HookCRC();

//---------------------------------------------------------------------------------------------

void Log(const char* format, ...);

void PatchJmp(DWORD dwAddress, DWORD dwDest);
void PatchRetZero(DWORD dwAddress);
void PatchNop(DWORD dwAddress, DWORD dwCount);

DWORD GetFuncAddress(LPCSTR lpModule, LPCSTR lpFunc);

BOOL SetHook(BOOL bInstall, PVOID* ppvTarget, PVOID pvDetour);
