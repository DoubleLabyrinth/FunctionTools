#ifndef AES192_ENCRYPTION_H_INCLUDED
#define AES192_ENCRYPTION_H_INCLUDED
#include "../../TypeDefine.h"
#include "AES.h"

namespace Encryption {
    namespace AES {
        namespace AES192 {
            void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[52]);
            void KeyExpansion(BYTE srcKey[24], UINT32 dstExpandedKey[52]);
        }
    }
}

#endif // AES192_ENCRYPTION_H_INCLUDED
