#ifndef CryptoFBC_CIPHERBASE_H
#define CryptoFBC_CIPHERBASE_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

typedef enum _tagENUM_KEY_BITS
{
	enumKeyBits_128 = 0,
	enumKeyBits_192,
	enumKeyBits_256
} ENUM_KEY_BITS;

//template <class T>
class CIPHERBASE
{
	public:
		enum CipherDir{ENCRYPTION=0,DECRYPTION};
		
		//virtual void ECB_Encryption(const T inblock[2],T outblock[2])=0;
};

NAMESPACE_END

#endif