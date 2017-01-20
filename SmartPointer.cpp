#include "SmartPointer.h"

std::vector<PointerCounter> PointerManager::m_Pointers;
std::vector<INT32> PointerManager::IndexOfEmptyElement;

PointerManager::PointerManager(void* srcPointer, INT32* dstIndex) {
    if(srcPointer == nullptr) {
        if(dstIndex == nullptr) return;
        *dstIndex = -1;
        return;
    }
    for(INT32 i = m_Pointers.size() - 1; i >= 0; i--) {
        if(srcPointer == m_Pointers[i].Pointer){
            m_Pointers[i].ReferenceCount++;
            *dstIndex = i;
            return;
        }
    }
    if(IndexOfEmptyElement.size() != 0) {
        m_Pointers[IndexOfEmptyElement.back()].Pointer = srcPointer;
        m_Pointers[IndexOfEmptyElement.back()].ReferenceCount = 1;
        *dstIndex = IndexOfEmptyElement.back();
        IndexOfEmptyElement.pop_back();
        return;
    }
    PointerCounter newPointer;
    newPointer.Pointer = srcPointer;
    newPointer.ReferenceCount = 1;
    m_Pointers.push_back(newPointer);
    *dstIndex = m_Pointers.size() - 1;
}

INT32 PointerManager::GetIndexByPointer(void* srcPointer) {
    for(INT32 i = m_Pointers.size() - 1; i >= 0; i--) if(m_Pointers[i].Pointer == srcPointer) return i;
    return -1;
}

void PointerManager::ReferPointer(INT32 Index) {
    if(Index < 0) return;
    m_Pointers[Index].ReferenceCount++;
}

void PointerManager::ReferPointer(void* srcPointer) {
    INT32 Index = GetIndexByPointer(srcPointer);
    if(Index == -1) return;
    m_Pointers[Index].ReferenceCount++;
}

void PointerManager::ReleasePointer(INT32 Index) {
    if(Index < 0) return;
    if(m_Pointers[Index].ReferenceCount == 1) {
        DestoryPointer(Index);
    } else m_Pointers[Index].ReferenceCount--;
}

void PointerManager::ReleasePointer(void* srcPointer) {
    INT32 IndexOfsrcPointer = -1;
    for(INT32 i = m_Pointers.size() - 1; i >= 0; i--) {
        if(m_Pointers[i].Pointer == srcPointer) {
            IndexOfsrcPointer = i;
            break;
        }
    }
    if(IndexOfsrcPointer != -1) {
        if(m_Pointers[IndexOfsrcPointer].ReferenceCount == 1) {
            DestoryPointer(IndexOfsrcPointer);
            return;
        } else m_Pointers[IndexOfsrcPointer].ReferenceCount--;
    }
}

void PointerManager::DestoryPointer(INT32 Index) {
    if(Index < 0) return;
    free(m_Pointers[Index].Pointer);
    m_Pointers[Index].Pointer = nullptr;
    m_Pointers[Index].ReferenceCount = 0;
    IndexOfEmptyElement.push_back(Index);
}

void PointerManager::DestoryPointer(void* srcPointer) {
    INT32 IndexOfsrcPointer = GetIndexByPointer(srcPointer);
    if(IndexOfsrcPointer == -1) return;
    DestoryPointer(IndexOfsrcPointer);
}
