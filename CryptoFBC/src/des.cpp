#include "des.h"

NAMESPACE_BEGIN(CryptoFBC)

const FBC_Byte DES::IP_table[64]={
	    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const FBC_Byte DES::IP_inverse_table[64]={
	    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};

const FBC_Byte DES::E_bstable[48]={
	    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
		8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};

const FBC_Byte DES::Des_SBOX[8][64]={
	    // S1 
	    14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
		0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
		4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
		15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
		// S2 
		15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
		3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
		0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
		13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
		// S3 
		10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
		13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
		13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
		1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
		// S4 
		7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
		13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
		10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
		// S5 
		2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
		4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
		// S6 
		12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
		9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
		// S7 
		4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
		1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
		// S8 
		13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
		7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

const FBC_Byte DES::P_table[32]={
	    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
		2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};

const FBC_Byte DES::PC1_table[56]={
    	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
		10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
		14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};

const FBC_Byte DES::PC2_table[48]={
	    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const FBC_Byte DES::Loop_Table[16]={
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

const int DES::ROUNDS=16;
const int DES::BLOCKSIZE=8;

void DES::Transform(bool* Out, bool* In, const FBC_Byte* Table, int tablelen) const
{
	bool Tmp[256];

    for(int i=0; i<tablelen; ++i)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy_FBC(Out, Tmp, tablelen);
}

inline void DES::RotateL(bool* In,int inLen,int nShift) const
{
	bool Tmp[256];

    memcpy_FBC(Tmp, In, nShift);
    memcpy_FBC(In, In+nShift, inLen-nShift);
    memcpy_FBC(In+inLen-nShift, Tmp, nShift);
}

void DES::SetKey(CipherDir dir,const char keystring[8])
{
	bool K[64];
	bool *KL=&K[0], *KR=&K[28];

	BytesToBits(K,keystring,64);
	Transform(K,K,PC1_table,56);
	for(int i=0;i<ROUNDS;i++)
	{
		RotateL(KL,28,Loop_Table[i]);
		RotateL(KR,28,Loop_Table[i]);
		Transform((bool*)(&DesSubKey[i]),K,PC2_table,48);
	}
	
	if(dir == DECRYPTION)
	{
		bool temp[48];
		for(int i=0;i<ROUNDS/2;i++)
		{
			memcpy_FBC(temp,DesSubKey[i],48);
			memcpy_FBC(DesSubKey[i],DesSubKey[ROUNDS-i-1],48);
			memcpy_FBC(DesSubKey[ROUNDS-i-1],temp,48);
		}
	}
}

void DES::S_func(bool Out[32], const bool In[48]) const
{
    for(char i=0,j,k; i<8; ++i,In+=6,Out+=4) 
	{
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
		
		for(int l=0; l<4; ++l)
			Out[l] = (Des_SBOX[i][j*16+k]>>(3 - l)) & 1;
    }
}

void DES::Xor(bool* InA, const bool* InB, int inLen) const
{
    for(int i=0; i<inLen; ++i)
        InA[i] ^= InB[i];
}

void DES::F_func(bool In[32], const bool Ki[48]) const
{
    bool MR[48];
    Transform(MR, In, E_bstable, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_table, 32);
}

void DES::ECB_Encryption(const char in[8],char out[8]) const
{
	bool M[64],tmp[32], *Li=&M[0], *Ri=&M[32];

	BytesToBits(M,in,64);
	Transform(M,M,IP_table,64);
	for(int i=0;i<ROUNDS;i++)
	{
		memcpy_FBC(tmp, Ri, 32);		
        F_func(Ri, DesSubKey[i]);	
        Xor(Ri, Li, 32);			
        memcpy_FBC(Li, tmp, 32);
	}
	RotateL(M,64,32);
	Transform(M,M,IP_inverse_table,64);
	BitsToBytes(out,M,64);
}

NAMESPACE_END