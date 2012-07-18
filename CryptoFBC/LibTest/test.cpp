#include <iostream>
#include <windows.h>

#include "../blowfish.h"
#include "../baseroutine.h"
#include "../md5.h"
#include "../des.h"
#include "../tea.h"
#include "../gf256.h"
#include "../twofish.h"
#include "../idea.h"
#include "../rc4.h"
#include "../ModifiedBlowfish.h"
#include "../aes.h"
#include "../base64.h"
#include "../sha1.h"
#include "../crc32.h"
#include "../base32.h"
#include "../rc5.h"
#include "../sha256.h"

using namespace std;
using namespace CryptoFBC;

#ifdef _DEBUG
#pragma comment(lib, "../Debug/CryptoFBC.lib")
#else
#pragma comment(lib, "../Release/CryptoFBC.lib")
#endif

//void Test_CRC32()
//{
//	FBC_CRC32 crc32test;
//	FBC_Dword dwX = 0;
//
//	dwX = crc32test.CalculateCRC32("123456", lstrlen("123456"));
//}

void Test_SHA1()
{
	SHA1 sha1test;
	FBC_Byte a[]="abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

	sha1test.SHA1_Update(a,56);

	FBC_Byte* Digest=new FBC_Byte[SHA1::DigestSize];
	sha1test.SHA1_Final(Digest);
}

void Test_Base64()
{
	int i;
	WORD wtemp=1,wA8=0;
	WORD wX[9]={0};
	WORD wSum=0;
	BYTE sztemp[19]={0x3d,0x2c,0x8e,0x61,0x6d,0x4f,0x0a,0x27,0x7d,0x1c,0x75,0x67,0x09,0x64,0x11,0x49,0x4e,0x35};
	BYTE szSn[256]={0};
	BYTE szOut[256]={0};
	FBCBase64 b641;
		
	srand(GetTickCount());
	while (wtemp%2==1)
	{
		wtemp=rand();
	}
	wA8=wtemp>>1;
	wX[8]=wA8;
	for (i=0;i<7;i++)
	{
		wX[i]=rand();
		wSum+=wX[i];
	}
	wX[i]=wA8-wSum;
	memcpy(sztemp,wX,18);
	for (i=0;i<18;i++)
	{
		std::cout<<hex<<int(sztemp[i])<<" ";
	}
	std::cout<<endl;
//	b641.Base64Encode(sztemp,18,szSn);
	std::cout<<szSn<<endl;
	b641.Base64Decode(szSn,24,szOut);
	for (i=0;i<18;i++)
	{
		std::cout<<hex<<(unsigned int)szOut[i]<<" ";
	}
	std::cout<<endl;
}

void Test_Rijndael()
{
	//RIJNDAEL rijndael1;
	//FBC_Dword dwX[4]={0x78563412,0xF1DEBC0A,0x78563412,0xF1DEBC0A};
	//FBC_Dword dwIn[4]={0x89674523,0x78563412,0xF1DEBC0A,0x89674523};
	//FBC_Dword dwTest[4]={0xEE685D4F,0xAC5EB33E,0xABDEACED,0xAFCBAD98};
	//FBC_Dword dwOut[4]={0};

	////rijndael1.SetKey(CIPHERBASE::DECRYPTION,dwX);
	////rijndael1.ECB_Decryption(dwIn,dwOut);
	//rijndael1.SetKey(CIPHERBASE::ENCRYPTION,dwX);
	//rijndael1.ECB_Encryption(dwTest,dwOut);
}

void Test_AES()
{
    FBC_AES aesTest;
    BYTE p128bitsKey[16] = { 
        // this 128-bit key is from fips-197 A.1
        0x2b, 0x7e, 0x15, 0x16, 
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 
        0x09, 0xcf, 0x4f, 0x3c 
    };
    FBC_Byte p192bitsKey[24] = {
        // this 192-bit key is from fips-197 A.2
        0x8e, 0x73, 0xb0, 0xf7,
        0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b,
        0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2,
        0x52, 0x2c, 0x6b, 0x7b
    };
    FBC_Byte p256bitsKey[32] = {
        // this 256-bit key is from fips-197 A.3
        0x60, 0x3d, 0xeb, 0x10,
        0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 
        0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07,
        0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3,
        0x09, 0x14, 0xdf, 0xf4
    };
    FBC_Byte p256bitsKeyWinHex[32] = {
        0xD8, 0x62, 0x7D, 0xC8, 
        0x08, 0x30, 0x21, 0x52, 
        0xC4, 0xC2, 0x01, 0x57, 
        0xA7, 0x3D, 0xE0, 0xFB, 
        0xE7, 0xAE, 0xD2, 0xC4, 
        0xC4, 0x8C, 0x91, 0x4B, 
        0x30, 0x91, 0xF4, 0x10, 
        0x00, 0x79, 0xDF, 0x66
    };
	//fbyte plain[16] = {
	//	0x32, 0x43, 0xf6, 0xa8,
	//	0x88, 0x5a, 0x30, 0x8d,
	//	0x31, 0x31, 0x98, 0xa2,
	//	0xe0, 0x37, 0x07, 0x34
	//};
    fbyte plain[16] = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff
    };
	fbyte cipher[16] = { 0 };
	int cbLen = 16;
    fbyte ptemp[16] = { 0 };
    fbyte paes128key[16] = {
        // this 128 bit keys is from fips-197 C.1
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f
    };
    
    //aesTest.SetKey( (char*)p128bitsKey, enumKeyBits_128 );

    //aesTest.SetKey( (char*)p192bitsKey, enumKeyBits_192 );

    //aesTest.SetKey( (char*)p256bitsKey, enumKeyBits_256 );

    //aesTest.SetKey( (char*)p256bitsKeyWinHex, enumKeyBits_256 );
    
    aesTest.SetKey( (char*)paes128key, enumKeyBits_128 );

	aesTest.AES_ECB_Encryption( plain, 16, cipher, &cbLen );
    aesTest.AES_ECB_Decryption( cipher, 16, ptemp, &cbLen );
}

void Test_Para()
{
	BYTE bString1[]={0x23,0x54,0xF3,0x32,0x70,0x01,0xA4};

	MD5 md5test;
	FBC_Byte bin[]="Cnbragon";
	md5test.MD5Update(bString1,7);
	md5test.MD5Update(bin,8);

	
	FBC_Byte* bhash=new BYTE[MD5::DigestSize];
	md5test.MD5Final(bhash);

	MODIFYBLOWFISH mblowfishtest;
	FBC_Byte bplain[]="BLOWFISH";
	FBC_Dword dwin[2],dwout[2],plaintext[2];
	
	dwin[0]=0x424C4F57;
	dwin[1]=0x46495348;
	
    mblowfishtest.SetKey(CIPHERBASE::ENCRYPTION,bhash,16);
	mblowfishtest.ECB_Encryption(dwin,dwout);
	mblowfishtest.SetKey(CIPHERBASE::DECRYPTION,bhash,16);
	mblowfishtest.ECB_Encryption(dwout,plaintext);
}

void Test_RC4()
{
	FBC_Byte RC4_Key[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	FBC_Byte iN[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	RC4 rc4test;

	rc4test.RC4_Initial((FBC_Byte*)RC4_Key,8);
	rc4test.RC4_Crypt((FBC_Byte*)iN,8);

	RC4 rc4test2;
	rc4test2.RC4_Initial((FBC_Byte*)RC4_Key,8);
	rc4test2.RC4_Crypt((FBC_Byte*)iN,8);
}

void Test_GF256()
{
	GF256::Element a=0x57;
	GF256::Element b=0x83;
	GF256::Element c=0x13;
	GF256::Element result;
	GF256 gf256test(0x1B);

	result=gf256test.GF256_Add(a,b);
	result=gf256test.GF256_Multiply(a,b);
	result=gf256test.GF256_Multiply(a,c);
	result=gf256test.GF256_MultiplicativeInverse(0xAF);
	result=gf256test.GF256_MultiplicativeInverse(result);
}

#define GF 256
#define PP 283

void FillLogArrays() /* make Log[] and antiLog[] tables */
{
	int i;
	int Log[256],antiLog[256];
	
	Log[0]=1-GF;
	antiLog[0]=1;
	for(i=1;i<GF;i++)
	{
		antiLog[i]=antiLog[i-1]*2;
		if(antiLog[i]>=GF)antiLog[i]^=PP;
		Log[antiLog[i]]=i;
	}
}

void Twofish_test()
{
	TWOFISH twofishtest;
	//FBC_Byte twofishkey[]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10,0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
	FBC_Byte twofishkey[16];
	FBC_Byte twoin[16],twoout[16];
	int i;

	memset(twofishkey,0,16);
	memset(twoin,0,16);
	memset(twoout,0,16);

	for(i=0;i<49;i++)
	{
		memcpy(twofishkey,twoin,16);
		memcpy(twoin,twoout,16);
		twofishtest.SetKey(twofishkey,128);
		twofishtest.ECB_Encryption(twoin,twoout);	
	}
	//twofishtest.ECB_Decryption(twoout,twoin);
}

void IDEATest()
{
	FBC_Byte ideakey[]={0x00,0x01,0x00,0x02,0x00,0x03,0x00,0x04,0x00,0x05,0x00,0x06,0x00,0x07,0x00,0x08};
	//FBC_Byte ideakey[]="1234567890ABCDEF";
	FBC_Word ideain[4]={0x0000,0x0001,0x0002,0x0003};
	FBC_Word ideaout[4];
	IDEA ideatest;

	ideatest.SetKey(CIPHERBASE::ENCRYPTION,ideakey);
	ideatest.ECB_Encryption(ideain,ideaout);
	// if not sure about the cipherdir ,use SetKey to set it
	ideatest.SetKey(CIPHERBASE::DECRYPTION,ideakey);
	ideatest.ECB_Encryption(ideaout,ideain);
}

void Test_Base32()
{
	CBASE32 base32Inst;

	char* scSource1 = "f";
	char* scSource2 = "fo";
	char* scSource3 = "foo";
	char* scSource4 = "foob";
	char* scSource5 = "fooba";
	char* scSource6 = "foobar";
	char* scDst = NULL;
	int nDstLen = 0;
	int nLen2 = 0;
	bool bRet = FALSE;
	char* scDecode = NULL;
	char* szTest = "AAATEA3EAYDARCAKBJFT3DJRB7ZW09XY8NAPZWW3";

	bRet = base32Inst.Base32Encode(scSource1, 
								   strlen_FBC(scSource1),
								   NULL,
								   &nDstLen
								  );
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource1,
								   strlen_FBC(scSource1),
								   scDst,
								   &nDstLen
								  );
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;

	bRet = base32Inst.Base32Encode(scSource2, 
		strlen_FBC(scSource2),
		NULL,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource2,
		strlen_FBC(scSource2),
		scDst,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;

	bRet = base32Inst.Base32Encode(scSource3, 
		strlen_FBC(scSource3),
		NULL,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource3,
		strlen_FBC(scSource3),
		scDst,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;

	bRet = base32Inst.Base32Encode(scSource4, 
		strlen_FBC(scSource4),
		NULL,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource4,
		strlen_FBC(scSource4),
		scDst,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;

	bRet = base32Inst.Base32Encode(scSource5, 
		strlen_FBC(scSource5),
		NULL,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource5,
		strlen_FBC(scSource5),
		scDst,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;

	bRet = base32Inst.Base32Encode(scSource6, 
		strlen_FBC(scSource6),
		NULL,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	
	scDst = new char[ nDstLen + 1 ];
	FBC_PROCESS_POINTER(scDst);
	memset_FBC(scDst, 0, nDstLen + 1);
	bRet = base32Inst.Base32Encode(scSource6,
		strlen_FBC(scSource6),
		scDst,
		&nDstLen
		);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDst << endl;
	delete[] scDst;
	
	bRet = base32Inst.Base32Decode( szTest, 40, NULL, &nLen2 );
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	scDecode = new char[ nLen2 ];
	FBC_PROCESS_POINTER(scDecode);
	memset(scDecode, 0, nLen2);
	bRet = base32Inst.Base32Decode(szTest, 40, scDecode, &nLen2);
	if ( bRet == FALSE )
	{
		goto Exit0;
	}
	std::cout << scDecode << endl;
	
	base32Inst.Base32Encode(scDecode, nLen2, NULL, &nDstLen);
	scDst = new char[nDstLen + 1];
	memset(scDst, 0, nDstLen + 1);
	base32Inst.Base32Encode(scDecode, nLen2, scDst, &nDstLen);
	std::cout << scDst << endl;
	delete[] scDecode;
	delete[] scDst;
	return;
Exit0:
	if ( scDecode )
	{
		delete[] scDecode;
	}
	if ( scDst )
	{
		delete[] scDst;
	}
	return;
}

void Test_Blowfish()
{
	BYTE szKey2[] = {
		0x5F, 0xB0, 0x45, 0xA2, 0x94, 0x17, 0xD9, 0x16, 
		0xC6, 0xC6, 0xA2, 0xFF, 0x06, 0x41, 0x82, 0xB7
	};
	FBC_Byte szKey1[] = {
		0x24, 0xA6, 0x3D, 0xDE, 0x5B, 0xD3, 0xB3, 0x82, 
		0x9C, 0x7E, 0x06, 0xF4, 0x08, 0x16, 0xAA, 0x07 
	};
	FBC_Byte szPlain[] = {
		0xc2, 0xda, 0xd2, 0x74, 0xdf, 0x29, 0x01, 0x20,
		0xce, 0x33, 0x68, 0x6f, 0x2b, 0xab, 0x1b, 0x08,
		0xd5, 0xd1, 0xa8, 0x1d, 0x0d, 0x80, 0x7c, 0x65
	};
	FBC_Byte szTemp[25] = { 0 };
	char szIV[ 8 ] = { 0 };
	int nLen = 25;
	BLOWFISH bfInst1;
	BLOWFISH bfInst2;

	bfInst1.SetKey(CIPHERBASE::DECRYPTION, szKey1, 16);
	bfInst1.SetIV(szIV, 8);
	bfInst1.CBC_Decryption2((char*)szPlain, 24, (char*)szTemp, &nLen);

	nLen -= 8;
	bfInst2.SetKey(CIPHERBASE::DECRYPTION, szKey2, 16);
	bfInst2.SetIV(szIV, 8);
	bfInst2.CBC_Decryption2((char*)(szTemp + 4), nLen, (char*)szTemp, &nLen);
}

void Test_RC5()
{
	RC5 rc5Inst;
	FBC_Dword dwKey[2] = { 0, 0};
	FBC_Dword dwPlain[2] = {0x019EED82, 0x26BAF7F3};
	FBC_Dword dwCipher[2] = { 0, 0 };
	FBC_Dword dwTemp[2] = { 0, 0 };

	rc5Inst.SetKey(dwKey, 2);
	rc5Inst.ECB_Decryption(dwPlain, 2, dwCipher, 2);
	rc5Inst.ECB_Encryption(dwCipher, 2, dwTemp, 2);
}

void Test_SHA256()
{
	FBC_CLASS_SHA256 sha256Inst;
	char* szMessage = "abc";
	char* szMessage2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	FBC_Byte szHash[32] = { 0 };

	sha256Inst.Update( (FBC_Byte*)szMessage2, strlen_FBC(szMessage2) );
	sha256Inst.Final(szHash, 32);
}

void main()
{
	//Test_GF256();
	//FillLogArrays();
	//Twofish_test();
	//IDEATest();
	//Test_RC4();
	//Test_Para();
	//Test_Rijndael();
	//Test_Base64();
	//Test_SHA1();
	////Test_CRC32();
	//Test_Base32();
	//Test_Blowfish();
	//Test_RC5();
	//Test_SHA256();
    Test_AES();
}