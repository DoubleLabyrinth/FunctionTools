#include "3DES.h"

void Decryption::DES::TripleDES::Decipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]) {
    Decryption::DES::Decipher(srcBytes, K3);
    Encryption::DES::Encipher(srcBytes, K2);
    Decryption::DES::Decipher(srcBytes, K1);
}
