#include "ModifiedMD5.h"

NAMESPACE_BEGIN(CryptoFBC)

const int MODIFIEDMD5::DigestSize=16;

void MODIFIEDMD5::MD5Transform(FBC_Dword state[4], FBC_Byte block[64])
{
	FBC_Dword a = state[0];
	FBC_Dword b = state[1];
	FBC_Dword c = state[2];
	FBC_Dword d = state[3];
	FBC_Dword x[MODIFIEDMD5::DigestSize];
	
	BytesToDword(x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xF57C0FAF); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xE8C7B756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0xA672338E); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xC1BDCEEE); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xD76BB478); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787C62A); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xF3CF5CB1); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x1B24F7AF); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xFD469501); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895CD7BE); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0x242340DB); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xA2E3F905); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xC040B340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265E5A51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0x676422D9); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0x425A14ED); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0xF62F105D); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xE61E7562); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0xE9B4A7AA); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xE5A127FA); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0x1FA29CF8); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4BDECFA9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xF6BB4B60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xBEBFBC70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289B7EC6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xC3FA3942); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xD4EF3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881D05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0xA4BFEA44); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0x5D3AF2E5); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xEE08B1A1); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0xAFE3F47D); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xF429E2F4); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  memset_FBC(x,0,sizeof(x));
}

void MODIFIEDMD5::MD5Update (POINTER input,int inputLen)
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

void MODIFIEDMD5::MD5Final (FBC_Byte digest[16])
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