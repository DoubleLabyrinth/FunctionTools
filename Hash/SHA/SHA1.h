#ifndef SHA1_H_INCLUDED
#define SHA1_H_INCLUDED
#include "../../TypeDefine.h"

typedef struct _SHA1HashResult {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
    UINT32 E;
} SHA1HashResult;

class SHA1 {
private:
    SHA1() = delete;
public:
    static SHA1HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
};

#endif //SHA1_H_INCLUDED
