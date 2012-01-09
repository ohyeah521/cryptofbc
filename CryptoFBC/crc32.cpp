#include "crc32.h"

NAMESPACE_BEGIN(CryptoFBC)

bool FBC_CRC32::CalculateCRC32(__in const char* p, 
							   __in const FBC_Dword dwLen, 
							   __in const FBC_Dword dwInitCRC,
							   __in __out FBC_Dword* dwCRCValue
)
{
	bool bRet = false;
	FBC_Dword dwCRC = 0;
	FBC_Dword dwLen_Inner = 0;
	FBC_Byte* ptemp = NULL;

	FBC_PROCESS_POINTER(p);

	if ( dwLen == 0 )
	{
		goto Exit0;
	}
	
	ptemp = (FBC_Byte*)p;
	dwCRC = dwInitCRC;
	dwLen_Inner = dwLen;

	do
	{
		dwCRC = dwCRC32Table[ (*ptemp) ^ (dwCRC & 0xff) ] ^ 
								(dwCRC >> 8);
		ptemp++;
		dwLen_Inner--;
	}while(dwLen_Inner > 0);
	
	bRet = true;
	*dwCRCValue = dwCRC;
Exit0:
	return bRet;
}

NAMESPACE_END