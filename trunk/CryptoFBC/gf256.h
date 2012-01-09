#ifndef CryptoFBC_GF256_H
#define CryptoFBC_GF256_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

/* GF256 implementation references to AES(Rijndael) */

class GF256
{
	public:
		typedef FBC_Byte Element;
		GF256(FBC_Word A):gf256_modulus(A){}
		Element GF256_Add(Element a,Element b) const;
		Element GF256_Subtract(Element a,Element b) const;
		/* this is common way to calculate Multiplier over GF(2^n)
		 * but there is another way to implement multiply operation
		 * See 
		 * <<Fast Software Implementations of Finite Field Operations>>
		 * by Cheng Huang Lihao Xu
		 * it's the table look up way , and very fast
		 */
		Element GF256_Multiply(Element a,Element b) const;
		Element GF256_Square(Element a) const;
		Element GF256_MultiplicativeInverse(Element a) const;
		Element GF256_Divide(Element a,Element b) const;
		Element GF256_Inverse(Element a) const;
	private:
		FBC_Word gf256_modulus; 
};

NAMESPACE_END

#endif