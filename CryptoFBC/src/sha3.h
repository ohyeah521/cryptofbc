/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michal Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

*/
#ifndef CryptoFBC_SHA3_H
#define CryptoFBC_SHA3_H

#include "cfbconfig.h"
#include "baseroutine.h"

NAMESPACE_BEGIN(CryptoFBC)

#define KeccakPermutationSize 1600
#define KeccakPermutationSizeInBytes (KeccakPermutationSize/8)
#define KeccakMaximumRate 1536
#define KeccakMaximumRateInBytes (KeccakMaximumRate/8)

#define ProvideFast576
#define ProvideFast832
#define ProvideFast1024
#define ProvideFast1088
#define ProvideFast1152
#define ProvideFast1344

#define FIXED_WIDTH     1600

#define RATE_1344   1344
#define RATE_1152   1152
#define RATE_1088   1088
#define RATE_1024   1024
#define RATE_832    832
#define RATE_576    576

#define CAPACITY_448    448
#define CAPACITY_512    512
#define CAPACITY_576    576
#define CAPACITY_768    768
#define CAPACITY_1024   1024

#if defined(_MSC_VER)
#define ROL32(a, offset) ((((fuint)a) << (offset)) ^ (((fuint)a) >> (32-(offset))))
#elif (defined (__arm__) && defined(__ARMCC_VERSION))
#define ROL32(a, offset) __ror(a, 32-(offset))
#else
#define ROL32(a, offset) ((((fuint)a) << (offset)) ^ (((fuint)a) >> (32-(offset))))
#endif

#define Unrolling 2
//#define UseBebigokimisa
#define UseInterleaveTables
#define UseSchedule 3

class FBC_SHA3
{
public:
    
    FBC_SHA3();

    FBC_SHA3(int nHashBitLen);

    void Update(const fbyte* pData, unsigned __int64 ui64DataBitLen);

    void Final(fbyte* pHashVal);

private:

    void InitSponge(fuint nRate, fuint nCapacity);

    void KeccakInitialize();

    void BuildInterleaveTables();

    void KeccakInitializeState();

    void AbsorbQueue();

    void Absorb(const fbyte* pData, unsigned __int64 ui64DataBitLen);

    void PadAndSwitchToSqueezingPhase();

    void Squeeze(fbyte* pOutput, unsigned __int64 ui64OutputLength);

    void Xor8bytesIntoInterleavedWords(fuint *even, fuint *odd, const fbyte* source);

    void SetInterleavedWordsInto8bytes(fbyte* dest, fuint even, fuint odd);

    void KeccakPermutation();

    void KeccakPermutationOnWords(fuint* state);

    void KeccakPermutationOnWordsAfterXoring(fuint *state, 
                                             const fbyte *input, 
                                             unsigned int laneCount);

#ifdef ProvideFast576
    void KeccakAbsorb576bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring576bits(fuint *state, const fbyte *input);
#endif

#ifdef ProvideFast832
    void KeccakAbsorb832bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring832bits(fuint *state, const fbyte *input);
#endif

#ifdef ProvideFast1024
    void KeccakAbsorb1024bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring1024bits(fuint *state, const fbyte *input);
#endif

#ifdef ProvideFast1088
    void KeccakAbsorb1088bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring1088bits(fuint *state, const fbyte *input);
#endif

#ifdef ProvideFast1152
    void KeccakAbsorb1152bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring1152bits(fuint *state, const fbyte *input);
#endif

#ifdef ProvideFast1344
    void KeccakAbsorb1344bits(unsigned char *state, const unsigned char *data);
    void KeccakPermutationOnWordsAfterXoring1344bits(fuint *state, const fbyte *input);
#endif

    

#ifdef ProvideFast1024
    void KeccakExtract1024bits(const unsigned char *state, unsigned char *data);
#endif

    void KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount);

    void KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount);

private:

    fbyte m_State[KeccakPermutationSizeInBytes];
    fbyte m_DataQueue[KeccakMaximumRateInBytes];
    fuint m_nRate;
    fuint m_nCapacity;
    fuint m_nBitsInQueue;
    fuint m_nFixedOutputLength;
    int m_iSqueezing;
    fuint m_nBitsAvailableForSqueezing;
    
};

NAMESPACE_END

#endif