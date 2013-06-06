// QR_Encode.h : CQR_Encode �N���X�錾����уC���^�[�t�F�C�X��`
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

// To compile as Objective-C++
//
//    #import "QR_Encode.h" // in Objective-C++ code (.mm)
//
//    Compile and link
//      QR_Encode.mm
//         QR_Encode.cpp
//             QR_Encode.c
//
//
// To compile as Objective-C
//
//    #import "QR_Encode.h" // in Objective-C code (.m)
//
//    Compile and link
//      QR_Encode.m
//         QR_Encode.c
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
//        QR_Encode.c

#ifndef _QR_ENCODE_H
#define _QR_ENCODE_H

#include <string.h>
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////
// �萔

// ���������x��
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// �f�[�^���[�h
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// �o�[�W����(�^��)�O���[�v
#define QR_VRESION_S	0 // 1 �` 9
#define QR_VRESION_M	1 // 10 �` 26
#define QR_VRESION_L	2 // 27 �` 40

#define MAX_ALLCODEWORD	 3706 // ���R�[�h���[�h���ő�l
#define MAX_DATACODEWORD 2956 // �f�[�^�R�[�h���[�h�ő�l(�o�[�W����40-L)
#define MAX_CODEBLOCK	  153 // �u���b�N�f�[�^�R�[�h���[�h���ő�l(�q�r�R�[�h���[�h���܂�)
#define MAX_MODULESIZE	  177 // ��Ӄ��W���[�����ő�l

// �r�b�g�}�b�v�`�掞�}�[�W��
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// �q�r�u���b�N��
	int ncAllCodeWord;	// �u���b�N���R�[�h���[�h��
	int ncDataCodeWord;	// �f�[�^�R�[�h���[�h��(�R�[�h���[�h�� - �q�r�R�[�h���[�h��)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR�R�[�h�o�[�W����(�^��)�֘A���

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // �o�[�W����(�^��)�ԍ�(1�`40)
	int ncAllCodeWord; // ���R�[�h���[�h��

	// �ȉ��z��Y���͌�������(0 = L, 1 = M, 2 = Q, 3 = H) 
	int ncDataCodeWord[4];	// �f�[�^�R�[�h���[�h��(���R�[�h���[�h�� - �q�r�R�[�h���[�h��)

	int ncAlignPoint;	// �A���C�����g�p�^�[�����W��
	int nAlignPoint[6];	// �A���C�����g�p�^�[�����S���W

	RS_BLOCKINFO RS_BlockInfo1[4]; // �q�r�u���b�N���(1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // �q�r�u���b�N���(2)

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
	int m_nVersion;		// �o�[�W����(�^��)
	BOOL m_bAutoExtent;	// �o�[�W����(�^��)�����g���w��t���O
	int m_nMaskingNo;	// �}�X�L���O�p�^�[���ԍ�

	int m_nSymbleSize;
	ModuleData m_byModuleData; // [x][y]
	// bit5:�@�\���W���[���i�}�X�L���O�ΏۊO�j�t���O
	// bit4:�@�\���W���[���`��f�[�^
	// bit1:�G���R�[�h�f�[�^
	// bit0:�}�X�N��G���R�[�h�`��f�[�^
	// 20h�Ƃ̘_���a�ɂ��@�\���W���[������A11h�Ƃ̘_���a�ɂ��`��i�ŏI�I�ɂ�bool�l���j

    void *privateData;

// �f�[�^�G���R�[�h�֘A�t�@���N�V����
//public:
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

class CQR_Encode : CQR_Encode_Proxy {
public:
     CQR_Encode();
    ~CQR_Encode();

    BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);
};
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
