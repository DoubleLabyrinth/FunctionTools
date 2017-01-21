#ifndef AES128_H_INCLUDED
#define AES128_H_INCLUDED
#include "../TypeDefine.h"

class AES128 {
private:
    static BYTE SBox[256];
    static UINT32 Rcon[11];
    AES128();
public:
    static void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[44]);
    static void KeyExpansion(BYTE srcKey[16], UINT32 dstExpandedKey[44]);
};

#endif // AES128_H_INCLUDED
