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


#endif // CRC_H_INCLUDED
