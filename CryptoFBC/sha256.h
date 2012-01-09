/********************************************************************
* Created: 2011.6.24
* Author: cnbragon
* Email: cnbragon@vip.qq.com 
* Description: 
********************************************************************/
#ifndef CryptoFBC_SHA256_H
#define CryptoFBC_SHA256_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class FBC_CLASS_SHA256: public DwordAndBytes
{
private:

	FBC_Dword dwLength;
	FBC_Dword dwCurLen;
	FBC_Dword dwState[8];
	FBC_Byte cBuffer[64];
	static const FBC_Dword K[64];

	void Transform();

	inline FBC_Dword ROTR(FBC_Dword x, FBC_Dword n)
	{
		return ( x >> n ) | ( x << (32 - n) );
	}
	
	inline FBC_Dword SHR(FBC_Dword x, FBC_Dword n)
	{
		return x >> n;
	}

	inline FBC_Dword Sigma0(FBC_Dword x)
	{
		return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
	}

	inline FBC_Dword Sigma1(FBC_Dword x)
	{
		return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
	}

	inline FBC_Dword Gamma0(FBC_Dword x)
	{
		return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);
	}

	inline FBC_Dword Gamma1(FBC_Dword x)
	{
		return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
	}

	inline FBC_Dword Ch(FBC_Dword x, FBC_Dword y, FBC_Dword z)
	{
		return ( x & y ) ^ ( ~x & z );
	}

	inline FBC_Dword Maj(FBC_Dword x, FBC_Dword y, FBC_Dword z)
	{
		return ( x & y ) ^ ( x & z ) ^ ( y & z );
	}
public:

	FBC_CLASS_SHA256()
	{
		dwLength = 0;
		dwCurLen = 0;

		dwState[0] = 0x6a09e667;
		dwState[1] = 0xbb67ae85;
		dwState[2] = 0x3c6ef372;
		dwState[3] = 0xa54ff53a;
		dwState[4] = 0x510e527f;
		dwState[5] = 0x9b05688c;
		dwState[6] = 0x1f83d9ab;
		dwState[7] = 0x5be0cd19;
	}

	void Update(unsigned char* szMessage, int cbBytes);
	void Final(FBC_Byte* szHash, int cbBytes);
};

NAMESPACE_END

#endif