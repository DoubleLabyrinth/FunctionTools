#ifndef BASE64_ENCODING_H_INCLUDED
#define BASE64_ENCODING_H_INCLUDED
#include "../../TypeDefine.h"
#include "../../SmartPointer.h"

class Base64Encoding {
private:
    static char Base64Table[64];
    static char URL_Filename_Safe_Base64Table[64];
    Base64Encoding() = delete;
public:
    static SmartPointer<char> GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength, bool URL_Filename_Safe = false);
};

#endif // BASE64_ENCODING_H_INCLUDED
