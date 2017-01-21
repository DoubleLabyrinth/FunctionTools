#ifndef AES128_ENCRYPTION_H_INCLUDED
#define AES128_ENCRYPTION_H_INCLUDED
#include "../TypeDefine.h"

class AES128Encryption {
private:
    static BYTE SBox[256];
    static UINT32 Rcon[11];
    static BYTE Multiply0x02[256];
    static BYTE Multiply0x03[256];
    AES128Encryption();
public:
    static void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[44]);
    static void KeyExpansion(BYTE srcKey[16], UINT32 dstExpandedKey[44]);
};

#endif // AES128_ENCRYPTION_H_INCLUDED
