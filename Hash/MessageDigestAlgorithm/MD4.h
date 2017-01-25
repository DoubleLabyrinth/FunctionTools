#ifndef MD4_H_INCLUDED
#define MD4_H_INCLUDED
#include "MD_Define.h"

class MD4 {
private:
    static char HashString[33];
    MD4() = delete;
public:
    static HashResult GetChecksum(const BYTE* srcByte, UINT64 srcByteLength);
    static const char* GetHashString(HashResult srcHashResult, bool UseUppercase = true);
};

#endif // MD4_H_INCLUDED
