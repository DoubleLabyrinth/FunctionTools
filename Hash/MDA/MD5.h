#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED
#include "../../TypeDefine.h"
#include "MDA.h"

namespace Hash {
    namespace MDA {
        namespace MD5 {
            Hash::MDA::HashResult GetChecksum(const BYTE* srcByte, UINT64 srcByteLength);
        }
    }
}

#endif // MD5_H_INCLUDED
