#ifndef CryptoFBC_SHA1_H
#define CryptoFBC_SHA1_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

static FBC_Byte SHA1_PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

class SHA1:public DwordAndBytes
{
	private:
		FBC_Dword count[2];
		FBC_Dword state[5];
		FBC_Dword buffer[16];
		void SHA1Transform(FBC_Dword state[5], FBC_Dword  block[16]);
		inline FBC_Dword ROTL(FBC_Dword dwA,	FBC_Byte	n)
		{
			return ((dwA<<n)|(dwA>>(32-n)));
		}
		inline FBC_Dword Ch(FBC_Dword x, FBC_Dword y, FBC_Dword z)
		{
			return (x&y)^(~x&z);
		}
		inline FBC_Dword Parity(FBC_Dword x, FBC_Dword y, FBC_Dword z)
		{
			return x^y^z;
		}
		inline FBC_Dword Maj(FBC_Dword x, FBC_Dword y, FBC_Dword z)
		{
			return (x&y)^(y&z)^(z&x);
		}
	public:
		static const int DigestSize;
		SHA1()
		{
			count[0]=count[1]=0;
			state[0]=0x67452301;
			state[1]=0xefcdab89;
			state[2]=0x98badcfe;
			state[3]=0x10325476;
			state[4]=0xc3d2e1f0;
		}
		SHA1(const FBC_Dword* lpCustom)
		{
			count[0] = count[ 1 ] = 0;
			state[ 0 ] = lpCustom[ 0 ];
			state[ 1 ] = lpCustom[ 1 ];
			state[ 2 ] = lpCustom[ 2 ];
			state[ 3 ] = lpCustom[ 3 ];
			state[ 4 ] = lpCustom[ 4 ];
		}
		void SHA1_Update(FBC_Byte* szMessage, FBC_Dword dwLenOfM);
		void SHA1_Final(FBC_Byte szDigest[20]);
};

NAMESPACE_END

#endif