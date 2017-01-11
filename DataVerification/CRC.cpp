#include "CRC.h"

BYTE CRC8::CRC8Table[] = {0};
void CRC8::GenerateTable(BYTE Polynomial) {
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE A = (BYTE)i;
        BYTE Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & A) {
                A ^= pointer;
                A ^= (Polynomial >> j);
                Result ^= Polynomial << (8 - j);
            }
        }
        CRC8Table[i] = Result;
    }
    CRC8Table[256] = Polynomial;
}

BYTE CRC8::Calculate(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial) {
    BYTE Result = srcData[0];
    if(GenerationPolynomial != CRC8Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 1; i < srcDataLength; i++) {
        Result = CRC8Table[Result] ^ srcData[i];
    }
    return CRC8Table[Result];
}

BOOL CRC8::CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRCValue) {
    return CRCValue == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}
