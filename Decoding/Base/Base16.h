#ifndef BASE16_DECODING_H_INCLUDED
#define BASE16_DECODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base16Decoding {
private:
    static BYTE InverseBase16Table[0x47];   //So, the largest index is 0x46 which is 'F';
    Base16Decoding();
public:
    static SmartPointer<char> GetDecodedString(const BYTE* srcBytes, UINT64 BytesLength);
};

#endif //BASE16_DECODING_H_INCLUDED
