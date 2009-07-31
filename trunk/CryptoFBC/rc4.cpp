#include "rc4.h"

NAMESPACE_BEGIN(CryptoFBC)

void RC4::RC4_Initial(FBC_Byte* RC4_Key,int iKeyLength)
{
	int i,j,k;
	FBC_Byte TempA;

	RC4_X=RC4_Y=0;
	for(i=0;i<256;i++)
	{
		RC4_M[i]=FBC_Byte(i);
	}
	j=k=0;
	for(i=0;i<256;i++)
	{
		TempA=RC4_M[i];
		j=FBC_Byte(j+TempA+RC4_Key[k]);
		RC4_M[i]=RC4_M[j];
		RC4_M[j]=TempA;
		if(++k>=iKeyLength)
		{
			k=0;
		}
	}
}

void RC4::RC4_Crypt(FBC_Byte* szMessage,int iMessageLength)
{
	int i;
	FBC_Byte TempA,TempB;

	for(i=0;i<iMessageLength;i++)
	{
		RC4_X=FBC_Byte(RC4_X+1);
		TempA=RC4_M[RC4_X];
		RC4_Y=FBC_Byte(RC4_Y+TempA);
		RC4_M[RC4_X]=TempB=RC4_M[RC4_Y];
		RC4_M[RC4_Y]=TempA;
		szMessage[i]^=RC4_M[FBC_Byte(TempA+TempB)];		
	}
}

NAMESPACE_END