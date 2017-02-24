#ifndef AES256_ENCRYPTION_H_INCLUDED
#define AES256_ENCRYPTION_H_INCLUDED
#include "../../TypeDefine.h"
#include "AES.h"

namespace Encryption {
    namespace AES {
        namespace AES256 {
            void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[60]);
            void KeyExpansion(BYTE srcKey[32], UINT32 dstExpandedKey[60]);
        }
    }
}

#endif // AES256_ENCRYPTION_H_INCLUDED
