/********************************************************************
	created:	2010/12/07   23:21
	file base:	rc5.cpp
	author:		cnbragon
	purpose:	RC5 Implementation
*********************************************************************/
#include "rc5.h"

NAMESPACE_BEGIN(CryptoFBC)

bool RC5::SetKey(FBC_Dword* pdwKey, FBC_Dword dwWordCount)
{
	bool bRet = false;
	FBC_Dword i = 0;
	FBC_Dword k = 0;
	FBC_Dword j = 0;
	FBC_Dword t = 0;
	FBC_Dword A = 0;
	FBC_Dword B = 0;
	FBC_Dword* pdwTemp = NULL;

	FBC_PROCESS_POINTER(pdwKey);
	// here dwWordCount is a local variable
	if ( dwWordCount > 8 )
	{
		dwWordCount = 8;
	}
	pdwTemp = new FBC_Dword[dwWordCount];
	FBC_PROCESS_POINTER(pdwTemp);
	for ( i = 0; i < dwWordCount; i++ )
	{
		pdwTemp[i] = pdwKey[i];
	}
	dwSubKey[0] = RC5_P32;
	for ( i = 1; i < NUMBEROFSUBKEY; i++ )
	{
		dwSubKey[i] = dwSubKey[i - 1] + RC5_Q32;
	}

	t = 3 * ( dwWordCount > NUMBEROFSUBKEY ? dwWordCount : NUMBEROFSUBKEY );
	i = 0;
	j = 0;
	for ( k = 0; k < t; k++ )
	{
		dwSubKey[i] = ROL(dwSubKey[i] + ( A + B ), 3);
		A = dwSubKey[i];
		pdwTemp[j] = ROL(pdwTemp[j] + (A + B), (A + B) );
		B = pdwTemp[j];
		i = ( i + 1 ) % NUMBEROFSUBKEY;
		j = ( j + 1 ) % dwWordCount;
	}
	bRet = true;
Exit0:
	if ( pdwTemp )
	{
		delete[] pdwTemp;
	}
	return bRet;
}

bool RC5::ECB_Encryption(FBC_Dword* pdwIN, 
						FBC_Dword dwInCount,
						FBC_Dword* pdwOut,
						FBC_Dword dwOutCount)
{
	bool bRet = false;
	FBC_Dword i = 0;
	FBC_Dword A = 0;
	FBC_Dword B = 0;
	
	FBC_PROCESS_POINTER(pdwIN);
	FBC_PROCESS_POINTER(pdwOut);
	FBC_PROCESS_BOOL(dwInCount == 2);
	FBC_PROCESS_BOOL(dwOutCount == 2);

	A = pdwIN[0] + dwSubKey[0];
	B = pdwIN[1] + dwSubKey[1];
	for ( i = 1; i <= ROUNDS; i++ )
	{
		A = ROL( A ^ B, B ) + dwSubKey[ 2 * i ];
		B = ROL( B ^ A, A ) + dwSubKey[ 2 * i + 1 ];
	}
	pdwOut[0] = A;
	pdwOut[1] = B;
	bRet = true;
Exit0:
	return bRet;
}

bool RC5::ECB_Decryption(FBC_Dword* pdwIN, 
						FBC_Dword dwInCount,
						FBC_Dword* pdwOut,
						FBC_Dword dwOutCount)
{
	bool bRet = false;
	FBC_Dword i = 0;
	FBC_Dword A = 0;
	FBC_Dword B = 0;

	FBC_PROCESS_POINTER(pdwIN);
	FBC_PROCESS_POINTER(pdwOut);
	FBC_PROCESS_BOOL(dwInCount == 2);
	FBC_PROCESS_BOOL(dwOutCount == 2);

	A = pdwIN[0];
	B = pdwIN[1];
	for ( i = ROUNDS; i > 0; i-- )
	{
		B = ROR( B - dwSubKey[2 * i + 1], A ) ^ A;
		A = ROR( A - dwSubKey[2 * i], B ) ^ B;
	}
	pdwOut[0] = A - dwSubKey[0];
	pdwOut[1] = B - dwSubKey[1];
	bRet = true;
Exit0:
	return bRet;
}

NAMESPACE_END