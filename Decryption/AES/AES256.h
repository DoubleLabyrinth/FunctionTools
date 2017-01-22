#ifndef AES256_DECRYPTION_H_INCLUDED
#define AES256_DECRYPTION_H_INCLUDED
#include "../../TypeDefine.h"

class AES256Decryption {
private:
    static BYTE SBox[256];
    static BYTE InverseSBox[256];
    static UINT32 Rcon[11];
    static BYTE Multiply0x09[256];
    static BYTE Multiply0x0B[256];
    static BYTE Multiply0x0D[256];
    static BYTE Multiply0x0E[256];
    AES256Decryption();
public:
    static void InverseCipher(BYTE srcBytes[16], UINT32 srcExpandedKey[60]);
    static void KeyExpansion(BYTE srcKey[32], UINT32 dstExpandedKey[60]);
};

#endif // AES256_DECRYPTION_H_INCLUDED
