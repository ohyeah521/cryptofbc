#include "twofish.h"

NAMESPACE_BEGIN(CryptoFBC)

const int TWOFISH::BLOCKSIZE=128;
const int TWOFISH::ROUNDS=16;
const int TWOFISH::rho=0x01010101;
const int TWOFISH::DEFAULT_KEYLENGTH=128;
FBC_Word TWOFISH::KEYLENGTH=128;

const FBC_Byte TWOFISH::RS_Matrix[4][8]={
	{0x01,0xA4,0x55,0x87,0x5A,0x58,0xDB,0x9E},
	{0xA4,0x56,0x82,0xF3,0x1E,0xC6,0x68,0xE5},
	{0x02,0xA1,0xFC,0xC1,0x47,0xAE,0x3D,0x19},
	{0xA4,0x55,0x87,0x5A,0x58,0xDB,0x9E,0x03}
};

const FBC_Byte TWOFISH::MDS_Matrix[4][4]={
		{0x01,0xEF,0x5B,0x5B},
		{0x5B,0xEF,0xEF,0x01},
		{0xEF,0x5B,0x01,0xEF},
		{0xEF,0x01,0xEF,0x5B}
	};
FBC_Dword TWOFISH::Twofish_SubKey[40]={0};
FBC_Dword TWOFISH::Twofish_SboxKey[4]={0};
FBC_Dword TWOFISH::F[2]={0};

#define ROR4(X)  (((X)>>1)|((((X)<<3)&0x0F)))

FBC_Byte TWOFISH::Twofish_Q0(FBC_Byte X) const
{
	FBC_Byte A[5],B[5];
	FBC_Byte T0[]={8,1,7,13,6,15,3,2,0,11,5,9,14,12,10,4};
	FBC_Byte T1[]={14,12,11,8,1,2,3,5,15,4,10,6,7,0,9,13};
	FBC_Byte T2[]={11,10,5,14,6,13,9,0,12,8,15,3,2,4,7,1};
	FBC_Byte T3[]={13,7,15,4,1,2,6,14,9,11,3,0,8,5,12,10};
	FBC_Byte Y;

	memset_FBC(A,0,sizeof(A));
	memset_FBC(B,0,sizeof(B));

	A[0]=X/16;
	B[0]=X%16;

	A[1]=A[0]^B[0];
	B[1]=(A[0]^ROR4(B[0])^(A[0]<<3))%16;

	A[2]=T0[A[1]];
	B[2]=T1[B[1]];

	A[3]=A[2]^B[2];
	B[3]=(A[2]^ROR4(B[2])^(A[2]<<3))%16;

	A[4]=T2[A[3]];
	B[4]=T3[B[3]];

	Y=16*B[4]+A[4];

	return Y;
}

FBC_Byte TWOFISH::Twofish_Q1(FBC_Byte X) const
{
	FBC_Byte A[5],B[5];
	FBC_Byte T0[]={2,8,11,13,15,7,6,14,3,1,9,4,0,10,12,5};
	FBC_Byte T1[]={1,14,2,11,4,12,3,7,6,13,10,5,15,9,0,8};
	FBC_Byte T2[]={4,12,7,5,1,6,9,10,0,14,13,8,2,11,3,15};
	FBC_Byte T3[]={11,9,5,1,12,3,13,14,6,4,7,15,2,0,8,10};
	FBC_Byte Y;

	memset_FBC(A,0,sizeof(A));
	memset_FBC(B,0,sizeof(B));

	A[0]=X/16;
	B[0]=X%16;

	A[1]=A[0]^B[0];
	B[1]=(A[0]^ROR4(B[0])^(A[0]<<3))%16;

	A[2]=T0[A[1]];
	B[2]=T1[B[1]];

	A[3]=A[2]^B[2];
	B[3]=(A[2]^ROR4(B[2])^(A[2]<<3))%16;

	A[4]=T2[A[3]];
	B[4]=T3[B[3]];

	Y=16*B[4]+A[4];

	return Y;
}

FBC_Dword TWOFISH::Twofish_h(FBC_Dword X,FBC_Dword* L) const
{
	FBC_Byte bX[4],y[4],Z[4],bL[4][4];
	FBC_Dword dwZ=0;
	int i,j;
	int k=KEYLENGTH/64;

	memset_FBC(bX,0,sizeof(bX));
	memset_FBC(bL,0,sizeof(bL));
	memset_FBC(y,0,sizeof(4));
	memset_FBC(Z,0,sizeof(Z));

	for(i=0;i<4;i++)
	{
		bX[i]=FBC_Byte(X>>(8*i));
		y[i]=bX[i];
	}
	for(j=0;j<k;j++)
	{
		for(i=0;i<4;i++)
		{
			bL[j][i]=FBC_Byte(L[j]>>(8*i));
		}
	}

	switch(k)
	{
	case 4:
		y[0]=Twofish_Q1(y[0])^bL[3][0];
		y[1]=Twofish_Q0(y[1])^bL[3][1];
		y[2]=Twofish_Q0(y[2])^bL[3][2];
		y[3]=Twofish_Q1(y[3])^bL[3][3];
	case 3:
		y[0]=Twofish_Q1(y[0])^bL[2][0];
		y[1]=Twofish_Q1(y[1])^bL[2][1];
		y[2]=Twofish_Q0(y[2])^bL[2][2];
		y[3]=Twofish_Q0(y[3])^bL[2][3];
	case 2:
		y[0]=Twofish_Q0(y[0])^bL[1][0];
		y[1]=Twofish_Q1(y[1])^bL[1][1];
		y[2]=Twofish_Q0(y[2])^bL[1][2];
		y[3]=Twofish_Q1(y[3])^bL[1][3];

		y[0]=Twofish_Q0(y[0])^bL[0][0];
		y[1]=Twofish_Q0(y[1])^bL[0][1];
		y[2]=Twofish_Q1(y[2])^bL[0][2];
		y[3]=Twofish_Q1(y[3])^bL[0][3];

		y[0]=Twofish_Q1(y[0]);
		y[1]=Twofish_Q0(y[1]);
		y[2]=Twofish_Q1(y[2]);
		y[3]=Twofish_Q0(y[3]);
	}

	/* The resulting vector of y[i] is multiplied by the
	 * MDS matrix, just as in the g function
	 * the modulo 
	 * polynomial : v(x)=x^8+x^6+x^5+x^3+1
	 * binary     : 1 0110 1001
	 * hexadecimal: 0x169
	 * The MDS matrix is (using primitive polynomial 169):
     *      01  EF  5B  5B
     *      5B  EF  EF  01
     *      EF  5B  01  EF
     *      EF  01  EF  5B
	 */
	GF256 GF_MDS(0x169);

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Z[i]^=GF_MDS.GF256_Multiply(MDS_Matrix[i][j],y[j]);
		}
		dwZ|=(Z[i]<<(8*i));
	}
	return dwZ;
}

void TWOFISH::SetKey(const FBC_Byte* KeyMaterial,FBC_Word keyLenOfBits)
{
	int i,j; 
	FBC_Dword M[8];
	FBC_Dword Me[4],Mo[4];
	FBC_Byte Si[4];
	GF256 GF_RS(0x4D);
	
	memset_FBC(M,0,sizeof(M));
	memset_FBC(Me,0,sizeof(Me));
	memset_FBC(Mo,0,sizeof(Mo));
	memset_FBC(Twofish_SboxKey,0,sizeof(Twofish_SboxKey));
	memset_FBC(Si,0,sizeof(Si));

	KEYLENGTH=keyLenOfBits;
	int k=keyLenOfBits/64;
	for(i=0;i<2*k;i++)
	{
		for(j=0;j<4;j++)
		{
			M[i]|=KeyMaterial[4*i+j]<<(8*j);
		}
	}

	/* two word vectors of length k.
	 * Me = (M0;M2; : : :;M2k..2)
     * Mo = (M1;M3; : : :;M2k..1)
     */
	for(i=0;i<k;i++)
	{
		Me[i]=M[2*i];
		Mo[i]=M[2*i+1];
	}
	/* Matrix Multiplication with Reed-Solomon 4x8 Matrix
	 * the modulo 
	 * polynomial : w(x)=x^8+x^6+x^3+x^2+1
	 * binary     : 1 0100 1101
	 * hexadecimal: 0x14D
	 * the RS Matrix is:
	 *       01 A4 55 87 5A 58 DB 9E
	 *       A4 56 82 F3 1E C6 68 E5
	 *       02 A1 FC C1 47 AE 3D 19
	 *       A4 55 87 5A 58 DB 9E 03
	 */
	for(j=0;j<k;j++)
	{
		for(int t=0;t<4;t++)
		{
			Si[t]=0; /* This is important */
			for(i=0;i<8;i++)
			{
				Si[t]^=(GF_RS.GF256_Multiply(RS_Matrix[t][i],KeyMaterial[8*j+i]));
			}
			Twofish_SboxKey[k-j-1]|=Si[t]<<(8*t); /* form third vector of length k */
		}
		
	}

	/* The Expanded Key Words K[j] */
	FBC_Dword A[20],B[20];

	memset_FBC(A,0,sizeof(A));
	memset_FBC(B,0,sizeof(B));
	memset_FBC(Twofish_SubKey,0,sizeof(Twofish_SubKey));

	for(i=0;i<20;i++)
	{
		A[i]=Twofish_h(FBC_Dword(2*i*rho),Me);
		B[i]=Twofish_h(FBC_Dword((2*i+1)*rho),Mo);
		B[i]=ROL(B[i],8);
		Twofish_SubKey[2*i]=(A[i]+B[i]);
		Twofish_SubKey[2*i+1]=ROL((A[i]+2*B[i]),9);
	}
}

void TWOFISH::Twofish_F(FBC_Dword R0,FBC_Dword R1,int r)
{
	FBC_Dword T[2];

	T[0]=Twofish_h(R0,Twofish_SboxKey);
	T[1]=Twofish_h(ROL(R1,8),Twofish_SboxKey);
	F[0]=(T[0]+T[1]+Twofish_SubKey[2*r+8]);
	F[1]=(T[0]+2*T[1]+Twofish_SubKey[2*r+9]);
}

void TWOFISH::ECB_Encryption(const FBC_Byte* in,FBC_Byte* out)
{
	int i,j;
	FBC_Dword Pi[4];
	FBC_Dword Ci[4];

	memset_FBC(Pi,0,sizeof(Pi));
	memset_FBC(Ci,0,sizeof(Ci));

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Pi[i]|=in[4*i+j]<<(8*j);
		}
	}

	FBC_Dword X[4],temp=0;
	for(i=0;i<4;i++)
	{
		X[i]=(Pi[i]^Twofish_SubKey[i]);
	}
	
	for(i=0;i<16;i++)
	{
		Twofish_F(X[0],X[1],i);
		X[2]=ROR((X[2]^F[0]),1);
		X[3]=ROL(X[3],1)^F[1];
		if (i<15)
		{
			temp=X[2];X[2]=X[0];X[0]=temp;
			temp=X[1];X[1]=X[3];X[3]=temp;
		}
		
	}

	for(i=0;i<4;i++)
	{
		Ci[i]=X[i]^Twofish_SubKey[i+4];
	}

	for(i=0;i<16;i++)
	{
		out[i]=FBC_Byte(Ci[i/4]>>(8*(i%4)));
	}
}

void TWOFISH::ECB_Decryption(const FBC_Byte* in,FBC_Byte* out)
{
	int i,j;
	FBC_Dword Pi[4];
	FBC_Dword Ci[4];

	memset_FBC(Pi,0,sizeof(Pi));
	memset_FBC(Ci,0,sizeof(Ci));

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Pi[i]|=in[4*i+j]<<(8*j);
		}
	}

	FBC_Dword X[4],temp=0;
	
	for(i=0;i<4;i++)
	{
		X[i]=(Pi[i]^Twofish_SubKey[i+4]);
	}
	for(i=15;i>=0;i--)
	{
		Twofish_F(X[0],X[1],i);
		X[2]=ROL(X[2],1)^F[0];
		X[3]^=F[1];
		X[3]=ROR(X[3],1);
		if(i)
		{
			temp=X[2];X[2]=X[0];X[0]=temp;
			temp=X[1];X[1]=X[3];X[3]=temp;
		}
	}
	for(i=0;i<4;i++)
	{
		Ci[i]=X[i]^Twofish_SubKey[i];
	}
	
	for(i=0;i<16;i++)
	{
		out[i]=FBC_Byte(Ci[i/4]>>(8*(i%4)));
	}
}

NAMESPACE_END

