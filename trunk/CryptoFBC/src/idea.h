#ifndef CryptoFBC_IDEA_H
#define CryptoFBC_IDEA_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class IDEA
{
	public:
		static const int BLOCKSIZE;
		static const int DEFAULT_KEYLENGTH;
		static const int ROUNDS;
		/* Assume the length of userkey is 128 bits.I don't
		 * put some err code here to check that 
		 * but in next version i'll do that 
		 * Note::
		 * With Little Endian, UserKey is in the form as follows example:
		 * char UserKey[]="1234567890ABCDEF"
		 * then the first 8 encryption subkeys in the memory will be:
		 * 32 31 34 33 36 35 38 37 30 39 42 41 44 43 46 45 ,i.e.
		 * 2  1  4  3  6  5  8  7  0  9  B  A  D  C  F  E
		 */
		void SetKey(CipherDir dir,const FBC_Byte* UserKey);
		/* multiplication using the Low-High algorithm
		 * in Crypto++, Wei Dai use look log table to calculate the Mul
		 */
		FBC_Word IDEA_Mul(FBC_Word A,FBC_Word B) const;
		/* compute inverse of XiN by Euclidean GCD algorithm. */
		FBC_Word IDEA_Inv(FBC_Word XiN) const;
		void ECB_Encryption(FBC_Word iN[4],FBC_Word OuT[4]);
	private:
		static FBC_Word E_SubKey[9][6];
		static FBC_Word D_SubKey[9][6];
		static bool inited;
		CipherDir f_dir;
};

NAMESPACE_END
#endif