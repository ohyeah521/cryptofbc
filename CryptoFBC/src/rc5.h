/********************************************************************
	created:	2010/12/07   22:39
	file base:	rc5.h
	author:		cnbragon
	purpose:	RC5 Header File
*********************************************************************/
#ifndef CryptoFBC_RC5_H
#define CryptoFBC_RC5_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

#define RC5_P32	0xb7e15163
#define RC5_Q32	0x9e3779b9

class RC5
{
public:
	bool SetKey(FBC_Dword* pdwKey, FBC_Dword dwWordCount);
	bool ECB_Decryption(FBC_Dword* pdwIN, 
						FBC_Dword dwInCount,
						FBC_Dword* pdwOut,
						FBC_Dword dwOutCount);
	bool ECB_Encryption(FBC_Dword* pdwIN, 
						FBC_Dword dwInCount,
						FBC_Dword* pdwOut,
						FBC_Dword dwOutCount);
private:
	static const FBC_Dword ROUNDS = 16;
	static const FBC_Dword NUMBEROFSUBKEY = 2 * 16 + 2;
	FBC_Dword dwSubKey[ 2 * 16 + 2 ];
};

NAMESPACE_END
#endif