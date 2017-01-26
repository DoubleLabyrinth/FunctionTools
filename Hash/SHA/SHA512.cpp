#include "SHA512.h"

UINT64 SHA512::K[80] = {0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
                        0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
                        0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
                        0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
                        0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
                        0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
                        0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
                        0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
                        0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
                        0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

SHA512HashResult SHA512::GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength) {
    SHA512HashResult Ret;
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
