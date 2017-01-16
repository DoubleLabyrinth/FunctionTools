#ifndef MD4_H_INCLUDED
#define MD4_H_INCLUDED
#include "MD_Define.h"
#include <windows.h>

class MD4 {
private:
    MD4();
public:
    static HashResult FromByte(const BYTE* srcByte, UINT64 ByteLength);
};

#endif // MD4_H_INCLUDED
