#include "MP3.h"

ID3V2::ID3V2() {
    ID3V2_Header = NULL;
    ID3V2_ExtendedHeader = NULL;
    TotalFrameCRC = NULL;
    Valid = FALSE;
}

ID3V2::ID3V2(LPTSTR FilePath) {
    ID3V2_Header = NULL;
    ID3V2_ExtendedHeader = NULL;
    TotalFrameCRC = NULL;
    Valid = FALSE;
    HANDLE hSourceFile = CreateFile(FilePath,
                                    GENERIC_READ,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);
    if(hSourceFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        return;
    } // If pass, pay attention to CloseHandle(hSourceFile);
    ID3V2_Header = (LPID3V2Header)malloc(sizeof(ID3V2Header));      // Pay attention to free.
    if(ID3V2_Header == NULL) {
        std::cerr << "Error, could not malloc ID3V2_Header" << std::endl;
        return;
    }
    if(ReadFile(hSourceFile, ID3V2_Header, sizeof(ID3V2Header), NULL, NULL) == FALSE) {
        std::cerr << "Error Code: " << GetLastError() << std::endl;
        CloseHandle(hSourceFile);
        free(ID3V2_Header);
        ID3V2_Header = NULL;
        return;
    }
    Valid = IsValid();
    if(!Valid) {
        std::cerr << "Error: This is not a valid MP3 file." << std::endl;
        CloseHandle(hSourceFile);
        free(ID3V2_Header);
        ID3V2_Header = NULL;
        return;
    }
    if(HasExtendedHeaderFlag() == TRUE) {
        ID3V2_ExtendedHeader = (LPID3V2ExtendedHeader)malloc(sizeof(ID3V2ExtendedHeader));
        if(ID3V2_ExtendedHeader == NULL) {
            std::cerr << "Error, could not malloc ID3V2_ExtendedHeader" << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            ID3V2_Header = NULL;
            return;
        }
        if(ReadFile(hSourceFile, ID3V2_ExtendedHeader, sizeof(ID3V2ExtendedHeader), NULL, NULL) == FALSE) {
            std::cerr << "Error Code: " << GetLastError() << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_ExtendedHeader);
            free(ID3V2_Header);
            ID3V2_ExtendedHeader = NULL;
            ID3V2_Header = NULL;
            return;
        }
        if(ID3V2_ExtendedHeader->ExtendedFlags[0] == 0x80) {
            TotalFrameCRC = (UINT32*)malloc(sizeof(UINT32));
            if(TotalFrameCRC == NULL) {
                std::cerr << "Error, could not malloc TotalFrameCRC" << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                return;
            }
            if(ReadFile(hSourceFile, TotalFrameCRC, sizeof(UINT32), NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_ExtendedHeader);
                free(ID3V2_Header);
                free(TotalFrameCRC);
                ID3V2_ExtendedHeader = NULL;
                ID3V2_Header = NULL;
                TotalFrameCRC = NULL;
                return;
            }
        }


        //ToDo

        CloseHandle(hSourceFile);
    }
}

ID3V2::~ID3V2() {
    free(ID3V2_Header);
    free(ID3V2_ExtendedHeader);
    free(TotalFrameCRC);
}

BOOL ID3V2::IsValid() {
    if(ID3V2_Header == NULL) return FALSE;
    if(ID3V2_Header->Header[0] == 'I' && ID3V2_Header->Header[1] == 'D' && ID3V2_Header->Header[2] == '3' && (ID3V2_Header->Flags & 0x1F) == 0) return TRUE;
    return FALSE;
}

BOOL ID3V2::HasUnsynchronisationFlag() {
    if(!Valid) return FALSE;
    if((ID3V2_Header->Flags & Unsynchronisation_Flag) != 0) return TRUE;
    return FALSE;
}

BOOL ID3V2::HasExtendedHeaderFlag() {
    if (!Valid) return FALSE;
    if((ID3V2_Header->Flags & ExtendedHeader_Flag) != 0) return TRUE;
    return FALSE;
}

BOOL ID3V2::HasExperimentalIndicatorFlag() {
    if(!Valid) return FALSE;
    if((ID3V2_Header->Flags & ExperimentalIndicator_Flag) != 0) return TRUE;
    return FALSE;
}

BYTE ID3V2::GetID3V2MajorVersion() {
    return ID3V2_Header->MajorVersion;
}

BYTE ID3V2::GetID3V2ReversionNumber() {
    return ID3V2_Header->RevisionNumber;
}

UINT32 ID3V2::GetID3V2FrameSize() {
    if(!Valid) return 0;
    return ((((((UINT32)ID3V2_Header->Size[0] << 7) + ID3V2_Header->Size[1]) << 7) + ID3V2_Header->Size[2]) << 7) + ID3V2_Header->Size[3];
}
