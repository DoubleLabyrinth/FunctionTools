#ifndef BASE64_DECODING_H_INCLUDED
#define BASE64_DECODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"
#include "../DecodedBytesDefine.h"

class Base64Decoding {
private:
    static BYTE InverseBase64Table[256];
    static BYTE URL_Filename_Safe_InverseBase64Table[256];
    Base64Decoding() = delete;
public:
    static DecodedBytes GetDecodedBytes(const char* srcBase64String, UINT64 srcStringLength, bool URL_Filename_Safe = false);
};

#endif //BASE64_DECODING_H_INCLUDED
