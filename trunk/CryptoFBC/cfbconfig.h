#ifndef CryptoFBC_CFBCONFIG_H
#define CryptoFBC_CFBCONFIG_H

#define NAMESPACE_BEGIN(X) namespace X {
#define NAMESPACE_END }

NAMESPACE_BEGIN(CryptoFBC)
typedef unsigned char* POINTER;
typedef unsigned char FBC_Byte;
typedef unsigned short FBC_Word;
typedef unsigned long FBC_Dword;

#define ROL(X,n) (((X)<<(n))|((X)>>(32-(n))))
#define ROR(X,n) (((X)>>(n))|((X)<<(32-(n))))
#define NULL_FBC 0
#define FALSE_FBC 0
#define TRUE_FBC 1

static FBC_Byte PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

NAMESPACE_END

#endif