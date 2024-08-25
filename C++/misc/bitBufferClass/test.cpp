#include <iostream>

#include "bitBufferClass.hpp"

#define ONE_K 1024
#define ONE_MEG (ONE_K * ONE_K)

/*
   TODO: Test the getNextBitWithoutModifying the buffer code...

   NOTE: Will getting the next nondup if next is 0 or 1 that DOES result in a non dup
         AND Compress really not end up a dup? I think so. That should make skipping 
         rejected bytes simple as it wont matter what the next will be...

         HOWEVER, becuase that "next" bit WILL end up as part of the preived byte then
         the 120idx plus the [b] will not be a dup... but that bit cannot be ignored in
         decompression as it's a legit part of the real data.

         DONT forget the "comp data same as original" and "decomp same as comp" rejects.

         TODO: Make the compressor like this;

                if(willReject("BYTE")) <--- will Reject must make reject list for willDup,
                   writeRaw(dest);          Dup, and willBeSameAsOrig and willBeDecompSameAsComp
                else
                   writeModOrCompressed(dest);
*/
int main() {

    bitBufferClass* bitBuff = new bitBufferClass(static_cast<unsigned long>ONE_MEG);

    bitBuff->addNumBitsToBuffer(12, (long)2735); // 101010101111
  
    bitBuff->resetBitBuffer();
 
    unsigned long bits = bitBuff->getNumBitsFromBuffer(11);
    bitBuff->printULongBits(bits);


    BYTE bit = bitBuff->getNextBitWithoutModifyingBuffer();
    bitBuff->printByteBits(bit);

    bit = bitBuff->getNextBitWithoutModifyingBuffer();
    bitBuff->printByteBits(bit);

    delete bitBuff;
}