#include "Huffman.h"

void HuffmanCompression::GetHuffmanEncoding(BinaryTreeNode<struct BYTECounter>* srcNode, struct BYTETransformer* srcTransformer, BYTE* BitBuffer, UINT32 Layer, bool GoLeft) {
    if(GoLeft) {
        BitBuffer[Layer >> 3] &= ~(((BYTE)0x01) << (7 - (Layer & 0x07)));
        if(srcNode->LeftChildNode->LeftChildNode == nullptr && srcNode->LeftChildNode->RightChildNode == nullptr) {
            srcTransformer[srcNode->LeftChildNode->NodeData.Value].Value = srcNode->LeftChildNode->NodeData.Value;
            srcTransformer[srcNode->LeftChildNode->NodeData.Value].EncodedBitsLength = Layer + 1;
            for(BYTE i = 0; i < 32; i++) srcTransformer[srcNode->LeftChildNode->NodeData.Value].EncodedBytes[i] = BitBuffer[i];
            return;
        }
        GetHuffmanEncoding(srcNode->LeftChildNode, srcTransformer, BitBuffer, Layer + 1, true);
        GetHuffmanEncoding(srcNode->LeftChildNode, srcTransformer, BitBuffer, Layer + 1, false);
    } else {
        BitBuffer[Layer >> 3] |= ((BYTE)0x01) << (7 - (Layer & 0x07));
        if(srcNode->RightChildNode->LeftChildNode == nullptr && srcNode->RightChildNode->RightChildNode == nullptr) {
            srcTransformer[srcNode->RightChildNode->NodeData.Value].Value = srcNode->RightChildNode->NodeData.Value;
            srcTransformer[srcNode->RightChildNode->NodeData.Value].EncodedBitsLength = Layer + 1;
            for(BYTE i = 0; i < 32; i++) srcTransformer[srcNode->RightChildNode->NodeData.Value].EncodedBytes[i] = BitBuffer[i];
            BitBuffer[Layer >> 3] ^= ((BYTE)0x01) << (7 - (Layer & 0x07));
            return;
        }
        GetHuffmanEncoding(srcNode->RightChildNode, srcTransformer, BitBuffer, Layer + 1, true);
        GetHuffmanEncoding(srcNode->RightChildNode, srcTransformer, BitBuffer, Layer + 1, false);
    }
    BitBuffer[Layer >> 3] &= ~(((BYTE)0x01) << (7 - (Layer & 0x07)));
}

void HuffmanCompression::SortStatisticsData(BinaryTreeNode<struct BYTECounter>* srcStatistics[], UINT32 Length) {
    if(Length <= 1) return;
    UINT32 i = 0, j = Length - 1;
    while(i < j) {
        while(srcStatistics[j]->NodeData.Count <= srcStatistics[0]->NodeData.Count && i < j) j--;
        while(srcStatistics[i]->NodeData.Count >= srcStatistics[0]->NodeData.Count && i < j) i++;
        if(i < j) {
            BinaryTreeNode<struct BYTECounter>* tmp = srcStatistics[i];
            srcStatistics[i] = srcStatistics[j];
            srcStatistics[j] = tmp;
        }
    }
    BinaryTreeNode<struct BYTECounter>* tmp = srcStatistics[0];
    srcStatistics[0] = srcStatistics[j];
    srcStatistics[j] = tmp;

    SortStatisticsData(srcStatistics, i);
    SortStatisticsData(srcStatistics + i + 1, Length - i - 1);
}

bool HuffmanCompression::CompressFile(const char* srcFilePath, const char* dstFilePath) {
    HANDLE h_srcFile = CreateFileA(srcFilePath,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
    if(h_srcFile == INVALID_HANDLE_VALUE) return false;  //if fail to execute CreateFile, just return false;

    HANDLE h_dstFile = CreateFileA(dstFilePath,
                                   GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_READ,
                                   NULL,
                                   CREATE_NEW,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
    if(h_dstFile == INVALID_HANDLE_VALUE) {
        CloseHandle(h_srcFile);
        return false;
    }

    //Get the file's size;
    LARGE_INTEGER srcFileSize;
    if(!GetFileSizeEx(h_srcFile, &srcFileSize)) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false; // if fail, just return false;
    }
    if(!WriteFile(h_dstFile, &srcFileSize, sizeof(LARGE_INTEGER), NULL, NULL)) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }
    //Map the file to memory;
    HANDLE h_srcFileMapping = CreateFileMappingA(h_srcFile, NULL, PAGE_READONLY, srcFileSize.HighPart, srcFileSize.LowPart, NULL);
    if(h_srcFileMapping == INVALID_HANDLE_VALUE) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    //Get the file's pointer;
    BYTE* srcFile = (BYTE*)MapViewOfFile(h_srcFileMapping, FILE_MAP_READ, 0, 0, srcFileSize.QuadPart);
    if(srcFile == NULL) {
        CloseHandle(h_srcFileMapping);
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    //Count bytes
    BinaryTreeNode<struct BYTECounter>* Statistics[256];
    for(UINT16 i = 0; i < 0x100; i++) {
        Statistics[i] = new BinaryTreeNode<struct BYTECounter>();
        Statistics[i]->NodeData.Value = (BYTE)i;
        Statistics[i]->NodeData.Count = 0;
    }

    for(INT64 i = 0; i < srcFileSize.QuadPart; i++) Statistics[srcFile[i]]->NodeData.Count++;
    for(UINT16 i = 0; i < 0x100; i++) {
        if(!WriteFile(h_dstFile, &(Statistics[i]->NodeData), sizeof(struct BYTECounter), NULL, NULL)) {
            UnmapViewOfFile(srcFile);
            CloseHandle(h_srcFileMapping);
            CloseHandle(h_srcFile);
            CloseHandle(h_dstFile);
            return false;
        }
    }

    //Generate HuffmanTree
    for(UINT32 i = 0xFF; i > 0; i--) {
        SortStatisticsData(Statistics, i + 1);
        BinaryTreeNode<struct BYTECounter>* tmp = new BinaryTreeNode<struct BYTECounter>();
        tmp->LeftChildNode = Statistics[i];
        tmp->RightChildNode = Statistics[i - 1];
        tmp->NodeData.Count = tmp->LeftChildNode->NodeData.Count + tmp->RightChildNode->NodeData.Count;
        tmp->NodeData.Value = 0;
        Statistics[i] = nullptr;
        Statistics[i - 1] = tmp;
    }
    BYTE BitBuffer[32] = {0};
    struct BYTETransformer TransTable[256];

    BinaryTree<struct BYTECounter> HuffmanTree;
    HuffmanTree.RootNode = Statistics[0];
    Statistics[0] = nullptr;
    GetHuffmanEncoding(HuffmanTree.RootNode, TransTable, BitBuffer, 0, true);
    GetHuffmanEncoding(HuffmanTree.RootNode, TransTable, BitBuffer, 0, false);

    //Start compressing
    BYTE* CompressBuffer = (BYTE*)malloc(sizeof(BYTE) * 10485760);  //10485760 is 10MB.
    if(CompressBuffer == nullptr) {
        UnmapViewOfFile(srcFile);
        CloseHandle(h_srcFileMapping);
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    UINT32 Cur = 0;
    BYTE CurUsed = 0;
    for(INT64 i = 0; i < srcFileSize.QuadPart; i++) {
        if(Cur + ((CurUsed + TransTable[srcFile[i]].EncodedBitsLength) >> 3) > 10485759) {
            if(!WriteFile(h_dstFile, CompressBuffer, Cur, NULL, NULL)) {
                UnmapViewOfFile(srcFile);
                CloseHandle(h_srcFileMapping);
                CloseHandle(h_srcFile);
                CloseHandle(h_dstFile);
                free(CompressBuffer);
                return false;
            }
            CompressBuffer[0] = CompressBuffer[Cur];
            RtlSecureZeroMemory(CompressBuffer + 1, 10485759);
            Cur = 0;
        }
        if(CurUsed == 0) {
            for(INT8 j = (TransTable[srcFile[i]].EncodedBitsLength >> 3) - ((TransTable[srcFile[i]].EncodedBitsLength & 0x07) == 0 ? 1 : 0); j >= 0; j--) CompressBuffer[Cur + j] = TransTable[srcFile[i]].EncodedBytes[j];
            CurUsed = TransTable[srcFile[i]].EncodedBitsLength & 0x07;
            Cur += TransTable[srcFile[i]].EncodedBitsLength >> 3;
        } else {
            for(INT8 j = 0, end = (TransTable[srcFile[i]].EncodedBitsLength >> 3) - ((TransTable[srcFile[i]].EncodedBitsLength & 0x07) == 0 ? 1 : 0); j <= end; j++) {
                CompressBuffer[Cur + j] |= TransTable[srcFile[i]].EncodedBytes[j] >> CurUsed;
                CompressBuffer[Cur + j + 1] |= TransTable[srcFile[i]].EncodedBytes[j] << (8 - CurUsed);
            }
            Cur += (CurUsed + TransTable[srcFile[i]].EncodedBitsLength) >> 3;
            CurUsed = (CurUsed + TransTable[srcFile[i]].EncodedBitsLength) & 0x07;
        }
    }

    if(Cur != 0 || CurUsed != 0) {
        if(!WriteFile(h_dstFile, CompressBuffer, Cur + 1, NULL, NULL)) {
            UnmapViewOfFile(srcFile);
            CloseHandle(h_srcFileMapping);
            CloseHandle(h_srcFile);
            CloseHandle(h_dstFile);
            free(CompressBuffer);
            return false;
        }
    }

    UnmapViewOfFile(srcFile);
    CloseHandle(h_srcFileMapping);
    free(CompressBuffer);
    CloseHandle(h_srcFile);
    CloseHandle(h_dstFile);
    return true;
}

bool HuffmanCompression::DecompressFile(const char* srcFilePath, const char* dstFilePath) {
    HANDLE h_srcFile = CreateFileA(srcFilePath,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
    if(h_srcFile == INVALID_HANDLE_VALUE) return false;

    HANDLE h_dstFile = CreateFileA(dstFilePath,
                                   GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_READ,
                                   NULL,
                                   CREATE_ALWAYS,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
    if(h_dstFile == INVALID_HANDLE_VALUE) {
        CloseHandle(h_srcFile);
        return false;
    }

    LARGE_INTEGER srcFileSize;
    if(!GetFileSizeEx(h_srcFile, &srcFileSize)) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    LARGE_INTEGER dstFileSize;
    if(!ReadFile(h_srcFile, &dstFileSize, sizeof(LARGE_INTEGER), NULL, NULL)) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    BinaryTreeNode<struct BYTECounter>* Statistics[256];
    for(UINT16 i = 0; i < 0x100; i++) {
        Statistics[i] = new BinaryTreeNode<struct BYTECounter>();
        if(!ReadFile(h_srcFile, &(Statistics[i]->NodeData), sizeof(struct BYTECounter), NULL, NULL)) {
            for(UINT16 j = i; j > 0; j--) delete Statistics[j];
            delete Statistics[0];
            CloseHandle(h_srcFile);
            CloseHandle(h_dstFile);
            return false;
        }
    }

    //Generate HuffmanTree
    for(UINT32 i = 0xFF; i > 0; i--) {
        SortStatisticsData(Statistics, i + 1);
        BinaryTreeNode<struct BYTECounter>* tmp = new BinaryTreeNode<struct BYTECounter>();
        tmp->LeftChildNode = Statistics[i];
        tmp->RightChildNode = Statistics[i - 1];
        tmp->NodeData.Count = tmp->LeftChildNode->NodeData.Count + tmp->RightChildNode->NodeData.Count;
        tmp->NodeData.Value = 0;
        Statistics[i] = nullptr;
        Statistics[i - 1] = tmp;
    }
    BinaryTree<struct BYTECounter> HuffmanTree;
    HuffmanTree.RootNode = Statistics[0];
    Statistics[0] = nullptr;

    //Start decompressing
    BYTE* DecompressBuffer = (BYTE*)malloc(sizeof(BYTE) * 10485760); //10485760 is 10MB;
    if(DecompressBuffer == NULL) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    HANDLE h_srcFileMapping = CreateFileMappingA(h_srcFile, NULL, PAGE_READONLY, srcFileSize.HighPart, srcFileSize.LowPart, NULL);
    if(h_srcFileMapping == INVALID_HANDLE_VALUE) {
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }


    BYTE* srcFile = (BYTE*)MapViewOfFile(h_srcFileMapping, FILE_MAP_READ, 0, 0, srcFileSize.QuadPart);
    if(srcFile == NULL) {
        CloseHandle(h_srcFileMapping);
        CloseHandle(h_srcFile);
        CloseHandle(h_dstFile);
        return false;
    }

    srcFile = srcFile + sizeof(LARGE_INTEGER) + 256 * sizeof(struct BYTECounter);
    UINT32 Cur = 0;
    BinaryTreeNode<struct BYTECounter>* CurNode = HuffmanTree.RootNode;
    srcFileSize.QuadPart -= sizeof(LARGE_INTEGER) + 256 * sizeof(struct BYTECounter);
    for(INT64 i = 0; i < srcFileSize.QuadPart; i++) {
        for(BYTE Ptr = 0x80; Ptr > 0; Ptr >>= 1) {
            if(Ptr & srcFile[i]) CurNode = CurNode->RightChildNode;
            else CurNode = CurNode->LeftChildNode;
            if(CurNode != nullptr && CurNode->LeftChildNode == nullptr && CurNode->RightChildNode == nullptr) {
                DecompressBuffer[Cur++] = CurNode->NodeData.Value;
                CurNode = HuffmanTree.RootNode;
                if(--dstFileSize.QuadPart == 0) {
                    if(!WriteFile(h_dstFile, DecompressBuffer, Cur, NULL, NULL)) {
                        free(DecompressBuffer);
                        UnmapViewOfFile(srcFile);
                        CloseHandle(h_srcFileMapping);
                        CloseHandle(h_srcFile);
                        CloseHandle(h_dstFile);
                        return false;
                    }
                    goto Fin;
                }
                if(Cur == 10485760) {
                    if(!WriteFile(h_dstFile, DecompressBuffer, Cur, NULL, NULL)) {
                        free(DecompressBuffer);
                        UnmapViewOfFile(srcFile);
                        CloseHandle(h_srcFileMapping);
                        CloseHandle(h_srcFile);
                        CloseHandle(h_dstFile);
                        return false;
                    }
                    Cur = 0;
                }
            }
        }
    }

Fin:free(DecompressBuffer);
    UnmapViewOfFile(srcFile);
    CloseHandle(h_srcFileMapping);
    CloseHandle(h_srcFile);
    CloseHandle(h_dstFile);
    if(dstFileSize.QuadPart != 0) return false;
    else return true;
}
