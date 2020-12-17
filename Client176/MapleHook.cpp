#include "Global.h"
#include "CWvsContext.h"
#include <intrin.h>
#include <map>
#include <string>

std::map<int, std::string>	g_mStringPool;

typedef ZXString<char>* (__fastcall* StringPool__GetString_t)(void* ecx, void* edx, ZXString<char>* result, unsigned int nIdx, char formal);
auto StringPool__GetString = reinterpret_cast<StringPool__GetString_t>(0x00C4C980);

typedef void(__fastcall* ZXString_char__Assign_t)(void* pThis, void* edx, const char* s, int n);
auto ZXString_char__Assign = reinterpret_cast<ZXString_char__Assign_t>(0x00823160);


bool Hook_StringPool__GetString(bool enable)
{

	StringPool__GetString_t Hook = [](void* pThis, void* edx, ZXString<char>* result, unsigned int nIdx, char formal) -> ZXString<char>*
	{
		if (g_mStringPool.count(nIdx) > 0)
		{
			auto szEntry = g_mStringPool[nIdx];

			result->m_pStr = 0;
			ZXString_char__Assign(result, edx, szEntry.c_str(), -1);

			return result;
		}

		return StringPool__GetString(pThis, edx, result, nIdx, formal);
	};

	return SetHook(enable, reinterpret_cast<void**>(&StringPool__GetString), Hook);
}

void InitStringPool()
{
	//g_mStringPool[4716] = std::string("Ver. %d.%d"); //version
	//g_mStringPool[12750] = std::string("Super GM"); //supergm
	//g_mStringPool[14520] = std::string("Comic Sans MS"); //font
}



void FuckMaple()
{
	PatchRetZero(0x029EC300); //NGS Bypass

	PatchRetZero(0x029FA5D0); // CRC Bypass

	InitStringPool();
	Hook_StringPool__GetString(true);

}
