#include "cipherbase.h"

NAMESPACE_BEGIN(CryptoFBC)

template <class T>
CryptoFBC::CIPHERBASE<T>::CIPHERBASE()
{
    m_pCipherBase = NULL;
    m_bInited = false;
}

template <class T>
fbc_error_type CryptoFBC::CIPHERBASE<T>::Init( ENUM_CIPHER_MODES modes, 
                                               char* pInitialVector, 
                                               int nIvLength, 
                                               char* pkey, 
                                               ENUM_KEY_BITS keyBits )
{
    fbc_error_type nRet = fbc_err_unsuccessful;

    if ( !m_pCipherObject )
    {
        goto Exit0;
    }

Exit0:
    return nRet;
}

NAMESPACE_END