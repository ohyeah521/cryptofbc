#ifndef CryptoFBC_RC4_H
#define CryptoFBC_RC4_H

#include "cfbconfig.h"
#include "cipherbase.h"

NAMESPACE_BEGIN(CryptoFBC)

struct RC4
{
	private:
		FBC_Byte RC4_X;
		FBC_Byte RC4_Y;
		FBC_Byte RC4_M[256];
	public:
		/* @param: iKeyLength is represented by BYTES */
		void RC4_Initial(FBC_Byte* RC4_Key,int iKeyLength);
		void RC4_Crypt(FBC_Byte* szMessage,int iMessageLength);
};

NAMESPACE_END
#endif