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

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef BOOL
#define BOOL long
#endif

#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#define FBC_PROCESS_POINTER(p) if ( (p) == NULL ) { goto Exit0; }
#define FBC_PROCESS_BOOL(b) if ( (b) == false ) { goto Exit0; }

static char cBasePad = '=';

static FBC_Byte PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

NAMESPACE_END

#endif