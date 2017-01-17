#include "MD5.h"

HashResult MD5::GetChecksum(const BYTE* srcByte, UINT64 srcByteLength) {
    UINT32 A = 0x67452301;
    UINT32 B = 0xEFCDAB89;
    UINT32 C = 0x98BADCFE;
    UINT32 D = 0x10325476;
    UINT32 AA, BB, CC, DD;
    bool Final = false;
    BYTE Extra[128] = {0};

    UINT64 BitsOfsrcByte = srcByteLength << 3;
    UINT64 TimesOfMainCycle = srcByteLength >> 6;
    BYTE LeftoversrcByteLength = (BYTE)(srcByteLength & 0x3F);
    const UINT32(*Ptr)[16] = (const UINT32(*)[16])srcByte;
fin:if(Final) {
        TimesOfMainCycle = LeftoversrcByteLength >= 56 ? 2 : 1;
        Ptr = (const UINT32(*)[16])Extra;
    }
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        AA = A;
        BB = B;
        CC = C;
        DD = D;

        MD5_FF(A, B, C, D, Ptr[i][0], 7, 0xD76AA478)  //MD5_FF is a MACRO FUNCTION, no need to add ';'
        MD5_FF(D, A, B, C, Ptr[i][1], 12, 0xE8C7B756)
        MD5_FF(C, D, A, B, Ptr[i][2], 17, 0x242070DB)
        MD5_FF(B, C, D, A, Ptr[i][3], 22, 0xC1BDCEEE)
        MD5_FF(A, B, C, D, Ptr[i][4], 7, 0xF57C0FAF)
        MD5_FF(D, A, B, C, Ptr[i][5], 12, 0x4787C62A)
        MD5_FF(C, D, A, B, Ptr[i][6], 17, 0xA8304613)
        MD5_FF(B, C, D, A, Ptr[i][7], 22, 0xFD469501)
        MD5_FF(A, B, C, D, Ptr[i][8], 7, 0x698098D8)
        MD5_FF(D, A, B, C, Ptr[i][9], 12, 0x8B44F7AF)
        MD5_FF(C, D, A, B, Ptr[i][10], 17, 0xFFFF5BB1)
        MD5_FF(B, C, D, A, Ptr[i][11], 22, 0x895CD7BE)
        MD5_FF(A, B, C, D, Ptr[i][12], 7, 0x6B901122)
        MD5_FF(D, A, B, C, Ptr[i][13], 12, 0xFD987193)
        MD5_FF(C, D, A, B, Ptr[i][14], 17, 0xA679438E)
        MD5_FF(B, C, D, A, Ptr[i][15], 22, 0x49B40821)

        MD5_GG(A, B, C, D, Ptr[i][1], 5, 0xF61E2562)
        MD5_GG(D, A, B, C, Ptr[i][6], 9, 0xC040B340)
        MD5_GG(C, D, A, B, Ptr[i][11], 14, 0x265E5A51)
        MD5_GG(B, C, D, A, Ptr[i][0], 20, 0xE9B6C7AA)
        MD5_GG(A, B, C, D, Ptr[i][5], 5, 0xD62F105D)
        MD5_GG(D, A, B, C, Ptr[i][10], 9, 0x02441453)
        MD5_GG(C, D, A, B, Ptr[i][15], 14, 0xD8A1E681)
        MD5_GG(B, C, D, A, Ptr[i][4], 20, 0xE7D3FBC8)
        MD5_GG(A, B, C, D, Ptr[i][9], 5, 0x21E1CDE6)
        MD5_GG(D, A, B, C, Ptr[i][14], 9, 0xC33707D6)
        MD5_GG(C, D, A, B, Ptr[i][3], 14, 0xF4D50D87)
        MD5_GG(B, C, D, A, Ptr[i][8], 20, 0x455A14ED)
        MD5_GG(A, B, C, D, Ptr[i][13], 5, 0xA9E3E905)
        MD5_GG(D, A, B, C, Ptr[i][2], 9, 0xFCEFA3F8)
        MD5_GG(C, D, A, B, Ptr[i][7], 14, 0x676F02D9)
        MD5_GG(B, C, D, A, Ptr[i][12], 20, 0x8D2A4C8A)

        MD5_HH(A ,B ,C ,D , Ptr[i][5], 4, 0xFFFA3942)
        MD5_HH(D ,A ,B ,C , Ptr[i][8], 11, 0x8771F681)
        MD5_HH(C ,D ,A ,B , Ptr[i][11], 16, 0x6D9D6122)
        MD5_HH(B ,C ,D ,A , Ptr[i][14], 23, 0xFDE5380C)
        MD5_HH(A ,B ,C ,D , Ptr[i][1], 4, 0xA4BEEA44)
        MD5_HH(D ,A ,B ,C , Ptr[i][4], 11, 0x4BDECFA9)
        MD5_HH(C ,D ,A ,B , Ptr[i][7], 16, 0xF6BB4B60)
        MD5_HH(B ,C ,D ,A , Ptr[i][10], 23, 0xBEBFBC70)
        MD5_HH(A ,B ,C ,D , Ptr[i][13], 4, 0x289B7EC6)
        MD5_HH(D ,A ,B ,C , Ptr[i][0], 11, 0xEAA127FA)
        MD5_HH(C ,D ,A ,B , Ptr[i][3], 16, 0xD4EF3085)
        MD5_HH(B ,C ,D ,A , Ptr[i][6], 23, 0x04881D05)
        MD5_HH(A ,B ,C ,D , Ptr[i][9], 4, 0xD9D4D039)
        MD5_HH(D ,A ,B ,C , Ptr[i][12], 11, 0xE6DB99E5)
        MD5_HH(C ,D ,A ,B , Ptr[i][15], 16, 0x1FA27CF8)
        MD5_HH(B ,C ,D ,A , Ptr[i][2], 23, 0xC4AC5665)

        MD5_II(A ,B ,C ,D , Ptr[i][0], 6, 0xF4292244)
        MD5_II(D ,A ,B ,C ,Ptr[i][7], 10, 0x432AFF97)
        MD5_II(C ,D ,A ,B ,Ptr[i][14], 15, 0xAB9423A7)
        MD5_II(B ,C ,D ,A ,Ptr[i][5], 21, 0xFC93A039)
        MD5_II(A ,B ,C ,D ,Ptr[i][12], 6, 0x655B59C3)
        MD5_II(D ,A ,B ,C ,Ptr[i][3], 10, 0x8F0CCC92)
        MD5_II(C ,D ,A ,B ,Ptr[i][10], 15, 0xFFEFF47D)
        MD5_II(B ,C ,D ,A ,Ptr[i][1], 21, 0x85845DD1)
        MD5_II(A ,B ,C ,D ,Ptr[i][8], 6, 0x6FA87E4F)
        MD5_II(D ,A ,B ,C ,Ptr[i][15], 10, 0xFE2CE6E0)
        MD5_II(C ,D ,A ,B ,Ptr[i][6], 15, 0xA3014314)
        MD5_II(B ,C ,D ,A ,Ptr[i][13], 21, 0x4E0811A1)
        MD5_II(A ,B ,C ,D ,Ptr[i][4], 6, 0xF7537E82)
        MD5_II(D ,A ,B ,C ,Ptr[i][11], 10, 0xBD3AF235)
        MD5_II(C ,D ,A ,B ,Ptr[i][2], 15, 0x2AD7D2BB)
        MD5_II(B ,C ,D ,A ,Ptr[i][9], 21, 0xEB86D391)

        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    if(!Final) {
        const BYTE(*_Ptr)[64] = (const BYTE (*)[64])srcByte;
        for(BYTE i = LeftoversrcByteLength - 1; i >0; i--) Extra[i] = _Ptr[TimesOfMainCycle][i];
        Extra[0] = _Ptr[TimesOfMainCycle][0];
        Extra[LeftoversrcByteLength] = 0x80;
        *(UINT64*)(Extra + (LeftoversrcByteLength >= 56 ? 120 : 56)) = BitsOfsrcByte;
        Final = true;
        goto fin;
    }

    HashResult Ret;
    Ret.A = A;
    Ret.B = B;
    Ret.C = C;
    Ret.D = D;

    return Ret;
}

char MD5::HashString[33] = {0};
const char* MD5::GetHashString(HashResult srcHashResult, bool UseUppercase) {
    char TransTable[] = "0123456789ABCDEF";
    if(!UseUppercase) for(BYTE i = 10; i < 16; i++) TransTable[i] = 'a' - 10 + i;
    BYTE* Ptr = (BYTE*)&srcHashResult;
    for(BYTE i = 0; i < 32; i += 2) HashString[i] = TransTable[(Ptr[i >> 1] & 0xF0) >> 4];
    for(BYTE i = 1; i < 32; i += 2) HashString[i] = TransTable[Ptr[i >> 1] & 0x0F];
    return HashString;
}
