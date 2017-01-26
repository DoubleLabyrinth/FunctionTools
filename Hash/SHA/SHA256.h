#ifndef SHA256_H_INCLUDED
#define SHA256_H_INCLUDED
#include "../../TypeDefine.h"

typedef struct _SHA256HashResult {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
    UINT32 E;
    UINT32 F;
    UINT32 G;
    UINT32 H;
} SHA256HashResult;

class SHA256 {
private:
    static UINT32 K[64];
    SHA256() = delete;
public:
    static SHA256HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
};
#endif //SHA256_H_INCLUDED
