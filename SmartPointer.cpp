#include "SmartPointer.h"
#include <iostream>

std::vector<PointerCounter> PointerManager::m_Pointers;
std::vector<INT64> PointerManager::IndexOfEmptyElement;

PointerManager::PointerManager(void* srcPointer, INT64* dstIndex) {
    if(srcPointer == nullptr && dstIndex == nullptr) return;
    if(m_Pointers.size() != 0) {
        for(INT64 i = m_Pointers.size() - 1; i >= 0; i--) {
            if(srcPointer == m_Pointers[i].Pointer){
                m_Pointers[i].ReferenceCount++;
                *dstIndex = i;
                return;
            }
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

INT64 PointerManager::GetIndexByPointer(void* srcPointer) {
    for(INT64 i = m_Pointers.size() - 1; i >= 0; i--) if(m_Pointers[i].Pointer == srcPointer) return i;
    return -1;
}

void PointerManager::ReferPointer(INT64 Index) {
    m_Pointers[Index].ReferenceCount++;
}

void PointerManager::ReferPointer(void* srcPointer) {
    INT64 Index = GetIndexByPointer(srcPointer);
    if(Index == -1) return;
    m_Pointers[Index].ReferenceCount++;
}

void PointerManager::ReleasePointer(INT64 Index) {
    std::cout << m_Pointers[Index].ReferenceCount << std::endl;
    if(m_Pointers[Index].ReferenceCount == 1) {
        DestoryPointer(Index);
    } else m_Pointers[Index].ReferenceCount--;
}

void PointerManager::ReleasePointer(void* srcPointer) {
    INT64 IndexOfsrcPointer = -1;
    for(INT64 i = m_Pointers.size() - 1; i >= 0; i--) {
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

void PointerManager::DestoryPointer(INT64 Index) {
    free(m_Pointers[Index].Pointer);
    m_Pointers[Index].Pointer = nullptr;
    m_Pointers[Index].ReferenceCount = 0;
    IndexOfEmptyElement.push_back(Index);
}

void PointerManager::DestoryPointer(void* srcPointer) {
    INT64 IndexOfsrcPointer = GetIndexByPointer(srcPointer);
    if(IndexOfsrcPointer == -1) return;
    DestoryPointer(IndexOfsrcPointer);
}
