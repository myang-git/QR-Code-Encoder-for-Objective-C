// QR_Encode.h : CQR_Encode クラス宣言およびインターフェイス定義
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
// 定数

// 誤り訂正レベル
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// データモード
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// バージョン(型番)グループ
#define QR_VRESION_S	0 // 1 ～ 9
#define QR_VRESION_M	1 // 10 ～ 26
#define QR_VRESION_L	2 // 27 ～ 40

#define MAX_ALLCODEWORD	 3706 // 総コードワード数最大値
#define MAX_DATACODEWORD 2956 // データコードワード最大値(バージョン40-L)
#define MAX_CODEBLOCK	  153 // ブロックデータコードワード数最大値(ＲＳコードワードを含む)
#define MAX_MODULESIZE	  177 // 一辺モジュール数最大値

// ビットマップ描画時マージン
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// ＲＳブロック数
	int ncAllCodeWord;	// ブロック内コードワード数
	int ncDataCodeWord;	// データコードワード数(コードワード数 - ＲＳコードワード数)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QRコードバージョン(型番)関連情報

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // バージョン(型番)番号(1～40)
	int ncAllCodeWord; // 総コードワード数

	// 以下配列添字は誤り訂正率(0 = L, 1 = M, 2 = Q, 3 = H) 
	int ncDataCodeWord[4];	// データコードワード数(総コードワード数 - ＲＳコードワード数)

	int ncAlignPoint;	// アライメントパターン座標数
	int nAlignPoint[6];	// アライメントパターン中心座標

	RS_BLOCKINFO RS_BlockInfo1[4]; // ＲＳブロック情報(1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // ＲＳブロック情報(2)

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
	int m_nLevel;		// 誤り訂正レベル
	int m_nVersion;		// バージョン(型番)
	BOOL m_bAutoExtent;	// バージョン(型番)自動拡張指定フラグ
	int m_nMaskingNo;	// マスキングパターン番号

	int m_nSymbleSize;
	ModuleData m_byModuleData; // [x][y]
	// bit5:機能モジュール（マスキング対象外）フラグ
	// bit4:機能モジュール描画データ
	// bit1:エンコードデータ
	// bit0:マスク後エンコード描画データ
	// 20hとの論理和により機能モジュール判定、11hとの論理和により描画（最終的にはbool値化）

    void *privateData;

// データエンコード関連ファンクション
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
