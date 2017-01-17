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
