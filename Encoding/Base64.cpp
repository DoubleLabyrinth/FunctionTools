#include "Base64.h"

char Base64::Base64Table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

char Base64::URL_Filename_Safe_Base64Table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                                  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                                  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                                  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                                  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                                  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                                  'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                                  '4', '5', '6', '7', '8', '9', '-', '_'};
SmartPointer<char> Base64::GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength, bool URL_Filename_Safe) {
    char* TransTable = URL_Filename_Safe ? URL_Filename_Safe_Base64Table : Base64Table;
    UINT64 TimesOfCycle = BytesLength / 3;
    UINT64 EncodedStringLength = TimesOfCycle * 4;
    BYTE LeftoverBytesNumber = BytesLength % 3;
    EncodedStringLength += LeftoverBytesNumber == 0 ? 0 : 4;
    char* Ret = (char*)malloc(sizeof(char) * EncodedStringLength + 1);
    if(Ret == nullptr) return nullptr;

    for(UINT64 i = 0; i < TimesOfCycle; i++) {
        Ret[i * 4] = TransTable[(srcBytes[3 * i] & 0xFC) >> 2];
        Ret[i * 4 + 1] = TransTable[(srcBytes[3 * i] & 0x03) << 4 | (srcBytes[3 * i + 1] & 0xF0) >> 4];
        Ret[i * 4 + 2] = TransTable[(srcBytes[3 * i + 1] & 0x0F) << 2 | (srcBytes[3 * i + 2] & 0xC0) >> 6];
        Ret[i * 4 + 3] = TransTable[srcBytes[3 * i + 2] & 0x3F];
    }

    switch(LeftoverBytesNumber) {
        case 0:
            Ret[TimesOfCycle * 4] = '\0';
            return Ret;
        case 1:
            Ret[TimesOfCycle * 4] = TransTable[(srcBytes[BytesLength - 1] & 0xFC) >> 2];
            Ret[TimesOfCycle * 4 + 1] = TransTable[(srcBytes[BytesLength - 1] & 0x03) << 4];
            Ret[TimesOfCycle * 4 + 2] = '=';
            Ret[TimesOfCycle * 4 + 3] = '=';
            Ret[TimesOfCycle * 4 + 4] = '\0';
            return Ret;
        case 2:
            Ret[TimesOfCycle * 4] = TransTable[(srcBytes[BytesLength - 2] & 0xFC) >> 2];
            Ret[TimesOfCycle * 4 + 1] = TransTable[(srcBytes[BytesLength - 2] & 0x03) << 4 | (srcBytes[BytesLength - 1] & 0xF0) >> 4];
            Ret[TimesOfCycle * 4 + 2] = TransTable[(srcBytes[BytesLength - 1] & 0xF) << 2];
            Ret[TimesOfCycle * 4 + 3] = '=';
            Ret[TimesOfCycle * 4 + 4] = '\0';
            return Ret;
        default:
            free(Ret);
            return nullptr;
    }
    free(Ret);
    return nullptr;
}
