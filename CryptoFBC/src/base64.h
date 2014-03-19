/********************************************************************
	created:	2011/03/13   18:16
	file base:	base64.h
	author:		cnbragon@gmail.com
	purpose:	
*********************************************************************/
#ifndef _CryptoFBC_BASE64_H
#define _CryptoFBC_BASE64_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

static const char base64_alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class FBCBase64
{
	public:
		/* return value: the Length of encoded base64 string */
		bool Base64Encode(
						const FBC_Byte* pin, 
						const int inLen, 
						FBC_Byte* pout,
						int* outLen
						);

		/* @prame inputLen must be multiple of 4, or return -1 */
		int Base64Decode(const FBC_Byte* input, const int inputLen, FBC_Byte* output);

		/* Get the base64 table index value */
		char GetBase64Value(char ch);

		/* Get number of bytes which after encoded with base64 */
		int GetEncodedSize(int nLen);
};

NAMESPACE_END

#endif