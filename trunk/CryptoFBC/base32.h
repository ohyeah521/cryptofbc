/********************************************************************
	created:	2010/09/18
	created:	18:9:2010   0:18
	file base:	base32.h
	author:		cnbragon
	purpose:	Define Base32 Class
*********************************************************************/
#ifndef CryptoFBC_BASE32_H
#define CryptoFBC_BASE32_H

#include "cfbconfig.h"

NAMESPACE_BEGIN(CryptoFBC)

static const char* base32_StdTable = "ABCDEFGHJKMNPQRSTVWXYZ1234567890";

class CBASE32
{
public:
	CBASE32()
	{
		base32_Table = base32_StdTable;
	}
	CBASE32(const char* szUserTable)
	{
		base32_Table = szUserTable;
	}
	/* */
	bool Base32Encode(
					const char* pInput,
					int nInputBytes,
					char* pOutput,
					int* pNumOutputBytes
	);

	bool Base32Decode(
					const char* pInput,
					int nInputBytes,
					char* pOutput,
					int* pNumOutputBytes
	);
	
	void GenLookupTable();
	unsigned char GetBase32Index(const char cx);
	unsigned char base32_Lookup[ 256 ];
private:
	const char* base32_Table;
};

NAMESPACE_END

#endif