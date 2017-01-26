#ifndef SHA224_H_INCLUDED
#define SHA224_H_INCLUDED
#include "../../TypeDefine.h"

typedef struct _SHA224HashResult {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
    UINT32 E;
    UINT32 F;
    UINT32 G;
} SHA224HashResult;

class SHA224 {
private:
    static UINT32 K[64];
    SHA224() = delete;
public:
    static SHA224HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
};
#endif //SHA224_H_INCLUDED
