#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED
#include "MD_Define.h"

class MD5 {
private:
    static char HashString[33];
    MD5() = delete;
public:
    static HashResult GetChecksum(const BYTE* srcByte, UINT64 srcByteLength);
    static const char* GetHashString(HashResult srcHashResult, bool UseUppercase = true);
};

#endif // MD5_H_INCLUDED
