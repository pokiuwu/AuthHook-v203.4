#include "Global.h"
#include "NMCO\NMGeneral.h"
#include "NMCO\NMSerializable.h"
#include "NMCO\NMFunctionObject.h"

#define BUF_SIZE 2048

typedef BOOL(__cdecl* NMCO_CallNMFunc_t)(int uFuncCode, BYTE* pCallingData, BYTE** ppReturnData, UINT32& uReturnDataLen);
NMCO_CallNMFunc_t _NMCO_CallNMFunc;

CHAR* g_pUserName = new CHAR[LOGINID_SIZE];
BYTE* g_pReturnData = new BYTE[BUF_SIZE];

BOOL SetResponse(CNMSimpleStream& s, BYTE** ppReturnData, UINT32& uReturnDataLen)
{
	uReturnDataLen = s.GetBufferSize();

	if (uReturnDataLen > BUF_SIZE)
		return FALSE;

	memset(g_pReturnData, 0, BUF_SIZE);
	memcpy_s(g_pReturnData, BUF_SIZE, s.GetBufferPtr(), uReturnDataLen);

	*ppReturnData = g_pReturnData;

	return TRUE;
}

namespace Hooks
{
	BOOL __cdecl CallNMFunc(int uFuncCode, BYTE* pCallingData, BYTE** ppReturnData, UINT32& uReturnDataLen)
	{
		int nEBP = 0;
		__asm mov nEBP, ebp

		if (uFuncCode == kNMFuncCode_LoginAuth)
		{
			char* szNexonID = reinterpret_cast<LPSTR>(nEBP + 156); //677 or 64 
			memcpy_s(g_pUserName, LOGINID_SIZE, szNexonID, LOGINID_SIZE);

			Log("LoginAuth: %s", szNexonID);

			CNMLoginAuthFunc retFunc;
			retFunc.bSuccess = true;
			retFunc.nErrorCode = kLoginAuth_OK;
			retFunc.SetReturn();

			CNMSimpleStream retStream;

			if (!retFunc.Serialize(retStream))
				return FALSE;

			return SetResponse(retStream, ppReturnData, uReturnDataLen);
		}
		else if (uFuncCode == kNMFuncCode_GetNexonPassport)
		{
			CNMGetNexonPassportFunc retFunc;
			retFunc.bSuccess = true;
			retFunc.SetReturn();

			strcpy_s(retFunc.szNexonPassport, g_pUserName);

			CNMSimpleStream retStream;
			if (!retFunc.Serialize(retStream))
				return FALSE;

			return SetResponse(retStream, ppReturnData, uReturnDataLen);
		}
		else if (uFuncCode == kNMFuncCode_LogoutAuth)
		{
			CNMLogoutAuthFunc retFunc;
			retFunc.bSuccess = true;
			retFunc.SetReturn();

			CNMSimpleStream retStream;

			if (!retFunc.Serialize(retStream))
				return FALSE;

			return SetResponse(retStream, ppReturnData, uReturnDataLen);
		}

		return _NMCO_CallNMFunc(uFuncCode, pCallingData, ppReturnData, uReturnDataLen);
	}
}

bool HookNMCO()
{
	_NMCO_CallNMFunc = reinterpret_cast<NMCO_CallNMFunc_t>(GetFuncAddress("NMCOGAME", "NMCO_CallNMFunc"));
	return SetHook(TRUE, reinterpret_cast<void**>(&_NMCO_CallNMFunc), Hooks::CallNMFunc);
}