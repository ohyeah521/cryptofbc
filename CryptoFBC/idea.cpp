#include "idea.h"

NAMESPACE_BEGIN(CryptoFBC)

const int IDEA::BLOCKSIZE=64;
const int IDEA::DEFAULT_KEYLENGTH=128;
const int IDEA::ROUNDS=8;
bool IDEA::inited=FALSE;

FBC_Word IDEA::E_SubKey[9][6]={0};
FBC_Word IDEA::D_SubKey[9][6]={0};

#define one 65535
#define modulu 65536
#define maxim 65537

FBC_Word IDEA::IDEA_Mul(FBC_Word A,FBC_Word B) const
{
	long int p;
	long unsigned q;
	if(A==0) p=maxim-B;
	else
	if(B==0) p=maxim-A;
	else 
	{
		q=(unsigned long)A*(unsigned long)B;
		p=(q & one) - (q>>16); 
		if(p<=0) p=p+maxim;
	}
	return (unsigned)(p&one);
}

FBC_Word IDEA::IDEA_Inv(FBC_Word XiN) const
{
	long n1,n2,q,r,b1,b2,t;
	if(XiN==0) b2=0;
	else
	{ n1=maxim; n2=XiN; b2=1; b1=0;
		do { r = (n1 % n2); q = (n1-r)/n2;
			 if(r==0) { if(b2<0) b2=maxim+b2; }
			 else { n1=n2; n2=r; t=b2; b2=b1-q*b2; b1=t; }
		   } while (r!=0);
	}
	return (unsigned)b2;
}

void IDEA::SetKey(CipherDir dir,const FBC_Byte* UserKey)
{
	int i;
	int r = 0;
	FBC_Word Encryption_Key[54]={0};

	memset_FBC(Encryption_Key,0,sizeof(Encryption_Key));
	f_dir=dir;
	if(inited==FALSE) /* generate Encryption Key 1 time */
	{
		for(i=0;i<8;i++)
		{
			Encryption_Key[i]=(FBC_Word(UserKey[2*i]<<8)|FBC_Word(UserKey[2*i+1]));
		}
		
		for(;i<54;i++)
		{
			if((i+2)%8==0)			/* for S[14],S[22],... */
			{
				Encryption_Key[i]=((Encryption_Key[i-7]<<9)^(Encryption_Key[i-14]>>7))&one;
			}
			else if((i+1)%8==0)			/* for S[15],S[23],... */
			{
				Encryption_Key[i]=((Encryption_Key[i-15]<<9)^(Encryption_Key[i-14]>>7))&one;
			}
			else
			{
				Encryption_Key[i]=((Encryption_Key[i-7]<<9)^(Encryption_Key[i-6]>>7))&one;
			}
			//cout<<Encryption_Key[i]<<endl;
		}
		for(int r=0;r<ROUNDS+1;r++)
		{
			//cout<<r<<"-th round   ";
			for(i=0;i<6;i++)
			{
				E_SubKey[r][i]=Encryption_Key[6*r+i];
				//cout<<E_SubKey[r][i]<<"      ";
			}
			//cout<<endl;
		}
		inited=TRUE;
	}
	if(dir==DECRYPTION) //compute decryption subkeys
	{
		for(r=0;r<ROUNDS+1;r++)
		{
			D_SubKey[ROUNDS-r][0]=IDEA_Inv(E_SubKey[r][0]);
			D_SubKey[ROUNDS-r][3]=IDEA_Inv(E_SubKey[r][3]);
			if((r==0)||(r==ROUNDS))
			{
				D_SubKey[ROUNDS-r][1]=(modulu-E_SubKey[r][1])&one;
				D_SubKey[ROUNDS-r][2]=(modulu-E_SubKey[r][2])&one;
			}
			else
			{
				D_SubKey[ROUNDS-r][1]=(modulu-E_SubKey[r][2])&one;
				D_SubKey[ROUNDS-r][2]=(modulu-E_SubKey[r][1])&one;
			}
			/*for(i=0;i<4;i++)
			{
				cout<<D_SubKey[ROUNDS-r][i]<<"   ";
			}
			cout<<endl;*/
		}
		for(r=0;r<ROUNDS;r++)
		{
			D_SubKey[ROUNDS-r-1][4]=E_SubKey[r][4];
			D_SubKey[ROUNDS-r-1][5]=E_SubKey[r][5];
			//cout<<D_SubKey[ROUNDS-r-1][4]<<"  ";
			//cout<<D_SubKey[ROUNDS-r-1][5]<<"  ";
		}
		//cout<<endl;
	}
}

void IDEA::ECB_Encryption(FBC_Word iN[4],FBC_Word OuT[4])
{
	register FBC_Word X0,X1,X2,X3;
	FBC_Word SubKeys[9][6];
	FBC_Word wK,wT1,wT2,A;

	// if not sure about the cipher dir,use SetKey to Set the Dir
	if(f_dir==ENCRYPTION)
	{
		memcpy_FBC(SubKeys,E_SubKey,sizeof(E_SubKey));
	}
	else
	{
		memcpy_FBC(SubKeys,D_SubKey,sizeof(E_SubKey));
	}

	X0=iN[0];
	X1=iN[1];
	X2=iN[2];
	X3=iN[3];

	//an iterated cipher consisting of 8 rounds
	for(int r=0;r<ROUNDS;r++)
	{
		// the group operation on 64-bits block
		X0=IDEA_Mul(X0,SubKeys[r][0]);
		X1=(X1+SubKeys[r][1])&one;
		X2=(X2+SubKeys[r][2])&one;
		X3=IDEA_Mul(X3,SubKeys[r][3]);
		// the function of the MA structure
		wK=IDEA_Mul(SubKeys[r][4],(X0^X2));
		wT1=IDEA_Mul(SubKeys[r][5],((wK+(X1^X3))&one));
		wT2=(wK+wT1)&one;
		// the involuntary permutation PI
		X0^=wT1;
		A=X1^wT2;
		X1=X2^wT1;
		X2=A;
		X3^=wT2;
		//cout<<X0<<" "<<X1<<" "<<X2<<" "<<X3<<endl;
	}

	// the output transformation
	OuT[0]=IDEA_Mul(X0,SubKeys[ROUNDS][0]);
	OuT[3]=IDEA_Mul(X3,SubKeys[ROUNDS][3]);
	OuT[1]=(X2+SubKeys[ROUNDS][1])&one;
	OuT[2]=(X1+SubKeys[ROUNDS][2])&one;
}

NAMESPACE_END