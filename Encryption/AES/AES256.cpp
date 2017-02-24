#include "AES256.h"

void Encryption::AES::AES256::Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[60]) {
    *((UINT32*)srcBytes) ^= srcExpandedKey[0];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[1];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[2];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[3];

    BYTE ShiftTemp;

#define Swap(X, Y, Temp) Temp = X;\
    X = Y;\
    Y = Temp;

    for(BYTE i = 1; i < 14; i++) {
        for(BYTE j = 0; j < 16; j++) srcBytes[j] = Encryption::AES::SBox[srcBytes[j]];

        //Shift rows starts;
        //Shift the second row;
        Swap(srcBytes[1], srcBytes[5], ShiftTemp)  //Swap is a MACRO, no need to add ';'.
        Swap(srcBytes[5], srcBytes[9], ShiftTemp)
        Swap(srcBytes[9], srcBytes[13], ShiftTemp)
        //Shift the third row;
        Swap(srcBytes[2], srcBytes[10], ShiftTemp)
        Swap(srcBytes[6], srcBytes[14], ShiftTemp)
        //Shift the fourth row;
        Swap(srcBytes[3], srcBytes[15], ShiftTemp)
        Swap(srcBytes[15], srcBytes[11], ShiftTemp)
        Swap(srcBytes[11], srcBytes[7], ShiftTemp)
        //Shift rows ends;

        for(BYTE j = 0; j < 16; j += 4) {
            BYTE tmp[4];
            *(UINT32*)tmp = *((UINT32*)srcBytes + (j >> 2));
            srcBytes[j] = Encryption::AES::Multiply0x02[tmp[0]] ^ Encryption::AES::Multiply0x03[tmp[1]] ^ tmp[2] ^ tmp[3];
            srcBytes[j + 1] = tmp[0] ^ Encryption::AES::Multiply0x02[tmp[1]] ^ Encryption::AES::Multiply0x03[tmp[2]] ^ tmp[3];
            srcBytes[j + 2] = tmp[0] ^ tmp[1] ^ Encryption::AES::Multiply0x02[tmp[2]] ^ Encryption::AES::Multiply0x03[tmp[3]];
            srcBytes[j + 3] = Encryption::AES::Multiply0x03[tmp[0]] ^ tmp[1] ^ tmp[2] ^ Encryption::AES::Multiply0x02[tmp[3]];
        }
        *((UINT32*)srcBytes) ^= srcExpandedKey[i << 2];
        *((UINT32*)srcBytes + 1) ^= srcExpandedKey[(i << 2) + 1];
        *((UINT32*)srcBytes + 2) ^= srcExpandedKey[(i << 2) + 2];
        *((UINT32*)srcBytes + 3) ^= srcExpandedKey[(i << 2) + 3];
    }

    for(BYTE j = 0; j < 16; j++) srcBytes[j] = Encryption::AES::SBox[srcBytes[j]];
    //Shift rows starts;
    //Shift the second row;
    Swap(srcBytes[1], srcBytes[5], ShiftTemp)  //Swap is a MACRO, no need to add ';'.
    Swap(srcBytes[5], srcBytes[9], ShiftTemp)
    Swap(srcBytes[9], srcBytes[13], ShiftTemp)
    //Shift the third row;
    Swap(srcBytes[2], srcBytes[10], ShiftTemp)
    Swap(srcBytes[6], srcBytes[14], ShiftTemp)
    //Shift the fourth row;
    Swap(srcBytes[3], srcBytes[15], ShiftTemp)
    Swap(srcBytes[15], srcBytes[11], ShiftTemp)
    Swap(srcBytes[11], srcBytes[7], ShiftTemp)
    //Shift rows ends;

#undef Swap

    *((UINT32*)srcBytes) ^= srcExpandedKey[56];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[57];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[58];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[59];
}

void Encryption::AES::AES256::KeyExpansion(BYTE srcKey[32], UINT32 dstExpandedKey[60]) {
    for(BYTE i = 0; i < 8; i++) dstExpandedKey[i] = *((UINT32*)srcKey + i);
    for(BYTE i = 8; i < 60; i++) {
        UINT32 tmp = dstExpandedKey[i - 1];
        if(i % 8 == 0) {
            tmp = tmp >> 8 | tmp << 24;
            *((BYTE*)&tmp) = Encryption::AES::SBox[*((BYTE*)&tmp)];
            *((BYTE*)&tmp + 1) = Encryption::AES::SBox[*((BYTE*)&tmp + 1)];
            *((BYTE*)&tmp + 2) = Encryption::AES::SBox[*((BYTE*)&tmp + 2)];
            *((BYTE*)&tmp + 3) = Encryption::AES::SBox[*((BYTE*)&tmp + 3)];
            tmp ^= Encryption::AES::Rcon[i >> 3];
        }
        if(i % 8 == 4) {
            *((BYTE*)&tmp) = Encryption::AES::SBox[*((BYTE*)&tmp)];
            *((BYTE*)&tmp + 1) = Encryption::AES::SBox[*((BYTE*)&tmp + 1)];
            *((BYTE*)&tmp + 2) = Encryption::AES::SBox[*((BYTE*)&tmp + 2)];
            *((BYTE*)&tmp + 3) = Encryption::AES::SBox[*((BYTE*)&tmp + 3)];
        }
        dstExpandedKey[i] = dstExpandedKey[i - 8] ^ tmp;
    }
}
