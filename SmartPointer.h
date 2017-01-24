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
    static std::vector<INT32> IndexOfEmptyElement;
public:
    PointerManager(void* srcPointer, INT32& dstIndex);
    static INT32 GetIndexByPointer(void* srcPointer);
    static void ReferPointer(INT32 Index);
    static void ReleasePointer(INT32 Index);
    static void DestoryPointer(INT32 Index);
    static void ReferPointer(void* srcPointer);
    static void ReleasePointer(void* srcPointer);
    static void DestoryPointer(void* srcPointer);
};

template <typename _Type>
class SmartPointer: private PointerManager {
private:
    _Type* m_Pointer;
    INT32 Index;
public:
    SmartPointer() : PointerManager(nullptr, Index) {
        m_Pointer = nullptr;
    }

    SmartPointer(_Type* srcPointer) : PointerManager(srcPointer, Index) {
        m_Pointer = srcPointer;
    }

    ~SmartPointer() {
        ReleasePointer(Index);
    }

    SmartPointer(const SmartPointer<_Type>& srcSmartPointer) : PointerManager(nullptr, *(INT32*)nullptr) {
        m_Pointer = srcSmartPointer.m_Pointer;
        Index = srcSmartPointer.Index;
        ReferPointer(Index);
    }

    SmartPointer<_Type>& operator=(_Type* srcPointer) {
        if(m_Pointer == srcPointer) return *this;
        PointerManager(srcPointer, Index);
        m_Pointer = srcPointer;
        return *this;
    }


    SmartPointer<_Type>& operator=(const SmartPointer<_Type>& srcSmartPointer) {
        if(this == &srcSmartPointer) return *this;
        if(m_Pointer == srcSmartPointer.m_Pointer) ReferPointer(Index);
        ReleasePointer(Index);
        m_Pointer = srcSmartPointer.m_Pointer;
        Index = srcSmartPointer.Index;
        ReferPointer(Index);
        return *this;
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
