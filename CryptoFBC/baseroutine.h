#ifndef CryptoFBC_BASEROUTINE_H
#define CryptoFBC_BASEROUTINE_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

int strlen_FBC(char* str);
/* it is safe */
char* strcpy_FBC(char* dst, char* src, const int size_of_dest);
void* memset_FBC(void* s, int c, int n);
void* memcpy_FBC(void* dst, const void* src, int n);
void* memchr_FBC(const void* buf, int c, int count);

class DwordAndBytes
{
public:
	static void DwordToBytes(POINTER output,FBC_Dword *input,int dwordlen);
	static void BytesToDword(FBC_Dword * output,const FBC_Byte* input, int byteslen);	
};

class BytesAndBits
{
public:
	static void BytesToBits(bool* Out, const char* In, int bits);
	static void BitsToBytes(char* Out, const bool* In, int bits);
};

class TrimString
{
	TrimString(char* A)
	{
		TrimLeft(A);
		TrimRight(A);
	}
public:
	/* if string A is "all space string", return NULL_FBC (0) */
	static void TrimLeft(char* A);
	static void TrimRight(char* A);
};

template <class T>
inline void swap(T& a,T& b)
{
	T temp;
	temp = a;
	a = b;
	b = temp;
}

inline void ReverseDword(FBC_Dword& x, FBC_Dword& y)
{
	FBC_Dword z = 0;
	z |= ( (x & 0xff) << 24 );
	z |= ( (x & 0xff00) << 8 );
	z |= ( (x & 0xff0000) >> 8 );
	z |= ( (x & 0xff000000) >> 24 );
	y = z;
}

#define bytes_to_big_dword(p) ( ( (FBC_Byte)(p)[0] << 24 ) |	\
								( (FBC_Byte)(p)[1] << 16 ) |	\
								( (FBC_Byte)(p)[2] << 8  ) |	\
								( (FBC_Byte)(p)[3] ) )

#define big_dword_to_bytes(x, p) ( (p)[0] = (FBC_Byte)( (x) >> 24 ); \
                                  (p)[1] = (FBC_Byte)( (x) >> 16 ); \
                                  (p)[2] = (FBC_Byte)( (x) >> 8  ); \
                                  (p)[3] = (FBC_Byte)( x ) );                                         

#define bytes_to_little_dword(p) ( *(FBC_Dword*)(p) )

#define little_dword_to_bytes(x, p) ( \
    FBC_Byte* pu = (FBC_Byte*)&(x);  \
    (p)[0] = pu[0]; \
    (p)[1] = pu[1]; \
    (p)[2] = pu[2]; \
    (p)[3] = pu[3]; )

struct Padding
{
public:
	enum PadType{PAD_ISO_1=0,PAD_ISO_2,PAD_PKCS_7};
	inline void RunPad(PadType ptype,char* in,unsigned inLen,char* out,unsigned* padLen);
};

NAMESPACE_END

#endif