#include "base64.h"

NAMESPACE_BEGIN(CryptoFBC)

int Base64::Base64Encode(const FBC_Byte* input,const int inputLen,FBC_Byte* output)
{
	int iLen=0;
	int inLen=inputLen;

	while(inLen>0)
	{
		output[0]=base64_alphabet[FBC_Byte((input[0]>>2)&0x3F)];
		if(inLen>2)
		{
			output[0]=base64_alphabet[FBC_Byte((input[0]>>2)&0x3F)];
			output[1]=base64_alphabet[FBC_Byte(((input[0]&3)<<4)|(input[1]>>4))];
			output[2]=base64_alphabet[FBC_Byte(((input[1]&0x0F)<<2)|(input[2]>>6))];
			output[3]=base64_alphabet[FBC_Byte(input[2]&0x3F)];
		}
		else
		{
			switch(inLen)
			{
			case 1:
				{
					output[1]=base64_alphabet[FBC_Byte((input[0]&3)<<4)];
					output[2]='=';
					output[3]='=';
				}
				break;
			case 2:
				{
					output[1]=base64_alphabet[FBC_Byte(((input[0]&3)<<4)|(input[1]>>4))];
					output[2]=base64_alphabet[FBC_Byte((input[1]&0x0F)<<2)];
					output[3]='=';
				}
				break;
			}
		}
		output+=4;
		input+=3;
		inLen-=3;
		iLen+=4;
	}
	return iLen;
}

char Base64::GetBase64Value(char ch)
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

int Base64::Base64Decode(const FBC_Byte* input,const int inputLen,FBC_Byte* output)
{
	int iLen=0;
	int inLen=inputLen;
	char temp[4];

	if(inLen%4)
	{
		return FALSE_FBC;
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

NAMESPACE_END