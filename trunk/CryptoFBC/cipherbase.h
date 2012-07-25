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

    virtual bool SetKey(char* pkey, ENUM_KEY_BITS keyBits);

    //virtual void ECB_Encryption(const T inblock[2],T outblock[2])=0;

    virtual fbc_error_type ECB_Encryption( const fbyte* pin,
                                           const int cbInLen,
                                           fbyte* pout,
                                           int* cbOutLen);

    virtual fbc_error_type ECB_Decryption(	const fbyte* pin,
                                            const int cbInLen,
                                            fbyte* pout,
                                            int* cbOutLen);

    fbc_error_type Init( ENUM_CIPHER_MODES modes,
                         char* pInitialVector,
                         int nIvLength,
                         char* pkey,
                         ENUM_KEY_BITS keyBits);

    fbc_error_type Encrypt( fbyte* pin,
                            int nInLen,
                            fbyte* pout,
                            int* nOutLen);

    fbc_error_type Decrypt( fbyte* pin,
                            int nInLen,
                            fbyte* pout,
                            int* nOutLen);

private:
    bool m_bInited;
    CIPHERBASE* m_pCipherObject;
    char m_InitialVector[16];
    ENUM_CIPHER_MODES m_CipherMode;
};

NAMESPACE_END

#endif