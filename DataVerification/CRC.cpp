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
        Result = *(CRC8Table + (Result ^ srcData[i]));
    }
    return Result;
}

BOOL CRC8::CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Value) {
    return CRC8Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

UINT16 CRC16::CRC16Table[257] = {0};
void CRC16::GenerateTable(UINT16 Polynomial) {
    BYTE Polynomial_H = *(BYTE*)&Polynomial;
    BYTE Polynomial_L = *((BYTE*)&Polynomial + 1);
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT16 Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= (Polynomial_H >> j);
                *(BYTE*)&Result ^= ((Polynomial_H << (8 - j)) + (Polynomial_L >> j));
                *((BYTE*)&Result + 1) ^= Polynomial_L << (8 - j);
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
        Result = *(CRC16Table + (*(BYTE*)&Result ^ srcData[i])) ^ (UINT16)(*((BYTE*)&Result + 1));
    }
    return Result;
}

BOOL CRC16::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Value) {
    return CRC16Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

UINT32 CRC32::CRC32Table[257] = {0};
void CRC32::GenerateTable(UINT32 Polynomial) {
    BYTE Polynomial_HH = *(BYTE*)&Polynomial;
    BYTE Polynomial_HL = *((BYTE*)&Polynomial + 1);
    BYTE Polynomial_LH = *((BYTE*)&Polynomial + 2);
    BYTE Polynomial_LL = *((BYTE*)&Polynomial + 3);
    for(UINT16 i = 0; i < 0x100; i++) {
        BYTE Present_i = (BYTE)i;
        UINT32 Result = 0;
        for(BYTE j = 1, pointer; j <= 8; j++) {
            pointer = 0x80 >> (j - 1);
            if(pointer & Present_i) {
                Present_i ^= pointer;
                Present_i ^= (Polynomial_HH >> j);
                *(BYTE*)&Result ^= ((Polynomial_HH << (8 - j)) + (Polynomial_HL >> j));
                *((BYTE*)&Result + 1) ^= ((Polynomial_HL << (8 - j)) + (Polynomial_LH >> j));
                *((BYTE*)&Result + 2) ^= ((Polynomial_LH << (8 - j)) + (Polynomial_LL >> j));
                *((BYTE*)&Result + 3) ^= Polynomial_LL << (8 - j);
            }
        }
        CRC32Table[i] = Result;
    }
    CRC32Table[256] = Polynomial;
}

UINT32 CRC32::Calculate(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial) {
    if(srcDataLength == 0 || srcData == NULL) return 0;
    UINT32 Result = 0;
    if(GenerationPolynomial != CRC32Table[256]) GenerateTable(GenerationPolynomial);
    for(UINT64 i = 0; i < srcDataLength; i++) {
        Result = *(CRC32Table + (*(BYTE*)&Result ^ srcData[i])) ^ (UINT32)(*((BYTE*)&Result + 1)) ^ ((UINT32)(*((BYTE*)&Result + 2)) << 8) ^ ((UINT32)(*((BYTE*)&Result + 3)) << 16);
    }
    return Result;
}

BOOL CRC32::CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Value) {
    return CRC32Value == Calculate(srcData, srcDataLength, GenerationPolynomial) ? TRUE : FALSE;
}

