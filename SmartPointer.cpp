#include "SmartPointer.h"

std::vector<PointerCounter> PointerManager::m_Pointers;
std::vector<INT32> PointerManager::IndexOfEmptyElement;

PointerManager::PointerManager(void* srcPointer, INT32& dstIndex) {
    if(srcPointer == nullptr) {
        if(&dstIndex == nullptr) return;
        dstIndex = -1;
        return;
    }
    for(INT32 i = m_Pointers.size() - 1; i >= 0; i--) {
        if(srcPointer == m_Pointers[i].Pointer){
            m_Pointers[i].ReferenceCount++;
            dstIndex = i;
            return;
        }
    }
    if(IndexOfEmptyElement.size() != 0) {
        m_Pointers[IndexOfEmptyElement.back()].Pointer = srcPointer;
        m_Pointers[IndexOfEmptyElement.back()].ReferenceCount = 1;
        dstIndex = IndexOfEmptyElement.back();
        IndexOfEmptyElement.pop_back();
        return;
    }
    PointerCounter newPointer;
    newPointer.Pointer = srcPointer;
    newPointer.ReferenceCount = 1;
    m_Pointers.push_back(newPointer);
    dstIndex = m_Pointers.size() - 1;
}

INT32 PointerManager::GetIndexByPointer(void* srcPointer) {
    if(srcPointer == nullptr) return -1;
    for(INT32 i = m_Pointers.size() - 1; i >= 0; i--) if(m_Pointers[i].Pointer == srcPointer) return i;
    return -1;
}

void PointerManager::ReferPointer(INT32 srcIndex) {
    if(srcIndex < 0 || srcIndex >= m_Pointers.size()) return;
    if(m_Pointers[srcIndex].Pointer == nullptr) return;
    m_Pointers[srcIndex].ReferenceCount++;
}

void PointerManager::ReferPointer(void* srcPointer) {
    INT32 IndexOfsrcPointer = GetIndexByPointer(srcPointer);
    if(IndexOfsrcPointer == -1) return;
    m_Pointers[IndexOfsrcPointer].ReferenceCount++;
}

void PointerManager::ReleasePointer(INT32 srcIndex) {
    if(srcIndex < 0 || srcIndex >= m_Pointers.size()) return;
    if(m_Pointers[srcIndex].Pointer == nullptr) return;
    if(m_Pointers[srcIndex].ReferenceCount == 1) {
        free(m_Pointers[srcIndex].Pointer);
        m_Pointers[srcIndex].Pointer = nullptr;
        m_Pointers[srcIndex].ReferenceCount = 0;
        IndexOfEmptyElement.push_back(srcIndex);
    } else m_Pointers[srcIndex].ReferenceCount--;
}

void PointerManager::ReleasePointer(void* srcPointer) {
    INT32 IndexOfsrcPointer = GetIndexByPointer(srcPointer);
    if(IndexOfsrcPointer == -1) return;
    if(m_Pointers[IndexOfsrcPointer].ReferenceCount == 1) {
        free(m_Pointers[IndexOfsrcPointer].Pointer);
        m_Pointers[IndexOfsrcPointer].Pointer = nullptr;
        m_Pointers[IndexOfsrcPointer].ReferenceCount = 0;
        IndexOfEmptyElement.push_back(IndexOfsrcPointer);
    } else m_Pointers[IndexOfsrcPointer].ReferenceCount--;
}

void PointerManager::DestoryPointer(INT32 srcIndex) {
    if(srcIndex < 0 || srcIndex >= m_Pointers.size()) return;
    if(m_Pointers[srcIndex].Pointer == nullptr) return;
    free(m_Pointers[srcIndex].Pointer);
    m_Pointers[srcIndex].Pointer = nullptr;
    m_Pointers[srcIndex].ReferenceCount = 0;
    IndexOfEmptyElement.push_back(srcIndex);
}

void PointerManager::DestoryPointer(void* srcPointer) {
    INT32 IndexOfsrcPointer = GetIndexByPointer(srcPointer);
    if(IndexOfsrcPointer == -1) return;
    free(srcPointer);
    m_Pointers[IndexOfsrcPointer].Pointer = nullptr;
    m_Pointers[IndexOfsrcPointer].ReferenceCount = 0;
    IndexOfEmptyElement.push_back(IndexOfsrcPointer);
}
