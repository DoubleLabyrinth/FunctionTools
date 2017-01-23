#ifndef BASE16_H_INCLUDED
#define BASE16_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base16 {
private:
    static BYTE Base16Table[16];
    Base16();
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength);
};
#endif // BASE16_H_INCLUDED
