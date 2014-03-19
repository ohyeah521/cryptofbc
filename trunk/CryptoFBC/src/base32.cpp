/********************************************************************
	created:	2010/09/18
	created:	18:9:2010   0:55
	file base:	base32.cpp
	author:		cnbragon
	purpose:	Base32 Class Implementation
*********************************************************************/
#include "baseroutine.h"
#include "base32.h"

NAMESPACE_BEGIN(CryptoFBC)

unsigned char CBASE32::GetBase32Index(const char cx)
{
	GenLookupTable();
	return base32_Lookup[ cx ];
}

bool CBASE32::Base32Encode( const char* pInput, 
						    int nInputBytes, 
							char* pOutput, 
							int* pNumOutputBytes
						  )
{
	FBC_Dword dwBytesNeed = 0;
	FBC_Byte* pSrc = NULL;
	char* pDst = NULL;
	int nLen = 0;
	FBC_Byte ucIndex = -1;
	int nBytesProcessed = 0;
	int nBytesOutput = 0;
	int nTemp = 0;

	FBC_PROCESS_POINTER(pInput);
	FBC_PROCESS_POINTER(pNumOutputBytes);
	if ( nInputBytes <= 0 )
	{
		goto Exit0;
	}
	if ( pOutput == NULL )
	{
		nTemp = (nInputBytes % 5) ? (nInputBytes / 5 + 1) : (nInputBytes / 5 );
		*pNumOutputBytes = nTemp << 3;
		goto Exit1;
	}
	if ( *pNumOutputBytes <= 0 )
	{
		goto Exit0;
	}
		
	nTemp = (nInputBytes % 5) ? (nInputBytes / 5 + 1) : (nInputBytes / 5 );
	dwBytesNeed = nTemp << 3;
	if ( FBC_Dword(*pNumOutputBytes) < dwBytesNeed )
	{
		goto Exit0;
	}
	
	pSrc = (FBC_Byte*)pInput;
	pDst = pOutput;
	nLen = nInputBytes;

	while ( nLen > 0 )
	{
		if ( nLen >= 5 )
		{
			ucIndex	= *pSrc >> 3;
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = ( (*pSrc++ & 7) << 2 );
			ucIndex |= (*pSrc >> 6);
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = (*pSrc >> 1) & 0x1f;
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = ( (*pSrc++ & 1) << 4 );
			ucIndex |= (*pSrc >> 4);
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = ( (*pSrc++ & 0x0f) << 1 );
			ucIndex |= (*pSrc >> 7);
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = (*pSrc >> 2) & 0x1f;
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = ( (*pSrc++ & 3) << 3 );
			ucIndex |= (*pSrc >> 5);
			*pDst++ = base32_Table[ ucIndex ];
			ucIndex = *pSrc & 0x1f;
			*pDst++ = base32_Table[ ucIndex ];
			pSrc++;
			nBytesProcessed += 5;
			nLen -= 5;
		}
		else
		{
			switch ( nLen )
			{
			case 1:
				ucIndex = *pSrc >> 3;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc & 7) << 2;
				*pDst++ = base32_Table[ ucIndex ];
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst   = cBasePad;
				nBytesProcessed++;
				nLen--;
				break;
			case 2:
				ucIndex = *pSrc >> 3;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 7) << 2 );
				ucIndex |= (*pSrc >> 6);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc >> 1) & 0x1f;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc & 1) << 4;
				*pDst++ = base32_Table[ ucIndex ];
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst   = cBasePad;
				nBytesProcessed += 2;
				nLen -= 2;
				break;
			case 3:
				ucIndex	= *pSrc >> 3;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 7) << 2 );
				ucIndex |= (*pSrc >> 6);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc >> 1) & 0x1f;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 1) << 4 );
				ucIndex |= (*pSrc >> 4);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc & 0x0f) << 1;
				*pDst++ = base32_Table[ ucIndex ];
				*pDst++ = cBasePad;
				*pDst++ = cBasePad;
				*pDst   = cBasePad;
				nBytesProcessed += 3;
				nLen -= 3;
				break;
			case 4:
				ucIndex	= *pSrc >> 3;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 7) << 2 );
				ucIndex |= (*pSrc >> 6);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc >> 1) & 0x1f;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 1) << 4 );
				ucIndex |= (*pSrc >> 4);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = ( (*pSrc++ & 0x0f) << 1 );
				ucIndex |= (*pSrc >> 7);
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc >> 2) & 0x1f;
				*pDst++ = base32_Table[ ucIndex ];
				ucIndex = (*pSrc & 3) << 3;
				*pDst++ = base32_Table[ ucIndex ];
				*pDst   = cBasePad;
				nBytesProcessed += 4;
				nLen -= 4;
				break;
			}
		}
		nBytesOutput += 8;
	}
	*pNumOutputBytes = nBytesOutput;
Exit1:
	return true;
Exit0:
	return false;
}

void CBASE32::GenLookupTable()
{
	int i = 0;
	for ( i = 0; i < 256; i++ )
	{
		base32_Lookup[ i ] = -1;
	}
	for ( i = 0; i < 32; i++ )
	{
		base32_Lookup[ base32_Table[ i ] ] = i;
	}
	base32_Lookup[ cBasePad ] = i;
}

bool CBASE32::Base32Decode(
						   const char* pInput,
						   int nInputBytes,
						   char* pOutput,
						   int* pNumOutputBytes
						  )
{
	char* p = NULL;
	int i = 0;
	FBC_Byte* pSrc = NULL;
	FBC_Byte* pDst = NULL;
	int nLen = 0;
	FBC_Byte ucTemp = 0;
	unsigned int ulBits = 0;
	int nOffset = 0;
	int nOutputLen = 0;

	FBC_PROCESS_POINTER(pInput);
	FBC_PROCESS_POINTER(pNumOutputBytes);
	if ( nInputBytes <= 0 )
	{
		goto Exit0;
	}
	if ( pOutput == NULL )
	{
		p = (char*)memchr_FBC(pInput, cBasePad, nInputBytes);
		if ( p == NULL )
		{
			*pNumOutputBytes = ( (nInputBytes * 5) >> 3 ) + 1;
		}
		else
		{
			*pNumOutputBytes = ( ( int(p - pInput) * 5 ) >> 3 ) + 1;
		}
		goto Exit1;
	}
	if ( *pNumOutputBytes <= 0 )
	{
		goto Exit0;
	}
	GenLookupTable();
	for ( i = 0; i < nInputBytes; i++ )
	{
		if ( base32_Lookup[ pInput[ i ] ] == -1 )
		{
			goto Exit0;
		}
	}

	pSrc = (FBC_Byte*)pInput;
	pDst = (FBC_Byte*)pOutput;
	p = (char*)memchr_FBC(pInput, cBasePad, nInputBytes);
	if ( p != NULL )
	{
		nLen = int(p - pInput);
	}
	else
	{
		nLen = nInputBytes;
	}
	
	nOutputLen = ( ( nLen * 5 ) >> 3 ) + 1;
	if ( *pNumOutputBytes < nOutputLen )
	{
		goto Exit0;
	}

	for ( i = 0; i < nLen; i++ )
	{
		ucTemp = base32_Lookup[ pSrc[ i ] ];
		if ( ulBits <= 3 )
		{
			ulBits = (ulBits + 5) & 7;
			if ( ulBits == 0 )
			{
				pDst[ nOffset++ ] |= ucTemp;
			}
			else
			{
				pDst[ nOffset ] |= ( ucTemp << ( 8 - ulBits ) );
			}
		}
		else
		{
			ulBits = ( ulBits + 5) & 7;
			pDst[ nOffset++ ] |= (ucTemp >> ulBits);
			pDst[ nOffset ] |= ( ucTemp << (8 - ulBits) );
		}
	}
	*pNumOutputBytes = nOffset;
Exit1:
	return true;
Exit0:
	return false;
}
NAMESPACE_END