#ifndef MDA_H_INCLUDED
#define MDA_H_INCLUDED
#include "../../TypeDefine.h"

namespace Hash {
    namespace MDA {
        typedef struct _HashResult {
            UINT32 A;
            UINT32 B;
            UINT32 C;
            UINT32 D;
        } HashResult;

        void GetHashString(HashResult srcHashResult, char* out_CharString, bool UseUppercase = true);
    }
}


#endif //MDA_H_INCLUDED
