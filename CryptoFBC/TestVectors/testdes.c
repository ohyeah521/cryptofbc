#include <libtomcrypt/tomcrypt.h>
#include <windows.h>

#pragma comment(lib,"tomcrypt.lib")


int main()
{
	int err,i;
	unsigned char ct[8];
	//unsigned char key[]="avc_oier";
	//unsigned char pt[8]={0x0A,0xBC,0xDE,0xF1,0x23,0x45,0x67,0x89};
	unsigned char key[]="ABCDEDFH";
	unsigned char pt[8]={0x2D,0xD5,0x26,0xBC,0x45,0x33,0xDC,0xB2};
	//unsigned char pt[8]={0x31,0x48,0x57,0xD8,0x88,0x01,0xE6,0xEE};
	//unsigned char pt[]="2ABCDE36";
	symmetric_key skey;
	
	memset(ct,0,8);
	//memset(pt,0,8);
	//memset(key,0,8);
	if(err=des_setup(key,8,0,&skey)!=CRYPT_OK)
	{
		printf("Error setup des:%s\n",error_to_string(err));
		return -1;
	}
	des_ecb_encrypt(pt,ct,&skey);
	for(i=0;i<8;i++)
	{
		printf("%02X",ct[i]);
	}
	printf("\n");
	des_ecb_decrypt(ct,pt,&skey);
	for(i=0;i<8;i++)
	{
		printf("%02X",pt[i]);
	}
	printf("\n"); 
	return 0;
}
