#ifndef HUFFMAN_COMPRESSION_H_INCLUDED
#define HUFFMAN_COMPRESSION_H_INCLUDED
#include "../../TypeDefine.h"
#include "BinaryTree.h"
#include "windows.h"
#include "stdio.h"

struct BYTECounter {
    BYTE Value;
    UINT64 Count;
};

struct BYTETransformer {
    BYTE Value;
    BYTE EncodedBytes[32];
    BYTE EncodedBitsLength;
};

class HuffmanCompression {
private:
    static void GetHuffmanEncoding(BinaryTreeNode<struct BYTECounter>* srcNode, struct BYTETransformer* srcTransformer, BYTE* BitBuffer, UINT32 Layer, bool GoLeft);
    static void SortStatisticsData(BinaryTreeNode<struct BYTECounter>* srcStatistics[], UINT32 Length);
    HuffmanCompression() = delete;
public:
    static bool CompressFile(const char* srcFilePath, const char* dstFilePath);
    static bool DecompressFile(const char* srcFilePath, const char* dstFilePath);
};
#endif //HUFFMAN_COMPRESSION_H_INCLUDED
