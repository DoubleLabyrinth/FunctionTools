#ifndef AES192_ENCRYPTION_H_INCLUDED
#define AES192_ENCRYPTION_H_INCLUDED
#include "../../TypeDefine.h"

class AES192Encryption {
private:
    static BYTE SBox[256];
    static UINT32 Rcon[11];
    static BYTE Multiply0x02[256];
    static BYTE Multiply0x03[256];
    AES192Encryption();
public:
    static void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[52]);
    static void KeyExpansion(BYTE srcKey[24], UINT32 dstExpandedKey[52]);
};

#endif // AES192_ENCRYPTION_H_INCLUDED
