// QR_Encode.h : CQR_Encode �N���X�錾����уC���^�[�t�F�C�X��`
//                          Interface definitions and class declaration
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

// To compile as Objective-C++
//
//    #import "QR_Encode.h" // in Objective-C++ code (.mm)
//
//    Compile and link
//      QR_Encode.mm
//      QR_Encode.cpp
//      QR_Encode.c
//
//
// To compile as Objective-C
//
//    #import "QR_Encode.h" // in Objective-C code (.m)
//
//    Compile and link
//      QR_Encode.m
//      QR_Encode.c
//
//
// To compile as C
//
//    #include "QR_Encode.h" // in C code (.c)
//
//    Compile and link
//      QR_Encode.c
//
//
// To compile as C++
//
//    #include "QR_Encode.h" // in C++ code (.cxx or .cpp)
//
//    Compile and link
//      QR_Encode.cpp
//      QR_Encode.c

#ifndef _QR_ENCODE_H
#define _QR_ENCODE_H

#include <string.h>
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////
// �萔
// Constants

// ���������x��
// Error correction level
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// �f�[�^���[�h
// Data mode
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// �o�[�W����(�^��)�O���[�v
// Version (model number) group
#define QR_VERSION_S	0 // 1 �` 9
#define QR_VERSION_M	1 // 10 �` 26
#define QR_VERSION_L	2 // 27 �` 40

#define MAX_ALLCODEWORD	 3706 // ���R�[�h���[�h���ő�l
                              // Total number of codewords maximum value
#define MAX_DATACODEWORD 2956 // �f�[�^�R�[�h���[�h�ő�l(�o�[�W����40-L)
                              // Data code word maximum (version 40-L)
#define MAX_CODEBLOCK	  153 // �u���b�N�f�[�^�R�[�h���[�h���ő�l(�q�r�R�[�h���[�h���܂�)
                              // (Including the RS code word) data block number of codewords maximum value
#define MAX_MODULESIZE	  177 // ��Ӄ��W���[�����ő�l
                              // One side number of modules maximum value

// �r�b�g�}�b�v�`�掞�}�[�W��
// Margin bitmap when drawing
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// �q�r�u���b�N��
                        // RS number of blocks
	int ncAllCodeWord;	// �u���b�N���R�[�h���[�h��
                        // Block number of codewords
	int ncDataCodeWord;	// �f�[�^�R�[�h���[�h��(�R�[�h���[�h�� - �q�r�R�[�h���[�h��)
                        // Data code words (code number of words - RS code words)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR�R�[�h�o�[�W����(�^��)�֘A���
// QR code version (model number) related information

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // �o�[�W����(�^��)�ԍ�(1�`40)
                       // Version (model number) number (1-40)
	int ncAllCodeWord; // ���R�[�h���[�h��
                       // Total number of codewords

	// �ȉ��z��Y���͌�������(0 = L, 1 = M, 2 = Q, 3 = H) 
    // The array index (0 = L, 1 = M, 2 = Q, 3 = H) error correction rate below
	int ncDataCodeWord[4];	// �f�[�^�R�[�h���[�h��(���R�[�h���[�h�� - �q�r�R�[�h���[�h��)
                            // Data code words (the total number of codewords - RS code words)

	int ncAlignPoint;	// �A���C�����g�p�^�[�����W��
                        // Alignment pattern coordinate number
	int nAlignPoint[6];	// �A���C�����g�p�^�[�����S���W
                        // Alignment pattern center coordinates

	RS_BLOCKINFO RS_BlockInfo1[4]; // �q�r�u���b�N���(1)
                                   // RS block information (1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // �q�r�u���b�N���(2)
                                   // RS block information (2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;

#ifndef YES
typedef signed char    BOOL;
#define YES            (BOOL)1
#define NO             (BOOL)0
#endif
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef BYTE*          LPBYTE;
typedef char const*    LPCSTR;

typedef BYTE           ModuleData[MAX_MODULESIZE][MAX_MODULESIZE];

typedef struct tagCQR_Encode_Struct
{
	int m_nLevel;		// ���������x��
                        // Error correction level
	int m_nVersion;		// �o�[�W����(�^��)
                        // Error correction level
	BOOL m_bAutoExtent;	// �o�[�W����(�^��)�����g���w��t���O
                        // Version (model number) automatic extension specified flag
	int m_nMaskingNo;	// �}�X�L���O�p�^�[���ԍ�
                        // Masking pattern number

	int m_nSymbleSize;
	ModuleData m_byModuleData; // [x][y]
	// bit5:�@�\���W���[���i�}�X�L���O�ΏۊO�j�t���O
	// bit4:�@�\���W���[���`��f�[�^
	// bit1:�G���R�[�h�f�[�^
	// bit0:�}�X�N��G���R�[�h�`��f�[�^
    // Bit5: functional module (masking excluded) flag
    // Bit4: functional module drawing data
    // Bit1: encode data
    // Bit0: mask after drawing encoded data
	// 20h�Ƃ̘_���a�ɂ��@�\���W���[������A11h�Ƃ̘_���a�ɂ��`��i�ŏI�I�ɂ�bool�l���j
    // The function module determined by the logic sum of the 20h, (bool binarization eventually) drawn by the logic sum of the 11h

    void *privateData;

} CQR_Encode_Struct;

#ifdef __cplusplus
extern "C" {
#endif

CQR_Encode_Struct *CQR_Encode_Init       (void);
void               CQR_Encode_Free       (CQR_Encode_Struct *data);
BOOL               CQR_Encode_EncodeDataSource (CQR_Encode_Struct *data, int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource);

#define CQR_Encode_EncodeData(data, nLevel, nVersion, bAutoExtent, nMaskingNo, lpsSource) (CQR_Encode_EncodeDataSource(data, nLevel, nVersion, bAutoExtent, nMaskingNo, lpsSource, 0))

#ifdef __cplusplus
}

class CQR_Encode_Proxy {
public:
    int  &m_nLevel;
    int  &m_nVersion;
    BOOL &m_bAutoExtent;
    int  &m_nMaskingNo;
    int  &m_nSymbleSize;
    ModuleData &m_byModuleData;

protected:
    CQR_Encode_Proxy(CQR_Encode_Struct *);
    ~CQR_Encode_Proxy();

    CQR_Encode_Struct *data;
};

class CQR_Encode : public CQR_Encode_Proxy {
public:
     CQR_Encode();
    ~CQR_Encode();

    BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);
};
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
