/* C - program of block cipher IDEA */

#include <stdio.h>
#define maxim 	65537
#define fuyi	65536
#define one 	65535
#define round	8	

void cip(unsigned IN[5],unsigned OUT[5], unsigned Z[7][10]);
void key(short unsigned uskey[9], unsigned Z[7][10]);
void de_key(unsigned Z[7][10], unsigned DK[7][10]);
unsigned inv(unsigned xin);
unsigned mul(unsigned a, unsigned b);

void main() 
{
	unsigned Z[7][10], DK[7][10], XX[5],YY[5];
	short unsigned uskey[9];
	char format[]="%c%5u-%5u-%5u-%5u";
	unsigned long s1,s2,s3,s4,temp1,temp2;
    char machinecode[12];
	char sn1;
	char tempsn[25];
	printf("Your MachineCode is : \t");
	scanf("%s",machinecode);
	s1=temp1=temp2=s2=s3=s4=0;
	sn1=machinecode[0];
	sscanf((machinecode+1),"%u-%u",&temp1,&temp2);
	temp1*=0x186A0;
	temp1+=temp2;
	temp1^=0x55AA55AA;
	uskey[1]=0x0001;
	uskey[2]=0x0002;
	uskey[3]=0x0003;
	uskey[4]=0x0004;
	uskey[5]=0x0005;
	uskey[6]=0x0006;
	uskey[7]=0x0007;
	uskey[8]=0x0008;
	key(uskey,Z);	/* generate encryption subkeys Z[i][r] */

	de_key(Z,DK);	/* compute decryption subkeys DK[i][r] */
	
	XX[1]=(unsigned short)temp1;
	XX[2]=(unsigned short)(temp1>>0x10);
	XX[3]=0xFF04;
	XX[4]=0x0400;

	cip(XX,YY,Z);	/* encipher XX to YY with key Z */
	__asm
	{
		lea ecx,YY;
		add ecx,4;
		mov eax,dword ptr ds:[ecx];
		add ecx,4;
		mov edx,dword ptr ds:[ecx];
		shl edx,0x10;
		or eax,edx;
		mov temp1,eax;
		add ecx,4;
		mov eax,dword ptr ds:[ecx];
		add ecx,4;
		mov edx,dword ptr ds:[ecx];
		shl edx,0x10;
		or eax,edx;
		mov temp2,eax;		
	}
	s1=temp1/0x186A0;
	s2=temp1%0x186A0;
	s3=temp2/0x186A0;
	s4=temp2%0x186A0;
	sprintf(tempsn,format,sn1,s1,s2,s3,s4);
	tempsn[24]='\0';
	printf("Registration Code is :\t%s\n",tempsn);
}

	/* encrypt algorithm */
void cip(unsigned IN[5],unsigned OUT[5],unsigned Z[7][10]) 
{
	unsigned r,x1,x2,x3,x4,kk,t1,t2,a;
	x1=IN[1]; x2=IN[2]; x3=IN[3]; x4=IN[4];
	for(r=1;r<=8;r++) 			/* the round function */
	{
			/* the group operation on 64-bits block */
	x1 = mul(x1,Z[1][r]);		x4 = mul(x4,Z[4][r]);
	x2 = (x2 + Z[2][r]) & one;	x3 = (x3 + Z[3][r]) & one;
			/* the function of the MA structure */
	kk = mul(Z[5][r],(x1^x3));
	t1 = mul(Z[6][r],(kk+(x2^x4)) & one);
	t2 = (kk+t1) & one;
			/* the involutary permutation PI */
	x1 = x1^t1;		x4=x4^t2;
	a  = x2^t2;		x2=x3^t1;	x3=a;
	}

		/* the output transformation */
	OUT[1] = mul(x1,Z[1][round+1]);
	OUT[4] = mul(x4,Z[4][round+1]);
	OUT[2] = (x3+Z[2][round+1]) & one;
	OUT[3] = (x2+Z[3][round+1]) & one;
}

	/* multiplication using the Low-High algorithm */

unsigned mul(unsigned a,unsigned b) 
{
	long int p;
	long unsigned q;
		if(a==0) p=maxim-b;
		else
		if(b==0) p=maxim-a;
		else {
		q=(unsigned long)a*(unsigned long)b;
		p=(q & one) - (q>>16); 
		if(p<=0) p=p+maxim;
		}
	return (unsigned)(p&one);
}

	/* compute inverse of xin by Euclidean gcd alg. */

unsigned inv(unsigned xin)
{
	long n1,n2,q,r,b1,b2,t;
	if(xin==0) b2=0;
	else
	{ n1=maxim; n2=xin; b2=1; b1=0;
		do { r = (n1 % n2); q = (n1-r)/n2;
			 if(r==0) { if(b2<0) b2=maxim+b2; }
			 else { n1=n2; n2=r; t=b2; b2=b1-q*b2; b1=t; }
		   } while (r!=0);
	}
	return (unsigned)b2;
}

	/* generate encryption subkeys Z's */

void key(short unsigned uskey[9], unsigned Z[7][10]) 
{
	short unsigned S[54];
	int i,j,r;
	for(i=1;i<9;i++) S[i-1]=uskey[i];
		/* shifts */
	for(i=8;i<54;i++)
		{
			if((i+2)%8 == 0)			/* for S[14],S[22],... */
				S[i] = ((S[i-7]<<9) ^ (S[i-14]>>7)) & one;
			else if((i+1)%8==0)			/* for S[15],S[23],... */
				S[i] = ((S[i-15]<<9) ^ (S[i-14]>>7)) & one;
			else
				S[i] = ((S[i-7]<<9) ^ (S[i-6]>>7)) & one;
		}

	/* get subkeys */

	for(r=1;r<=round+1;r++) 
	 for(j=1;j<7;j++)
		Z[j][r]=S[6*(r-1)+j-1];
}

	/* compute decryption subkeys DK's */

void de_key(unsigned Z[7][10],unsigned DK[7][10])
{
	int j;
	for(j=1;j<=round+1;j++)
	{
		DK[1][round-j+2] = inv(Z[1][j]);
		DK[4][round-j+2] = inv(Z[4][j]);
	
		if (j==1 || j==round+1) {
			DK[2][round-j+2] = (fuyi-Z[2][j]) & one;
			DK[3][round-j+2] = (fuyi-Z[3][j]) & one;
		} else {
			DK[2][round-j+2] = (fuyi-Z[3][j]) & one;
			DK[3][round-j+2] = (fuyi-Z[2][j]) & one;
		}
	}

	for(j=1;j<=round+1;j++)
	{ DK[5][round+1-j]=Z[5][j];
	  DK[6][round+1-j]=Z[6][j];
	}
}


