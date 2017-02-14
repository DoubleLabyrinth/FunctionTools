#ifndef TRIPLEDES_DECRYPTION_H_INCLUDED
#define TRIPLEDES_DECRYPTION_H_INCLUDED
#include "DES.h"
#include "../../Encryption/DES/DES.h"

class TripleDESDecryption {
private:
    TripleDESDecryption() = delete;
public:
    static void Decipher(BYTE srcBytes[8], BYTE K1[16][6], BYTE K2[16][6], BYTE K3[16][6]);

    /**It means failed if function return false.
     * If and only if the number of bit '1' contained in each byte of array CipherKey is not odd, the function will return false.
     */
    static bool GenerateK(BYTE CipherKey[8], BYTE out_K[16][6]);
};

#endif //TRIPLEDES_DECRYPTION_H_INCLUDED
