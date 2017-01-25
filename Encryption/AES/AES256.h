#ifndef AES256_ENCRYPTION_H_INCLUDED
#define AES256_ENCRYPTION_H_INCLUDED
#include "../../TypeDefine.h"

class AES256Encryption {
private:
    static BYTE SBox[256];
    static UINT32 Rcon[11];
    static BYTE Multiply0x02[256];
    static BYTE Multiply0x03[256];
    AES256Encryption() = delete;
public:
    static void Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[60]);
    static void KeyExpansion(BYTE srcKey[32], UINT32 dstExpandedKey[60]);
};

#endif // AES256_ENCRYPTION_H_INCLUDED
