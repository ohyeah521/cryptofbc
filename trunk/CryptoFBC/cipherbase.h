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

typedef enum _tagCIPHER_MODES
{
    enumMode_ECB = 0,
    enumMode_CBC,
    enumMode_CFB,
    enumMode_OFB,
    enumMode_CTR
} ENUM_CIPHER_MODES;

template <class T>
class CIPHERBASE
{
public:
	enum CipherDir{ENCRYPTION=0,DECRYPTION};
    
    CIPHERBASE();
    ~CIPHERBASE();

	//virtual void ECB_Encryption(const T inblock[2],T outblock[2])=0;

    virtual bool SetKey(char* pkey, ENUM_KEY_BITS keyBits);

    fbc_error_type Init( ENUM_CIPHER_MODES modes,
                         char* pInitialVector,
                         int nIvLength,
                         char* pkey,
                         ENUM_KEY_BITS keyBits);
public:
    bool m_bInited;
    CIPHERBASE* m_pCipherObject;
};

NAMESPACE_END

#endif