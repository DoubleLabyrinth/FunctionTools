#include "Base16.h"

BYTE Base16Decoding::InverseBase16Table[0x47] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                                 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

DecodedBytes Base16Decoding::GetDecodedBytes(const char* srcBase16String, UINT64 srcStringLength) {
    DecodedBytes Ret;
    if((srcStringLength & 0x01) || srcBase16String == nullptr) return Ret;
    for(UINT64 i = 0; i < srcStringLength; i++) if(srcBase16String[i] < 0x30 ||
                                                   (srcBase16String[i] > 0x39 && srcBase16String[i] < 0x41) ||
                                                   srcBase16String[i] > 0x46) return Ret;
    Ret.BytesLength = srcStringLength >> 1;
    BYTE* RetBytes = (BYTE*)malloc(sizeof(BYTE) * (Ret.BytesLength));
    if(RetBytes == nullptr) {
        Ret.BytesLength = 0;
        return Ret;
    }

    for(UINT64 i = 0; i < Ret.BytesLength; i++) RetBytes[i] = InverseBase16Table[srcBase16String[i << 1]] << 4 | InverseBase16Table[srcBase16String[i << 1 | 0x01]];
    Ret.Bytes = RetBytes;
    return Ret;
}
