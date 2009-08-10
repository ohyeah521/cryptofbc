#include "crc32.h"

NAMESPACE_BEGIN(CryptoFBC)

FBC_Dword CRC32::CalculateCRC32(char* A,FBC_Dword iLen)
{
	FBC_Dword dwValueOfCRC32=0;
	if(iLen==0)
	{
		return -1;
	}
	else
	{
		/*__asm
		{
			pushad;
			mov eax,iLen;
			mov esi,A;
Label1:
			movzx ecx,byte PTR ds:[esi];
			mov ebx,dwValueOfCRC32;
			and ebx,0xFF;
			xor ecx,ebx;
			mov ecx,DWORD PTR ds:[ecx*4+dwCRC32Table];
			mov ebx,dwValueOfCRC32;
			shr ebx,8;
			xor ecx,ebx;
			mov dwValueOfCRC32,ecx;
			inc esi;
			dec eax;
			jnz Label1;
			popad;
		}*/
		do
		{
			dwValueOfCRC32 = dwCRC32Table[ FBC_Byte(*A) ^ ( dwValueOfCRC32 & 0xff ) ] ^ (dwValueOfCRC32 >> 8);
			A++;
			iLen--;
		}while(iLen > 0);
	}
	return dwValueOfCRC32;
}

NAMESPACE_END