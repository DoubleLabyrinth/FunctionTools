# FunctionTools
This repository contains tools which can be used while programming.

## 1. Audio  
~~~
Todo
~~~
## 2. Compression
*   Huffman Compression  
    *   This function is declared in Huffman.h and defined in Huffman.cpp.
    *   The declaration is here below:
        ~~~c++
        static bool CompressFile(const char* srcFilePath, const char* dstFilePath);
        static bool DecompressFile(const char* srcFilePath, const char* dstFilePath);
        ~~~
    *   If you want to compress a file through huffman compression, just pass the file's path to parameter `srcFilePath` and the output file's path to `dstFilePath`. If successed, the function will return true. Otherwise, it will return false. For example:
        ~~~c++
        //Compress "test.exe" at D:\, please ensure that D:\test.exe is existed.

        #include <iosteam>
        #include "Huffman.h"

        int main() {
            if(HuffmanCompression::CompressFile("D:\\test.exe", "D:\\test.exe.HuffmanCmp")) {
                std::cout << "Successed!\n";
            } else {
                std::cout << "Failed!\n";
            }
            return 0:
        }
        ~~~

        If you want to decompress test.exe.HuffmanCmp, just like here below:
        ~~~c++
        //Decompress "test.exe.HuffmanCmp" at D:\, please ensure that D:\test.exe.HuffmanCmp is existed.

        #include <iosteam>
        #include "Huffman.h"

        int main() {
            if(HuffmanCompression::DecompressFile("D:\\test.exe.HuffmanCmp", "D:\\test.exe")) {
                std::cout << "Successed!\n";
            } else {
                std::cout << "Failed!\n";
            }
            return 0:
        }
        ~~~
    * **About how Huffman compression is implemented**  
        ~~~
        Todo
        ~~~
## 3. DataVerification
*   CRC
    ~~~
    Todo
    ~~~
## 4. Decoding
*   Base16/Base32/Base64
    ~~~
    Todo
    ~~~
