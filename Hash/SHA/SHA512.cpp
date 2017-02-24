#include "SHA512.h"

Hash::SHA::SHA512::SHA512HashResult Hash::SHA::SHA512::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    Hash::SHA::SHA512::SHA512HashResult Ret;
    Ret.A = 0x6A09E667F3BCC908;
    Ret.B = 0xBB67AE8584CAA73B;
    Ret.C = 0x3C6EF372FE94F82B;
    Ret.D = 0xA54FF53A5F1D36F1;
    Ret.E = 0x510E527FADE682D1;
    Ret.F = 0x9B05688C2B3E6C1F;
    Ret.G = 0x1F83D9ABFB41BD6B;
    Ret.H = 0x5BE0CD19137E2179;
    UINT64 Buffer[80] = {0};
    BYTE Extra[256] = {0};
    UINT64 a, b, c, d, e, f, g, h;
    const BYTE (*MessageBlock)[128] = (const BYTE(*)[128])srcBytes;
    bool Final = false;

    UINT64 TimesOfMainCycle = srcBytesLength >> 7;
    BYTE LeftoverBytesCount = srcBytesLength & 0x7F;

fin:for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        for(BYTE j = 0; j < 128; j += 8) Buffer[j >> 3] = ((UINT64)MessageBlock[i][j + 7]) |
                                                          ((UINT64)MessageBlock[i][j + 6]) << 8 |
                                                          ((UINT64)MessageBlock[i][j + 5]) << 16 |
                                                          ((UINT64)MessageBlock[i][j + 4]) << 24 |
                                                          ((UINT64)MessageBlock[i][j + 3]) << 32 |
                                                          ((UINT64)MessageBlock[i][j + 2]) << 40 |
                                                          ((UINT64)MessageBlock[i][j + 1]) << 48 |
                                                          ((UINT64)MessageBlock[i][j]) << 56;
        for(BYTE j = 16; j < 80; j++) {
            Buffer[j] = (Buffer[j - 2] >> 19 | Buffer[j - 2] << 45) ^ (Buffer[j - 2] >> 61 | Buffer[j - 2] << 3) ^ Buffer[j - 2] >> 6;
            Buffer[j] += Buffer[j - 7];
            Buffer[j] += (Buffer[j - 15] >> 1 | Buffer[j - 15] << 63) ^ (Buffer[j - 15] >> 8 | Buffer[j - 15] << 56) ^ Buffer[j - 15] >> 7;
            Buffer[j] += Buffer[j - 16];
        }
        a = Ret.A;
        b = Ret.B;
        c = Ret.C;
        d = Ret.D;
        e = Ret.E;
        f = Ret.F;
        g = Ret.G;
        h = Ret.H;

        for(BYTE j = 0; j < 80; j++) {
            UINT64 T1 = h + ((e >> 14 | e << 50) ^ (e >> 18 | e << 46) ^ (e >> 41 | e << 23)) + ((e & f) ^ (~e & g)) + K[j] + Buffer[j];
            UINT64 T2 = ((a >> 28 | a << 36) ^ (a >> 34 | a << 30) ^ (a >> 39 | a << 25)) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }
        Ret.A += a;
        Ret.B += b;
        Ret.C += c;
        Ret.D += d;
        Ret.E += e;
        Ret.F += f;
        Ret.G += g;
        Ret.H += h;
    }

    if(!Final) {
        MessageBlock = (const BYTE (*)[128])Extra;
        TimesOfMainCycle <<= 7;
        for(BYTE i = 0; i < LeftoverBytesCount; i++) Extra[i] = srcBytes[TimesOfMainCycle + i];
        Extra[LeftoverBytesCount] = 0x80;

        BYTE OffsetOfExtra = LeftoverBytesCount >= 112 ? 240 : 112;
        UINT128 srcBytesLengthInBits(srcBytesLength, 0);
        srcBytesLengthInBits <<= 3;
        for(BYTE i = 0; i < 8; i++) {
            Extra[OffsetOfExtra + i] = *((BYTE*)&(srcBytesLengthInBits.H64) + 7 - i);
            Extra[OffsetOfExtra + 8 + i] = *((BYTE*)&(srcBytesLengthInBits.L64) + 7 - i);
        }

        TimesOfMainCycle = LeftoverBytesCount >= 112 ? 2 : 1;
        Final = true;
        goto fin;
    }

    return Ret;
}
