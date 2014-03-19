/********************************************************************
	created:	2011/03/13   18:18
	file base:	base64.cpp
	author:		cnbragon@gmail.com
	purpose:	
*********************************************************************/

#include "base64.h"

NAMESPACE_BEGIN(CryptoFBC)

bool FBCBase64::Base64Encode(const FBC_Byte* pin, 
							const int inLen, 
							FBC_Byte* pout,
							int* outLen)
{
	bool bRet = false;
	int nOutNeedLen = 0;
	FBC_Byte* pTemp = NULL;
	FBC_Byte* pBuffer = NULL;
	int cbin = inLen;
	
	FBC_PROCESS_POINTER(pin);
	if ( inLen <= 0 )
	{
		goto Exit0;
	}
	nOutNeedLen = GetEncodedSize(inLen);
	if ( pout == NULL			||
		 *outLen < nOutNeedLen
	)
	{
		*outLen = nOutNeedLen;
		goto Exit0;
	}
	
	pBuffer = new FBC_Byte[nOutNeedLen];
	FBC_PROCESS_POINTER(pBuffer);

	memset_FBC(pBuffer, 0, nOutNeedLen);
	pTemp = pBuffer;
	
	*outLen = 0;
	while(cbin > 0)
	{
		pTemp[0] = base64_alphabet[ FBC_Byte( ( pin[0] >> 2 ) & 0x3F ) ];
		if(cbin > 2)
		{
			pTemp[0] = base64_alphabet[ FBC_Byte( (pin[0] >> 2 ) & 0x3F ) ];
			pTemp[1] = base64_alphabet[ FBC_Byte( ( (pin[0] & 3) <<4 ) | (pin[1] >> 4 ))];
			pTemp[2] = base64_alphabet[ FBC_Byte( ( (pin[1] & 0x0F ) << 2 ) | (pin[2] >> 6))];
			pTemp[3] = base64_alphabet[ FBC_Byte( pin[2] & 0x3F ) ];
		}
		else
		{
			switch(cbin)
			{
			case 1:
				{
					pTemp[1]=base64_alphabet[FBC_Byte((pin[0]&3)<<4)];
					pTemp[2]='=';
					pTemp[3]='=';
				}
				break;
			case 2:
				{
					pTemp[1]=base64_alphabet[FBC_Byte(((pin[0]&3)<<4)|(pin[1]>>4))];
					pTemp[2]=base64_alphabet[FBC_Byte((pin[1]&0x0F)<<2)];
					pTemp[3]='=';
				}
				break;
			}
		}
		pTemp += 4;
		pin += 3;
		cbin -= 3;
		*outLen += 4;
	}
	
	memcpy_FBC(pout, pBuffer, *outLen);
	bRet = true;
Exit0:
	if ( pBuffer )
	{
		delete[] pBuffer;
		pBuffer = NULL;
	}
	return bRet;
}

char FBCBase64::GetBase64Value(char ch)
{
	char index=-1;

	if((ch>='A')&&(ch<='Z'))
	{
		index=ch-'A';
	}
	else if((ch>='a')&&(ch<='z'))
	{
		index=ch-'a'+26;
	}
	else if((ch>='0')&&(ch<='9'))
	{
		index=ch-'0'+52;
	}
	else
	{
		switch(ch)
		{
		case '+':
			index=62;
			break;
		case '/':
			index=63;
			break;
		case '=':
			index=0;
		}
	}
	return index;
}

int FBCBase64::Base64Decode(const FBC_Byte* input,const int inputLen,FBC_Byte* output)
{
	int iLen=0;
	int inLen=inputLen;
	char temp[4];

	if(inLen%4)
	{
		return FALSE;
	}

	while(inLen>0)
	{
		temp[0]=GetBase64Value(input[0]);
		temp[1]=GetBase64Value(input[1]);
		temp[2]=GetBase64Value(input[2]);
		temp[3]=GetBase64Value(input[3]);

		output[0]=((temp[0]<<2)|(temp[1]>>4));
		output[1]=((temp[1]<<4)|(temp[2]>>2));
		output[2]=((temp[2]<<6)|(temp[3]));

		input+=4;
		output+=3;
		inLen-=4;
		iLen+=3;
	}
	return iLen;
}

int FBCBase64::GetEncodedSize(int nLen)
{
	if ( ( nLen % 3 ) == 0 )
	{
		return ( nLen / 3 ) * 4;
	}
	else
	{
		return ( ( nLen / 3 ) + 1 ) * 4;
	}
}

NAMESPACE_END