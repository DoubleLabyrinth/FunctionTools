#ifndef CRC16_H_INCLUDED
#define CRC16_H_INCLUDED
#include "../../TypeDefine.h"

class CRC16 {
private:
    static UINT16 CRC16Table[257];

    CRC16() = delete;
    static void GenerateTable(UINT16 Polynomial);
public:
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits.*/
    static UINT16 GetChecksum(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial);
    /**Here, GenerationPolynomial should use Big-Endian, which means that you should input 0x5623 if you want to set GenerationPolynomial 0x2356 where 0x23 is high 8-bits and 0x56 is low 8-bits. So does CRC16Checksum.*/
    static bool CheckData(const BYTE* srcData, UINT64 srcDataLength, UINT16 GenerationPolynomial, UINT16 CRC16Checksum);
};

#endif //CRC16_H_INCLUDED
