#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED
#include <windows.h>

class CRC8 {
private:
    CRC8();
public:
    static BYTE Calculate(const BYTE* srcData, UINT64 srcDataLength);
    static BOOL CheckData(const BYTE* srcData, UINT64 srcDataLength, BYTE CRC8Value);
};

#endif // CRC_H_INCLUDED
