#ifndef CryptoFBC_MD4_H
#define CryptoFBC_MD4_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

#define md4S11 3
#define md4S12 7
#define md4S13 11
#define md4S14 19
#define md4S21 3
#define md4S22 5
#define md4S23 9
#define md4S24 13
#define md4S31 3
#define md4S32 9
#define md4S33 11
#define md4S34 15

#define md4F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define md4G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define md4H(x, y, z) ((x) ^ (y) ^ (z))

#define md4ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define md4FF(a, b, c, d, x, s) { \
    (a) += md4F ((b), (c), (d)) + (x); \
    (a) = md4ROTATE_LEFT ((a), (s)); \
}
#define md4GG(a, b, c, d, x, s) { \
    (a) += md4G ((b), (c), (d)) + (x) + (FBC_Dword)0x5a827999; \
    (a) = md4ROTATE_LEFT ((a), (s)); \
}
#define md4HH(a, b, c, d, x, s) { \
    (a) += md4H ((b), (c), (d)) + (x) + (FBC_Dword)0x6ed9eba1; \
    (a) = md4ROTATE_LEFT ((a), (s)); \
  }

class MD4:public DwordAndBytes
{
	FBC_Dword state[4];
	FBC_Dword count[2];
	FBC_Byte buffer[64];
	void MD4Transform(FBC_Dword state[4], FBC_Byte  block[64]);
public:
	static const int DigestSize;
	MD4()
	{
		count[0] = count[1] = 0;
		state[0] = 0x67452301;
		state[1] = 0xEFCDAB89;
		state[2] = 0x98BADCFE;
		state[3] = 0x10325476;
	}
	/* this constructor is for modified MD4*/
	MD4(FBC_Dword dwudstate[4]) 
	{
		count[0]=count[1]=0;
		state[0]=dwudstate[0];
		state[1]=dwudstate[1];
		state[2]=dwudstate[2];
		state[3]=dwudstate[3];
	}
	void MD4Update(POINTER input,int inputLen);
	void MD4Final(FBC_Byte digest[16]);
};

NAMESPACE_END

#endif