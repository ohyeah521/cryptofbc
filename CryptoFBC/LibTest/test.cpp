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
#include "../rijndael.h"
#include "../base64.h"
#include "../sha1.h"
#include "../crc32.h"

using namespace std;
using namespace CryptoFBC;

void Test_CRC32()
{
	CRC32 crc32test;
	FBC_Dword dwX = 0;

	dwX = crc32test.CalculateCRC32("123456", lstrlen("123456"));
}

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
	Base64 b641;
		
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
	b641.Base64Encode(sztemp,18,szSn);
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
	RIJNDAEL rijndael1;
	FBC_Dword dwX[4]={0x78563412,0xF1DEBC0A,0x78563412,0xF1DEBC0A};
	FBC_Dword dwIn[4]={0x89674523,0x78563412,0xF1DEBC0A,0x89674523};
	FBC_Dword dwTest[4]={0xEE685D4F,0xAC5EB33E,0xABDEACED,0xAFCBAD98};
	FBC_Dword dwOut[4]={0};

	//rijndael1.SetKey(CIPHERBASE::DECRYPTION,dwX);
	//rijndael1.ECB_Decryption(dwIn,dwOut);
	rijndael1.SetKey(CIPHERBASE::ENCRYPTION,dwX);
	rijndael1.ECB_Encryption(dwTest,dwOut);
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

void main()
{
	BLOWFISH blowfishtest;
	
	//FBC_Byte bkey[]="abcdefghijklmnopqrstuvwxyz";
	FBC_Byte bkey[] = "DR34gg4@##$G%^&FUi)&@gfjuol7$%";
	FBC_Byte bplain[]="BLOWFISH";
	FBC_Dword dwin[2],dwout[2],plaintext[2];
	
	dwin[0]=0x424C4F57;
	dwin[1]=0x46495348;

	//DwordAndBytes dwab;
	//dwab.BytesToDword(dwin,bplain,8);
	
	blowfishtest.SetKey(CIPHERBASE::ENCRYPTION,bkey,30);
	blowfishtest.ECB_Encryption(dwin,dwout);
	blowfishtest.SetKey(CIPHERBASE::DECRYPTION,bkey,30);
	blowfishtest.ECB_Encryption(dwout,plaintext);

	MD5 md5test;
	FBC_Byte bin[]="BLOWFISHBLOWFISHBLOWFISHBLOWFISHBLOWFISHBLOWFISHBLOWFISHBLOWFISH";
	md5test.MD5Update(bin,64);
	
	FBC_Byte* bhash=new BYTE[MD5::DigestSize];
	md5test.MD5Final(bhash);
	delete []bhash;

	DES destest;
	char deskey[]="avc_oier";
	char desin[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	char desout[8];
	char desplaintext[8];

	//memset(deskey,0,8);
	//memset(desin,0,8);

	destest.SetKey(CIPHERBASE::ENCRYPTION,deskey);
	destest.ECB_Encryption(desin,desout);
	destest.SetKey(CIPHERBASE::DECRYPTION,deskey);
	destest.ECB_Encryption(desout,desplaintext);

	TEA teatest;
	FBC_Dword teain[2]={0x12345678,0x11111111};
	FBC_Dword teakey[4];
	FBC_Dword teaout[2];

	memset(teakey,0,sizeof(teakey));
	memset(teaout,0,sizeof(teaout));

	teatest.ECB_Encryption(teain,teaout,teakey);
	teatest.ECB_Decryption(teaout,teaout,teakey);

	Test_GF256();
	FillLogArrays();
	Twofish_test();
	IDEATest();
	Test_RC4();
	Test_Para();
	Test_Rijndael();
	Test_Base64();
	Test_SHA1();
	Test_CRC32();
}