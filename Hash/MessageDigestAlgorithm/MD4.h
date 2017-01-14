#ifndef MD4_H
#define MD4_H
#include <windows.h>

typedef struct _HashResult {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
} HashResult, LPHashResult;

class MD4 {
private:
    MD4();
public:
    static HashResult FromByte(const BYTE* srcByte, UINT64 ByteLength);
};

#endif // MD4_H
