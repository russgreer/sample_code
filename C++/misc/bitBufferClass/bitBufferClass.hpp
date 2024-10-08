#include <iostream>

typedef unsigned char BYTE;

class bitBufferClass {

    private:
            unsigned long capacityInBytes;
            unsigned long capacityInBits;
            unsigned long currentBytePosInBuffer;
            unsigned long currentReverseBytePosInBuffer; // new
            unsigned long numBitsInBuffer;
            unsigned long numBytesInBuffer;
            int  currentBitPos;
            int  currentReverseBitPos;
            BYTE* dataPtr;
    public: 

    bitBufferClass(unsigned long capacityInBytes) {
        this->capacityInBytes = capacityInBytes;
        this->capacityInBits = (capacityInBytes*8);
        this->numBitsInBuffer = 0;
    
        this->dataPtr = new BYTE[capacityInBytes + 100];

        this->currentReverseBytePosInBuffer = capacityInBytes;
        this->currentReverseBitPos = 0;

        this->resetBitBuffer(); // set counters to start positions
    }


    void copyBytesIntoBuffer(BYTE* bytes, unsigned long numBytes) {
        if(numBytes > this->capacityInBytes) {
            std::cout << "Error: Trying to put more bits into the bit buffer than the capacity of the bitbuffer!" << std::endl;
            return;
        }

        memcpy(this->dataPtr, bytes, numBytes);
        this->numBitsInBuffer = (numBytes*8);
        this->numBytesInBuffer = numBytes;

    }

    unsigned long getCapacityInBytes(void) {
        return this->capacityInBytes;
    }

    void setTheNumberOfBitsInBuffer(unsigned long numBits) {
        this->numBitsInBuffer = numBits;
    }

    unsigned long getTheNumberOfBitsInBuffer(void) {
        return this->numBitsInBuffer;
    }

    unsigned long getTheNumberOfBytesInBuffer(void) {
        return this->numBytesInBuffer;
    }

    BYTE* getPtrToTopOfBuffer(void) {
        return this->dataPtr;
    }

    BYTE* getPtrToCurrentByte(void) {
        return &this ->dataPtr[this->currentBytePosInBuffer];
    }

    void printByteBits(BYTE byte) {

        for(int x = 0; x < 8; x++) {

            if(byte & (1 << (7-x)))
               std::cout << "1";
            else
               std::cout << "0";
        }

        std::cout << std::endl;
    }

    void printULongBits(unsigned long bits) {
        
        for(int x = 64-1; x; x--) {
    
            if((bits & (1ul << (x))))
                std::cout << "1";
            else 
                std::cout << "0"; 
        }

        std::cout << std::endl;
    }


    /// NOTE: If this is pointing to the next bit already this will work fine...
    //        BUT, if it's NOT already pointed to the next bit THIS WONT WORK.
    //        Adjusting the counts temporarily and then back again will be needed!
    BYTE getNextBitWithoutModifyingBuffer(void) {

        BYTE bit = ((this->dataPtr[this->currentBytePosInBuffer] & (1 << this->currentBitPos))) >> this->currentBitPos;
        return bit;
    }

    // Will handle up to 64 bits. 
    /// NOTE: All the reset and boundry code is in the add / get bit functions
    void addMultiBitsToBuffer(int numBitsToAdd, unsigned long bits) {
        /// NOTE: only add up to 64 bits to the buffer!
        //        Put the bits in left to right order at the LOWER end of the bits var!
        for(int x = 0; x < numBitsToAdd; x++) {
            // this ->addBitToBuffer(((bits & (1 << (numBitsToAdd-x-1))) >> (numBitsToAdd-x-1) & 1));
            BYTE bit = (bits >> (numBitsToAdd - x - 1)) & 1;
            this ->addBitToBuffer(bit);
        }
    }


    // Will handle up to 64 bits. 
    /// NOTE: All the reset and boundry code is in the add / get bit functions
    unsigned long getMultiBitsFromBuffer(int numBitsToGet) {
        /// NOTE: only get up to 64 bits to the buffer!
        //        Put the bits in left to right order at the LOWER end of the bits var!
        unsigned long bits = 0lu;
        for(int x = 0; x < numBitsToGet; x++) {
            BYTE bit = this ->getBitFromBuffer();
            bits |= (bit << (numBitsToGet-x-1));
        }

        return bits;
    }


    void addBitToBuffer(BYTE bit) {

        // zero the bit first? Quick hack fix.
        dataPtr[this->currentBytePosInBuffer] &= ~(1 << this->currentBitPos);

        dataPtr[this->currentBytePosInBuffer] |= ((bit & 1) << this->currentBitPos);

        // adjust counters to account for the new bit
        // time to reset the 8bit counter?
        if(this->currentBitPos == 0) {
           this->currentBitPos = 8 - 1;
           this->currentBytePosInBuffer++;  
        }
        else this->currentBitPos--;

        this->numBitsInBuffer++;
    }

    BYTE getBitFromBuffer(void) {

        BYTE bit = ((this->dataPtr[this->currentBytePosInBuffer] & (1 << this->currentBitPos))) >> this->currentBitPos;

        // adjust counters to account for the new bit
        // time to reset the 8bit counter?
        if(this->currentBitPos == 0) {
           this->currentBitPos = 8 - 1;
           this->currentBytePosInBuffer++;  
        }
        else this->currentBitPos--;

        return bit;
    }

    // new
    BYTE getReverseBitFromBuffer(void) {

        BYTE bit = ((this->dataPtr[this->currentReverseBytePosInBuffer] & (1 << this->currentReverseBitPos))) >> this->currentReverseBitPos;

        // adjust counters to account for the new bit
        // time to reset the 8bit counter?
        if(this->currentReverseBitPos == 7) {
           this->currentReverseBitPos = 0;
           this->currentReverseBytePosInBuffer--;  
        }
        else this->currentReverseBitPos++;

        // decrement the number of bits in the buffer by 1
        // NOTE: This is a getBit not just a readBit function...
        this->numBitsInBuffer--;

        // Adjust the numBytes in Buffer
        this->numBytesInBuffer = (this->numBitsInBuffer/8); // note this will shave off any bits not byte aligned

        return bit;
    }
    // /new

    void resetBitBuffer(void) {
        this->currentBitPos  = 8 - 1; // [xxx....] <--- put/get from left to right
        this->currentBytePosInBuffer = 0;  //

        this->currentReverseBytePosInBuffer = abs((int)this->numBitsInBuffer/8);
        this->currentReverseBitPos = abs((int)this->numBitsInBuffer%8);
    }


    ~bitBufferClass() {
        delete [] this->dataPtr;
    }

};