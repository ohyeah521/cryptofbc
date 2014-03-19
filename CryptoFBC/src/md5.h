#ifndef CryptoFBC_MD5_H
#define CryptoFBC_MD5_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

#define md5S11 7
#define md5S12 12
#define md5S13 17
#define md5S14 22
#define md5S21 5
#define md5S22 9
#define md5S23 14
#define md5S24 20
#define md5S31 4
#define md5S32 11
#define md5S33 16
#define md5S34 23
#define md5S41 6
#define md5S42 10
#define md5S43 15
#define md5S44 21

#define md5F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define md5G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define md5H(x, y, z) ((x) ^ (y) ^ (z))
#define md5I(x, y, z) ((y) ^ ((x) | (~z)))

#define md5ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define md5FF(a, b, c, d, x, s, ac) { \
 (a) += md5F ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = md5ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define md5GG(a, b, c, d, x, s, ac) { \
 (a) += md5G ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = md5ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define md5HH(a, b, c, d, x, s, ac) { \
 (a) += md5H ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = md5ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define md5II(a, b, c, d, x, s, ac) { \
 (a) += md5I ((b), (c), (d)) + (x) + FBC_Dword(ac); \
 (a) = md5ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

class MD5:public DwordAndBytes
{
	FBC_Dword state[4];
	FBC_Dword count[2];
	FBC_Byte buffer[64];
	void MD5Transform(FBC_Dword state[4], FBC_Byte  block[64]);
	//void MD5_memcpy(POINTER, POINTER, int);
	//void MD5_memset(POINTER, int, int);
public:
	static const int DigestSize;
	MD5()
	{
		count[0] = count[1] = 0;
		state[0] = 0x67452301;
		state[1] = 0xEFCDAB89;
		state[2] = 0x98BADCFE;
		state[3] = 0x10325476;
	}
	/* this constructor is for modified MD5*/
	MD5(FBC_Dword dwudstate[4]) 
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