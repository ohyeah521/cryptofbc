#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

int strlen_FBC(char* str)
{
	char* ptr = str;

	while (1)
	{
		if (!*(ptr++)) break;
		if (!*(ptr++)) break;
		if (!*(ptr++)) break;
		if (!*(ptr++)) break;
	}

	return int((ptr - 1) - str);
}

char* strcpy_FBC(char* dst, char* src, const int size_of_dest)
{
	char* save = dst;
	char* szTempSrc = src;
	int n = size_of_dest;

	while( ( n >= 0 ) && ( (*dst = *szTempSrc) != '\0' ) )
	{
		dst++;
		szTempSrc++;
		n--;
	}
	//for (; ((*dst = *szTempSrc) != '\0') && ( n >= 0 ); ++dst, ++szTempSrc, --n)

	return save;
}

void* memset_FBC(void* s, int c, int n)
{
	register POINTER us = (POINTER)s;
	register FBC_Byte uc = c;

	while( n-- )
	{
		*us++ = uc;
	}

	return s;
}

void* memcpy_FBC(void* dst, const void* src, int n)
{
	register char* t = (char*)dst;
	register const char* s = (const char*)src;

	while ( n-- )
	{
		*t++ = *s++;
	}

	return dst;
}

void* memchr_FBC(const void* buf, int c, int count)
{
	char* p = (char*)buf;
	char* pIndex = NULL;

	while ( count > 0 )
	{
		if ( *p == c )
		{
			pIndex = p;
			break;
		}
		else
		{
			p++;
			count--;
		}
	}
	return pIndex;
}

/* 0x12345678 in memory 78 56 34 12 */
void DwordAndBytes::DwordToBytes(POINTER output,FBC_Dword *input,int dwordlen)
{
	int i,j;
	for (i = 0, j = 0; j <dwordlen; i++, j += 4) 
	{
#ifdef Big_Endian
		output[j+3]=FBC_Byte(input[i]&0xFF);
		output[j+2]=FBC_Byte((input[i]>>8)&0xFF);
		output[j+1] =FBC_Byte((input[i] >> 16) & 0xff);
		output[j] =FBC_Byte(input[i] >> 24) & 0xff);
#else
		output[j] =FBC_Byte(input[i] & 0xff);
		output[j+1] =FBC_Byte((input[i] >> 8) & 0xff);
		output[j+2] =FBC_Byte((input[i] >> 16) & 0xff);
		output[j+3] =FBC_Byte((input[i] >> 24) & 0xff);
#endif
	}
}

void DwordAndBytes::BytesToDword(FBC_Dword * output,const FBC_Byte* input, int byteslen)
{
	FBC_Dword a;
	FBC_Dword b;
	FBC_Dword c;
	FBC_Dword d;
	a=b=c=d=0;
	int i, j;
	for (i = 0, j = 0; j <byteslen; i++, j += 4)
	{
#ifdef Big_Endian
		a=FBC_Dword(input[j+3]);
		b=(FBC_Dword(input[j+2])<<8);
		c=(FBC_Dword(input[j+1]) << 16);
		d=(FBC_Dword(input[j]) << 24);		
#else
		a=FBC_Dword(input[j]);
		b=(FBC_Dword(input[j+1])<<8);
		c=(FBC_Dword(input[j+2]) << 16);
		d=(FBC_Dword(input[j+3]) << 24);
#endif
		output[i]=(a | b | c | d);
	}
}

void BytesAndBits::BytesToBits(bool* Out, const char* In, int bits)
{
	for(int i=0; i<bits; ++i)
        Out[i] = (In[i>>3]>>(7 - i&7)) & 1;
}

void BytesAndBits::BitsToBytes(char* Out, const bool* In, int bits)
{
	memset_FBC(Out, 0, bits>>3);
    for(int i=0; i<bits; ++i)
        Out[i>>3] |= In[i]<<(7 - i&7);
}

void TrimString::TrimLeft(char* A)
{
	int iLen=strlen_FBC(A);

	while ( iLen-- ) /* if A is null, then no trim */
	{
		if ( *A == 0x20 )
		{
			if ( iLen == 1 )
			{
				A = NULL;
			} 
			else
			{
				A++;
			}
		} 
		else
		{
			break;
		}
	}
}

void TrimString::TrimRight(char* A)
{
	int iLen=strlen_FBC(A);
	
	while( iLen-- )
	{
		if ( A[ iLen - 1 ] == 0x20 )
		{
			A[ iLen - 1 ] = '\0';
		}
	}
}

/*inline void struct Padding::RunPad(PadType ptype,char* in,unsigned inLen,char* out,unsigned& padLen)
{
	int res=(inLen&0x00000007);

	padLen=(inLen+8-res);
	memcpy(out,in,inLen);

	if(ptype==PAD_ISO_1)
	{
		memset((out+inLen),0,(8-res));
	}
	else
	if(ptype==PAD_ISO_2)
	{
		memset((out+inLen),0x80,1);
		memset((out+inLen+1),0,(7-res));
	}
	else
	if(ptype=PAD_PKCS_7)
	{
		memset((out+inLen),(8-res),(8-res));
	}
};*/

NAMESPACE_END
