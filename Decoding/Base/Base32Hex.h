#ifndef BASE32HEX_DECODING_H_INCLUDED
#define BASE32HEX_DECODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"
#include "../DecodedBytesDefine.h"

class Base32HexDecoding {
private:
    static BYTE InverseBase32HexTable[256];
    Base32HexDecoding() = delete;
public:
    static DecodedBytes GetDecodedBytes(const char* srcBase32String, UINT64 srcStringLength);
};

#endif //BASE32HEX_DECODING_H_INCLUDED
