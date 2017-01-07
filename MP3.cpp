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
        std::cerr << "Error: Failed to create HANDLE hSourceFile." << std::endl;
        return;
    }
    // If pass, pay attention to CloseHandle(hSourceFile);
    //start to read ID3V2 header data
    ID3V2_Header = (LPID3V2Header)malloc(sizeof(ID3V2Header));      // Pay attention to free.
    if(ID3V2_Header == NULL) {
        std::cerr << "Error: Could not allocate memory for ID3V2_Header." << std::endl;
        return;
    }
    if(ReadFile(hSourceFile, ID3V2_Header, sizeof(ID3V2Header), NULL, NULL) == FALSE) {
        std::cerr << "Error: Failed to execute function ReadFile while loading ID3V2_Header." << std::endl;
        CloseHandle(hSourceFile);
        free(ID3V2_Header);
        ID3V2_Header = NULL;
        return;
    }
    Valid = IsValid();
    if(!Valid) {
        std::cerr << "Error: Could not read ID3V2 Tag." << std::endl;
        CloseHandle(hSourceFile);
        free(ID3V2_Header);
        ID3V2_Header = NULL;
        Valid = FALSE;
        return;
    }
    if(ID3V2_Header->Flags.ExtendedHeader_Flag == 1) {
        ID3V2_ExtendedHeader = (LPID3V2ExtendedHeader)malloc(sizeof(ID3V2ExtendedHeader));
        if(ID3V2_ExtendedHeader == NULL) {
            std::cerr << "Error: Could not allocate memory for ID3V2_ExtendedHeader." << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            ID3V2_Header = NULL;
            Valid = FALSE;
            return;
        }
        if(ReadFile(hSourceFile, ID3V2_ExtendedHeader, sizeof(ID3V2ExtendedHeader), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading ID3V2_ExtendedHeader." << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            Valid = FALSE;
            return;
        }
        if(ID3V2_ExtendedHeader->HasTotalFrameCRC == 1) {
            if(ReadFile(hSourceFile, &TotalFrameCRC, sizeof(UINT32), NULL, NULL) == FALSE) {
                std::cerr << "Error: Failed to execute function ReadFile while loading TotalFrameCRC." << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                Valid = FALSE;
                TotalFrameCRC = 0;
                return;
            }
        }
    }
    //Start to read ID3V2 frame data
    do{
        UINT32 tmpFrameID = 0;
        if(ReadFile(hSourceFile, &tmpFrameID, sizeof(UINT32), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading tmpFrameID." << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            Valid = FALSE;
            TotalFrameCRC = 0;
            return;
        }
        if(tmpFrameID == 0) break;
        LPID3V2Frame tmpLPID3V2Frame = (LPID3V2Frame)malloc(sizeof(LPID3V2Frame));
        if(tmpLPID3V2Frame == NULL) {
            std::cerr << "Error: Could not allocate memory for tmpLPID3V2Frame." << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            Valid = FALSE;
            TotalFrameCRC = 0;
            return;
        }
        *(UINT32*)(&(tmpLPID3V2Frame->Header.FrameID)) = tmpFrameID;
        if(ReadFile(hSourceFile, &(tmpLPID3V2Frame->Header.Size), sizeof(ID3V2FrameHeader) - sizeof(tmpFrameID), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading tmpLPID3V2Frame->Header.Size." << std::endl;
            CloseHandle(hSourceFile);
            free(ID3V2_Header);
            free(ID3V2_ExtendedHeader);
            free(tmpLPID3V2Frame);
            ID3V2_Header = NULL;
            ID3V2_ExtendedHeader = NULL;
            Valid = FALSE;
            TotalFrameCRC = 0;
            return;
        }
        if(SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size) > 0) {
            tmpLPID3V2Frame->FrameData = (BYTE*)malloc(SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size));
            if(tmpLPID3V2Frame->FrameData == NULL) {
                std::cerr << "Error: Could not allocate memory for tmpLPID3V2Frame->FrameData." << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                free(tmpLPID3V2Frame);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                Valid = FALSE;
                TotalFrameCRC = 0;
                return;
            }
            if(ReadFile(hSourceFile, tmpLPID3V2Frame->FrameData, SizeInBYTEToUINT32(tmpLPID3V2Frame->Header.Size), NULL, NULL) == FALSE) {
                std::cerr << "Error: Failed to execute function ReadFile while loading tmpLPID3V2Frame->FrameData." << std::endl;
                CloseHandle(hSourceFile);
                free(ID3V2_Header);
                free(ID3V2_ExtendedHeader);
                free(tmpLPID3V2Frame->FrameData);
                free(tmpLPID3V2Frame);
                ID3V2_Header = NULL;
                ID3V2_ExtendedHeader = NULL;
                Valid = FALSE;
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
    UINT32 ID3V2_FramesCount = ID3V2_Frames.size();
    for(UINT32 i = 0; i < ID3V2_FramesCount; i++) free(ID3V2_Frames[i]);
    ID3V2_Frames.clear();
}

BOOL ID3V2Tag::IsValid() {
    if(ID3V2_Header == NULL) return FALSE;
    if(ID3V2_Header->Head[0] == 'I' && ID3V2_Header->Head[1] == 'D' && ID3V2_Header->Head[2] == '3' && ID3V2_Header->Flags.Reserved == 0) return TRUE;
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

MPEGSegment::MPEGSegment() {
    Valid = FALSE;
}

MPEGSegment::MPEGSegment(LPCTSTR FilePath) {
    Valid = FALSE;

    HANDLE hSourceFile = CreateFile(FilePath,
                                    GENERIC_READ,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);
    if(hSourceFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Failed to create HANDLE hSourceFile." << std::endl;
        return;
    }
    ID3V2Header tmpID3V2Header;
    if(ReadFile(hSourceFile, &tmpID3V2Header, sizeof(ID3V2Header), NULL, NULL) == FALSE) {
        std::cerr << "Error: Failed to execute function ReadFile while loading tmpID3V2Header." << std::endl;
        CloseHandle(hSourceFile);
        return;
    }
    if(SetFilePointer(hSourceFile, SizeInBYTEToUINT32(tmpID3V2Header.Size), NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER) {
        std::cerr << "Error: Failed to execute function SetFilePointer." << std::endl;
        CloseHandle(hSourceFile);
        return;
    }
    do {
        LPMPEGFrame tmpLPMPEGFrame = (LPMPEGFrame)malloc(sizeof(MPEGFrame));
        if(tmpLPMPEGFrame == NULL) {
            std::cerr << "Error: Failed to allocate memory for tmpLPMPEGFrame." << std:: endl;
            CloseHandle(hSourceFile);
            return;
        }
        if(ReadFile(hSourceFile, &(tmpLPMPEGFrame->Header), sizeof(MPEGFrameHeader), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading tmpLPMPEGFrame->Header." << std::endl;
            CloseHandle(hSourceFile);
            free(tmpLPMPEGFrame);
            return;
        }
        if(tmpLPMPEGFrame->Header.FrameSyncWord != 0x7FF) {
            std::cerr << "Error: Not a valid MPEG frame." << std::endl;
            CloseHandle(hSourceFile);
            free(tmpLPMPEGFrame);
            return;
        }
        UINT32 FrameSize = 0;
        switch(tmpLPMPEGFrame->Header.LayerID) {
            //ToDo
        }
        //ToDo
    } while(TRUE);
    //ToDo
}

UINT32 MPEGSegment::GetSamplingFrequency() {

}
