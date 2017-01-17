#include "MD4.h"

HashResult MD4::GetChecksum(const BYTE* srcByte, UINT64 srcByteLength) {
    UINT32 A = 0x67452301;
    UINT32 B = 0xEFCDAB89;
    UINT32 C = 0x98BADCFE;
    UINT32 D = 0x10325476;
    UINT32 AA, BB, CC, DD;
    bool Final = false;
    BYTE Extra[128] = {0};

    UINT64 BitsOfsrcByte = srcByteLength << 3;
    UINT64 TimesOfMainCycle = srcByteLength >> 6;
    BYTE LeftoversrcByteLength = (BYTE)(srcByteLength & 0x3F);
    const UINT32(*Ptr)[16] = (const UINT32(*)[16])srcByte;
fin:if(Final) {
        TimesOfMainCycle = LeftoversrcByteLength >= 56 ? 2 : 1;
        Ptr = (const UINT32(*)[16])Extra;
    }
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        AA = A;
        BB = B;
        CC = C;
        DD = D;

        MD4_FF(A, B, C, D, Ptr[i][0], 3)  //MD4_FF is a MACRO FUNCTION, no need to add ';'
        MD4_FF(D, A, B, C, Ptr[i][1], 7)
        MD4_FF(C, D, A, B, Ptr[i][2], 11)
        MD4_FF(B, C, D, A, Ptr[i][3], 19)
        MD4_FF(A, B, C, D, Ptr[i][4], 3)
        MD4_FF(D, A, B, C, Ptr[i][5], 7)
        MD4_FF(C, D, A, B, Ptr[i][6], 11)
        MD4_FF(B, C, D, A, Ptr[i][7], 19)
        MD4_FF(A, B, C, D, Ptr[i][8], 3)
        MD4_FF(D, A, B, C, Ptr[i][9], 7)
        MD4_FF(C, D, A, B, Ptr[i][10], 11)
        MD4_FF(B, C, D, A, Ptr[i][11], 19)
        MD4_FF(A, B, C, D, Ptr[i][12], 3)
        MD4_FF(D, A, B, C, Ptr[i][13], 7)
        MD4_FF(C, D, A, B, Ptr[i][14], 11)
        MD4_FF(B, C, D, A, Ptr[i][15], 19)

        MD4_GG(A, B, C, D, Ptr[i][0], 3)
        MD4_GG(D, A, B, C, Ptr[i][4], 5)
        MD4_GG(C, D, A, B, Ptr[i][8], 9)
        MD4_GG(B, C, D, A, Ptr[i][12], 13)
        MD4_GG(A, B, C, D, Ptr[i][1], 3)
        MD4_GG(D, A, B, C, Ptr[i][5], 5)
        MD4_GG(C, D, A, B, Ptr[i][9], 9)
        MD4_GG(B, C, D, A, Ptr[i][13], 13)
        MD4_GG(A, B, C, D, Ptr[i][2], 3)
        MD4_GG(D, A, B, C, Ptr[i][6], 5)
        MD4_GG(C, D, A, B, Ptr[i][10], 9)
        MD4_GG(B, C, D, A, Ptr[i][14], 13)
        MD4_GG(A, B, C, D, Ptr[i][3], 3)
        MD4_GG(D, A, B, C, Ptr[i][7], 5)
        MD4_GG(C, D, A, B, Ptr[i][11], 9)
        MD4_GG(B, C, D, A, Ptr[i][15], 13)

        MD4_HH(A, B, C, D, Ptr[i][0], 3)
        MD4_HH(D, A, B, C, Ptr[i][8], 9)
        MD4_HH(C, D, A, B, Ptr[i][4], 11)
        MD4_HH(B, C, D, A, Ptr[i][12], 15)
        MD4_HH(A, B, C, D, Ptr[i][2], 3)
        MD4_HH(D, A, B, C, Ptr[i][10], 9)
        MD4_HH(C, D, A, B, Ptr[i][6], 11)
        MD4_HH(B, C, D, A, Ptr[i][14], 15)
        MD4_HH(A, B, C, D, Ptr[i][1], 3)
        MD4_HH(D, A, B, C, Ptr[i][9], 9)
        MD4_HH(C, D, A, B, Ptr[i][5], 11)
        MD4_HH(B, C, D, A, Ptr[i][13], 15)
        MD4_HH(A, B, C, D, Ptr[i][3], 3)
        MD4_HH(D, A, B, C, Ptr[i][11], 9)
        MD4_HH(C, D, A, B, Ptr[i][7], 11)
        MD4_HH(B, C, D, A, Ptr[i][15], 15)

        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    if(!Final) {
        const BYTE(*_Ptr)[64] = (const BYTE (*)[64])srcByte;
        for(BYTE i = LeftoversrcByteLength - 1; i >0; i--) Extra[i] = _Ptr[TimesOfMainCycle][i];
        Extra[0] = _Ptr[TimesOfMainCycle][0];
        Extra[LeftoversrcByteLength] = 0x80;
        *(UINT64*)(Extra + (LeftoversrcByteLength >= 56 ? 120 : 56)) = BitsOfsrcByte;
        Final = true;
        goto fin;
    }

    HashResult Ret;
    Ret.A = A;
    Ret.B = B;
    Ret.C = C;
    Ret.D = D;

    return Ret;
}

char MD4::HashString[33] = {0};
const char* MD4::GetHashString(HashResult srcHashResult, bool UseUppercase) {
    char TransTable[] = "0123456789ABCDEF";
    if(!UseUppercase) for(BYTE i = 10; i < 16; i++) TransTable[i] = 'a' - 10 + i;
    BYTE* Ptr = (BYTE*)&srcHashResult;
    for(BYTE i = 0; i < 32; i += 2) HashString[i] = TransTable[(Ptr[i >> 1] & 0xF0) >> 4];
    for(BYTE i = 1; i < 32; i += 2) HashString[i] = TransTable[Ptr[i >> 1] & 0x0F];
    return HashString;
}
