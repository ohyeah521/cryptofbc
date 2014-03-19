/* Twofish Official Site:http://www.schneier.com/twofish.html */

#ifndef CryptoFBC_TWOFISH_H
#define CryptoFBC_TWOFISH_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "gf256.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class TWOFISH
{
	public:
		static const int BLOCKSIZE;
		static const int ROUNDS;
		static const int rho;
		static const int DEFAULT_KEYLENGTH;
		static FBC_Word KEYLENGTH;
		void SetKey(const FBC_Byte* KeyMaterial,FBC_Word keyLen);
		FBC_Byte Twofish_Q0(FBC_Byte X) const;
		FBC_Byte Twofish_Q1(FBC_Byte X) const;
		FBC_Dword Twofish_h(FBC_Dword X,FBC_Dword* L) const;
		void Twofish_F(FBC_Dword R0,FBC_Dword R1,int r);
		void ECB_Encryption(const FBC_Byte* in,FBC_Byte* out);
		void ECB_Decryption(const FBC_Byte* in,FBC_Byte* out);
	private:
		static FBC_Dword Twofish_SubKey[40];
		static FBC_Dword Twofish_SboxKey[4];
		static FBC_Dword F[2];
		static const FBC_Byte RS_Matrix[4][8];
		static const FBC_Byte MDS_Matrix[4][4];
};

NAMESPACE_END

#endif