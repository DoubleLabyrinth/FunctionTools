#ifndef BASE32_ENCODING_H_INCLUDED
#define BASE32_ENCODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base32Encoding {
private:
    static char Base32Table[32];
    Base32Encoding() = delete;
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength);
};


#endif // BASE32_ENCODING_H_INCLUDED
