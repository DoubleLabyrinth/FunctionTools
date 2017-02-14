#include "3DES.h"

void TripleDESDecryption::Decipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]) {
    DESDecryption::Decipher(srcBytes, K3);
    DESEncryption::Encipher(srcBytes, K2);
    DESDecryption::Decipher(srcBytes, K1);
}

bool TripleDESDecryption::GenerateK(BYTE CipherKey[8], BYTE out_K[16][6]) {
    return DESEncryption::GenerateK(CipherKey, out_K);

}
