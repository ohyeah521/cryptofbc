#include "sha3.h"

NAMESPACE_BEGIN(CryptoFBC)

static bool g_bInterleaveTablesBuilt = false;

static fword g_InterleaveTable[65536];
static fword g_DeinterleaveTable[65536];

void FBC_SHA3::InitSponge( fuint nRate, fuint nCapacity )
{
    KeccakInitialize();
    m_nRate = nRate;
    m_nCapacity = nCapacity;
    m_nFixedOutputLength = 0;
    KeccakInitializeState();
    memset_FBC(m_DataQueue, 0, KeccakMaximumRateInBytes);
    m_nBitsInQueue = 0;
    m_iSqueezing = 0;
    m_nBitsAvailableForSqueezing = 0;
}

void FBC_SHA3::BuildInterleaveTables()
{
    fuint i, j;
    fword x;

    if ( !g_bInterleaveTablesBuilt ) 
    {
        for( i = 0; i < 65536; i++ ) 
        {
            x = 0;
            for( j = 0; j < 16; j++) 
            {
                if (i & (1 << j))
                {
                    x |= (1 << (j/2 + 8*(j%2)));
                }
            }
            g_InterleaveTable[i] = x;
            g_DeinterleaveTable[x] = (fword)i;
        }
        g_bInterleaveTablesBuilt = false;
    }
}

void FBC_SHA3::KeccakInitialize()
{
    BuildInterleaveTables();
}

void FBC_SHA3::KeccakInitializeState()
{
    memset_FBC(m_State, 0, KeccakPermutationSize / 8);
}

FBC_SHA3::FBC_SHA3()
{
    InitSponge(RATE_1024, CAPACITY_576);
}

FBC_SHA3::FBC_SHA3( int nHashBitLen )
{
    switch(nHashBitLen) 
    {
        case 0: // Default parameters, arbitrary length output
            InitSponge(RATE_1024, CAPACITY_576);
            break;
        case 224:
            InitSponge(RATE_1152, CAPACITY_448);
            break;
        case 256:
            InitSponge(RATE_1088, CAPACITY_512);
            break;
        case 384:
            InitSponge(RATE_832, CAPACITY_768);
            break;
        case 512:
            InitSponge(RATE_576, CAPACITY_1024);
            break;
    }
    m_nFixedOutputLength = nHashBitLen;
}

void FBC_SHA3::AbsorbQueue()
{
#ifdef ProvideFast576
    if ( m_nRate == RATE_576 )
        KeccakAbsorb576bits(m_State, m_DataQueue);
    else 
#endif
#ifdef ProvideFast832
    if ( m_nRate == RATE_832 )
        KeccakAbsorb832bits(m_State, m_DataQueue);
    else 
#endif
#ifdef ProvideFast1024
    if ( m_nRate == RATE_1024 )
        KeccakAbsorb1024bits(m_State, m_DataQueue);
    else 
#endif
#ifdef ProvideFast1088
    if ( m_nRate == RATE_1088 )
        KeccakAbsorb1088bits(m_State, m_DataQueue);
    else
#endif
#ifdef ProvideFast1152
    if ( m_nRate == RATE_1152 )
        KeccakAbsorb1152bits(m_State, m_DataQueue);
    else 
#endif
#ifdef ProvideFast1344
    if ( m_nRate == RATE_1344 )
        KeccakAbsorb1344bits(m_State, m_DataQueue);
    else 
#endif
    KeccakAbsorb(m_State, m_DataQueue, m_nRate/64);
    m_nBitsInQueue = 0;
}

void FBC_SHA3::Absorb( const fbyte* pData, unsigned __int64 ui64DataBitLen )
{
    unsigned __int64 i, j, wholeBlocks;
    fuint partialBlock, partialByte;
    const fbyte* curData;

    if ((m_nBitsInQueue % 8) != 0)
    {
        return; // Only the last call may contain a partial byte
    }

    if ( m_iSqueezing )
    {
        return; // Too late for additional input
    }

    i = 0;
    while(i < ui64DataBitLen) 
    {
        if ( (m_nBitsInQueue == 0) && (ui64DataBitLen >= m_nRate) && (i <= (ui64DataBitLen - m_nRate))) 
        {
            wholeBlocks = (ui64DataBitLen - i) / m_nRate;

            curData = pData + i / 8;

#ifdef ProvideFast576
            if (m_nRate == RATE_576) 
            {
                for( j = 0; j < wholeBlocks; j++, curData += RATE_576 / 8) 
                {
                    KeccakAbsorb576bits(m_State, curData);
                }
            }
            else
#endif
#ifdef ProvideFast832
            if (m_nRate == RATE_832) 
            {
                for( j = 0; j < wholeBlocks; j++, curData += RATE_832/8) 
                {
                        KeccakAbsorb832bits(m_State, curData);
                }
            }
            else
#endif
#ifdef ProvideFast1024
            if (m_nRate == RATE_1024)
            {
                for(j = 0; j < wholeBlocks; j++, curData += RATE_1024/8)
                {
                    KeccakAbsorb1024bits(m_State, curData);
                }
            }
            else
#endif
#ifdef ProvideFast1088
            if (m_nRate == RATE_1088) 
            {
                for( j = 0; j < wholeBlocks; j++, curData += RATE_1088/8) 
                {
                    KeccakAbsorb1088bits(m_State, curData);
                }
            }
            else
#endif
#ifdef ProvideFast1152
            if (m_nRate == RATE_1152) 
            {
                for( j = 0; j < wholeBlocks; j++, curData += RATE_1152/8) 
                {

                    KeccakAbsorb1152bits(m_State, curData);
                }
            }
            else
#endif
#ifdef ProvideFast1344
            if (m_nRate == RATE_1344) 
            {
                for( j = 0; j < wholeBlocks; j++, curData += RATE_1344/8) 
                {
                    KeccakAbsorb1344bits(m_State, curData);
                }
            }
            else
#endif
            {
                for( j = 0; j < wholeBlocks; j++, curData += m_nRate/8) 
                {
                    KeccakAbsorb(m_State, curData, m_nRate / 64);
                }
            }
            i += wholeBlocks * m_nRate;
        }
        else 
        {
            partialBlock = (unsigned int)(ui64DataBitLen - i);
            if ( partialBlock + m_nBitsInQueue > m_nRate)
            {
                partialBlock = m_nRate - m_nBitsInQueue;
            }
            partialByte = partialBlock % 8;
            partialBlock -= partialByte;
            memcpy_FBC(m_DataQueue + m_nBitsInQueue / 8, pData + i / 8, partialBlock / 8);
            m_nBitsInQueue += partialBlock;
            i += partialBlock;
            if (m_nBitsInQueue == m_nRate)
            {
                AbsorbQueue();
            }
            if (partialByte > 0) 
            {
                unsigned char mask = (1 << partialByte)-1;
                m_DataQueue[m_nBitsInQueue / 8] = pData[ i / 8 ] & mask;
                m_nBitsInQueue += partialByte;
                i += partialByte;
            }
        }
    }
}

void FBC_SHA3::PadAndSwitchToSqueezingPhase()
{
    // Note: the bits are numbered from 0=LSB to 7=MSB
    if (m_nBitsInQueue + 1 == m_nRate) 
    {
        m_DataQueue[m_nBitsInQueue/8 ] |= 1 << (m_nBitsInQueue % 8);
        AbsorbQueue();
        memset_FBC(m_DataQueue, 0, m_nRate/8);
    }
    else 
    {
        memset_FBC(m_DataQueue + (m_nBitsInQueue+7)/8, 0, m_nRate/8 - (m_nBitsInQueue+7)/8);
        m_DataQueue[m_nBitsInQueue/8 ] |= 1 << (m_nBitsInQueue % 8);
    }
    m_DataQueue[(m_nRate-1)/8] |= 1 << ((m_nRate-1) % 8);
    AbsorbQueue();

#ifdef ProvideFast1024
    if (m_nRate == RATE_1024) 
    {
        KeccakExtract1024bits(m_State, m_DataQueue);
        m_nBitsAvailableForSqueezing = 1024;
    }
    else
#endif
    {
        KeccakExtract(m_State, m_DataQueue, m_nRate / 64);
        m_nBitsAvailableForSqueezing = m_nRate;
    }

    m_iSqueezing = 1;
}

void FBC_SHA3::Squeeze( fbyte* pOutput, unsigned __int64 ui64OutputLength )
{
    unsigned __int64 i;
    fuint partialBlock;

    if ( !m_iSqueezing )
    {
        PadAndSwitchToSqueezingPhase();
    }

    i = 0;
    while( i < ui64OutputLength ) 
    {
        if ( m_nBitsAvailableForSqueezing == 0 ) 
        {
            KeccakPermutation();
#ifdef ProvideFast1024
            if ( m_nRate == RATE_1024 ) 
            {
                KeccakExtract1024bits( m_State, m_DataQueue );
                m_nBitsAvailableForSqueezing = 1024;
            }
            else
#endif
            {
                KeccakExtract(m_State, m_DataQueue, m_nRate / 64);
                m_nBitsAvailableForSqueezing = m_nRate;
            }

        }
        partialBlock = m_nBitsAvailableForSqueezing;
        if ((unsigned long long)partialBlock > ui64OutputLength - i)
            partialBlock = (unsigned int)(ui64OutputLength - i);
        memcpy_FBC(pOutput + i / 8, m_DataQueue + ( m_nRate - m_nBitsAvailableForSqueezing ) / 8, partialBlock / 8);
        m_nBitsAvailableForSqueezing -= partialBlock;
        i += partialBlock;
    }
}

void FBC_SHA3::Final( fbyte* pHashVal )
{
    Squeeze(pHashVal, m_nFixedOutputLength);
}

void FBC_SHA3::Update( const fbyte* pData, unsigned __int64 ui64DataBitLen )
{
    if ( (ui64DataBitLen % 8) == 0 )
    {
        Absorb(pData, ui64DataBitLen);
    }
    else 
    {
        Absorb(pData, ui64DataBitLen - (ui64DataBitLen % 8));
        {
            unsigned char lastByte; 
            // Align the last partial byte to the least significant bits
            lastByte = pData[ui64DataBitLen/8] >> (8 - (ui64DataBitLen % 8));
            return Absorb(&lastByte, ui64DataBitLen % 8);
        }
    }       
}

#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)

#define xor2bytesIntoInterleavedWords(even, odd, source, j) \
    i##j = g_InterleaveTable[((const fword*)source)[j]]; \
    ((fbyte*)even)[j] ^= i##j & 0xFF; \
    ((fbyte*)odd)[j] ^= i##j >> 8;

#define setInterleavedWordsInto2bytes(dest, even, odd, j) \
    d##j = g_DeinterleaveTable[((even >> (j*8)) & 0xFF) ^ (((odd >> (j*8)) & 0xFF) << 8)]; \
    ((fword*)dest)[j] = d##j;

#else // (PLATFORM_BYTE_ORDER == IS_BIG_ENDIAN)

#define xor2bytesIntoInterleavedWords(even, odd, source, j) \
    i##j = g_InterleaveTable[source[2*j] ^ ((fword)source[2*j+1] << 8)]; \
    *even ^= (i##j & 0xFF) << (j*8); \
    *odd ^= ((i##j >> 8) & 0xFF) << (j*8);

#define setInterleavedWordsInto2bytes(dest, even, odd, j) \
    d##j = g_DeinterleaveTable[((even >> (j*8)) & 0xFF) ^ (((odd >> (j*8)) & 0xFF) << 8)]; \
    dest[2*j] = d##j & 0xFF; \
    dest[2*j+1] = d##j >> 8;

#endif // Endianness

void FBC_SHA3::Xor8bytesIntoInterleavedWords( fuint *even, fuint *odd, const fbyte* source )
{
    fword i0, i1, i2, i3;

    xor2bytesIntoInterleavedWords(even, odd, source, 0)
    xor2bytesIntoInterleavedWords(even, odd, source, 1)
    xor2bytesIntoInterleavedWords(even, odd, source, 2)
    xor2bytesIntoInterleavedWords(even, odd, source, 3)
}

#define xorLanesIntoState(laneCount, state, input) \
{ \
    fuint i; \
    for(i = 0; i < (laneCount); i++) \
    Xor8bytesIntoInterleavedWords(state+i*2, state+i*2+1, input+i*8); \
}

void FBC_SHA3::SetInterleavedWordsInto8bytes( fbyte* dest, fuint even, fuint odd )
{
    fword d0, d1, d2, d3;

    setInterleavedWordsInto2bytes(dest, even, odd, 0)
    setInterleavedWordsInto2bytes(dest, even, odd, 1)
    setInterleavedWordsInto2bytes(dest, even, odd, 2)
    setInterleavedWordsInto2bytes(dest, even, odd, 3)
}

#define extractLanes(laneCount, state, data) \
{ \
    fuint i; \
    for(i = 0; i < (laneCount); i++) \
    SetInterleavedWordsInto8bytes(data+i*8, ((fuint*)state)[i*2], ((fuint*)state)[i*2+1]); \
    }

#include "KeccakF-1600-unrolling.macros"
#include "KeccakF-1600-32.macros"

void FBC_SHA3::KeccakPermutation()
{
    KeccakPermutationOnWords( (fuint*)m_State );
}

void FBC_SHA3::KeccakPermutationOnWords( fuint* state )
{
    rounds
}

void FBC_SHA3::KeccakPermutationOnWordsAfterXoring(fuint *state, const fbyte *input, unsigned int laneCount)
{
    xorLanesIntoState(laneCount, state, input)
    rounds
}

#ifdef ProvideFast576
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring576bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(9, state, input)
    rounds
}
#endif

#ifdef ProvideFast832
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring832bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(13, state, input)
    rounds
}
#endif

#ifdef ProvideFast1024
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring1024bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(16, state, input)
    rounds
}
#endif

#ifdef ProvideFast1088
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring1088bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(17, state, input)
    rounds
}
#endif

#ifdef ProvideFast1152
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring1152bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(18, state, input)
    rounds
}
#endif

#ifdef ProvideFast1344
void FBC_SHA3::KeccakPermutationOnWordsAfterXoring1344bits(fuint *state, const fbyte *input)
{
    xorLanesIntoState(21, state, input)
    rounds
}
#endif

#ifdef ProvideFast576
void FBC_SHA3::KeccakAbsorb576bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring576bits((fuint*)state, data);
}
#endif

#ifdef ProvideFast832
void FBC_SHA3::KeccakAbsorb832bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring832bits((fuint*)state, data);
}
#endif

#ifdef ProvideFast1024
void FBC_SHA3::KeccakAbsorb1024bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring1024bits((fuint*)state, data);
}
#endif

#ifdef ProvideFast1088
void FBC_SHA3::KeccakAbsorb1088bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring1088bits((fuint*)state, data);
}
#endif

#ifdef ProvideFast1152
void FBC_SHA3::KeccakAbsorb1152bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring1152bits((fuint*)state, data);
}
#endif

#ifdef ProvideFast1344
void FBC_SHA3::KeccakAbsorb1344bits(unsigned char *state, const unsigned char *data)
{
    KeccakPermutationOnWordsAfterXoring1344bits((fuint*)state, data);
}
#endif

void FBC_SHA3::KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount)
{
    KeccakPermutationOnWordsAfterXoring((fuint*)state, data, laneCount);
}

#ifdef ProvideFast1024
void FBC_SHA3::KeccakExtract1024bits(const unsigned char *state, unsigned char *data)
{
    extractLanes(16, state, data)
}
#endif

void FBC_SHA3::KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount)
{
    extractLanes(laneCount, state, data)
}

NAMESPACE_END
