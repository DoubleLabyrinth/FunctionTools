#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED
#include <windows.h>

class CRC8 {
private:
    static BYTE CRC8Table[257];

    CRC8();
    static void GenerateTable(BYTE Polynomial);
public:
    static BYTE Calculate(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial);
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Value);
};

class CRC16 {
private:
    static UINT16 CRC16Table[257];

    CRC16();
    static void GenerateTable(UINT16 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits.*/
    static UINT16 Calculate(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits. So does CRC16Value.*/
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Value);
};

class CRC32 {
private:
    static BYTE CRC32Table_HH[257];
    static BYTE CRC32Table_HL[257];
    static BYTE CRC32Table_LH[257];
    static BYTE CRC32Table_LL[257];

    CRC32();
    static void GenerateTable(UINT32 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits.*/
    static UINT32 Calculate(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits. So does CRC32Value.*/
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Value);
};

#endif // CRC_H_INCLUDED
