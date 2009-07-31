#include "md4.h"

NAMESPACE_BEGIN(CryptoFBC)

const int MD4::DigestSize=16;

void MD4::MD4Transform(FBC_Dword state[4], FBC_Byte block[64])
{
	FBC_Dword a = state[0];
	FBC_Dword b = state[1];
	FBC_Dword c = state[2];
	FBC_Dword d = state[3];
	FBC_Dword x[MD4::DigestSize];
	
	BytesToDword(x, block, 64);

  /* Round 1 */
  md4FF (a, b, c, d, x[ 0], md4S11); /* 1 */
  md4FF (d, a, b, c, x[ 1], md4S12); /* 2 */
  md4FF (c, d, a, b, x[ 2], md4S13); /* 3 */
  md4FF (b, c, d, a, x[ 3], md4S14); /* 4 */
  md4FF (a, b, c, d, x[ 4], md4S11); /* 5 */
  md4FF (d, a, b, c, x[ 5], md4S12); /* 6 */
  md4FF (c, d, a, b, x[ 6], md4S13); /* 7 */
  md4FF (b, c, d, a, x[ 7], md4S14); /* 8 */
  md4FF (a, b, c, d, x[ 8], md4S11); /* 9 */
  md4FF (d, a, b, c, x[ 9], md4S12); /* 10 */
  md4FF (c, d, a, b, x[10], md4S13); /* 11 */
  md4FF (b, c, d, a, x[11], md4S14); /* 12 */
  md4FF (a, b, c, d, x[12], md4S11); /* 13 */
  md4FF (d, a, b, c, x[13], md4S12); /* 14 */
  md4FF (c, d, a, b, x[14], md4S13); /* 15 */
  md4FF (b, c, d, a, x[15], md4S14); /* 16 */

 /* Round 2 */
  md4GG (a, b, c, d, x[ 0], md4S21); /* 17 */
  md4GG (d, a, b, c, x[ 4], md4S22); /* 18 */
  md4GG (c, d, a, b, x[ 8], md4S23); /* 19 */
  md4GG (b, c, d, a, x[12], md4S24); /* 20 */
  md4GG (a, b, c, d, x[ 1], md4S21); /* 21 */
  md4GG (d, a, b, c, x[ 5], md4S22); /* 22 */
  md4GG (c, d, a, b, x[ 9], md4S23); /* 23 */
  md4GG (b, c, d, a, x[13], md4S24); /* 24 */
  md4GG (a, b, c, d, x[ 2], md4S21); /* 25 */
  md4GG (d, a, b, c, x[ 6], md4S22); /* 26 */
  md4GG (c, d, a, b, x[10], md4S23); /* 27 */
  md4GG (b, c, d, a, x[14], md4S24); /* 28 */
  md4GG (a, b, c, d, x[ 3], md4S21); /* 29 */
  md4GG (d, a, b, c, x[ 7], md4S22); /* 30 */
  md4GG (c, d, a, b, x[11], md4S23); /* 31 */
  md4GG (b, c, d, a, x[15], md4S24); /* 32 */

  /* Round 3 */
  md4HH (a, b, c, d, x[ 0], md4S31); /* 33 */
  md4HH (d, a, b, c, x[ 8], md4S32); /* 34 */
  md4HH (c, d, a, b, x[ 4], md4S33); /* 35 */
  md4HH (b, c, d, a, x[12], md4S34); /* 36 */
  md4HH (a, b, c, d, x[ 2], md4S31); /* 37 */
  md4HH (d, a, b, c, x[10], md4S32); /* 38 */
  md4HH (c, d, a, b, x[ 6], md4S33); /* 39 */
  md4HH (b, c, d, a, x[14], md4S34); /* 40 */
  md4HH (a, b, c, d, x[ 1], md4S31); /* 41 */
  md4HH (d, a, b, c, x[ 9], md4S32); /* 42 */
  md4HH (c, d, a, b, x[ 5], md4S33); /* 43 */
  md4HH (b, c, d, a, x[13], md4S34); /* 44 */
  md4HH (a, b, c, d, x[ 3], md4S31); /* 45 */
  md4HH (d, a, b, c, x[11], md4S32); /* 46 */
  md4HH (c, d, a, b, x[ 7], md4S33); /* 47 */
  md4HH (b, c, d, a, x[15], md4S34); /* 48 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  memset_FBC(x,0,sizeof(x));
}

void MD4::MD4Update (POINTER input,int inputLen)
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
		MD4Transform (state, buffer);
		for (i = partLen; i + 63 < inputLen; i += 64)
			MD4Transform (state, &input[i]);
		index = 0;
	}
	else
		i = 0;
	memcpy_FBC((POINTER)&buffer[index], (POINTER)&input[i], inputLen-i);
}

void MD4::MD4Final (FBC_Byte digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;
	DwordToBytes (bits, count, 8);
	index = (unsigned int)((count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD4Update (PADDING, padLen);
	MD4Update (bits, 8);
	DwordToBytes(digest, state, 16);
	memset_FBC(state,0,sizeof(state));
}

NAMESPACE_END