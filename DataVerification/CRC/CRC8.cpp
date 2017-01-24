#include "CRC8.h"

BYTE CRC8::CRC8Table[] = {0};

void CRC8::GenerateTable(BYTE Polynomial) {
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        BYTE Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= Polynomial >> j;
                Result ^= Polynomial << (8 - j);
            }
        }
        CRC8Table[i] = Result;
    }
    CRC8Table[256] = Polynomial;
}

BYTE CRC8::GetChecksum(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == nullptr) return 0;
    BYTE Result = 0;
    if(GenerationPolynomial != CRC8Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) Result = CRC8Table[Result ^ srcData[i]];
    return Result;
}

bool CRC8::CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Checksum) {
    return CRC8Checksum == GetChecksum(srcData, srcDataLength, GenerationPolynomial) ? true : false;
}
