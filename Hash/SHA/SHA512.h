#ifndef SHA512_H_INCLUDED
#define SHA512_H_INCLUDED
#include "../../TypeDefine.h"

typedef struct _SHA512HashResult {
    UINT64 A;
    UINT64 B;
    UINT64 C;
    UINT64 D;
    UINT64 E;
    UINT64 F;
    UINT64 G;
    UINT64 H;
} SHA512HashResult;

class SHA512 {
private:
    static UINT64 K[80];
    SHA512() = delete;
public:
    static SHA512HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
};
#endif //SHA512_H_INCLUDED
