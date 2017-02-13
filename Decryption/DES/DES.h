#ifndef DES_DECRYPTION_H_INCLUDED
#define DES_DECRYPTION_H_INCLUDED
#include "../../TypeDefine.h"

class DESDecryption {
private:
    static const BYTE S1[4][16];
    static const BYTE S2[4][16];
    static const BYTE S3[4][16];
    static const BYTE S4[4][16];
    static const BYTE S5[4][16];
    static const BYTE S6[4][16];
    static const BYTE S7[4][16];
    static const BYTE S8[4][16];
    DESDecryption() = delete;
public:
    static void Decipher(BYTE srcBytes[8], BYTE K[16][6]);
    static bool GenerateK(BYTE CipherKey[8], BYTE out_K[16][6]);  /*It means failed if function return false.
                                                                    If and only if the number of bit '1' contained in each byte of array CipherKey is not odd, the function will return false.*/
};

#endif //DES_DECRYPTION_H_INCLUDED
