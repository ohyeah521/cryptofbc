#include "md5.h"

NAMESPACE_BEGIN(CryptoFBC)

const int MD5::DigestSize=16;

void MD5::MD5Transform(FBC_Dword state[4], FBC_Byte block[64])
{
	FBC_Dword a = state[0];
	FBC_Dword b = state[1];
	FBC_Dword c = state[2];
	FBC_Dword d = state[3];
	FBC_Dword x[MD5::DigestSize];
	
	BytesToDword(x, block, 64);

  /* Round 1 */
  md5FF (a, b, c, d, x[ 0], md5S11, 0xd76aa478); /* 1 */
  md5FF (d, a, b, c, x[ 1], md5S12, 0xe8c7b756); /* 2 */
  md5FF (c, d, a, b, x[ 2], md5S13, 0x242070db); /* 3 */
  md5FF (b, c, d, a, x[ 3], md5S14, 0xc1bdceee); /* 4 */
  md5FF (a, b, c, d, x[ 4], md5S11, 0xf57c0faf); /* 5 */
  md5FF (d, a, b, c, x[ 5], md5S12, 0x4787c62a); /* 6 */
  md5FF (c, d, a, b, x[ 6], md5S13, 0xa8304613); /* 7 */
  md5FF (b, c, d, a, x[ 7], md5S14, 0xfd469501); /* 8 */
  md5FF (a, b, c, d, x[ 8], md5S11, 0x698098d8); /* 9 */
  md5FF (d, a, b, c, x[ 9], md5S12, 0x8b44f7af); /* 10 */
  md5FF (c, d, a, b, x[10], md5S13, 0xffff5bb1); /* 11 */
  md5FF (b, c, d, a, x[11], md5S14, 0x895cd7be); /* 12 */
  md5FF (a, b, c, d, x[12], md5S11, 0x6b901122); /* 13 */
  md5FF (d, a, b, c, x[13], md5S12, 0xfd987193); /* 14 */
  md5FF (c, d, a, b, x[14], md5S13, 0xa679438e); /* 15 */
  md5FF (b, c, d, a, x[15], md5S14, 0x49b40821); /* 16 */

 /* Round 2 */
  md5GG (a, b, c, d, x[ 1], md5S21, 0xf61e2562); /* 17 */
  md5GG (d, a, b, c, x[ 6], md5S22, 0xc040b340); /* 18 */
  md5GG (c, d, a, b, x[11], md5S23, 0x265e5a51); /* 19 */
  md5GG (b, c, d, a, x[ 0], md5S24, 0xe9b6c7aa); /* 20 */
  md5GG (a, b, c, d, x[ 5], md5S21, 0xd62f105d); /* 21 */
  md5GG (d, a, b, c, x[10], md5S22,  0x2441453); /* 22 */
  md5GG (c, d, a, b, x[15], md5S23, 0xd8a1e681); /* 23 */
  md5GG (b, c, d, a, x[ 4], md5S24, 0xe7d3fbc8); /* 24 */
  md5GG (a, b, c, d, x[ 9], md5S21, 0x21e1cde6); /* 25 */
  md5GG (d, a, b, c, x[14], md5S22, 0xc33707d6); /* 26 */
  md5GG (c, d, a, b, x[ 3], md5S23, 0xf4d50d87); /* 27 */
  md5GG (b, c, d, a, x[ 8], md5S24, 0x455a14ed); /* 28 */
  md5GG (a, b, c, d, x[13], md5S21, 0xa9e3e905); /* 29 */
  md5GG (d, a, b, c, x[ 2], md5S22, 0xfcefa3f8); /* 30 */
  md5GG (c, d, a, b, x[ 7], md5S23, 0x676f02d9); /* 31 */
  md5GG (b, c, d, a, x[12], md5S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  md5HH (a, b, c, d, x[ 5], md5S31, 0xfffa3942); /* 33 */
  md5HH (d, a, b, c, x[ 8], md5S32, 0x8771f681); /* 34 */
  md5HH (c, d, a, b, x[11], md5S33, 0x6d9d6122); /* 35 */
  md5HH (b, c, d, a, x[14], md5S34, 0xfde5380c); /* 36 */
  md5HH (a, b, c, d, x[ 1], md5S31, 0xa4beea44); /* 37 */
  md5HH (d, a, b, c, x[ 4], md5S32, 0x4bdecfa9); /* 38 */
  md5HH (c, d, a, b, x[ 7], md5S33, 0xf6bb4b60); /* 39 */
  md5HH (b, c, d, a, x[10], md5S34, 0xbebfbc70); /* 40 */
  md5HH (a, b, c, d, x[13], md5S31, 0x289b7ec6); /* 41 */
  md5HH (d, a, b, c, x[ 0], md5S32, 0xeaa127fa); /* 42 */
  md5HH (c, d, a, b, x[ 3], md5S33, 0xd4ef3085); /* 43 */
  md5HH (b, c, d, a, x[ 6], md5S34,  0x4881d05); /* 44 */
  md5HH (a, b, c, d, x[ 9], md5S31, 0xd9d4d039); /* 45 */
  md5HH (d, a, b, c, x[12], md5S32, 0xe6db99e5); /* 46 */
  md5HH (c, d, a, b, x[15], md5S33, 0x1fa27cf8); /* 47 */
  md5HH (b, c, d, a, x[ 2], md5S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  md5II (a, b, c, d, x[ 0], md5S41, 0xf4292244); /* 49 */
  md5II (d, a, b, c, x[ 7], md5S42, 0x432aff97); /* 50 */
  md5II (c, d, a, b, x[14], md5S43, 0xab9423a7); /* 51 */
  md5II (b, c, d, a, x[ 5], md5S44, 0xfc93a039); /* 52 */
  md5II (a, b, c, d, x[12], md5S41, 0x655b59c3); /* 53 */
  md5II (d, a, b, c, x[ 3], md5S42, 0x8f0ccc92); /* 54 */
  md5II (c, d, a, b, x[10], md5S43, 0xffeff47d); /* 55 */
  md5II (b, c, d, a, x[ 1], md5S44, 0x85845dd1); /* 56 */
  md5II (a, b, c, d, x[ 8], md5S41, 0x6fa87e4f); /* 57 */
  md5II (d, a, b, c, x[15], md5S42, 0xfe2ce6e0); /* 58 */
  md5II (c, d, a, b, x[ 6], md5S43, 0xa3014314); /* 59 */
  md5II (b, c, d, a, x[13], md5S44, 0x4e0811a1); /* 60 */
  md5II (a, b, c, d, x[ 4], md5S41, 0xf7537e82); /* 61 */
  md5II (d, a, b, c, x[11], md5S42, 0xbd3af235); /* 62 */
  md5II (c, d, a, b, x[ 2], md5S43, 0x2ad7d2bb); /* 63 */
  md5II (b, c, d, a, x[ 9], md5S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  memset_FBC(x,0,sizeof(x));
}

void MD5::MD5Update (POINTER input,int inputLen)
{
	int i, index, partLen;
	index = int((count[0] >> 3) & 0x3F);
	if ((count[0] += (FBC_Dword(inputLen) << 3))< (FBC_Dword(inputLen) << 3))
        count[1]++;
	count[1] += (FBC_Dword(inputLen) >> 29);
	partLen = 64 - index;
	if (inputLen >= partLen)
	{
		memcpy_FBC((POINTER)&buffer[index], (POINTER)input, partLen);
		MD5Transform (state, buffer);
		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (state, &input[i]);
		index = 0;
	}
	else
		i = 0;
	memcpy_FBC((POINTER)&buffer[index], (POINTER)&input[i], inputLen-i);
}

void MD5::MD5Final (FBC_Byte digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;
	DwordToBytes (bits, count, 8);
	index = (unsigned int)((count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update (PADDING, padLen);
	MD5Update (bits, 8);
	DwordToBytes(digest, state, 16);
	memset_FBC(state,0,sizeof(state));
}

NAMESPACE_END