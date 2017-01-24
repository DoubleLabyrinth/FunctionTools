#ifndef CRC64_H_INCLUDED
#define CRC64_H_INCLUDED
#include "../../TypeDefine.h"

class CRC64 {
private:
    static UINT64 CRC64Table[257];

    CRC64() = delete;
    static void GenerateTable(UINT64 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0xF0DEBC9A78563412 if you want to set GenerationPolynomial 0x123456789ABCDEF0 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits.*/
    static UINT64 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT64 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0xF0DEBC9A78563412 if you want to set GenerationPolynomial 0x123456789ABCDEF0 where 0x12 is the highest 8-bits and 0x78 is lowest 8-bits. So does CRC64Value*/
    static bool CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT64 Generationpolynomial, UINT64 CRC64Checksum);
};

#endif //CRC64_H_INCLUDED
