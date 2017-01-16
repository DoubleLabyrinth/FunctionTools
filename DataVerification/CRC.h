#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED
#include <windows.h>

class CRC8 {
private:
    static BYTE CRC8Table[257];

    CRC8();
    static void GenerateTable(BYTE Polynomial);
public:
    static BYTE GetChecksum(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial);
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Checksum);
};

class CRC16 {
private:
    static UINT16 CRC16Table[257];

    CRC16();
    static void GenerateTable(UINT16 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits.*/
    static UINT16 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits. So does CRC16Checksum.*/
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Checksum);
};

class CRC32 {
private:
    static UINT32 CRC32Table[257];

    CRC32();
    static void GenerateTable(UINT32 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits.*/
    static UINT32 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits. So does CRC32Checksum.*/
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Checksum);
};

class CRC64 {
private:
    static UINT64 CRC64Table[257];

    CRC64();
    static void GenerateTable(UINT64 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0xF0DEBC9A78563412 if you want to set GenerationPolynomial 0x123456789ABCDEF0 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits.*/
    static UINT64 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT64 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0xF0DEBC9A78563412 if you want to set GenerationPolynomial 0x123456789ABCDEF0 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits. So does CRC64Value*/
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT64 Generationpolynomial, UINT64 CRC64Checksum);
};
#endif // CRC_H_INCLUDED
