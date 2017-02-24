#include "MD5.h"

Hash::MDA::HashResult Hash::MDA::MD5::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    Hash::MDA::HashResult Ret;
    Ret.A = 0x67452301;
    Ret.B = 0xEFCDAB89;
    Ret.C = 0x98BADCFE;
    Ret.D = 0x10325476;
    UINT32 AA, BB, CC, DD;
    bool Final = false;
    BYTE Extra[128] = {0};

    UINT64 TimesOfMainCycle = srcBytesLength >> 6;
    BYTE LeftoverBytesLength = (BYTE)(srcBytesLength & 0x3F);
    const UINT32(*Ptr)[16] = (const UINT32(*)[16])srcBytes;

fin:for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        AA = Ret.A;
        BB = Ret.B;
        CC = Ret.C;
        DD = Ret.D;

#define UINT32_ROL(X, Y) X << Y | X >> (32 - Y)
#define MD5_F(X, Y, Z) ((X & Y) | (~X & Z))
#define MD5_G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define MD5_H(X, Y, Z) (X ^ Y ^ Z)
#define MD5_I(X, Y, Z) (Y ^ (X | ~Z))

#define MD5_FF(A, B, C, D, K, s, T) \
    A += MD5_F(B, C, D) + K + T;\
    A = UINT32_ROL(A, s);\
    A += B;

#define MD5_GG(A, B, C, D, K, s, T) \
    A += MD5_G(B, C, D) + K + T;\
    A = UINT32_ROL(A, s);\
    A += B;

#define MD5_HH(A, B, C, D, K, s, T) \
    A += MD5_H(B, C, D) + K + T;\
    A = UINT32_ROL(A, s);\
    A += B;

#define MD5_II(A, B, C, D, K, s, T) \
    A += MD5_I(B, C, D) + K + T;\
    A = UINT32_ROL(A, s);\
    A += B;

        MD5_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][0], 7, 0xD76AA478)  //MD5_FF is a MACRO, no need to add ';'
        MD5_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][1], 12, 0xE8C7B756)
        MD5_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][2], 17, 0x242070DB)
        MD5_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][3], 22, 0xC1BDCEEE)
        MD5_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][4], 7, 0xF57C0FAF)
        MD5_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][5], 12, 0x4787C62A)
        MD5_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][6], 17, 0xA8304613)
        MD5_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][7], 22, 0xFD469501)
        MD5_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][8], 7, 0x698098D8)
        MD5_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][9], 12, 0x8B44F7AF)
        MD5_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][10], 17, 0xFFFF5BB1)
        MD5_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][11], 22, 0x895CD7BE)
        MD5_FF(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][12], 7, 0x6B901122)
        MD5_FF(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][13], 12, 0xFD987193)
        MD5_FF(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][14], 17, 0xA679438E)
        MD5_FF(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][15], 22, 0x49B40821)

        MD5_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][1], 5, 0xF61E2562)
        MD5_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][6], 9, 0xC040B340)
        MD5_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][11], 14, 0x265E5A51)
        MD5_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][0], 20, 0xE9B6C7AA)
        MD5_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][5], 5, 0xD62F105D)
        MD5_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][10], 9, 0x02441453)
        MD5_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][15], 14, 0xD8A1E681)
        MD5_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][4], 20, 0xE7D3FBC8)
        MD5_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][9], 5, 0x21E1CDE6)
        MD5_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][14], 9, 0xC33707D6)
        MD5_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][3], 14, 0xF4D50D87)
        MD5_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][8], 20, 0x455A14ED)
        MD5_GG(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][13], 5, 0xA9E3E905)
        MD5_GG(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][2], 9, 0xFCEFA3F8)
        MD5_GG(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][7], 14, 0x676F02D9)
        MD5_GG(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][12], 20, 0x8D2A4C8A)

        MD5_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][5], 4, 0xFFFA3942)
        MD5_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][8], 11, 0x8771F681)
        MD5_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][11], 16, 0x6D9D6122)
        MD5_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][14], 23, 0xFDE5380C)
        MD5_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][1], 4, 0xA4BEEA44)
        MD5_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][4], 11, 0x4BDECFA9)
        MD5_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][7], 16, 0xF6BB4B60)
        MD5_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][10], 23, 0xBEBFBC70)
        MD5_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][13], 4, 0x289B7EC6)
        MD5_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][0], 11, 0xEAA127FA)
        MD5_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][3], 16, 0xD4EF3085)
        MD5_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][6], 23, 0x04881D05)
        MD5_HH(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][9], 4, 0xD9D4D039)
        MD5_HH(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][12], 11, 0xE6DB99E5)
        MD5_HH(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][15], 16, 0x1FA27CF8)
        MD5_HH(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][2], 23, 0xC4AC5665)

        MD5_II(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][0], 6, 0xF4292244)
        MD5_II(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][7], 10, 0x432AFF97)
        MD5_II(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][14], 15, 0xAB9423A7)
        MD5_II(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][5], 21, 0xFC93A039)
        MD5_II(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][12], 6, 0x655B59C3)
        MD5_II(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][3], 10, 0x8F0CCC92)
        MD5_II(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][10], 15, 0xFFEFF47D)
        MD5_II(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][1], 21, 0x85845DD1)
        MD5_II(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][8], 6, 0x6FA87E4F)
        MD5_II(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][15], 10, 0xFE2CE6E0)
        MD5_II(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][6], 15, 0xA3014314)
        MD5_II(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][13], 21, 0x4E0811A1)
        MD5_II(Ret.A, Ret.B, Ret.C, Ret.D, Ptr[i][4], 6, 0xF7537E82)
        MD5_II(Ret.D, Ret.A, Ret.B, Ret.C, Ptr[i][11], 10, 0xBD3AF235)
        MD5_II(Ret.C, Ret.D, Ret.A, Ret.B, Ptr[i][2], 15, 0x2AD7D2BB)
        MD5_II(Ret.B, Ret.C, Ret.D, Ret.A, Ptr[i][9], 21, 0xEB86D391)

#undef UINT32_ROL
#undef MD5_F
#undef MD5_G
#undef MD5_H
#undef MD5_I
#undef MD5_FF
#undef MD5_GG
#undef MD5_HH
#undef MD5_II

        Ret.A += AA;
        Ret.B += BB;
        Ret.C += CC;
        Ret.D += DD;
    }

    if(!Final) {
        const BYTE* _Ptr = srcBytes + (TimesOfMainCycle << 6);
        for(BYTE i = 0; i < LeftoverBytesLength; i++) Extra[i] = _Ptr[i];
        Extra[LeftoverBytesLength] = 0x80;
        *(UINT64*)(Extra + (LeftoverBytesLength >= 56 ? 120 : 56)) = srcBytesLength << 3;
        TimesOfMainCycle = LeftoverBytesLength >= 56 ? 2 : 1;
        Ptr = (const UINT32(*)[16])Extra;
        Final = true;
        goto fin;
    }

    return Ret;
}
