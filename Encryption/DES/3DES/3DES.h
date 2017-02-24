#ifndef TRIPLEDES_ENCRYPTION_H_INCLUDED
#define TRIPLEDES_ENCRYPTION_H_INCLUDED
#include "../../../TypeDefine.h"
#include "../DES.h"
#include "../../../Decryption/DES/DES.h"

namespace Encryption {
    namespace DES {
        namespace TripleDES {
            /**About how to choose K1, K2 and K3:
             * Actually TripleDES acts as below:
             *      1. srcBytes is encrypted by DES using key K1.
             *      2. srcBytes is decrypted by DES using key K2.
             *      3. srcBytes is encrypted by DES using key K3.
             *      4. After 3 operations above, srcBytes is encrypted by TripleDES.
             *
             * So, there are 5 ways to choose K1, K2 and K3.
             *      1. K1 != K2 != K3. This is the safest.
             *      2. K1 == K2 != K3. Actually srcBytes is just encrypted by DES using key K3.
             *      3. K1 != K2 == K3. Actually srcBytes is just encrypted by DES using key K1.
             *      4. K1 == K3 != K2. Better then 2 and 3, but worse then 1.
             *      5. K1 == K2 == K3. Actually srcBytes is just encrypted by DES using key K3.
             *
             * In order to keep data security, you'd better choose three keys that are totally independent.
             */
            void Encipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]);
        }
    }
}

#endif //TRIPLEDES_ENCRYPTION_H_INCLUDED
