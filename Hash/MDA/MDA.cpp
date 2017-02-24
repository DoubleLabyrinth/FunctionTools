#include "MDA.h"

void Hash::MDA::GetHashString(HashResult srcHashResult, char* out_CharString, bool UseUppercase) {
    char TransTable[] = "0123456789ABCDEF";
    if(!UseUppercase) for(BYTE i = 10; i < 16; i++) TransTable[i] = 'a' - 10 + i;
    BYTE* Ptr = (BYTE*)&srcHashResult;
    for(BYTE i = 0; i < 32; i += 2) out_CharString[i] = TransTable[(Ptr[i >> 1] & 0xF0) >> 4];
    for(BYTE i = 1; i < 32; i += 2) out_CharString[i] = TransTable[Ptr[i >> 1] & 0x0F];
}
