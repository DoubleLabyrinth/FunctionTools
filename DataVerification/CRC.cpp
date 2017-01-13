#include "CRC.h"

BYTE CRC8::CRC8Table[] = {0};
void CRC8::GenerateTable(BYTE Polynomial) {
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        BYTE Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= (Polynomial >> j);
                Result ^= Polynomial << (8 - j);
            }
        }
        CRC8Table[i] = Result;
    }
    CRC8Table[256] = Polynomial;
}

BYTE CRC8::Calculate(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == NULL) return 0;
    BYTE Result = 0;
    if(GenerationPolynomial != CRC8Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) {
        Result = CRC8Table[Result] ^ srcData[i];
    }
    return CRC8Table[Result];
}

BOOL CRC8::CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Value) {
    return CRC8Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

UINT16 CRC16::CRC16Table[257] = {0};
void CRC16::GenerateTable(UINT16 Polynomial) {
    BYTE Polynomial_H = *(BYTE*)&Polynomial;
    BYTE Polynomial_L = *(BYTE*)(&Polynomial + 1);
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT16 Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= (Polynomial_H >> j);
                *(BYTE*)&Result ^= ((Polynomial_H << (8 - j)) + (Polynomial_L >> j));
                *(BYTE*)(&Result + 1) ^= Polynomial_L << (8 - j);
            }
        }
        CRC16Table[i] = Result;
    }
    CRC16Table[256] = Polynomial;
}

UINT16 CRC16::Calculate(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == NULL) return 0;
    UINT16 Result = 0;
    if(GenerationPolynomial != CRC16Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) {
        *(*BYTE)&Result = CRC16Table[*(*BYTE)&Result] ^ srcData[i];
        *(*BYTE)(&Result + 1) = CRC16Table[*(*BYTE)(&Result + 1)] ^ srcData[i + 1];
    }

}

BOOL CRC16::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Value) {
    return CRC16Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

BYTE CRC32::CRC32Table_HH[257] = {0};
BYTE CRC32::CRC32Table_HL[257] = {0};
BYTE CRC32::CRC32Table_LH[257] = {0};
BYTE CRC32::CRC32Table_LL[257] = {0};
void CRC32::GenerateTable(UINT32 Polynomial) {

}

UINT32 CRC32::Calculate(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial) {

}

BOOL CRC32::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Value) {
    return CRC32Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

