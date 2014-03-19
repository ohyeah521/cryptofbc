#ifndef CryptoFBC_BLOWFISH_H
#define CryptoFBC_BLOWFISH_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class BLOWFISH
{
	private:
		/* PBOX_init & SBOX_init store the initial PI values */
		static const FBC_Dword PBOX_init[18];
		static const FBC_Dword SBOX_init[4][256];
		/* PBOX_var & SBOX_var store the values that after modified by Key */
		FBC_Dword PBOX_var[18];
		FBC_Dword SBOX_var[4][256];
		bool Inited;
		CipherDir enumLastDir;
		FBC_Dword dwInitialVector[2];
	public:
		BLOWFISH()
		{
			Inited = false;
			enumLastDir = ENCRYPTION;
		}
		inline FBC_Dword F_Blow(FBC_Dword x) const;
		static const int BLOCKSIZE;
		static const int ROUNDS;
		/* @param keylength must between 4 to 72 bytes, i.e. 1 word to 18 words */
		void SetKey(CipherDir dir,const FBC_Byte* keystring,FBC_Word keylength);
		/* @param inblock & outblock must be 64 bits */ 
		/* @param inblock & outblock note that:
		 * test vector: 
		 * KEY: "abcdefghijklmnopqrstuvwxyz"
		 * PlainText: 'B' 'L' 'O' 'W' 'F' 'I' 'S' 'H'
		 * HexEncode:  42  4C  4F  57  46  49  53  48
		 * inblock[0]: 0x424c4f57     inblock[1]=0x46495348
		 * outblock[0]:0x324ed0fe     outblock[1]=0xf413a203
		 * In other word, this is called "Little Endian"
		 */
		void SetKey(CipherDir dir, const char* keyString, FBC_Word keyLength)
		{
			SetKey(dir, (FBC_Byte*)keyString, keyLength);
		}
		void ECB_Encryption(const FBC_Dword inblock[2],FBC_Dword outblock[2]) const;
		void Reset()
		{
			Inited = false;
		}
		void SetIV(const char* strIV, int nLen);
		bool CBC_Encryption(const char* pIn,
							int nInLen,
							char* pOut,
							int* nOut
							);
		bool CBC_Decryption(const char* pIn,
							int nInLen,
							char* pOut,
							int* nOut
							);
		bool CBC_Decryption2(const char* pIn,
							 int nInLen,
							 char* pOut,
							 int* nOut
							 );
};

NAMESPACE_END

#endif