#include "MD4.h"

Hash::MDA::HashResult Hash::MDA::MD4::GetChecksum(const BYTE* srcBytes, UINT64 srcByteLength) {
    Hash::MDA::HashResult Ret;
    Ret.A = 0x67452301;
    Ret.B = 0xEFCDAB89;
    Ret.C = 0x98BADCFE;
    Ret.D = 0x10325476;
    UINT32 AA, BB, CC, DD;
    bool Final = false;
    BYTE Extra[128] = {0};

    UINT64 BitsOfsrcByte = srcByteLength << 3;
    UINT64 TimesOfMainCycle = srcByteLength >> 6;
    BYTE LeftoversrcByteLength = (BYTE)(srcByteLength & 0x3F);
    const UINT32(*Ptr)[16] = (const UINT32(*)[16])srcBytes;
fin:if(Final) {
        TimesOfMainCycle = LeftoversrcByteLength >= 56 ? 2 : 1;
        Ptr = (const UINT32(*)[16])Extra;
    }
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        AA = Ret.A;
        BB = Ret.B;
        CC = Ret.C;
        DD = Ret.D;

#define UINT32_ROL(X, Y) X << Y | X >> (32 - Y)
#define MD4_F(X, Y, Z) ((X & Y) | (~X & Z))
#define MD4_G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define MD4_H(X, Y, Z) (X ^ Y ^ Z)

#define MD4_FF(A, B, C, D, K, s)\
    A += MD4_F(B, C, D) + K;\
    A = UINT32_ROL(A, s);

#define MD4_GG(A, B, C, D, K, s)\
    A += MD4_G(B, C, D) + K + 0x5A827999;\
    A = UINT32_ROL(A, s);

#define MD4_HH(A, B, C, D, K, s)\
    A += MD4_H(B, C, D) + K + 0x6ED9EBA1;\
    A = UINT32_ROL(A, s);

        MD4_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][0], 3)  //MD4_FF is a MACRO, no need to add ';'
        MD4_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][1], 7)
        MD4_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][2], 11)
        MD4_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][3], 19)
        MD4_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][4], 3)
        MD4_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][5], 7)
        MD4_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][6], 11)
        MD4_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][7], 19)
        MD4_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][8], 3)
        MD4_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][9], 7)
        MD4_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][10], 11)
        MD4_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][11], 19)
        MD4_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][12], 3)
        MD4_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][13], 7)
        MD4_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][14], 11)
        MD4_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][15], 19)

        MD4_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][0], 3)
        MD4_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][4], 5)
        MD4_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][8], 9)
        MD4_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][12], 13)
        MD4_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][1], 3)
        MD4_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][5], 5)
        MD4_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][9], 9)
        MD4_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][13], 13)
        MD4_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][2], 3)
        MD4_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][6], 5)
        MD4_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][10], 9)
        MD4_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][14], 13)
        MD4_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][3], 3)
        MD4_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][7], 5)
        MD4_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][11], 9)
        MD4_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][15], 13)

        MD4_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][0], 3)
        MD4_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][8], 9)
        MD4_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][4], 11)
        MD4_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][12], 15)
        MD4_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][2], 3)
        MD4_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][10], 9)
        MD4_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][6], 11)
        MD4_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][14], 15)
        MD4_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][1], 3)
        MD4_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][9], 9)
        MD4_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][5], 11)
        MD4_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][13], 15)
        MD4_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][3], 3)
        MD4_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][11], 9)
        MD4_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][7], 11)
        MD4_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][15], 15)

#ifdef UINT32_ROL
#undef UINT32_ROL
#endif //#ifdef UINT32_ROL
#undef MD4_F
#undef MD4_G
#undef MD4_H
#undef MD4_FF
#undef MD4_GG
#undef MD4_HH

        Ret.A += AA;
        Ret.B += BB;
        Ret.C += CC;
        Ret.D += DD;
    }

    if(!Final) {
        const BYTE(*_Ptr)[64] = (const BYTE (*)[64])srcBytes;
        for(BYTE i = LeftoversrcByteLength - 1; i >0; i--) Extra[i] = _Ptr[TimesOfMainCycle][i];
        Extra[0] = _Ptr[TimesOfMainCycle][0];
        Extra[LeftoversrcByteLength] = 0x80;
        *(UINT64*)(Extra + (LeftoversrcByteLength >= 56 ? 120 : 56)) = BitsOfsrcByte;
        Final = true;
        goto fin;
    }

    return Ret;
}
