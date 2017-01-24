#ifndef DECODEDBYTES_H_INCLUDED
#define DECODEDBYTES_H_INCLUDED

typedef struct _DecodedBytes{
    SmartPointer<BYTE> Bytes = nullptr;
    UINT64 BytesLength = 0;
} DecodedBytes;

#endif //DECODEDBYTES_H_INCLUDED
