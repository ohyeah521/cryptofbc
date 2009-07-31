#include "tea.h"

NAMESPACE_BEGIN(CryptoFBC)

const int TEA::BLOCKSIZE=8;
const int TEA::ROUNDS=32;
const int TEA::Delta=0x9E3779B9;
const int TEA::DEFAULT_KEYLENGTH=128;

void TEA::ECB_Encryption(const FBC_Dword inblock[2],FBC_Dword outblock[2],const FBC_Dword dwkey[4]) const
{
	unsigned long y=inblock[0],z=inblock[1],dwsum=0,n=ROUNDS;

	while(n-->0)
	{
		dwsum+=Delta;
		y+=(z<<4)+dwkey[0]^z+dwsum^(z>>5)+dwkey[1];
		z+=(y<<4)+dwkey[2]^y+dwsum^(y>>5)+dwkey[3];
	}
	outblock[0]=y;
	outblock[1]=z;
}

void TEA::ECB_Decryption(const FBC_Dword inblock[2],FBC_Dword outblock[2],const FBC_Dword dwkey[4]) const
{
	unsigned long y=inblock[0],z=inblock[1],dwsum=0,n=ROUNDS;

	dwsum=(Delta<<5);
	while(n-->0)
	{
		z-=(y<<4)+dwkey[2]^y+dwsum^(y>>5)+dwkey[3];
		y-=(z<<4)+dwkey[0]^z+dwsum^(z>>5)+dwkey[1];
		dwsum-=Delta;
	}
	outblock[0]=y;
	outblock[1]=z;
}

NAMESPACE_END