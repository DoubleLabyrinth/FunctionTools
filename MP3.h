#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED
#include <iostream>
#include <windows.h>
//------------------------------------Flags in ID3V2Header
#define Unsynchronisation_Flag 0x80
#define ExtendedHeader_Flag 0x40
#define ExperimentalIndicator_Flag 0x20

#define FooterPresentFlag 0x10
#define TagIsAnUpdateFlag 0x40
#define CRCDataPresentFlag 0x20
#define TagRestrictionsFlag 0x10
//--------------------------------------Flags in ID3V2FrameHeader
#define TagAlterPreservation 0x80
#define FileAlterPreservation 0x40
#define ReadOnly 0x20
#define Compression 0x8000
#define Encryption 0x4000
#define GroupingIdentity 0x2000
//-------------------------------------FrameID in ID3V2FrameHeader
/*#define AENC 0x434E4541    //[[#sec4.20|Audio encryption]]
#define APIC 0x43495041    //[#sec4.15 Attached picture]
#define COMM 0x4D4D4F43    //[#sec4.11 Comments]
#define COMR 0x524D4F43    //[#sec4.25 Commercial frame]
#define ENCR 0x52434E45    //[#sec4.26 Encryption method registration]
#define EQUA 0x41555145    //[#sec4.13 Equalization]
#define ETCO 0x4F435445    //[#sec4.6 Event timing codes]
#define GEOB 0x424F4547    //[#sec4.16 General encapsulated object]
#define GRID 0x44495247    //[#sec4.27 Group identification registration]
#define IPLS 0x534C5049    //[#sec4.4 Involved people list]
#define LINK 0x4B4E494C    //[#sec4.21 Linked information]
#define MCDI 0x4944434D    //[#sec4.5 Music CD identifier]
#define MLLT 0x544C4C4D    //[#sec4.7 MPEG location lookup table]
#define OWNE 0x454E574F    //[#sec4.24 Ownership frame]
#define PRIV 0x56495250    //[#sec4.28 Private frame]
#define PCNT 0x544E4350    //[#sec4.17 Play counter]
#define POPM 0x4D504F50    //[#sec4.18 Popularimeter]
#define POSS 0x53534F50    //[#sec4.22 Position synchronisation frame]
#define RBUF 0x46554252    //[#sec4.19 Recommended buffer size]
#define RVAD 0x44415652    //[#sec4.12 Relative volume adjustment]
#define RVRB 0x42525652    //[#sec4.14 Reverb]
#define SYLT 0x544C5953    //[#sec4.10 Synchronized lyric/text]
#define SYTC 0x43545953    //[#sec4.8 Synchronized tempo codes]
#define TALB 0x424C4154    //[#TALB Album/Movie/Show title]
#define TBPM 0x4D504254    //[#TBPM BPM (beats per minute)]
#define TCOM 0x4D4F4354    //[#TCOM Composer]
#define TCON 0x4E4F4354    //[#TCON Content type]
#define TCOP 0x504F4354    //[#TCOP Copyright message]
#define TDAT 0x54414454    //[#TDAT Date]
#define TDLY 0x594C4454    //[#TDLY Playlist delay]
#define TENC 0x434E4554    //[#TENC Encoded by]
#define TEXT 0x54584554    //[#TEXT Lyricist/Text writer]
#define TFLT 0x544C4654    //[#TFLT File type]
#define TIME 0x454D4954    //[#TIME Time]
#define TIT1 0x31544954    //[#TIT1 Content group description]
#define TIT2 0x32544954    //[#TIT2 Title/songname/content description]
#define TIT3 0x33544954    //[#TIT3 Subtitle/Description refinement]
#define TKEY 0x59454B54    //[#TKEY Initial key]
#define TLAN 0x4E414C54    //[#TLAN Language(s)]
#define TLEN 0x4E454C54    //[#TLEN Length]
#define TMED 0x44454D54    //[#TMED Media type]
#define TOAL 0x4C414F54    //[#TOAL Original album/movie/show title]
#define TOFN 0x4E464F54    //[#TOFN Original filename]
#define TOLY 0x594C4F54    //[#TOLY Original lyricist(s)/text writer(s)]
#define TOPE 0x45504F54    //[#TOPE Original artist(s)/performer(s)]
#define TORY 0x59524F54    //[#TORY Original release year]
#define TOWN 0x4E574F54    //[#TOWN File owner/licensee]
#define TPE1 0x31455054    //[#TPE1 Lead performer(s)/Soloist(s)]
#define TPE2 0x32455054    //[#TPE2 Band/orchestra/accompaniment]
#define TPE3 0x33455054    //[#TPE3 Conductor/performer refinement]
#define TPE4 0x34455054    //[#TPE4 Interpreted, remixed, or otherwise modified by]
#define TPOS 0x534F5054    //[#TPOS Part of a set]
#define TPUB 0x42555054    //[#TPUB Publisher]
#define TRCK 0x4B435254    //[#TRCK Track number/Position in set]
#define TRDA 0x41445254    //[#TRDA Recording dates]
#define TRSN 0x4E535254    //[#TRSN Internet radio station name]
#define TRSO 0x4F535254    //[#TRSO Internet radio station owner]
#define TSIZ 0x5A495354    //[#TSIZ Size]
#define TSRC 0x43525354    //[#TSRC ISRC (international standard recording code)]
#define TSSE 0x45535354    //[#TSEE Software/Hardware and settings used for encoding]
#define TYER 0x52455954    //[#TYER Year]
#define TXXX 0x58585854    //[#TXXX User defined text information frame]
#define UFID 0x44494655    //[#sec4.1 Unique file identifier]
#define USER 0x52455355    //[#sec4.23 Terms of use]
#define USLT 0x544C5355    //[#sec4.9 Unsychronized lyric/text transcription]
#define WCOM 0x4D4F4357    //[#WCOM Commercial information]
#define WCOP 0x504F4357    //[#WCOP Copyright/Legal information]
#define WOAF 0x46414F57    //[#WOAF Official audio file webpage]
#define WOAR 0x52414F57    //[#WOAR Official artist/performer webpage]
#define WOAS 0x53414F57    //[#WOAS Official audio source webpage]
#define WORS 0x53524F57    //[#WORS Official internet radio station homepage]
#define WPAY 0x59415057    //[#WPAY Payment]
#define WPUB 0x42555057    //[#WPUB Publishers official webpage]
#define WXXX 0x58585857    //[#WXXX User defined URL link frame]*/


typedef struct _ID3V2Header {
    char Header[3];  //must be "ID3"(0x49 0x44 0x33)
    BYTE MajorVersion;
    BYTE RevisionNumber;
    BYTE Flags;
    BYTE Size[4];
} ID3V2Header, *LPID3V2Header;

typedef struct _ID3V2ExtendedHeader {
    BYTE ExtendedHeaderSize[4];
    BYTE ExtendedFlags[2];
    BYTE SizeOfPadding[4];
} ID3V2ExtendedHeader, *LPID3V2ExtendedHeader;

typedef struct _ID3V2FrameHeader {
    char FrameID[4];
    BYTE Size[4];
    BYTE Flags[2];
} ID3V2FrameHeader, *LPID3V2FrameHeader;

class ID3V2 {
private:
    LPID3V2Header ID3V2_Header;
    LPID3V2ExtendedHeader ID3V2_ExtendedHeader;
    UINT32* TotalFrameCRC;
    BOOL Valid;
public:
    ID3V2();
    ID3V2(LPTSTR FilePath);

    ~ID3V2();

    BOOL IsValid();    //To check wheather ID3V2_Header is a valid ID3V2Header.

    BOOL HasUnsynchronisationFlag();
    BOOL HasExtendedHeaderFlag();
    BOOL HasExperimentalIndicatorFlag();

    BYTE GetID3V2MajorVersion();
    BYTE GetID3V2ReversionNumber();
    UINT32 GetID3V2FrameSize();
};

#endif // MP3_H_INCLUDED
