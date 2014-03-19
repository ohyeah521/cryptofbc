/********************************************************************
	created:	2011/08/28   19:41
	file base:	crc16.cpp
	author:		cnbragon@gmail.com
	purpose:	
*********************************************************************/
#include "crc16.h"

NAMESPACE_BEGIN(CryptoFBC)

bool FBC_CRC16::CalculateCRC16(__in const char* p, 
							   __in const FBC_Dword dwLen, 
							   __in const FBC_Word wInitCRC,
							   __in __out FBC_Word* wCRCValue
							   )
{
	bool bRet = false;
	FBC_Word wCRC = 0;
	FBC_Dword dwLen_Inner = 0;
	FBC_Byte* ptemp = NULL;

	FBC_PROCESS_POINTER(p);

	if ( dwLen == 0 )
	{
		goto Exit0;
	}

	ptemp = (FBC_Byte*)p;
	wCRC = wInitCRC;
	dwLen_Inner = dwLen;

	do
	{
		wCRC = (wCRC << 8) ^ wCRC16Table[ wCRC >> 8 ] ^ (*ptemp++);
		dwLen_Inner--;
	}while(dwLen_Inner > 0);

	bRet = true;
	*wCRCValue = wCRC;
Exit0:
	return bRet;
}

NAMESPACE_END