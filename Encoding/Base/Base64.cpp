#include "Base64.h"

char Base64Encoding::Base64Table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                        '4', '5', '6', '7', '8', '9', '+', '/'};

char Base64Encoding::URL_Filename_Safe_Base64Table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                                          'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                                          'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                                          'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                                          'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                                          'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                                          'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                                          '4', '5', '6', '7', '8', '9', '-', '_'};

SmartPointer<char> Base64Encoding::GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength, bool URL_Filename_Safe) {
    if(srcBytes == nullptr) return nullptr;
    char* TransTable = URL_Filename_Safe ? URL_Filename_Safe_Base64Table : Base64Table;
    UINT64 TimesOfMainCycle = BytesLength / 3;
    BYTE LeftoverBytesCount = BytesLength % 3;
    char* Ret = (char*)malloc(sizeof(char) * (TimesOfMainCycle << 2) + (LeftoverBytesCount == 0 ? 1 : 5));
    if(Ret == nullptr) return nullptr;

    UINT64 OffsetOfsrcBytes = 0;
    UINT64 OffsetOfRet = 0;
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        Ret[OffsetOfRet] = TransTable[srcBytes[OffsetOfsrcBytes] >> 2];
        Ret[OffsetOfRet + 1] = TransTable[(srcBytes[OffsetOfsrcBytes] & 0x03) << 4 | srcBytes[OffsetOfsrcBytes + 1] >> 4];
        Ret[OffsetOfRet + 2] = TransTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x0F) << 2 | srcBytes[OffsetOfsrcBytes + 2] >> 6];
        Ret[OffsetOfRet + 3] = TransTable[srcBytes[OffsetOfsrcBytes + 2] & 0x3F];
        OffsetOfRet += 4;
        OffsetOfsrcBytes += 3;
    }

    switch(LeftoverBytesCount) {
        case 0:
            Ret[OffsetOfRet] = '\0';
            return Ret;
        case 1:
            Ret[OffsetOfRet] = TransTable[srcBytes[OffsetOfsrcBytes] >> 2];
            Ret[OffsetOfRet + 1] = TransTable[(srcBytes[OffsetOfsrcBytes] & 0x03) << 4];
            Ret[OffsetOfRet + 2] = '=';
            Ret[OffsetOfRet + 3] = '=';
            Ret[OffsetOfRet + 4] = '\0';
            return Ret;
        case 2:
            Ret[OffsetOfRet] = TransTable[srcBytes[OffsetOfsrcBytes] >> 2];
            Ret[OffsetOfRet + 1] = TransTable[(srcBytes[OffsetOfsrcBytes] & 0x03) << 4 | srcBytes[OffsetOfsrcBytes + 1] >> 4];
            Ret[OffsetOfRet + 2] = TransTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x0F) << 2];
            Ret[OffsetOfRet + 3] = '=';
            Ret[OffsetOfRet + 4] = '\0';
            return Ret;
        default:
            free(Ret);
            return nullptr;
    }
    free(Ret);
    return nullptr;
}
