#ifndef AES128_ENCRYPTION_H_INCLUDED
#define AES128_ENCRYPTION_H_INCLUDED
#include "../../TypeDefine.h"
#include "AES.h"

namespace Encryption {
    namespace AES {
        namespace AES128 {
            void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[44]);
            void KeyExpansion(BYTE srcKey[16], UINT32 dstExpandedKey[44]);
        }
    }
}

#endif // AES128_ENCRYPTION_H_INCLUDED
