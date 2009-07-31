#ifndef CryptoFBC_MODIFIEDMD5_H
#define CryptoFBC_MODIFIEDMD5_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

class MODIFIEDMD5:public DwordAndBytes
{
	FBC_Dword state[4];
	FBC_Dword count[2];
	FBC_Byte buffer[64];
	void MD5Transform(FBC_Dword state[4], FBC_Byte  block[64]);
	//void MD5_memcpy(POINTER, POINTER, int);
	//void MD5_memset(POINTER, int, int);
public:
	static const int DigestSize;
	MODIFIEDMD5()
	{
		count[0] = count[1] = 0;
		state[0] = 0x7654321F;
		state[1] = 0xAF28B6C9;
		state[2] = 0xED5890AC;
		state[3] = 0x65FAA45E;
	}
	/* this constructor is for modified MD5*/
	MODIFIEDMD5(FBC_Dword dwudstate[4]) 
	{
		count[0]=count[1]=0;
		state[0]=dwudstate[0];
		state[1]=dwudstate[1];
		state[2]=dwudstate[2];
		state[3]=dwudstate[3];
	}
	void MD5Update(POINTER input,int inputLen);
	void MD5Final(FBC_Byte digest[16]);
};

NAMESPACE_END

#endif