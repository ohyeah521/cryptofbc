#include	"aes.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<memory.h>
#include	<ctype.h>
//#include    <windows.h>
void main()
{
	cipherInstance cInst;
	keyInstance kInst;
	char material[]="31323334353637383930414243444546";
	char plaintext[16];
	char a[16];
	
	char ciphertext[16];
	
	int i,j;

	memset(plaintext,0,16);
	

	cipherInit(&cInst,MODE_ECB,NULL);
	makeKey(&kInst,DIR_ENCRYPT,128,material);
	blockEncrypt(&cInst,&kInst,plaintext,128,ciphertext);
//	makeKey(&kInst,DIR_DECRYPT,128,material);
	blockDecrypt(&cInst,&kInst,ciphertext,128,a);
	
	printf("\n");
}