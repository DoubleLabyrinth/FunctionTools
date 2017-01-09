#include "CRC.h"

BYTE CRC8::Calculate(const BYTE* srcData, UINT64 srcDataLength) {

}

BOOL CRC8::CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE CRCValue) {
    return CRCValue == Calculate(srcData, srcDataLength) ? TRUE : FALSE;
}
