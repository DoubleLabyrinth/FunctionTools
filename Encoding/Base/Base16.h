#ifndef BASE16_ENCODING_H_INCLUDED
#define BASE16_ENCODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base16Encoding {
private:
    static UINT16 Base16Table[256];
    Base16Encoding() = delete;
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength);
};
#endif // BASE16_ENCODING_H_INCLUDED
