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

typedef enum _tagCIPHER_DIRECTION
{
    ENCRYPTION = 0,
    DECRYPTION
} CipherDir;

template <class T>
class CIPHERBASE
{
public:	
    
    CIPHERBASE();
    ~CIPHERBASE();

    //bool SetKey(char* pkey, ENUM_KEY_BITS keyBits);

    //virtual void ECB_Encryption(const T inblock[2],T outblock[2])=0;

    /*fbc_error_type ECB_Encryption( const fbyte* pin,
                                           const int cbInLen,
                                           fbyte* pout,
                                           int* cbOutLen);

    fbc_error_type ECB_Decryption(	const fbyte* pin,
                                            const int cbInLen,
                                            fbyte* pout,
                                            int* cbOutLen);*/

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
    T* m_pCipherObject;
    char m_InitialVector[16];
    ENUM_CIPHER_MODES m_CipherMode;
};

template <class T>
CIPHERBASE<T>::CIPHERBASE()
{
    m_pCipherObject = NULL;
    m_bInited = false;

    memset_FBC( m_InitialVector, 0, 16 );

    m_CipherMode = enumMode_ECB;

    m_pCipherObject = new T;
}

template <class T>
CIPHERBASE<T>::~CIPHERBASE()
{
    if ( m_pCipherObject )
    {
        delete m_pCipherObject;
        m_pCipherObject = NULL;
    }

    memset_FBC( m_InitialVector, 0, 16 );
}

template <class T>
fbc_error_type CIPHERBASE<T>::Init( ENUM_CIPHER_MODES modes, 
                                   char* pInitialVector, 
                                   int nIvLength, 
                                   char* pkey, 
                                   ENUM_KEY_BITS keyBits )
{
    fbc_error_type nRet = fbc_err_unsuccessful;
    bool bResult = false;

    if ( !m_pCipherObject )
    {
        nRet = fbc_no_cipher_object;
        goto Exit0;
    }

    if ( !pkey )
    {
        nRet = fbc_invalid_parameter;
        goto Exit0;
    }

    if ( enumMode_ECB != modes )
    {
        if ( !pInitialVector || nIvLength <= 0 )
        {
            nRet = fbc_invalid_parameter;
            goto Exit0;
        }
    }

    m_CipherMode = modes;

    bResult = m_pCipherObject -> SetKey( pkey, keyBits );
    if ( !bResult )
    {
        nRet = fbc_set_key_fail;
        goto Exit0;
    }

    memcpy_FBC( m_InitialVector, pInitialVector, nIvLength > 16 ? 16 : nIvLength );

    m_bInited = true;
    nRet = fbc_err_success;
Exit0:
    return nRet;
}


template <class T>
fbc_error_type CIPHERBASE<T>::Encrypt( fbyte* pin, 
                                      int nInLen, 
                                      fbyte* pout, 
                                      int* nOutLen )
{
    fbc_error_type nRet = fbc_err_unsuccessful;
    fbc_error_type nResult = fbc_err_unsuccessful;
    fbyte ptemp[16];
    char piv[16]; 
    int i;
    int j;

    if ( !pin || nInLen <= 0 )
    {
        nRet = fbc_invalid_parameter;
        goto Exit0;
    }

    if ( !m_pCipherObject )
    {
        nRet = fbc_no_cipher_object;
        goto Exit0;
    }

    if ( !m_bInited )
    {
        nRet = fbc_key_not_initialize;
        goto Exit0;
    }

    switch ( m_CipherMode )
    {
    case enumMode_ECB:
        nResult = m_pCipherObject -> ECB_Encryption( pin, nInLen, pout, nOutLen );
        break;
    case enumMode_CBC:

        for ( i = 0; i < 16; i++ )
        {
            piv[i] = m_InitialVector[i];
        }

        for ( i = 0; i < nInLen / 16; i++ )
        {
            for ( j = 0; j < 16; j++ )
            {
                ptemp[j] = pin[ i * 16 + j ] ^ piv[j];
            }
            nResult = m_pCipherObject -> ECB_Encryption( ptemp, 16, ptemp, &j );
            for ( j = 0; j < 16; j++ )
            {
                piv[j] = ptemp[j];
                pout[ i * 16 + j] = ptemp[j];
            }
        }    
    default:
        break;
    }

    nRet = nResult;
Exit0:
    return nRet;
}

NAMESPACE_END

#endif