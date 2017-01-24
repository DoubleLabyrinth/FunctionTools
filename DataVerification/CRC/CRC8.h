#ifndef CRC8_H_INCLUDED
#define CRC8_H_INCLUDED
#include "../../TypeDefine.h"

class CRC8 {
private:
    static BYTE CRC8Table[257];

    CRC8() = delete;
    static void GenerateTable(BYTE Polynomial);
public:
    static BYTE GetChecksum(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial);
    static bool CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE GenerationPolynomial, BYTE CRC8Checksum);
};

#endif // CRC8_H_INCLUDED
