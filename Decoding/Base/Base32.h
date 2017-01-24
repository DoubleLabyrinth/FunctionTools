#ifndef BASE32_DECODING_H_INCLUDED
#define BASE32_DECODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"
#include "../DecodedBytesDefine.h"

class Base32Decoding {
private:
    static BYTE InverseBase32Table[0x5B];   //So, the largest index is 0x5A which is 'Z';
    Base32Decoding() = delete;
public:
    static DecodedBytes GetDecodedBytes(const char* srcBase32String, UINT64 srcStringLength);
};

#endif //BASE32_DECODING_H_INCLUDED
