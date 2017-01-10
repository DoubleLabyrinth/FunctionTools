#include "Bitmap.h"

BYTE MaskFor1[] = {0x1, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02};
BYTE OffsetFor1[] = {0, 7, 6, 5, 4, 3, 2, 1};
BYTE MaskFor4[] = {0xF, 0xF0};
BYTE OffsetFor4[] = {0, 4};
BYTE MaskFor8[] = {0xFF};
BYTE OffsetFor8[] = {0};

Bitmap::Bitmap() {
    lpBitmapFileData = NULL;
    IsValid = FALSE;
}

Bitmap::Bitmap(LPCSTR szFilePath) {
    HANDLE hSourceFile = CreateFileA(szFilePath,
                                     GENERIC_READ,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL);
    if(hSourceFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        Bitmap();
        return;
    }
    lpBitmapFileData = (LPBitmapFileData)malloc(sizeof(BitmapFileData));
    if(ReadFile(hSourceFile, &(lpBitmapFileData->FileHeader.wBitmapFileType), 14, NULL, NULL) == FALSE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        CloseHandle(hSourceFile);
        Bitmap();
        return;
    }
    if(ReadFile(hSourceFile, &(lpBitmapFileData->InfoHeader), 40, NULL, NULL) == FALSE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        CloseHandle(hSourceFile);
        Bitmap();
        return;
    }
    if(lpBitmapFileData->FileHeader.wBitmapFileType != 0x4d42) {
        std::cerr << "Error: " << "The file is not a BMP file." << std::endl;
        CloseHandle(hSourceFile);
        Bitmap();
        return;
    }
    switch(lpBitmapFileData->InfoHeader.wBitmapInfoBitCount) {
        case 1:
            if(ReadFile(hSourceFile, lpBitmapFileData->Palette, 2 * 4, NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                Bitmap();
                return;
            }
            for(int i = 2; i < 256; i++) lpBitmapFileData->Palette[i] = {0, 0, 0, 0};
            Mask = MaskFor1;
            Offset = OffsetFor1;
            break;
        case 4:
            if(ReadFile(hSourceFile, lpBitmapFileData->Palette, 16 * 4, NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                Bitmap();
                return;
            }
            for(int i = 16; i < 256; i++) lpBitmapFileData->Palette[i] = {0, 0, 0, 0};
            Mask = MaskFor4;
            Offset = OffsetFor4;
            break;
        case 8:
            if(ReadFile(hSourceFile, lpBitmapFileData->Palette, 256 * 4, NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                Bitmap();
                return;
            }
            Mask = MaskFor8;
            Offset = OffsetFor8;
            break;
        default:
            Mask = NULL;
            Offset = NULL;
            for(int i = 0; i < 256; i++) lpBitmapFileData->Palette[i] = {0, 0, 0, 0};
    }
    lpBitmapFileData->PixelData = (BYTE*)malloc(lpBitmapFileData->InfoHeader.dwBitmapInfoSizeImage);
    if(ReadFile(hSourceFile, lpBitmapFileData->PixelData, lpBitmapFileData->InfoHeader.dwBitmapInfoSizeImage, NULL, NULL) == FALSE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        CloseHandle(hSourceFile);
        Bitmap();
        return;
    }
    CloseHandle(hSourceFile);
    IsValid = TRUE;
}

Bitmap::~Bitmap() {
    if(lpBitmapFileData != NULL) {
        free(lpBitmapFileData->PixelData);
        free(lpBitmapFileData);
    }
}

BOOL Bitmap::IsBitmapFileValid() {
    return IsValid;
}

DWORD Bitmap::GetBitmapFileSize() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->FileHeader.dwBitmapFileSize;
}

DWORD Bitmap::GetBitmapFilePixelDataOffset() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->FileHeader.dwBitmapFileOffBits;
}

LONG Bitmap::GetBitmapWidth() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->InfoHeader.lBitmapInfoWidth;
}

LONG Bitmap::GetBitmapHeight() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->InfoHeader.lBitmapInfoHeight;
}

WORD Bitmap::GetBitmapBitCount() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->InfoHeader.wBitmapInfoBitCount;
}

DWORD Bitmap::GetPixelDataLength() {
    if(IsValid == FALSE) return 0;
    return lpBitmapFileData->InfoHeader.dwBitmapInfoSizeImage;
}

BOOL Bitmap::GetRGBFromPixel(DWORD dwX, DWORD dwY, Color3B& dstColor3B) {
    DWORD MaxX = lpBitmapFileData->InfoHeader.lBitmapInfoWidth;
    DWORD MaxY = lpBitmapFileData->InfoHeader.lBitmapInfoHeight;
    if(dwX > MaxX || dwX < 1 || dwY < 1 || dwY > MaxY) return FALSE;
    DWORD _dwX = dwX - 1;
    DWORD _dwY = dwY - 1;
    WORD BitmapBitCount = lpBitmapFileData->InfoHeader.wBitmapInfoBitCount;
    DWORD BytesPerLine = MaxX * BitmapBitCount;
    BytesPerLine += 8 - (BytesPerLine % 8);
    BytesPerLine >>= 3;
    BytesPerLine += 4 -(BytesPerLine % 4);
    BYTE (*Ptr)[BytesPerLine];
    Ptr = (BYTE(*)[BytesPerLine])lpBitmapFileData->PixelData;
    _dwX = (_dwX * BitmapBitCount) >> 3;
    void* ColorAddress = *(Ptr + _dwY) + _dwX;

    switch(BitmapBitCount) {
        case 1:
        case 4:
        case 8: {
            BYTE ColorValue = Ptr[_dwY][_dwX];
            BYTE Flag = dwX % (8 / BitmapBitCount);
            ColorValue &= Mask[Flag];
            ColorValue >>= Offset[Flag];
            dstColor3B = *(LPColor3B)(lpBitmapFileData->Palette + ColorValue);
            return TRUE;
        }
        case 16: {
            WORD ColorValue = *(WORD*)ColorAddress;
            dstColor3B.Blue = ColorValue & 0x1F;
            dstColor3B.Green = (ColorValue & 0x3E0) >> 5;
            dstColor3B.Red = (ColorValue & 0x7C00) >> 10;
            return TRUE;
        }
        case 24:
        case 32:
            dstColor3B = *(LPColor3B)ColorAddress;
            return TRUE;
        default:
            return FALSE;
    }
}

BOOL Bitmap::GetARGBFromPixel(DWORD dwX, DWORD dwY, Color4B& dstColor4B) {
    DWORD MaxX = lpBitmapFileData->InfoHeader.lBitmapInfoWidth;
    DWORD MaxY = lpBitmapFileData->InfoHeader.lBitmapInfoHeight;
    if(dwX > MaxX || dwX < 1 || dwY < 1 || dwY > MaxY) return FALSE;
    DWORD _dwX = dwX - 1;
    DWORD _dwY = dwY - 1;
    WORD BitmapBitCount = lpBitmapFileData->InfoHeader.wBitmapInfoBitCount;
    DWORD BytesPerLine = MaxX * BitmapBitCount;
    BytesPerLine += 8 - (BytesPerLine % 8);
    BytesPerLine >>= 3;
    BytesPerLine += 4 -(BytesPerLine % 4);
    BYTE (*Ptr)[BytesPerLine];
    Ptr = (BYTE(*)[BytesPerLine])lpBitmapFileData->PixelData;
    _dwX = (_dwX * BitmapBitCount) >> 3;
    void* ColorAddress = *(Ptr + _dwY) + _dwX;

    switch(BitmapBitCount) {
        case 1:
        case 4:
        case 8: {
            BYTE ColorValue = Ptr[_dwY][_dwX];
            BYTE Flag = dwX % (8 / BitmapBitCount);
            ColorValue &= Mask[Flag];
            ColorValue >>= Offset[Flag];
            dstColor4B = *(LPColor4B)(lpBitmapFileData->Palette + ColorValue);
            dstColor4B.Alpha = 0xFF;
            return TRUE;
        }
        case 16: {
            WORD ColorValue = *(WORD*)ColorAddress;
            dstColor4B.Blue = ColorValue & 0x1F;
            dstColor4B.Green = (ColorValue & 0x3E0) >> 5;
            dstColor4B.Red = (ColorValue & 0x7C00) >> 10;
            dstColor4B.Alpha = 0x1F;
            return TRUE;
        }
        case 24: {
            DWORD ColorValue = *(DWORD*)ColorAddress;
            dstColor4B.Blue = ColorValue & 0xFF;
            dstColor4B.Green = (ColorValue & 0xFF00) >> 8;
            dstColor4B.Red = (ColorValue & 0xFF0000) >> 16;
            dstColor4B.Alpha = 0xFF;
            return TRUE;
        }
        case 32:
            dstColor4B = *(LPColor4B)ColorAddress;
            return TRUE;
        default:
            return FALSE;
    }
}

BOOL Bitmap::SetRGBFromPixel(DWORD dwX, DWORD dwY, Color3B& srcColor3B) {
    DWORD MaxX = lpBitmapFileData->InfoHeader.lBitmapInfoWidth;
    DWORD MaxY = lpBitmapFileData->InfoHeader.lBitmapInfoHeight;
    if(dwX > MaxX || dwX < 1 || dwY < 1 || dwY > MaxY) return FALSE;
    DWORD _dwX = dwX - 1;
    DWORD _dwY = dwY - 1;
    WORD BitmapBitCount = lpBitmapFileData->InfoHeader.wBitmapInfoBitCount;
    DWORD BytesPerLine = MaxX * BitmapBitCount;
    BytesPerLine += 8 - (BytesPerLine % 8);
    BytesPerLine >>= 3;
    BytesPerLine += 4 -(BytesPerLine % 4);
    BYTE (*Ptr)[BytesPerLine];
    Ptr = (BYTE(*)[BytesPerLine])lpBitmapFileData->PixelData;
    _dwX = (_dwX * BitmapBitCount) >> 3;
    void* ColorAddress = *(Ptr + _dwY) + _dwX;

    switch(BitmapBitCount) {
        case 1:
        case 4:
        case 8: {
            int PaletteNumber = 0x01 << BitmapBitCount;
            BYTE PaletteDst = -1;
            LPPaletteEntry tmp = lpBitmapFileData->Palette;
            for(int i = 0; i < PaletteNumber; i++) {
                if(tmp->peBlue == srcColor3B.Blue && tmp->peGreen == srcColor3B.Green && tmp->peRed == srcColor3B.Red) { PaletteDst = i; break; }
                tmp++;
            }
            if(PaletteDst == -1) return FALSE;
            BYTE Flag = dwX % (8 / BitmapBitCount);
            PaletteDst <<= Offset[Flag];
            Ptr[_dwY][_dwX] = (Ptr[_dwY][_dwX] & ~Mask[Flag]) | PaletteDst;
            return TRUE;
        }
        case 16: {
            if(srcColor3B.Red > 0x1F || srcColor3B.Green > 0x1F || srcColor3B.Blue > 0x1F) return FALSE;
            *(WORD*)ColorAddress = (WORD)srcColor3B.Blue | (WORD)srcColor3B.Green << 5 | (WORD)srcColor3B.Red << 10;
            break;
        }
        case 24:
            *((BYTE*)ColorAddress) = srcColor3B.Blue;
            *((BYTE*)ColorAddress + 1) = srcColor3B.Green;
            *((BYTE*)ColorAddress + 2) = srcColor3B.Red;
            return TRUE;
        default:
            return FALSE;
    }
}

BOOL Bitmap::SetARGBFromPixel(DWORD dwX, DWORD dwY, Color4B& srcColor4B) {
    DWORD MaxX = lpBitmapFileData->InfoHeader.lBitmapInfoWidth;
    DWORD MaxY = lpBitmapFileData->InfoHeader.lBitmapInfoHeight;
    if(dwX > MaxX || dwX < 1 || dwY < 1 || dwY > MaxY || lpBitmapFileData->InfoHeader.wBitmapInfoBitCount != 32) return FALSE;
    DWORD _dwX = dwX - 1;
    DWORD BytesPerLine = MaxX << 2;
    BYTE (*Ptr)[BytesPerLine];
    Ptr = (BYTE(*)[BytesPerLine])lpBitmapFileData->PixelData;
    _dwX <<= 2;
    *(DWORD*)(*(Ptr + dwY - 1) + _dwX) = *(DWORD*)&srcColor4B;
    return TRUE;
}

