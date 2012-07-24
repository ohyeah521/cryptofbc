#ifndef CryptoFBC_CFBCONFIG_H
#define CryptoFBC_CFBCONFIG_H

#define NAMESPACE_BEGIN(X) namespace X {
#define NAMESPACE_END }

NAMESPACE_BEGIN(CryptoFBC)

typedef unsigned char* POINTER;
typedef unsigned char FBC_Byte;
typedef unsigned short FBC_Word;
typedef unsigned long FBC_Dword;

typedef unsigned char   fbyte;
typedef unsigned short  fword;
typedef unsigned long   fdword;

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
#define FBC_PROCESS_ERROR(x) { if ( !(x) ) goto Exit0; }

static char cBasePad = '=';

static FBC_Byte PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

typedef int fbc_error_type;

#define fbc_err_success         0
#define fbc_err_unsuccessful    1
#define fbc_invalid_parameter   2
#define fbc_buffer_too_small    3
#define fbc_key_not_initialize  4
#define fbc_no_cipher_object    5
#define fbc_set_key_fail        6

NAMESPACE_END

#endif