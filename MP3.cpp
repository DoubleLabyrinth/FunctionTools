#include "MP3.h"

ID3V2Tag::ID3V2Tag() {
    ID3V2_Header = NULL;
    ID3V2_ExtendedHeader = NULL;
    TotalFrameCRC = 0;
    Valid = FALSE;
}

ID3V2Tag::ID3V2Tag(LPTSTR FilePath) {
    ID3V2_Header = NULL;
    ID3V2_ExtendedHeader = NULL;
    TotalFrameCRC = 0;
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
    //start to read ID3V2 header data
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
            if(ReadFile(hSourceFile, &TotalFrameCRC, sizeof(UINT32), NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_ExtendedHeader);
                free(ID3V2_Header);
                ID3V2_ExtendedHeader = NULL;
                ID3V2_Header = NULL;
                TotalFrameCRC = 0;
                return;
            }
        }
    }
    //Start to read ID3V2 frame data
    do{
        UINT32 tmpFrameID = 0;
        if(ReadFile(hSourceFile, &tmpFrameID, sizeof(UINT32), NULL, NULL) == FALSE) {
            std::cerr << "Error Code: " << GetLastError() << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_ExtendedHeader);
            free(ID3V2_Header);
            ID3V2_ExtendedHeader = NULL;
            ID3V2_Header = NULL;
            TotalFrameCRC = 0;
            return;
        }
        if(tmpFrameID == 0) break;
        LPID3V2Frame tmpLPID3V2Frame = (LPID3V2Frame)malloc(sizeof(LPID3V2Frame));
        if(tmpLPID3V2Frame == NULL) {
            std::cerr << "Error, could not malloc tmpLPID3V2Frame" << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            TotalFrameCRC = 0;
            return;
        }
        *(UINT32*)(&(tmpLPID3V2Frame->Header.FrameID)) = tmpFrameID;
        if(ReadFile(hSourceFile, &(tmpLPID3V2Frame->Header.Size), sizeof(ID3V2FrameHeader) - sizeof(tmpFrameID), NULL, NULL) == FALSE) {
            std::cerr << "Error Code: " << GetLastError() << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            TotalFrameCRC = 0;
            return;
        }
        if(SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size) > 0) {
            tmpLPID3V2Frame->FrameData = (BYTE*)malloc(SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size));
            if(tmpLPID3V2Frame->FrameData == NULL) {
                std::cerr << "Error, could not malloc tmpLPID3V2Frame->FrameData" << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                TotalFrameCRC = 0;
                return;
            }
            if(ReadFile(hSourceFile, tmpLPID3V2Frame->FrameData, SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size), NULL, NULL) == FALSE) {
                std::cerr << "Error Code: " << GetLastError() << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                TotalFrameCRC = 0;
                return;
            }
        }
        ID3V2_Frames.push_back(tmpLPID3V2Frame);
    } while(TRUE);
    CloseHandle(hSourceFile);
}

ID3V2Tag::~ID3V2Tag() {
    free(ID3V2_Header);
    free(ID3V2_ExtendedHeader);
}

BOOL ID3V2Tag::IsValid() {
    if(ID3V2_Header == NULL) return FALSE;
    if(ID3V2_Header->Header[0] == 'I' && ID3V2_Header->Header[1] == 'D' && ID3V2_Header->Header[2] == '3' && (ID3V2_Header->Flags & 0x1F) == 0) return TRUE;
    return FALSE;
}

BOOL ID3V2Tag::HasUnsynchronisationFlag() {
    if(!Valid) return FALSE;
    if((ID3V2_Header->Flags & Unsynchronisation_Flag) != 0) return TRUE;
    return FALSE;
}

BOOL ID3V2Tag::HasExtendedHeaderFlag() {
    if (!Valid) return FALSE;
    if((ID3V2_Header->Flags & ExtendedHeader_Flag) != 0) return TRUE;
    return FALSE;
}

BOOL ID3V2Tag::HasExperimentalIndicatorFlag() {
    if(!Valid) return FALSE;
    if((ID3V2_Header->Flags & ExperimentalIndicator_Flag) != 0) return TRUE;
    return FALSE;
}

BYTE ID3V2Tag::GetID3V2MajorVersion() {
    return ID3V2_Header->MajorVersion;
}

BYTE ID3V2Tag::GetID3V2ReversionNumber() {
    return ID3V2_Header->RevisionNumber;
}

UINT32 ID3V2Tag::GetID3V2FrameSize() {
    if(!Valid) return 0;
    return ((((((UINT32)ID3V2_Header->Size[0] << 7) + ID3V2_Header->Size[1]) << 7) + ID3V2_Header->Size[2]) << 7) + ID3V2_Header->Size[3];
}

UINT32 ID3V2Tag::GetID3V2FrameCount() {
    return ID3V2_Frames.size();
}

const LPID3V2Frame ID3V2Tag::GetID3V2Frame(UINT32 i) {
    return ID3V2_Frames[i];
}
