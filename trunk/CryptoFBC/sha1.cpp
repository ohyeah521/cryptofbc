#include "sha1.h"

NAMESPACE_BEGIN(CryptoFBC)

const int SHA1::DigestSize=20;

void SHA1::SHA1Transform(FBC_Dword state[5], FBC_Dword  block[16])
{
	FBC_Dword dwMS[80]={0};									/* a message schedule of eighty 32-bit words	*/
	FBC_Dword a,b,c,d,e;									/* five working variables of 32-bit each		*/
	int t=0;
	FBC_Dword dwtemp=0;

	for (t=0;t<16;t++)
	{
		dwtemp=block[t];
		__asm
		{
			mov		eax,	dwtemp
			bswap	eax
			mov		dwtemp,	eax
		}
		dwMS[t]=dwtemp;
	}

	for (;t<80;t++)
	{
		dwMS[t]=ROTL(dwMS[t-3]^dwMS[t-8]^dwMS[t-14]^dwMS[t-16],1);
	}

	a=state[0];												/* initialize the five working variables		*/
	b=state[1];
	c=state[2];
	d=state[3];
	e=state[4];
	
	for (t=0;t<20;t++)
	{
		dwtemp=ROTL(a,5)+Ch(b,c,d)+e+0x5a827999+dwMS[t];
		e=d;
		d=c;
		c=ROTL(b,30);
		b=a;
		a=dwtemp;
	}

	for (;t<40;t++)
	{
		dwtemp=ROTL(a,5)+Parity(b,c,d)+e+0x6ed9eba1+dwMS[t];
		e=d;
		d=c;
		c=ROTL(b,30);
		b=a;
		a=dwtemp;
	}

	for (;t<60;t++)
	{
		dwtemp=ROTL(a,5)+Maj(b,c,d)+e+0x8f1bbcdc+dwMS[t];
		e=d;
		d=c;
		c=ROTL(b,30);
		b=a;
		a=dwtemp;
	}

	for (;t<80;t++)
	{
		dwtemp=ROTL(a,5)+Parity(b,c,d)+e+0xca62c1d6+dwMS[t];
		e=d;
		d=c;
		c=ROTL(b,30);
		b=a;
		a=dwtemp;
	}

	state[0]+=a;
	state[1]+=b;
	state[2]+=c;
	state[3]+=d;
	state[4]+=e;
}

void SHA1::SHA1_Update(FBC_Byte* szMessage, FBC_Dword dwLenOfM)
{
	FBC_Dword dwindex=0,i=0,partLen;

	dwindex=(count[0]>>3)&0x3f;
	if ((count[0]+=(dwLenOfM<<3))<(dwLenOfM<<3))
	{
		count[1]++;
	}
	count[1]+=(dwLenOfM>>29);
	partLen=64-dwindex;
	if (dwLenOfM>=partLen)
	{
		memcpy_FBC(((FBC_Byte*)buffer+dwindex),szMessage,partLen);
		SHA1Transform(state,buffer);
		for (i=partLen;i+63<dwLenOfM;i+=64)
		{
			SHA1Transform(state,&buffer[i]);
		}
		dwindex=0;
	} 
	else
	{
		i=0;
	}
	memcpy_FBC(((FBC_Byte*)buffer+dwindex),(szMessage+i),dwLenOfM-i);
}

void SHA1::SHA1_Final(FBC_Byte szDigest[20])
{
	FBC_Dword bits[2]={0};
	FBC_Dword dwx[5]={0};
	unsigned int index, padLen;
	FBC_Dword dwtemp=0;
	bits[0]=count[1];
	bits[1]=count[0];
	for (index=0;index<2;index++)
	{
		dwtemp=bits[index];
		__asm
		{
			mov		eax,	dwtemp
			bswap	eax
			mov		dwtemp,	eax
		}
		bits[index]=dwtemp;
	}
	index = (unsigned int)((count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	SHA1_Update (SHA1_PADDING, padLen);
	SHA1_Update ((FBC_Byte*)bits, 8);
	for (index=0;index<5;index++)
	{
		dwtemp=state[index];
		__asm
		{
			mov		eax,	dwtemp
			bswap	eax
			mov		dwtemp,	eax
		}
		dwx[index]=dwtemp;
	}
	memcpy_FBC(szDigest,dwx,20);
	memset_FBC(state,0,sizeof(state));
}

NAMESPACE_END