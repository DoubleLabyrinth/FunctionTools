#include "CRC16.h"

UINT16 CRC16::CRC16Table[257] = {0};

void CRC16::GenerateTable(UINT16 Polynomial) {
    BYTE* pPolynomial = (BYTE*)&Polynomial;
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT16 Result = 0;
        BYTE* pResult = (BYTE*)&Result;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= pPolynomial[0] >> j;
                pResult[0] ^= (pPolynomial[0] << (8 - j)) + (pPolynomial[1] >> j);
                pResult[1] ^= pPolynomial[1] << (8 - j);
            }
        }
        CRC16Table[i] = Result;
    }
    CRC16Table[256] = Polynomial;
}

UINT16 CRC16::GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == nullptr) return 0;
    UINT16 Result = 0;
    BYTE* pResult = (BYTE*)&Result;
    if(GenerationPolynomial != CRC16Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) Result = CRC16Table[pResult[0] ^ srcData[i]] ^
                                                       (UINT16)pResult[1];
    return Result;
}

bool CRC16::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Checksum) {
    return CRC16Checksum == GetChecksum(srcData, srcDataLength, GenerationPolynomial) ? true : false;
}
