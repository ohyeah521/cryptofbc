#include <tomcrypt.h>
#include <windows.h>

#pragma comment(lib,"tomcrypt.lib")


int main()
{
	int err,i;
	unsigned char ct[16];
	unsigned char key[16];
	unsigned char pt[16]; 
	symmetric_key skey;
	
	memset(key,0,16);
	memset(pt,0,16);
	memset(ct,0,16);
	
	if(err=twofish_setup(key,16,0,&skey)!=CRYPT_OK)
	{
		printf("Error setup des:%s\n",error_to_string(err));
		return -1;
	}
	twofish_ecb_encrypt(pt,ct,&skey);
	for(i=0;i<16;i++)
	{
		printf("%02X",ct[i]);
	}
	printf("\n");
	twofish_ecb_decrypt(ct,pt,&skey);
	for(i=0;i<16;i++)
	{
		printf("%02X",pt[i]);
	}
	printf("\n"); 
	return 0;
}