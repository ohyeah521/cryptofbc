/********************************************************************
	created:	2010/10/20   15:40
	filename: 	sms4.h
	author:		cnbragon@gmail.com
	purpose:	SMS4 Header File
*********************************************************************/
#ifndef CryptoFBC_SMS4_H
#define CryptoFBC_SMS4_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

class SMS4
{
public:
	const static int BLOCKSIZE = 128;
	const static int ROUNDS = 32;
	const static int DEFAULT_KEYLENGTH = 128;
	FBC_Dword T1(FBC_Dword dwA);
	FBC_Dword T2(FBC_Dword dwA);
	void SetKey(const char* key, FBC_Dword* rk);
	void ECB_Encryption(const char* plainText, FBC_Dword* rk, char* cipherText);
	void ECB_Decryption(const char* plainText, FBC_Dword* rk, char* cipherText);
private:
	const static FBC_Dword FK[ 4 ];
	const static FBC_Dword CK[ 32 ];
	const static FBC_Byte  SBox[ 256 ];
};

NAMESPACE_END
#endif