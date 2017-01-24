#ifndef BASE16_DECODING_H_INCLUDED
#define BASE16_DECODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"
#include "../DecodedBytesDefine.h"

class Base16Decoding {
private:
    static BYTE InverseBase16Table[0x47];   //So, the largest index is 0x46 which is 'F';
    Base16Decoding();
public:
    static DecodedBytes GetDecodedBytes(const char* srcBase16String, UINT64 srcStringLength);
};

#endif //BASE16_DECODING_H_INCLUDED
