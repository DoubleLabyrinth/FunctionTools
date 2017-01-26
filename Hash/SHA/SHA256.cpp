#include "SHA256.h"

UINT32 SHA256::K[64] = {0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
                        0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
                        0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
                        0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
                        0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
                        0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
                        0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
                        0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

SHA256HashResult SHA256::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    SHA256HashResult Ret;
    Ret.A = 0x6A09E667;
    Ret.B = 0xBB67AE85;
    Ret.C = 0x3C6EF372;
    Ret.D = 0xA54FF53A;
    Ret.E = 0x510E527F;
    Ret.F = 0x9B05688C;
    Ret.G = 0x1F83D9AB;
    Ret.H = 0x5BE0CD19;
    UINT32 Buffer[64] = {0};
    BYTE Extra[128] = {0};
    UINT32 a, b, c, d, e, f, g, h;
    const BYTE (*MessageBlock)[64] = (const BYTE (*)[64])srcBytes;
    bool Final = false;

    UINT64 TimesOfMainCycle = srcBytesLength >> 6;
    BYTE LeftoverBytesCount = (BYTE)(srcBytesLength & 0x3F);

fin:for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        for(BYTE j = 0; j < 64; j += 4) Buffer[j >> 2] = ((UINT32)MessageBlock[i][j + 3]) | ((UINT32)MessageBlock[i][j + 2]) << 8 | ((UINT32)MessageBlock[i][j + 1]) << 16 | ((UINT32)MessageBlock[i][j]) << 24;
        for(BYTE j = 16; j < 64; j++) {
            Buffer[j] = (Buffer[j - 2] >> 17 | Buffer[j - 2] << 15) ^ (Buffer[j - 2] >> 19 | Buffer[j - 2] << 13) ^ Buffer[j - 2] >> 10;
            Buffer[j] += Buffer[j - 7];
            Buffer[j] += (Buffer[j - 15] >> 7 | Buffer[j - 15] << 25) ^ (Buffer[j - 15] >> 18 | Buffer[j - 15] << 14) ^ Buffer[j - 15] >> 3;
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

        for(BYTE j = 0; j < 64; j++) {
            UINT32 T1 = h + ((e >> 6 | e << 26) ^ (e >> 11 | e << 21) ^ (e >> 25 | e << 7)) + ((e & f) ^ (~e & g)) + K[j] + Buffer[j];
            UINT32 T2 = ((a >> 2 | a << 30) ^ (a >> 13 | a << 19) ^ (a >> 22 | a << 10)) + ((a & b) ^ (a & c) ^ (b & c));
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
