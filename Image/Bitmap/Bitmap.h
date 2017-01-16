#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED
#include <iostream>
#include <windows.h>


typedef struct _BitmapFileHeader {  // 16 bytes
    WORD wUseless;   // just for alignment.
    WORD wBitmapFileType;
    DWORD dwBitmapFileSize;
    WORD wBitmapFileReserved1;
    WORD wBitmapFileReserved2;
    DWORD dwBitmapFileOffBits;
} BitmapFileHeader, *LPBitmapFileHeader;

typedef struct _BitmapInfoHeader {  // 40 bytes
    DWORD  dwBitmapInfoSize;
    LONG   lBitmapInfoWidth;
    LONG   lBitmapInfoHeight;
    WORD   wBitmapInfoPlanes;
    WORD   wBitmapInfoBitCount;
    DWORD  dwBitmapInfoCompression;
    DWORD  dwBitmapInfoSizeImage;
    LONG   lBitmapInfoXPixelsPerMeter;
    LONG   lBitmapInfoYPixelsPerMeter;
    DWORD  dwBitmapInfoColorUsed;
    DWORD  dwBitmapInfoColorImportant;
} BitmapInfoHeader, *LPBitmapInfoHeader;

typedef struct _PaletteEntry {  // 4 bytes
    BYTE peBlue;
    BYTE peGreen;
    BYTE peRed;
    BYTE peFlags;
} PaletteEntry, *LPPaletteEntry;

typedef struct _BitmapFileData {
    BitmapFileHeader FileHeader;
    BitmapInfoHeader InfoHeader;
    PaletteEntry Palette[256];
    BYTE* PixelData;
} BitmapFileData, *LPBitmapFileData;

typedef struct _Color3B {
    BYTE Blue;
    BYTE Green;
    BYTE Red;
} Color3B, *LPColor3B;

typedef struct _Color4B {
    BYTE Blue;
    BYTE Green;
    BYTE Red;
    BYTE Alpha;
} Color4B, *LPColor4B;

class Bitmap {
private:
    LPBitmapFileData lpBitmapFileData;
    BOOL IsValid;
    BYTE *Mask, *Offset;
public:
    Bitmap();
    Bitmap(LPCSTR szFilePath);
    ~Bitmap();
    BOOL IsBitmapFileValid();
    DWORD GetBitmapFileSize();
    DWORD GetBitmapFilePixelDataOffset();
    LONG GetBitmapWidth();
    LONG GetBitmapHeight();
    WORD GetBitmapBitCount();
    DWORD GetPixelDataLength();
    BOOL GetRGBFromPixel(DWORD dwX, DWORD dwY, Color3B& dstColor3B);
    BOOL GetARGBFromPixel(DWORD dwX, DWORD dwY, Color4B& dstColor4B);
    BOOL SetRGBFromPixel(DWORD dwX, DWORD dwY, Color3B& srcColor3B);
    BOOL SetARGBFromPixel(DWORD dwX, DWORD dwY, Color4B& srcColor4B);
};

#endif // BITMAP_H_INCLUDED
