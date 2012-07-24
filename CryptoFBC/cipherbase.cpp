#include "cipherbase.h"

NAMESPACE_BEGIN(CryptoFBC)

template <class T>
CryptoFBC::CIPHERBASE<T>::CIPHERBASE()
{
    m_pCipherBase = NULL;
    m_bInited = false;
    
    memset_FBC( m_InitialVector, 0, 16 );

    m_CipherMode = enumMode_ECB;

    m_pCipherBase = new T;
}

template <class T>
CryptoFBC::CIPHERBASE<T>::~CIPHERBASE()
{
    if ( m_pCipherObject )
    {
        delete m_pCipherObject;
        m_pCipherObject = NULL;
    }

    memset_FBC( m_InitialVector, 0, 16 );
}

template <class T>
fbc_error_type CryptoFBC::CIPHERBASE<T>::Init( ENUM_CIPHER_MODES modes, 
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
fbc_error_type CryptoFBC::CIPHERBASE<T>::Encrypt( fbyte* pin, 
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
    int nLen;

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