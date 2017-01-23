#include "Base16.h"

BYTE Base16::Base16Table[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

SmartPointer<char> Base16::GetEncodedString(const BYTE* srcBytes, UINT64 BytesLength) {
    char* Ret = (char*)malloc((BytesLength << 1) + 1);
    if(Ret == nullptr) return nullptr;

    UINT64 OffsetOfRet = 0;
    for(UINT64 i = 0; i < BytesLength; i++) {
        Ret[OffsetOfRet] = Base16Table[srcBytes[i] >> 4];
        Ret[OffsetOfRet + 1] = Base16Table[srcBytes[i] & 0x0F];
        OffsetOfRet += 2;
    }
    Ret[BytesLength << 1] = '\0';

    return Ret;
}
