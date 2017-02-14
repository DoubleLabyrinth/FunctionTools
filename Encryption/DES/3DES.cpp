#include "3DES.h"

void TripleDESEncryption::Encipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]) {
    DESEncryption::Encipher(srcBytes, K1);
    DESDecryption::Decipher(srcBytes, K2);
    DESEncryption::Encipher(srcBytes, K3);
}

bool TripleDESEncryption::GenerateK(BYTE CipherKey[8], BYTE out_K[16][6]) {
    return DESEncryption::GenerateK(CipherKey, out_K);

}
