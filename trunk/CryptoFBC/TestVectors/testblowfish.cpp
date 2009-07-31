#include <tomcrypt.h>
#include <windows.h>

#pragma comment(lib,"tomcrypt.lib")


int main()
{
	int keysize,err,i;
	unsigned char ct[16];
	unsigned char key[]="abcdefghijklmnopqrstuvwxyz";
	unsigned char pt[8]={'B','L','O','W','F','I','S','H'}; 
	symmetric_key skey;
	
	if(err=blowfish_setup(key,26,0,&skey)!=CRYPT_OK)
	{
		printf("Error setup des:%s\n",error_to_string(err));
		return -1;
	}
	blowfish_ecb_encrypt(pt,ct,&skey);
	for(i=0;i<8;i++)
	{
		printf("%02X",ct[i]);
	}
	printf("\n");
	blowfish_ecb_decrypt(ct,pt,&skey);
	for(i=0;i<8;i++)
	{
		printf("%02X",pt[i]);
	}
	printf("\n"); 
	return 0;
}
