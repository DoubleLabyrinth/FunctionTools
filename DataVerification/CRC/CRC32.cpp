#include "CRC32.h"

UINT32 CRC32::CRC32Table[257] = {0};

void CRC32::GenerateTable(UINT32 Polynomial) {
    BYTE* pPolynomial = (BYTE*)&Polynomial;
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT32 Result = 0;
        BYTE* pResult = (BYTE*)&Result;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= pPolynomial[0] >> j;
                pResult[0] ^= (pPolynomial[0] << (8 - j)) + (pPolynomial[1] >> j);
                pResult[1] ^= (pPolynomial[1] << (8 - j)) + (pPolynomial[2] >> j);
                pResult[2] ^= (pPolynomial[2] << (8 - j)) + (pPolynomial[3] >> j);
                pResult[3] ^= pPolynomial[3] << (8 - j);
            }
        }
        CRC32Table[i] = Result;
    }
    CRC32Table[256] = Polynomial;
}

UINT32 CRC32::GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == nullptr) return 0;
    UINT32 Result = 0;
    BYTE* pResult = (BYTE*)&Result;
    if(GenerationPolynomial != CRC32Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) Result = CRC32Table[pResult[0] ^ srcData[i]] ^
                                                       (UINT32)pResult[1] ^
                                                       (UINT32)pResult[2] << 8 ^
                                                       (UINT32)pResult[3] << 16;
    return Result;
}

bool CRC32::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Checksum) {
    return CRC32Checksum == GetChecksum(srcData, srcDataLength, GenerationPolynomial) ? true : false;
}
