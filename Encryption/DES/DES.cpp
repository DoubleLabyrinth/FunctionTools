#include "DES.h"

void Encryption::DES::Encipher(BYTE srcBytes[8], BYTE K[16][6]) {
    BYTE Buffer[8] = {0};

    //Start initial permutation
    Buffer[0] = (srcBytes[7] & 0x40) << 1 | (srcBytes[6] & 0x40) | (srcBytes[5] & 0x40) >> 1 | (srcBytes[4] & 0x40) >> 2 | (srcBytes[3] & 0x40) >> 3 | (srcBytes[2] & 0x40) >> 4 | (srcBytes[1] & 0x40) >> 5 | (srcBytes[0] & 0x40) >> 6;
    Buffer[1] = (srcBytes[7] & 0x10) << 3 | (srcBytes[6] & 0x10) << 2 | (srcBytes[5] & 0x10) << 1 | (srcBytes[4] & 0x10) | (srcBytes[3] & 0x10) >> 1 | (srcBytes[2] & 0x10) >> 2 | (srcBytes[1] & 0x10) >> 3 | (srcBytes[0] & 0x10) >> 4;
    Buffer[2] = (srcBytes[7] & 0x04) << 5 | (srcBytes[6] & 0x04) << 4 | (srcBytes[5] & 0x04) << 3 | (srcBytes[4] & 0x04) << 2 | (srcBytes[3] & 0x04) << 1 | (srcBytes[2] & 0x04) | (srcBytes[1] & 0x04) >> 1 | (srcBytes[0] & 0x04) >> 2;
    Buffer[3] = (srcBytes[7] & 0x01) << 7 | (srcBytes[6] & 0x01) << 6 | (srcBytes[5] & 0x01) << 5 | (srcBytes[4] & 0x01) << 4 | (srcBytes[3] & 0x01) << 3 | (srcBytes[2] & 0x01) << 2 | (srcBytes[1] & 0x01) << 1 | (srcBytes[0] & 0x01);
    Buffer[4] = (srcBytes[7] & 0x80) | (srcBytes[6] & 0x80) >> 1 | (srcBytes[5] & 0x80) >> 2 | (srcBytes[4] & 0x80) >> 3 | (srcBytes[3] & 0x80) >> 4 | (srcBytes[2] & 0x80) >> 5 | (srcBytes[1] & 0x80) >> 6 | (srcBytes[0] & 0x80) >> 7;
    Buffer[5] = (srcBytes[7] & 0x20) << 2 | (srcBytes[6] & 0x20) << 1 | (srcBytes[5] & 0x20) | (srcBytes[4] & 0x20) >> 1 | (srcBytes[3] & 0x20) >> 2 | (srcBytes[2] & 0x20) >> 3 | (srcBytes[1] & 0x20) >> 4 | (srcBytes[0] & 0x20) >> 5;
    Buffer[6] = (srcBytes[7] & 0x08) << 4 | (srcBytes[6] & 0x08) << 3 | (srcBytes[5] & 0x08) << 2 | (srcBytes[4] & 0x08) << 1 | (srcBytes[3] & 0x08) | (srcBytes[2] & 0x08) >> 1 | (srcBytes[1] & 0x08) >> 2 | (srcBytes[0] & 0x08) >> 3;
    Buffer[7] = (srcBytes[7] & 0x02) << 6 | (srcBytes[6] & 0x02) << 5 | (srcBytes[5] & 0x02) << 4 | (srcBytes[4] & 0x02) << 3 | (srcBytes[3] & 0x02) << 2 | (srcBytes[2] & 0x02) << 1 | (srcBytes[1] & 0x02) | (srcBytes[0] & 0x02) >> 1;

    //Start enciphering
    BYTE* LeftPart = Buffer;
    BYTE* RightPart = Buffer + 4;
    for(BYTE i = 0; i < 16; i++) {
        UINT32 temp = *(UINT32*)RightPart;
        //Cipher function f
        //E BIT-SELECTION and xor by K
        BYTE tmp[6] = {0};
        tmp[0] = (RightPart[3] << 7 | (RightPart[0] & 0xF8) >> 1 | (RightPart[0] & 0x18) >> 3) ^ K[i][0];
        tmp[1] = (RightPart[0] << 5 | (RightPart[1] & 0x80) >> 3 | (RightPart[0] & 0x01) << 3 | RightPart[1] >> 5) ^ K[i][1];
        tmp[2] = ((RightPart[1] & 0x18) << 3 | (RightPart[1] & 0x1F) << 1 | RightPart[2] >> 7) ^ K[i][2];
        tmp[3] = (RightPart[1] << 7 | (RightPart[2] & 0xF8) >> 1 | (RightPart[2] & 0x18) >> 3) ^ K[i][3];
        tmp[4] = (RightPart[2] << 5 | (RightPart[3] & 0x80) >> 3 | (RightPart[2] & 0x01) << 3 | RightPart[3] >> 5) ^ K[i][4];
        tmp[5] = ((RightPart[3] & 0x18) << 3 | (RightPart[3] & 0x1F) << 1 | RightPart[0] >> 7) ^ K[i][5];
        //S transformation
        RightPart[0] = Encryption::DES::S1[(tmp[0] & 0x80) >> 6 | (tmp[0] & 0x04) >> 2][(tmp[0] & 0x78) >> 3] << 4 |
                       Encryption::DES::S2[(tmp[0] & 0x02) | (tmp[1] & 0x10) >> 4][(tmp[0] & 0x01) << 3 | tmp[1] >> 5];
        RightPart[1] = Encryption::DES::S3[(tmp[1] & 0x08) >> 2 | (tmp[2] & 0x40) >> 6][(tmp[1] & 0x07) << 1 | tmp[2] >> 7] << 4 |
                       Encryption::DES::S4[(tmp[2] & 0x20) >> 4 | (tmp[2] & 0x01)][(tmp[2] & 0x1E) >> 1];
        RightPart[2] = Encryption::DES::S5[(tmp[3] & 0x80) >> 6 | (tmp[3] & 0x04) >> 2][(tmp[3] & 0x78) >> 3] << 4 |
                       Encryption::DES::S6[(tmp[3] & 0x02) | (tmp[4] & 0x10) >> 4][(tmp[3] & 0x01) << 3 | tmp[4] >> 5];
        RightPart[3] = Encryption::DES::S7[(tmp[4] & 0x08) >> 2 | (tmp[5] & 0x40) >> 6][(tmp[4] & 0x07) << 1 | tmp[5] >> 7] << 4 |
                       Encryption::DES::S8[(tmp[5] & 0x20) >> 4 | (tmp[5] & 0x01)][(tmp[5] & 0x1E) >> 1];
        //P transformation and xor by LeftPart
        tmp[0] = ((RightPart[1] << 7) | (RightPart[0] & 0x02) << 5 | (RightPart[2] & 0x10) << 1 | (RightPart[2] & 0x08) << 1 | (RightPart[3] & 0x08) | (RightPart[1] & 0x10) >> 2 | (RightPart[3] & 0x10) >> 3 | RightPart[2] >> 7) ^ LeftPart[0];
        tmp[1] = ((RightPart[0] & 0x80) | (RightPart[1] & 0x02) << 5 | (RightPart[2] & 0x02) << 4 | (RightPart[3] & 0x40) >> 2 | (RightPart[0] & 0x08) | (RightPart[2] & 0x40) >> 4 | (RightPart[3] & 0x02) | (RightPart[1] & 0x40) >> 6) ^ LeftPart[1];
        tmp[2] = ((RightPart[0] & 0x40) << 1 | (RightPart[0] & 0x01) << 6 | (RightPart[2] & 0x01) << 5 | (RightPart[1] & 0x04) << 2 | (RightPart[3] & 0x01) << 3 | (RightPart[3] & 0x20) >> 3 | (RightPart[0] & 0x20) >> 4 | RightPart[1] >> 7) ^ LeftPart[2];
        tmp[3] = ((RightPart[2] & 0x20) << 2 | (RightPart[1] & 0x08) << 3 | (RightPart[3] & 0x04) << 3 | (RightPart[0] & 0x04) << 2 | (RightPart[2] & 0x04) << 1 | (RightPart[1] & 0x20) >> 3 | (RightPart[0] & 0x10) >> 3 | RightPart[3] >> 7) ^ LeftPart[3];
        *(UINT32*)RightPart = *(UINT32*)tmp;
        *(UINT32*)LeftPart = temp;
    }

    //Start inverse initial permutation
    srcBytes[0] = (LeftPart[0] << 7) | (RightPart[0] & 0x01) << 6 | (LeftPart[1] & 0x01) << 5 | (RightPart[1] & 0x01) << 4 | (LeftPart[2] & 0x01) << 3 | (RightPart[2] & 0x01) << 2 | (LeftPart[3] & 0x01) << 1 | (RightPart[3] & 0x01);
    srcBytes[1] = (LeftPart[0] & 0x02) << 6 | (RightPart[0] & 0x02) << 5 | (LeftPart[1] & 0x02) << 4 | (RightPart[1] & 0x02) << 3 | (LeftPart[2] & 0x02) << 2 | (RightPart[2] & 0x02) << 1 | (LeftPart[3] & 0x02) | (RightPart[3] & 0x02) >> 1;
    srcBytes[2] = (LeftPart[0] & 0x04) << 5 | (RightPart[0] & 0x04) << 4 | (LeftPart[1] & 0x04) << 3 | (RightPart[1] & 0x04) << 2 | (LeftPart[2] & 0x04) << 1 | (RightPart[2] & 0x04) | (LeftPart[3] & 0x04) >> 1 | (RightPart[3] & 0x04) >> 2;
    srcBytes[3] = (LeftPart[0] & 0x08) << 4 | (RightPart[0] & 0x08) << 3 | (LeftPart[1] & 0x08) << 2 | (RightPart[1] & 0x08) << 1 | (LeftPart[2] & 0x08) | (RightPart[2] & 0x08) >> 1 | (LeftPart[3] & 0x08) >> 2 | (RightPart[3] & 0x08) >> 3;
    srcBytes[4] = (LeftPart[0] & 0x10) << 3 | (RightPart[0] & 0x10) << 2 | (LeftPart[1] & 0x10) << 1 | (RightPart[1] & 0x10) | (LeftPart[2] & 0x10) >> 1 | (RightPart[2] & 0x10) >> 2 | (LeftPart[3] & 0x10) >> 3 | (RightPart[3] & 0x10) >> 4;
    srcBytes[5] = (LeftPart[0] & 0x20) << 2 | (RightPart[0] & 0x20) << 1 | (LeftPart[1] & 0x20) | (RightPart[1] & 0x20) >> 1 | (LeftPart[2] & 0x20) >> 2 | (RightPart[2] & 0x20) >> 3 | (LeftPart[3] & 0x20) >> 4 | (RightPart[3] & 0x20) >> 5;
    srcBytes[6] = (LeftPart[0] & 0x40) << 1 | (RightPart[0] & 0x40) | (LeftPart[1] & 0x40) >> 1 | (RightPart[1] & 0x40) >> 2 | (LeftPart[2] & 0x40) >> 3 | (RightPart[2] & 0x40) >> 4 | (LeftPart[3] & 0x40) >> 5 | (RightPart[3] & 0x40) >> 6;
    srcBytes[7] = (LeftPart[0] & 0x80) | (RightPart[0] & 0x80) >> 1 | (LeftPart[1] & 0x80) >> 2 | (RightPart[1] & 0x80) >> 3 | (LeftPart[2] & 0x80) >> 4 | (RightPart[2] & 0x80) >> 5 | (LeftPart[3] & 0x80) >> 6 | (RightPart[3] & 0x80) >> 7;

    *(UINT64*)Buffer = 0;
}

bool Encryption::DES::GenerateK(BYTE CipherKey[8], BYTE out_K[16][6]) {
    //check if CipherKey is legal
    for(BYTE i = 0; i < 8; i++) {
        BYTE tmp = CipherKey[i];
        BYTE count = 0;
        while(tmp) {
            tmp &= tmp - 1;
            count++;
        }
        if((count & 0x01) == 0) return false;
    }

    //Permuted choice 1
    BYTE C_Part[4];
    BYTE D_Part[4];
    C_Part[0] = (CipherKey[7] & 0x80) | (CipherKey[6] & 0x80) >> 1 | (CipherKey[5] & 0x80) >> 2 | (CipherKey[4] & 0x80) >> 3 | (CipherKey[3] & 0x80) >> 4 | (CipherKey[2] & 0x80) >> 5 | (CipherKey[1] & 0x80) >> 6 | (CipherKey[0] & 0x80) >> 7;
    C_Part[1] = (CipherKey[7] & 0x40) << 1 | (CipherKey[6] & 0x40) | (CipherKey[5] & 0x40) >> 1 | (CipherKey[4] & 0x40) >> 2 | (CipherKey[3] & 0x40) >> 3 | (CipherKey[2] & 0x40) >> 4 | (CipherKey[1] & 0x40) >> 5 | (CipherKey[0] & 0x40) >> 6;
    C_Part[2] = (CipherKey[7] & 0x20) << 2 | (CipherKey[6] & 0x20) << 1 | (CipherKey[5] & 0x20) | (CipherKey[4] & 0x20) >> 1 | (CipherKey[3] & 0x20) >> 2 | (CipherKey[2] & 0x20) >> 3 | (CipherKey[1] & 0x20) >> 4 | (CipherKey[0] & 0x20) >> 5;
    C_Part[3] = (CipherKey[7] & 0x10) << 3 | (CipherKey[6] & 0x10) << 2 | (CipherKey[5] & 0x10) << 1 | (CipherKey[4] & 0x10);

    D_Part[0] = (CipherKey[7] & 0x02) << 6 | (CipherKey[6] & 0x02) << 5 | (CipherKey[5] & 0x02) << 4 | (CipherKey[4] & 0x02) << 3 | (CipherKey[3] & 0x02) << 2 | (CipherKey[2] & 0x02) << 1 | (CipherKey[1] & 0x02) | (CipherKey[0] & 0x02) >> 1;
    D_Part[1] = (CipherKey[7] & 0x04) << 5 | (CipherKey[6] & 0x04) << 4 | (CipherKey[5] & 0x04) << 3 | (CipherKey[4] & 0x04) << 2 | (CipherKey[3] & 0x04) << 1 | (CipherKey[2] & 0x04) | (CipherKey[1] & 0x04) >> 1 | (CipherKey[0] & 0x04) >> 2;
    D_Part[2] = (CipherKey[7] & 0x08) << 4 | (CipherKey[6] & 0x08) << 3 | (CipherKey[5] & 0x08) << 2 | (CipherKey[4] & 0x08) << 1 | (CipherKey[3] & 0x08) | (CipherKey[2] & 0x08) >> 1 | (CipherKey[1] & 0x08) >> 2 | (CipherKey[0] & 0x08) >> 3;
    D_Part[3] = (CipherKey[3] & 0x10) << 3 | (CipherKey[2] & 0x10) << 2 | (CipherKey[1] & 0x10) << 1 | (CipherKey[0] & 0x10);

    //Generate K
#define ROL_28Bits(x, s) x[3] |= (x[0] >> (8 - s)) << (4 - s);\
    x[0] = x[0] << s | x[1] >> (8 - s);\
    x[1] = x[1] << s | x[2] >> (8 - s);\
    x[2] = x[2] << s | x[3] >> (8 - s);\
    x[3] = x[3] << s;

    BYTE NumberOfROL[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    for(BYTE i = 0; i < 16; i++) {
        ROL_28Bits(C_Part, NumberOfROL[i])
        ROL_28Bits(D_Part, NumberOfROL[i])
        //Permuted choice 2
        out_K[i][0] = (C_Part[1] & 0x04) << 5 | (C_Part[2] & 0x80) >> 1 | (C_Part[1] & 0x20) | (C_Part[2] & 0x01) << 4 | (C_Part[0] & 0x80) >> 4 | (C_Part[0] & 0x08) >> 1 | (C_Part[0] & 0x20) >> 4 | (C_Part[3] & 0x10) >> 4;
        out_K[i][1] = (C_Part[1] & 0x02) << 6 | (C_Part[0] & 0x04) << 4 | (C_Part[2] & 0x08) << 2 | (C_Part[1] & 0x40) >> 2 | (C_Part[2] & 0x02) << 2 | (C_Part[2] & 0x20) >> 3 | (C_Part[1] & 0x10) >> 3 | (C_Part[0] & 0x10) >> 4;
        out_K[i][2] = (C_Part[3] & 0x40) << 1 | (C_Part[0] & 0x01) << 6 | (C_Part[1] & 0x01) << 5 | (C_Part[0] & 0x02) << 3 | (C_Part[3] & 0x20) >> 2 | (C_Part[2] & 0x10) >> 2 | (C_Part[1] & 0x08) >> 2 | (C_Part[0] & 0x40) >> 6;
        out_K[i][3] = (D_Part[1] & 0x08) << 4 | (D_Part[2] & 0x01) << 6 | (D_Part[0] & 0x20) | (D_Part[1] & 0x80) >> 3 | (D_Part[2] & 0x20) >> 2 | (D_Part[3] & 0x20) >> 3 | (D_Part[0] & 0x40) >> 5 | (D_Part[1] & 0x10) >> 4;
        out_K[i][4] = (D_Part[2] & 0x02) << 6 | (D_Part[2] & 0x80) >> 1 | (D_Part[0] & 0x08) << 2 | (D_Part[2] & 0x10) | (D_Part[1] & 0x01) << 3 | (D_Part[2] & 0x08) >> 1 | (D_Part[1] & 0x20) >> 4 | (D_Part[3] & 0x10) >> 4;
        out_K[i][5] = (D_Part[0] & 0x04) << 5 | (D_Part[3] & 0x80) >> 1 | (D_Part[2] & 0x40) >> 1 | (D_Part[1] & 0x04) << 2 | (D_Part[2] & 0x04) << 1 | (D_Part[0] & 0x01) << 2 | (D_Part[0] & 0x80) >> 6 | (D_Part[0] & 0x10) >> 4;
    }
#undef ROL_28Bits
    return true;
}
