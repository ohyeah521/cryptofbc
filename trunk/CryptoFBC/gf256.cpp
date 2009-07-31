#include "gf256.h"

NAMESPACE_BEGIN(CryptoFBC)

GF256::Element GF256::GF256_Add(Element a,Element b) const
{
	return a^b;
}

GF256::Element GF256::GF256_Subtract(Element a,Element b) const
{
	return a^b;
}

GF256::Element GF256::GF256_Multiply(Element a,Element b) const
{
	FBC_Word result=0, temp=b;

	for(int i=0;i<8;i++)
	{
		result<<=1;
		if(result&0x100)
		{
			result^=gf256_modulus;
		}

		temp<<=1;
		if(temp&0x100)
		{
			result^=a;
		}
	}

	return (GF256::Element) result;
}

GF256::Element GF256::GF256_Square(Element a) const
{
	return GF256_Multiply(a,a);
}

GF256::Element GF256::GF256_MultiplicativeInverse(Element a) const
{
	Element result=a;

	for(int i=1;i<7;i++)
	{
		result=GF256_Multiply(GF256_Square(result),a);
	}
	return GF256_Square(result);
}

GF256::Element GF256::GF256_Divide(Element a,Element b) const
{
	return GF256_Multiply(a,GF256_MultiplicativeInverse(b));
}

GF256::Element GF256::GF256_Inverse(Element a) const
{
	return a;
}

NAMESPACE_END