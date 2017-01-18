#ifndef SMARTPOINTER_H_INCLUDED
#define SMARTPOINTER_H_INCLUDED
#include <stdlib.h>
#include <vector>
#include "TypeDefine.h"

typedef struct _PointerCounter {
    void* Pointer;
    UINT64 ReferenceCount;
} PointerCounter;

class PointerManager {
private:
    static std::vector<PointerCounter> m_Pointers;
    static std::vector<INT64> IndexOfEmptyElement;
public:
    PointerManager(void* srcPointer, INT64* dstIndex);
    static INT64 GetIndexByPointer(void* srcPointer);
    static void ReferPointer(INT64 Index);
    static void ReleasePointer(INT64 Index);
    static void DestoryPointer(INT64 Index);
    static void ReferPointer(void* srcPointer);
    static void ReleasePointer(void* srcPointer);
    static void DestoryPointer(void* srcPointer);
};

template <class _Type>
class SmartPointer: private PointerManager {
private:
    _Type* m_Pointer;
    INT64 Index;
public:
    SmartPointer(_Type* srcPointer) : PointerManager(srcPointer, &Index) {
        m_Pointer = srcPointer;
    }

    ~SmartPointer() {
        ReleasePointer(Index);
    }

    SmartPointer(const SmartPointer<_Type>& srcSmartPointer) : PointerManager(NULL, NULL) {
        m_Pointer = srcSmartPointer.m_Pointer;
        Index = srcSmartPointer.Index;
        ReferPointer(Index);
    }

    SmartPointer<_Type>& operator=(const SmartPointer<_Type>& srcSmartPointer) {
        if(this == *srcSmartPointer) return *this;
        m_Pointer = srcSmartPointer.m_Pointer;
        Index = srcSmartPointer.Index;
        ReferPointer(Index);
    }

    _Type& operator*() {
        return *m_Pointer;
    }

    _Type& operator[](UINT64 i) {
        return m_Pointer[i];
    }

    _Type* operator+(INT64 i) {
        return m_Pointer + i;
    }

    _Type* GetPointer() {
        return m_Pointer;
    }
};

#endif // SMARTPOINTER_H_INCLUDED
