#include "Base32Hex.h"

BYTE Base32HexDecoding::InverseBase32HexTable[87] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                                     0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                                     0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                                     0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

DecodedBytes Base32HexDecoding::GetDecodedBytes(const char* srcBase32String, UINT64 srcStringLength) {
    DecodedBytes Ret;
    //Input is illegal when srcStringLength is not the multiple of 8 or srcBase32String is a null pointer.
    if((srcStringLength & 0x07) || srcBase32String == nullptr) return Ret;

    /*If srcStringLength is larger than 8, we should check weather every character in srcBase32String except the last 8 characters is between '0' ~ '9' or between 'A' ~ 'V'.
     * If not, the input is illegal, just return a empty DecodecBytes.
     */
    if (srcStringLength > 8) {
        srcStringLength -= 8;
        for(UINT64 i = 0; i < srcStringLength; i++) if(srcBase32String[i] < '0' || (srcBase32String[i] > '9' && srcBase32String[i] < 'A') || srcBase32String[i] > 'V') return Ret;
        srcStringLength += 8;
    }

    /*In the last 8 characters, '=' can be existed. But '=' must behind every other character.
     *That means characters behind must be '=' once '=' showed.
     */
    BYTE PaddingCount = 0;
    for(UINT64 i = srcStringLength - 8; i < srcStringLength; i++) {
        if(srcBase32String[i] < '0' || (srcBase32String[i] != '=' && srcBase32String[i] > '9' && srcBase32String[i] < 'A') || srcBase32String[i] > 'V') return Ret;
        if(srcBase32String[i] == '=') PaddingCount++;
    }
    for(UINT64 i = srcStringLength - PaddingCount; i < srcStringLength; i++) if(srcBase32String[i] != '=') return Ret;

    /*
     *
     */
    switch(PaddingCount) {
        case 0:
            Ret.BytesLength = (srcStringLength >> 3) * 5;
            break;
        case 1:
            if(InverseBase32HexTable[srcBase32String[srcStringLength - 2]] & 0x07) return Ret;
            Ret.BytesLength = (srcStringLength >> 3) * 5 - 1;
            break;
        case 2:
            return Ret;
        case 3:
            if(InverseBase32HexTable[srcBase32String[srcStringLength - 4]] & 0x01) return Ret;
            Ret.BytesLength = (srcStringLength >> 3) * 5 - 2;
            break;
        case 4:
            if(InverseBase32HexTable[srcBase32String[srcStringLength - 5]] & 0x0F) return Ret;
            Ret.BytesLength = (srcStringLength >> 3) * 5 - 3;
            break;
        case 5:
            return Ret;
        case 6:
            if(InverseBase32HexTable[srcBase32String[srcStringLength - 7]] & 0x03) return Ret;
            Ret.BytesLength = (srcStringLength >> 3) * 5 - 4;
            break;
        case 7:
        case 8:
        default:
            return Ret;
    }

    BYTE* RetBytes = (BYTE*)malloc(sizeof(BYTE) * Ret.BytesLength);
    if(RetBytes == nullptr) {
        Ret.BytesLength = 0;
        return Ret;
    }
    if(PaddingCount != 0) srcStringLength -= 8;
    UINT64 OffsetOfRetBytes = 0;
    for(UINT64 i = 0; i < srcStringLength; i += 8) {
        RetBytes[OffsetOfRetBytes] = InverseBase32HexTable[srcBase32String[i]] << 3 | InverseBase32HexTable[srcBase32String[i + 1]] >> 2;
        RetBytes[OffsetOfRetBytes + 1] = InverseBase32HexTable[srcBase32String[i + 1]] << 6 | InverseBase32HexTable[srcBase32String[i + 2]] << 1 | InverseBase32HexTable[srcBase32String[i + 3]] >> 4;
        RetBytes[OffsetOfRetBytes + 2] = InverseBase32HexTable[srcBase32String[i + 3]] << 4 | InverseBase32HexTable[srcBase32String[i + 4]] >> 1;
        RetBytes[OffsetOfRetBytes + 3] = InverseBase32HexTable[srcBase32String[i + 4]] << 7 | InverseBase32HexTable[srcBase32String[i + 5]] << 2 | InverseBase32HexTable[srcBase32String[i + 6]] >> 3;
        RetBytes[OffsetOfRetBytes + 4] = InverseBase32HexTable[srcBase32String[i + 6]] << 5 | InverseBase32HexTable[srcBase32String[i + 7]];
        OffsetOfRetBytes += 5;
    }

    switch(PaddingCount) {
        case 0:
            Ret.Bytes = RetBytes;
            return Ret;
        case 1:
            RetBytes[OffsetOfRetBytes] = InverseBase32HexTable[srcBase32String[srcStringLength]] << 3 | InverseBase32HexTable[srcBase32String[srcStringLength + 1]] >> 2;
            RetBytes[OffsetOfRetBytes + 1] = InverseBase32HexTable[srcBase32String[srcStringLength + 1]] << 6 | InverseBase32HexTable[srcBase32String[srcStringLength + 2]] << 1 | InverseBase32HexTable[srcBase32String[srcStringLength + 3]] >> 4;
            RetBytes[OffsetOfRetBytes + 2] = InverseBase32HexTable[srcBase32String[srcStringLength + 3]] << 4 | InverseBase32HexTable[srcBase32String[srcStringLength + 4]] >> 1;
            RetBytes[OffsetOfRetBytes + 3] = InverseBase32HexTable[srcBase32String[srcStringLength + 4]] << 7 | InverseBase32HexTable[srcBase32String[srcStringLength + 5]] << 2 | InverseBase32HexTable[srcBase32String[srcStringLength + 6]] >> 3;
            Ret.Bytes = RetBytes;
            return Ret;
        case 3:
            RetBytes[OffsetOfRetBytes] = InverseBase32HexTable[srcBase32String[srcStringLength]] << 3 | InverseBase32HexTable[srcBase32String[srcStringLength + 1]] >> 2;
            RetBytes[OffsetOfRetBytes + 1] = InverseBase32HexTable[srcBase32String[srcStringLength + 1]] << 6 | InverseBase32HexTable[srcBase32String[srcStringLength + 2]] << 1 | InverseBase32HexTable[srcBase32String[srcStringLength + 3]] >> 4;
            RetBytes[OffsetOfRetBytes + 2] = InverseBase32HexTable[srcBase32String[srcStringLength + 3]] << 4 | InverseBase32HexTable[srcBase32String[srcStringLength + 4]] >> 1;
            Ret.Bytes = RetBytes;
            return Ret;
        case 4:
            RetBytes[OffsetOfRetBytes] = InverseBase32HexTable[srcBase32String[srcStringLength]] << 3 | InverseBase32HexTable[srcBase32String[srcStringLength + 1]] >> 2;
            RetBytes[OffsetOfRetBytes + 1] = InverseBase32HexTable[srcBase32String[srcStringLength + 1]] << 6 | InverseBase32HexTable[srcBase32String[srcStringLength + 2]] << 1 | InverseBase32HexTable[srcBase32String[srcStringLength + 3]] >> 4;
            Ret.Bytes = RetBytes;
            return Ret;
        case 6:
            RetBytes[OffsetOfRetBytes] = InverseBase32HexTable[srcBase32String[srcStringLength]] << 3 | InverseBase32HexTable[srcBase32String[srcStringLength + 1]] >> 2;
            Ret.Bytes = RetBytes;
            return Ret;
        default:
            break;
    }
    free(RetBytes);
    Ret.BytesLength = 0;
    return Ret;
}
