#include "SHA1.h"

Hash::SHA::SHA1::SHA1HashResult Hash::SHA::SHA1::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    Hash::SHA::SHA1::SHA1HashResult Ret;
    Ret.A = 0x67452301;
    Ret.B = 0xEFCDAB89;
    Ret.C = 0x98BADCFE;
    Ret.D = 0x10325476;
    Ret.E = 0xC3D2E1F0;
    UINT32 Buffer[80] = {0};
    BYTE Extra[128] = {0};
    UINT32 a, b, c, d, e;
    const BYTE (*MessageBlock)[64] = (const BYTE (*)[64])srcBytes;
    bool Final = false;

    UINT64 TimesOfMainCycle = srcBytesLength >> 6;
    BYTE LeftoverBytesCount = (BYTE)(srcBytesLength & 0x3F);

fin:for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        for(BYTE j = 0; j < 64; j += 4) Buffer[j >> 2] = ((UINT32)MessageBlock[i][j + 3]) | ((UINT32)MessageBlock[i][j + 2]) << 8 | ((UINT32)MessageBlock[i][j + 1]) << 16 | ((UINT32)MessageBlock[i][j]) << 24;
        for(BYTE j = 16; j < 80; j++) {
            UINT32 temp = Buffer[j - 3] ^ Buffer[j - 8] ^ Buffer[j - 14] ^ Buffer[j - 16];
            Buffer[j] = temp << 1 | temp >> 31;
        }
        a = Ret.A;
        b = Ret.B;
        c = Ret.C;
        d = Ret.D;
        e = Ret.E;

        for(BYTE j = 0; j < 20; j++) {
            UINT32 T = a << 5 | a >> 27;
            T += ((b & c) ^ (~b & d)) + e + 0x5A827999 + Buffer[j];
            e = d;
            d = c;
            c = b << 30 | b >> 2;
            b = a;
            a = T;
        }
        for(BYTE j = 20; j < 40; j++) {
            UINT32 T = a << 5 | a >> 27;
            T += (b ^ c ^ d) + e + 0x6ED9EBA1 + Buffer[j];
            e = d;
            d = c;
            c = b << 30 | b >> 2;
            b = a;
            a = T;
        }
        for(BYTE j = 40; j < 60; j++) {
            UINT32 T = a << 5 | a >> 27;
            T += ((b & c) ^ (b & d) ^ (c & d)) + e + 0x8F1BBCDC + Buffer[j];
            e = d;
            d = c;
            c = b << 30 | b >> 2;
            b = a;
            a = T;
        }
        for(BYTE j = 60; j < 80; j++) {
            UINT32 T = a << 5 | a >> 27;
            T += (b ^ c ^ d) + e + 0xCA62C1D6 + Buffer[j];
            e = d;
            d = c;
            c = b << 30 | b >> 2;
            b = a;
            a = T;
        }
        Ret.A += a;
        Ret.B += b;
        Ret.C += c;
        Ret.D += d;
        Ret.E += e;
    }

    if(!Final) {
        MessageBlock = (const BYTE (*)[64])Extra;
        TimesOfMainCycle <<= 6;
        for(BYTE i = 0; i < LeftoverBytesCount; i++) Extra[i] = srcBytes[TimesOfMainCycle + i];
        Extra[LeftoverBytesCount] = 0x80;
        srcBytesLength <<= 3;
        BYTE OffsetOfExtra = LeftoverBytesCount >= 56 ? 120 : 56;
        for(BYTE i = 0; i < 8; i++) Extra[OffsetOfExtra + i] = *((BYTE*)&srcBytesLength + 7 - i);
        TimesOfMainCycle = LeftoverBytesCount >= 56 ? 2 : 1;
        Final = true;
        goto fin;
    }

    return Ret;
}
