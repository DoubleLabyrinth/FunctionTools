#ifndef SHA1_H_INCLUDED
#define SHA1_H_INCLUDED
#include "../../TypeDefine.h"

namespace Hash {
    namespace SHA {
        namespace SHA1 {
            typedef struct _SHA1HashResult {
                UINT32 A;
                UINT32 B;
                UINT32 C;
                UINT32 D;
                UINT32 E;
            } SHA1HashResult;

            SHA1HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcBytesLength);
        }
    }
}

#endif //SHA1_H_INCLUDED
