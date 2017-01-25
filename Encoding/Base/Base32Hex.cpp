#include "Base32Hex.h"

char Base32HexEncoding::Base32HexTable[32] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                                              'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                              'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};

SmartPointer<char> Base32HexEncoding::GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength) {
    if(srcBytes == nullptr) return nullptr;
    UINT64 TimesOfMainCycle = BytesLength / 5;
    BYTE LeftoverBytesCount = BytesLength % 5;
    char* Ret = (char*)malloc(sizeof(char) * (TimesOfMainCycle << 3) + (LeftoverBytesCount == 0 ? 1 : 9));
    if(Ret == nullptr) return nullptr;

    UINT64 OffsetOfsrcBytes = 0;
    UINT64 OffsetOfRet = 0;
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        Ret[OffsetOfRet] = Base32HexTable[srcBytes[OffsetOfsrcBytes] >> 3];
        Ret[OffsetOfRet + 1] = Base32HexTable[(srcBytes[OffsetOfsrcBytes] & 0x07) << 2 | srcBytes[OffsetOfsrcBytes + 1] >> 6];
        Ret[OffsetOfRet + 2] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x3E) >> 1];
        Ret[OffsetOfRet + 3] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x01) << 4 | srcBytes[OffsetOfsrcBytes + 2] >> 4];
        Ret[OffsetOfRet + 4] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 2] & 0x0F) << 1 | srcBytes[OffsetOfsrcBytes + 3] >> 7];
        Ret[OffsetOfRet + 5] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 3] & 0x7C) >> 2];
        Ret[OffsetOfRet + 6] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 3] & 0x03) << 3 | srcBytes[OffsetOfsrcBytes + 4] >> 5];
        Ret[OffsetOfRet + 7] = Base32HexTable[srcBytes[OffsetOfsrcBytes + 4] & 0x1F];
        OffsetOfRet += 8;
        OffsetOfsrcBytes += 5;
    }

    switch(LeftoverBytesCount) {
        case 0:
            Ret[OffsetOfRet] = '\0';
            return Ret;
        case 1:
            Ret[OffsetOfRet] = Base32HexTable[srcBytes[OffsetOfsrcBytes] >> 3];
            Ret[OffsetOfRet + 1] = Base32HexTable[(srcBytes[OffsetOfsrcBytes] & 0x07) << 2];
            Ret[OffsetOfRet + 2] = '=';
            Ret[OffsetOfRet + 3] = '=';
            Ret[OffsetOfRet + 4] = '=';
            Ret[OffsetOfRet + 5] = '=';
            Ret[OffsetOfRet + 6] = '=';
            Ret[OffsetOfRet + 7] = '=';
            Ret[OffsetOfRet + 8] = '\0';
            return Ret;
        case 2:
            Ret[OffsetOfRet] = Base32HexTable[srcBytes[OffsetOfsrcBytes] >> 3];
            Ret[OffsetOfRet + 1] = Base32HexTable[(srcBytes[OffsetOfsrcBytes] & 0x07) << 2 | srcBytes[OffsetOfsrcBytes + 1] >> 6];
            Ret[OffsetOfRet + 2] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x3E) >> 1];
            Ret[OffsetOfRet + 3] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x01) << 4];
            Ret[OffsetOfRet + 4] = '=';
            Ret[OffsetOfRet + 5] = '=';
            Ret[OffsetOfRet + 6] = '=';
            Ret[OffsetOfRet + 7] = '=';
            Ret[OffsetOfRet + 8] = '\0';
            return Ret;
        case 3:
            Ret[OffsetOfRet] = Base32HexTable[srcBytes[OffsetOfsrcBytes] >> 3];
            Ret[OffsetOfRet + 1] = Base32HexTable[(srcBytes[OffsetOfsrcBytes] & 0x07) << 2 | srcBytes[OffsetOfsrcBytes + 1] >> 6];
            Ret[OffsetOfRet + 2] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x3E) >> 1];
            Ret[OffsetOfRet + 3] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x01) << 4 | srcBytes[OffsetOfsrcBytes + 2] >> 4];
            Ret[OffsetOfRet + 4] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 2] & 0x0F) << 1];
            Ret[OffsetOfRet + 5] = '=';
            Ret[OffsetOfRet + 6] = '=';
            Ret[OffsetOfRet + 7] = '=';
            Ret[OffsetOfRet + 8] = '\0';
            return Ret;
        case 4:
            Ret[OffsetOfRet] = Base32HexTable[srcBytes[OffsetOfsrcBytes] >> 3];
            Ret[OffsetOfRet + 1] = Base32HexTable[(srcBytes[OffsetOfsrcBytes] & 0x07) << 2 | srcBytes[OffsetOfsrcBytes + 1] >> 6];
            Ret[OffsetOfRet + 2] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x3E) >> 1];
            Ret[OffsetOfRet + 3] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 1] & 0x01) << 4 | srcBytes[OffsetOfsrcBytes + 2] >> 4];
            Ret[OffsetOfRet + 4] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 2] & 0x0F) << 1 | srcBytes[OffsetOfsrcBytes + 3] >> 7];
            Ret[OffsetOfRet + 5] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 3] & 0x7C) >> 2];
            Ret[OffsetOfRet + 6] = Base32HexTable[(srcBytes[OffsetOfsrcBytes + 3] & 0x03) << 3];
            Ret[OffsetOfRet + 7] = '=';
            Ret[OffsetOfRet + 8] = '\0';
            return Ret;
        default:
            free(Ret);
            return nullptr;
    }
    free(Ret);
    return nullptr;
}
