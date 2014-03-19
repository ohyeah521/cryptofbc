/********************************************************************
* Created: 2011.6.25
* Author: cnbragon
* Email: cnbragon@vip.qq.com 
* Description: 
********************************************************************/
#include "sha256.h"

NAMESPACE_BEGIN(CryptoFBC)

const FBC_Dword FBC_CLASS_SHA256::K[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
	0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
	0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
	0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
	0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
	0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
	0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
	0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
	0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
	0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

void FBC_CLASS_SHA256::Transform()
{
	FBC_Dword dwS[8] = { 0 };
	FBC_Dword dwW[64] = { 0 };
	FBC_Dword dwT0 = 0;
	FBC_Dword dwT1 = 0;
	int i = 0;
	
	for ( i = 0; i < 8; i++ )
	{
		dwS[i] = dwState[i];
	}

	for ( i = 0; i < 16; i++ )
	{
		dwW[i] = FBC_Dword( cBuffer[ 4 * i + 0 ] << 24 ) |
				 FBC_Dword( cBuffer[ 4 * i + 1 ] << 16 ) |
				 FBC_Dword( cBuffer[ 4 * i + 2 ] << 8 )  |
				 FBC_Dword( cBuffer[ 4 * i + 3 ] );
	}

	for ( i = 16; i < 64; i++ )
	{
		dwW[i] = Gamma1( dwW[ i - 2 ] ) +
				 dwW[ i - 7 ] +
				 Gamma0( dwW[ i - 15 ] ) +
				 dwW[ i - 16 ];
	}

	for ( i = 0; i < 64; i++ )
	{
		dwT0 = dwS[7] + Sigma1(dwS[4]) + Ch(dwS[4], dwS[5], dwS[6]) + K[i] + dwW[i];
		dwT1 = Sigma0(dwS[0]) + Maj(dwS[0], dwS[1], dwS[2]);
		dwS[7] = dwS[6];
		dwS[6] = dwS[5];
		dwS[5] = dwS[4];
		dwS[4] = dwS[3] + dwT0;
		dwS[3] = dwS[2];
		dwS[2] = dwS[1];
		dwS[1] = dwS[0];
		dwS[0] = dwT0 + dwT1;
	}

	for ( i = 0; i < 8; i++ )
	{
		dwState[i] += dwS[i];
	}
}

void FBC_CLASS_SHA256::Update(unsigned char* szMessage, int cbBytes)
{
	while ( cbBytes-- )
	{
		cBuffer[ dwCurLen++ ] = *szMessage++;

		if ( dwCurLen == 64 )
		{
			Transform();
			dwLength += 512;
			dwCurLen = 0;
		}
	}
}

void FBC_CLASS_SHA256::Final(FBC_Byte* szHash, int cbBytes)
{
	int i = 0;

	if ( cbBytes < 32 )
	{
		return;
	}

	dwLength += dwCurLen * 8;

	cBuffer[ dwCurLen++ ] = 0x80;

	if ( dwCurLen >= 56 )
	{
		while( dwCurLen < 64 )
		{
			cBuffer[ dwCurLen++ ] = 0;
		}
		Transform();
		dwCurLen = 0;
	}

	while( dwCurLen < 56 )
	{
		cBuffer[ dwCurLen++ ] = 0;
	}

	for ( i = 56; i < 60; i++ )
	{
		cBuffer[i] = 0;
	}

	for ( i = 60; i < 64; i++ )
	{
		cBuffer[i] = FBC_Byte( ( dwLength >> ( ( 63 - i ) * 8 ) ) & 255 );
	}

	Transform();

	for ( i = 0; i < 32; i++ )
	{
		szHash[i] = FBC_Byte( ( dwState[ i >> 2 ] >> ( ( ( 3 - i ) & 3 ) << 3 ) ) & 255 );
	}
}

void FBC_CLASS_SHA256::Reset()
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

NAMESPACE_END