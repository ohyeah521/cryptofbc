#ifndef CryptoFBC_CIPHERBASE_H
#define CryptoFBC_CIPHERBASE_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)
//template <class T>
class CIPHERBASE
{
	public:
		enum CipherDir{ENCRYPTION=0,DECRYPTION};
		
		//virtual void ECB_Encryption(const T inblock[2],T outblock[2])=0;
};

NAMESPACE_END

#endif