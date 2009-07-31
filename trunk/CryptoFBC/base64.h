#ifndef CryptoFBC_BASE64_H
#define CryptoFBC_BASE64_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

static const char base64_alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class Base64
{
	public:
		/* return value: the Length of encoded base64 string */
		int Base64Encode(const FBC_Byte* input,const int inputLen,FBC_Byte* output);
		/* @prame inputLen must be multiple of 4, or return -1 */
		int Base64Decode(const FBC_Byte* input,const int inputLen,FBC_Byte* output);
		/* Get the base64 table index value */
		char GetBase64Value(char ch);
};

NAMESPACE_END

#endif