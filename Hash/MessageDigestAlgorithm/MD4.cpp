#include "MD4.h"
#define F(X, Y, Z) (((X) & (Y)) | (~(X) & (Z)))
#define G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define RotateLeft(X, s) (((X) << (s)) | ((X) >> (32 - (s))))
#define RotateRight(X, s) (((X) >> (s)) | ((X) << (32 - (s))))
#define FF(a, b, c, d, k, s) ((a) = RotateLeft((a) + F((b), (c), (d)) + M[0][(k)], (s)))
#define GG(a, b, c, d, k, s) ((a) = RotateLeft((a) + G((b), (c), (d)) + M[0][(k)] + 0x5A827999, (s)))
#define HH(a, b, c, d, k, s) ((a) = RotateLeft((a) + H((b), (c), (d)) + M[0][(k)] + 0x6ED9EBA1, (s)))

HashResult MD4::FromByte(const BYTE* srcByte, UINT64 ByteLength) {
    UINT64 LeftLength = ByteLength % 64;
    BOOL AddCycleFlag = LeftLength < 56;
    UINT64 CycleNumber = (ByteLength >> 6) + (AddCycleFlag ? 1 : 2);
    UINT64 LengthInBit = ByteLength << 3;
    UINT32 A = 0x67452301;
    UINT32 B = 0xEFCDAB89;
    UINT32 C = 0x98BADCFE;
    UINT32 D = 0x10325476;
    UINT32 AA, BB, CC, DD;
    const UINT32 (*M)[16];
    UINT32 X[16];

    for(UINT64 i = 0; i < CycleNumber; i++) {
        if(AddCycleFlag && i == CycleNumber - 1) {
            BYTE* Ptr = (BYTE*)X;
            for(UINT64 j = 0; j < LeftLength; j++) *(Ptr + j) = *(srcByte + i * 64 + j);
            *(Ptr + LeftLength) = 0x80;
            for(UINT64 j = LeftLength + 1; j < 56; j++) *(Ptr + j) = 0x00;
            *(UINT64*)(Ptr + 56) = LengthInBit;
            M = (const UINT32 (*)[16])X;
        } else if(!AddCycleFlag && i == CycleNumber - 2) {
            BYTE* Ptr = (BYTE*)X;
            for(UINT64 j = 0; j < LeftLength; j++) *(Ptr + j) = *(srcByte + i * 64 + j);
            *(Ptr + LeftLength) = 0x80;
            for(UINT64 j = LeftLength + 1; j < 64; j++) *(Ptr + j) = 0x00;
            M = (const UINT32 (*)[16])X;
        } else if(!AddCycleFlag && i == CycleNumber - 1) {
            BYTE* Ptr = (BYTE*)X;
            for(int j = 0; j < 56; j++) *(Ptr + j) = 0x00;
            *(UINT64*)(Ptr + 56) = LengthInBit;
            M = (const UINT32 (*)[16])X;
        } else {
            M = (const UINT32 (*)[16])(srcByte + i * 64);
        }
        AA = A;
        BB = B;
        CC = C;
        DD = D;

        FF(A, B, C, D, 0, 3);
        FF(D, A, B, C, 1, 7);
        FF(C, D, A, B, 2, 11);
        FF(B, C, D, A, 3, 19);
        FF(A, B, C, D, 4, 3);
        FF(D, A, B, C, 5, 7);
        FF(C, D, A, B, 6, 11);
        FF(B, C, D, A, 7, 19);
        FF(A, B, C, D, 8, 3);
        FF(D, A, B, C, 9, 7);
        FF(C, D, A, B, 10, 11);
        FF(B, C, D, A, 11, 19);
        FF(A, B, C, D, 12, 3);
        FF(D, A, B, C, 13, 7);
        FF(C, D, A, B, 14, 11);
        FF(B, C, D, A, 15, 19);

        GG(A, B, C, D, 0, 3);
        GG(D, A, B, C, 4, 5);
        GG(C, D, A, B, 8, 9);
        GG(B, C, D, A, 12, 13);
        GG(A, B, C, D, 1, 3);
        GG(D, A, B, C, 5, 5);
        GG(C, D, A, B, 9, 9);
        GG(B, C, D, A, 13, 13);
        GG(A, B, C, D, 2, 3);
        GG(D, A, B, C, 6, 5);
        GG(C, D, A, B, 10, 9);
        GG(B, C, D, A, 14, 13);
        GG(A, B, C, D, 3, 3);
        GG(D, A, B, C, 7, 5);
        GG(C, D, A, B, 11, 9);
        GG(B, C, D, A, 15, 13);

        HH(A, B, C, D, 0, 3);
        HH(D, A, B, C, 8, 9);
        HH(C, D, A, B, 4, 11);
        HH(B, C, D, A, 12, 15);
        HH(A, B, C, D, 2, 3);
        HH(D, A, B, C, 10, 9);
        HH(C, D, A, B, 6, 11);
        HH(B, C, D, A, 14, 15);
        HH(A, B, C, D, 1, 3);
        HH(D, A, B, C, 9, 9);
        HH(C, D, A, B, 5, 11);
        HH(B, C, D, A, 13, 15);
        HH(A, B, C, D, 3, 3);
        HH(D, A, B, C, 11, 9);
        HH(C, D, A, B, 7, 11);  // A: DF 50 13 CB
                                // B: 05 E4 DA 20
                                // C: E3 BC 57 8C
                                // D: 6F F2 10 EE

        HH(B, C, D, A, 15, 15); // A: DF 50 13 CB
                                // B: 84 9C FC 76
                                // C: E3 BC 57 8C
                                // D: 6F F2 10 EE
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
    HashResult Ret;
    Ret.A = A;
    Ret.B = B;
    Ret.C = C;
    Ret.D = D;

    return Ret;
}
