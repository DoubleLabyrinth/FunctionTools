#ifndef BASE32HEX_H_INCLUDED
#define BASE32HEX_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base32Hex {
private:
    static char Base32HexTable[32];
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength);
};

#endif // BASE32HEX_H_INCLUDED
