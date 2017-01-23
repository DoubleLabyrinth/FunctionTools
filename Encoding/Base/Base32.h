#ifndef BASE32_H_INCLUDED
#define BASE32_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base32 {
private:
    static char Base32Table[32];
    Base32();
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength);
};


#endif // BASE32_H_INCLUDED
