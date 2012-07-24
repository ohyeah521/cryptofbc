#ifndef CryptoFBC_TEA_H
#define CryptoFBC_TEA_H

#include "cfbconfig.h"
#include "cipherbase.h"

NAMESPACE_BEGIN(CryptoFBC)

struct TEA:public CIPHERBASE<TEA>
{
	public:
		static const int BLOCKSIZE;
		static const int ROUNDS;
		static const int Delta;
		static const int DEFAULT_KEYLENGTH;
		void ECB_Encryption(const FBC_Dword inblock[2],FBC_Dword outblock[2],const FBC_Dword dwkey[4]) const;
		void ECB_Decryption(const FBC_Dword inblock[2],FBC_Dword outblock[2],const FBC_Dword dwkey[4]) const;
};

NAMESPACE_END
#endif