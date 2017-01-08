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
            ClearID3V2_Frames();
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
            ClearID3V2_Frames();
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
            ClearID3V2_Frames();
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
                ClearID3V2_Frames();
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
                ClearID3V2_Frames();
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
    ClearID3V2_Frames();
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
    return CalculateSize(ID3V2_Header->Size);
}

UINT32 ID3V2Tag::GetID3V2FrameCount() {
    return ID3V2_Frames.size();
}

const LPID3V2Frame ID3V2Tag::GetID3V2Frame(UINT32 i) {
    return ID3V2_Frames[i];
}

void ID3V2Tag::ClearID3V2_Frames() {
    UINT32 ID3V2_FramesCount = ID3V2_Frames.size();
    for(UINT32 i = 0; i < ID3V2_FramesCount; i++) {
        free(ID3V2_Frames[i]->FrameData);
        free(ID3V2_Frames[i]);
    }
    ID3V2_Frames.clear();
}

MPEGTag::MPEGTag() {
    Valid = FALSE;
}

MPEGTag::MPEGTag(LPCTSTR FilePath) {
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
    if(SetFilePointer(hSourceFile, CalculateSize(tmpID3V2Header.Size), NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER) {
        std::cerr << "Error: Failed to execute function SetFilePointer." << std::endl;
        CloseHandle(hSourceFile);
        return;
    }
    do {
        LPMPEGFrame tmpLPMPEGFrame = (LPMPEGFrame)malloc(sizeof(MPEGFrame));
        if(tmpLPMPEGFrame == NULL) {
            std::cerr << "Error: Failed to allocate memory for tmpLPMPEGFrame." << std:: endl;
            CloseHandle(hSourceFile);
            ClearMPEGFrames();
            return;
        }
        if(ReadFile(hSourceFile, &(tmpLPMPEGFrame->Header), sizeof(MPEGFrameHeader), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading tmpLPMPEGFrame->Header." << std::endl;
            CloseHandle(hSourceFile);
            free(tmpLPMPEGFrame);
            ClearMPEGFrames();
            return;
        }
        if(tmpLPMPEGFrame->Header.FrameSyncWord0 != 0xFF || tmpLPMPEGFrame->Header.FrameSyncWord1 != 0x07) break;
        UINT32 FrameSize = 0;
        switch(tmpLPMPEGFrame->Header.LayerID) {
            case Layer1:
                FrameSize = (12 * GetBitrate(tmpLPMPEGFrame->Header.MPEGVersionID, tmpLPMPEGFrame->Header.LayerID, tmpLPMPEGFrame->Header.BitrateIndex) / SamplingFrequencyTable[tmpLPMPEGFrame->Header.MPEGVersionID][tmpLPMPEGFrame->Header.FrequencyIndex] + tmpLPMPEGFrame->Header.PaddingBit) << 2;
            case Layer2:
            case Layer3:
                FrameSize = 144 * GetBitrate(tmpLPMPEGFrame->Header.MPEGVersionID, tmpLPMPEGFrame->Header.LayerID, tmpLPMPEGFrame->Header.BitrateIndex) / SamplingFrequencyTable[tmpLPMPEGFrame->Header.MPEGVersionID][tmpLPMPEGFrame->Header.FrequencyIndex] + tmpLPMPEGFrame->Header.PaddingBit;
        }
        tmpLPMPEGFrame->FrameData = (BYTE*)malloc(FrameSize - sizeof(MPEGFrameHeader));
        if(tmpLPMPEGFrame->FrameData == NULL) {
            std::cerr << "Error: Could not allocate memory for tmpLPMPEGFrame->FrameData." << std::endl;
            CloseHandle(hSourceFile);
            free(tmpLPMPEGFrame);
            ClearMPEGFrames();
            return;
        }
        if(ReadFile(hSourceFile, tmpLPMPEGFrame->FrameData, FrameSize - sizeof(MPEGFrameHeader), NULL, NULL) == FALSE) {
            std::cerr << "Error: Failed to execute function ReadFile while loading tmpLPMPEGFrame->FrameData." << std::endl;
            CloseHandle(hSourceFile);
            free(tmpLPMPEGFrame->FrameData);
            free(tmpLPMPEGFrame);
            ClearMPEGFrames();
            return;
        }
        MPEGFrames.push_back(tmpLPMPEGFrame);
    } while(TRUE);
    CloseHandle(hSourceFile);
}

MPEGTag::~MPEGTag() {
    ClearMPEGFrames();
}

UINT32 MPEGTag::GetBitrate(BYTE MPEGVersionID, BYTE LayerID, BYTE BitrateIndex) {
    if(MPEGVersionID > 3 || LayerID > 3 || BitrateIndex > 15) return 0xFFFFFFFF;
    switch(MPEGVersionID) {
        case MPEG10:
            if(LayerID == Layer1) return BitrateTableForM1L1[BitrateIndex];
            if(LayerID == Layer2) return BitrateTableForM1L2[BitrateIndex];
            if(LayerID == Layer3) return BitrateTableForM1L3[BitrateIndex];
            break;
        case MPEG20:
        case MPEG25:
            if(LayerID == Layer1) return BitrateTableForM2L1[BitrateIndex];
            if(LayerID == Layer2 || LayerID == Layer3) return BitrateTableForM2L23[BitrateIndex];
            break;
        default:
            break;
    }
    return 0xFFFFFFFF;
}

const LPMPEGFrame MPEGTag::GetMPEGFrame(UINT32 i) {
    if(i >= MPEGFrames.size()) return NULL;
    return MPEGFrames[i];
}

UINT32 MPEGTag::GetMPEGFramesCount() {
    return MPEGFrames.size();
}

UINT32 MPEGTag::GetBitrate(const LPMPEGFrame srcFrame) {
    return GetBitrate(srcFrame->Header.MPEGVersionID, srcFrame->Header.LayerID, srcFrame->Header.BitrateIndex);
}

UINT32 MPEGTag::GetBitrate(UINT32 i) {
    if(i >= MPEGFrames.size()) return 0;
    return GetBitrate(MPEGFrames[i]);
}

UINT32 MPEGTag::GetSamplingFrequency(UINT32 i) {
    if(i >= MPEGFrames.size()) return 0;
    return SamplingFrequencyTable[MPEGFrames[i]->Header.MPEGVersionID][MPEGFrames[i]->Header.FrequencyIndex];
}

void MPEGTag::ClearMPEGFrames() {
    UINT32 MPEGFramesCount = MPEGFrames.size();
    for(UINT32 i = 0; i < MPEGFramesCount; i++) {
        free(MPEGFrames[i]->FrameData);
        free(MPEGFrames[i]);
    }
    MPEGFrames.clear();
}
