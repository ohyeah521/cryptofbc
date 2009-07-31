#ifndef CryptoFBC_RIJNDAEL_H
#define CryptoFBC_RIJNDAEL_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class RIJNDAEL:public CIPHERBASE
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
	private:
		FBC_Dword dwSubKey[44];
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
};

NAMESPACE_END

#endif