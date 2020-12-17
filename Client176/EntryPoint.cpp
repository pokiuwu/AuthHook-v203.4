//---------------------------------------------------------------------------------------------
// v203.4 Localhost Enabler - Rajan
//---------------------------------------------------------------------------------------------

#include "Global.h"

VOID WINAPI HeaderProc(DWORD dwPID)
{
	
}

DWORD WINAPI MainProc(PVOID)
{
	DWORD dwCurProcId = GetCurrentProcessId();
	HeaderProc(dwCurProcId);

	if (!HookSockApi())
		Log("Failed Hooking SockApi");

	if (!HookWinApi())
		Log("Failed Hooking WinApi");

	if (!HookNMCO())
		Log("Failed Hooking NMCO");

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MainProc, NULL, NULL, NULL);
	}

	return TRUE;
}
