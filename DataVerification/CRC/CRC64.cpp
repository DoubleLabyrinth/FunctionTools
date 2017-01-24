#include "CRC64.h"

UINT64 CRC64::CRC64Table[257] = {0};

void CRC64::GenerateTable(UINT64 Polynomial) {
    BYTE* pPolynomial = (BYTE*)&Polynomial;
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT64 Result = 0;
        BYTE* pResult = (BYTE*)&Result;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= pPolynomial[0] >> j;
                pResult[0] ^= (pPolynomial[0] << (8 - j)) + (pPolynomial[1] >> j);
                pResult[1] ^= (pPolynomial[1] << (8 - j)) + (pPolynomial[2] >> j);
                pResult[2] ^= (pPolynomial[2] << (8 - j)) + (pPolynomial[3] >> j);
                pResult[3] ^= (pPolynomial[3] << (8 - j)) + (pPolynomial[4] >> j);
                pResult[4] ^= (pPolynomial[4] << (8 - j)) + (pPolynomial[5] >> j);
                pResult[5] ^= (pPolynomial[5] << (8 - j)) + (pPolynomial[6] >> j);
                pResult[6] ^= (pPolynomial[6] << (8 - j)) + (pPolynomial[7] >> j);
                pResult[7] ^= pPolynomial[7] << (8 - j);
            }
        }
        CRC64Table[i] = Result;
    }
    CRC64Table[256] = Polynomial;
}

UINT64 CRC64::GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT64 GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == nullptr) return 0;
    UINT64 Result = 0;
    BYTE* pResult = (BYTE*)&Result;
    if(GenerationPolynomial != CRC64Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) Result = CRC64Table[pResult[0] ^ srcData[i]] ^
                                                       (UINT64)pResult[1] ^
                                                       (UINT64)pResult[2] << 8 ^
                                                       (UINT64)pResult[3] << 16 ^
                                                       (UINT64)pResult[4] << 24 ^
                                                       (UINT64)pResult[5] << 32 ^
                                                       (UINT64)pResult[6] << 40 ^
                                                       (UINT64)pResult[7] << 48;
    return Result;
}

bool CRC64::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT64 GenerationPolynomial, UINT64 CRC64Checksum) {
    return CRC64Checksum == GetChecksum(srcData, srcDataLength, GenerationPolynomial) ? true : false;
}
