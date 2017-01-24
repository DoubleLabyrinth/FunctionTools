#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED
#include "../../TypeDefine.h"

class CRC32 {
private:
    static UINT32 CRC32Table[257];

    CRC32() = delete;
    static void GenerateTable(UINT32 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits.*/
    static UINT32 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x78563412 if you want to set GenerationPolynomial 0x12345678 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits. So does CRC32Checksum.*/
    static bool CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT32 GenerationPolynomial, UINT32 CRC32Checksum);
};

#endif //CRC32_H_INCLUDED
