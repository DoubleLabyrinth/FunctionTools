#include "SHA384.h"

UINT64 SHA384::K[80] = {0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC, 0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
                        0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2, 0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
                        0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65, 0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
                        0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4, 0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
                        0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF, 0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
                        0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30, 0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
                        0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8, 0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
                        0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC, 0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
                        0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178, 0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
                        0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C, 0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817};

SHA384HashResult SHA384::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    SHA384HashResult Ret;
    Ret.A = 0xCBBB9D5DC1059ED8;
    Ret.B = 0x629A292A367CD507;
    Ret.C = 0x9159015A3070DD17;
    Ret.D = 0x152FECD8F70E5939;
    Ret.E = 0x67332667FFC00B31;
    Ret.F = 0x8EB44A8768581511;
    UINT64 G = 0xDB0C2E0D64F98FA7;
    UINT64 H = 0x47B5481DBEFA4FA4;
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
        g = G;
        h = H;

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
        G += g;
        H += h;
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
