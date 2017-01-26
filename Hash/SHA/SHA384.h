#ifndef SHA384_H_INCLUDED
#define SHA384_H_INCLUDED
#include "../../TypeDefine.h"

typedef struct _SHA384HashResult {
    UINT64 A;
    UINT64 B;
    UINT64 C;
    UINT64 D;
    UINT64 E;
    UINT64 F;
} SHA384HashResult;

class SHA384 {
private:
    static UINT64 K[80];
    SHA384() = delete;
public:
    static SHA384HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
};
#endif //SHA384_H_INCLUDED
