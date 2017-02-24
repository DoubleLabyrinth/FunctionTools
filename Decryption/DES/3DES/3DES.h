#ifndef TRIPLEDES_DECRYPTION_H_INCLUDED
#define TRIPLEDES_DECRYPTION_H_INCLUDED
#include "../../../TypeDefine.h"
#include "../DES.h"
#include "../../../Encryption/DES/DES.h"

namespace Decryption {
    namespace DES {
        namespace TripleDES {
            void Decipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]);
        }
    }
}

#endif //TRIPLEDES_DECRYPTION_H_INCLUDED
