#include "3DES.h"

void Encryption::DES::TripleDES::Encipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]) {
    Encryption::DES::Encipher(srcBytes, K1);
    Decryption::DES::Decipher(srcBytes, K2);
    Encryption::DES::Encipher(srcBytes, K3);
}
