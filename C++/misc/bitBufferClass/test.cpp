#include <iostream>

#include "bitBufferClass.hpp"

#define ONE_K 1024
#define ONE_MEG (ONE_K * ONE_K)

/*
   Tests for the bitbuffer class...
*/
int main() {

    bitBufferClass* bitBuff = new bitBufferClass(static_cast<unsigned long>ONE_MEG);

    bitBuff->addMultiBitsToBuffer(12, (long)2735); // 101 0101 01111
  
    bitBuff->resetBitBuffer();

    unsigned long bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    printf("\n\nShould be 11110!\n");      

     bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

    bit = bitBuff->getReverseBitFromBuffer();
    printf("%lu", bit);

   printf("\nShould be 1010101!\n");
 /*
    unsigned long bits = bitBuff->getMultiBitsFromBuffer(11);
    bitBuff->printULongBits(bits);


    BYTE bit = bitBuff->getNextBitWithoutModifyingBuffer();
    bitBuff->printByteBits(bit);

    bit = bitBuff->getNextBitWithoutModifyingBuffer();
    bitBuff->printByteBits(bit);
*/

    delete bitBuff;
}