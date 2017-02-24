#include "MD2.h"

Hash::MDA::HashResult Hash::MDA::MD2::GetChecksum(const BYTE* srcByte, UINT64 srcByteLength) {
    UINT64 TimesOfMainCycle = srcByteLength >> 4;
    BYTE LeftoversrcByteLength = (BYTE)(srcByteLength & 0x0F);
    bool Final = false;
    BYTE Extra[32] = {0};
    const BYTE(*Ptr)[16] = (const BYTE(*)[16])srcByte;
    BYTE BufferX[48] = {0};

fin:if(Final) {
        Ptr = (const BYTE(*)[16])Extra;
        TimesOfMainCycle = 2;
    }
    for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
        for(BYTE j = 0; j < 16; j++) {
            BufferX[16 + j] = Ptr[i][j];
            BufferX[32 + j] = BufferX[16 + j] ^ BufferX[j];
        }
        BYTE t = 0;
        for(BYTE j = 0; j < 18; j++) {
            for(BYTE k = 0; k < 48; k++) t = BufferX[k] = BufferX[k] ^ PI_SUBST[t];
            t += j;
        }
    }

    if(!Final) {
        if(LeftoversrcByteLength != 0) {
            for(BYTE i = LeftoversrcByteLength - 1; i > 0; i--) Extra[i] = Ptr[TimesOfMainCycle][i];
            Extra[0] = Ptr[TimesOfMainCycle][0];
        }
        BYTE padding = 16 - LeftoversrcByteLength;
        for(BYTE i = 0; i < padding; i++) Extra[LeftoversrcByteLength + i] = padding;
        BYTE L = 0;
        for(UINT64 i = 0; i < TimesOfMainCycle; i++) {
            for(BYTE j = 0; j < 16; j++) {
                Extra[16 + j] = PI_SUBST[Ptr[i][j] ^ L];
                L = Extra[16 + j];
            }
        }
        for(BYTE j = 0; j < 16; j++) {
            Extra[16 + j] = PI_SUBST[Extra[j] ^ L];
            L = Extra[16 + j];
        }
        Final = true;
        goto fin;
    }

    Hash::MDA::HashResult Ret;
    Ret.A = *(UINT32*)BufferX;
    Ret.B = *((UINT32*)BufferX + 1);
    Ret.C = *((UINT32*)BufferX + 2);
    Ret.D = *((UINT32*)BufferX + 3);

    return Ret;
}
