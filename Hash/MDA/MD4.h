#ifndef MD4_H_INCLUDED
#define MD4_H_INCLUDED
#include "../../TypeDefine.h"
#include "MDA.h"

namespace Hash {
    namespace MDA {
        namespace MD4 {
            Hash::MDA::HashResult GetChecksum(const BYTE* srcBytes, UINT64 srcByteLength);
        }
    }
}

#endif // MD4_H_INCLUDED
