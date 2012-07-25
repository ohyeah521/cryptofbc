#ifndef CryptoFBC_AES_H
#define CryptoFBC_AES_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class FBC_AES
{
	public:
		static const int BLOCKSIZE;
		static const int DEFAULT_KEYLENGTH;
		static const int ROUNDS;
		inline FBC_Dword aes_swap(FBC_Dword dwX)
		{
			register int tempA,tempB;
			tempA=dwX;
			_asm{ rol tempA, 8          }
			_asm{ and tempA, 0xFF00FF00 }
			tempB=dwX;
			_asm{ ror tempB, 8 }
			_asm{ and tempB, 0x00FF00FF }
			return (tempA|tempB);
		}
		void SetKey(CipherDir dir, const FBC_Dword dwKey[4]);
		void ECB_Encryption(const FBC_Dword inBlock[4], FBC_Dword outBlock[4]);
		void ECB_Decryption(const FBC_Dword inBlock[4], FBC_Dword outBlock[4]);

		bool SetKey(char* pkey, ENUM_KEY_BITS keyBits);

        fbc_error_type ECB_Encryption( const FBC_Byte* pin,
                                       const int cbInLen,
                                       FBC_Byte* pout,
                                       int* cbOutLen);

		fbc_error_type ECB_Decryption(	const fbyte* pin,
										const int cbInLen,
										fbyte* pout,
										int* cbOutLen);

        FBC_AES();
	private:
		FBC_Dword m_dwEnSubKey[ 4 * ( 14 + 1 ) ];
        fdword m_dwDeSubKey[ 4 * ( 14 + 1 ) ];
		static const FBC_Dword dwTe0[256];
		static const FBC_Dword dwTe1[256];
		static const FBC_Dword dwTe2[256];
		static const FBC_Dword dwTe3[256];
		static const FBC_Dword dwTe4[256];
		static const FBC_Dword dwTd0[256];
		static const FBC_Dword dwTd1[256];
		static const FBC_Dword dwTd2[256];
		static const FBC_Dword dwTd3[256];
		static const FBC_Dword dwTd4[256];
		static const FBC_Dword RCon[10];
		int m_nb;  // number of block
		int m_nk;  // number of key
		int m_nr;  // number of round
        bool m_bKeyInited;
};

NAMESPACE_END

#endif