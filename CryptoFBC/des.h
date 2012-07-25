#ifndef CryptoFBC_DES_H
#define CryptoFBC_DES_H

#include "cfbconfig.h"
#include "cipherbase.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

class DES:public Padding,public BytesAndBits
{
	private:
		/* Initial Permutation Table */
		static const FBC_Byte IP_table[64];
		/* the inverse of the initial permutation table */
		static const FBC_Byte IP_inverse_table[64];
		/* E Bit-Selection Table */
		static const FBC_Byte E_bstable[48];
		static const FBC_Byte Des_SBOX[8][64];
		/* 32-bit permutation function P used on the output of the S-boxes */
		static const FBC_Byte P_table[32];
		/* Permuted Choice 1 Table for Key Schedule */
		static const FBC_Byte PC1_table[56];
		/* Permuted Choice 2 Table for Key Schedule */
		static const FBC_Byte PC2_table[48];
		/* Number Of Left Shifts each iteration */
		static const FBC_Byte Loop_Table[16];
		bool DesSubKey[16][48];
		//bool inited;
	public:
		static const int BLOCKSIZE;
		static const int ROUNDS;
		void SetKey(CipherDir dir,const char keystring[8]);
		void Transform(bool* Out, bool* In, const FBC_Byte* Table, int tablelen) const ;
		inline void RotateL(bool* In,int inLen,int nShift) const ;
		void ECB_Encryption(const char in[8],char out[8]) const;
		void S_func(bool Out[32], const bool In[48]) const;
		void F_func(bool In[32], const bool Ki[48]) const;
		void Xor(bool* InA, const bool* InB, int inLen) const;
};

NAMESPACE_END

#endif