// QR_Encode.cpp : CQR_Encode ƒNƒ‰ƒX ƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“ ƒtƒ@ƒCƒ‹
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

#include "QR_Encode.h"

// forward declarations
static  int CQR_Encode_GetEncodeVersion     (CQR_Encode_Struct *data, int nVersion, LPCSTR lpsSource, int ncLength);
static BYTE CQR_Encode_AlphabetToBinaly     (unsigned char c);
static WORD CQR_Encode_LengthKanjiToBinaly  (WORD wc);
static BOOL CQR_Encode_EncodeSourceData     (CQR_Encode_Struct *data, LPCSTR lpsSource, int ncLength, int nVerGroup);
static  int CQR_Encode_CountPenalty         (CQR_Encode_Struct *data);
static  int CQR_Encode_GetBitLength         (BYTE nMode, int ncData, int nVerGroup);
static  int CQR_Encode_SetBitStream         (CQR_Encode_Struct *data, int nIndex, WORD wData, int ncData);
static void CQR_Encode_FormatModule         (CQR_Encode_Struct *data);
static void CQR_Encode_GetRSCodeWord        (CQR_Encode_Struct *data, LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);
static BOOL CQR_Encode_IsKanjiData          (unsigned char c1, unsigned char c2);
static void CQR_Encode_SetAlignmentPattern  (CQR_Encode_Struct *data, int x, int y);
static void CQR_Encode_SetCodeWordPattern   (CQR_Encode_Struct *data);
static void CQR_Encode_SetFinderPattern     (CQR_Encode_Struct *data, int x, int y);
static void CQR_Encode_SetFormatInfoPattern (CQR_Encode_Struct *data, int nPatternNo);
static void CQR_Encode_SetFunctionModule    (CQR_Encode_Struct *data);
static void CQR_Encode_SetMaskingPattern    (CQR_Encode_Struct *data, int nPatternNo);
static void CQR_Encode_SetVersionPattern    (CQR_Encode_Struct *data);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// QRƒR[ƒhƒo[ƒWƒ‡ƒ“(Œ^”Ô)î•ñ
static QR_VERSIONINFO QR_VersonInfo[] = {{0}, // (ƒ_ƒ~[:Ver.0)
										 { 1, // Ver.1
										    26,   19,   16,   13,    9,
										   0,   0,   0,   0,   0,   0,   0,
										   1,  26,  19,
										   1,  26,  16,
										   1,  26,  13,
										   1,  26,   9,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 2, // Ver.2
										    44,   34,   28,   22,   16,
										   1,  18,   0,   0,   0,   0,   0,
										   1,  44,  34,
										   1,  44,  28,
										   1,  44,  22,
										   1,  44,  16,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 3, // Ver.3
										    70,   55,   44,   34,   26,
										   1,  22,   0,   0,   0,   0,   0,
										   1,  70,  55,
										   1,  70,  44,
										   2,  35,  17,
										   2,  35,  13,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 4, // Ver.4
										   100,   80,   64,   48,   36,
										   1,  26,   0,   0,   0,   0,   0,
										   1, 100,  80,
										   2,  50,  32,
										   2,  50,  24,
										   4,  25,   9,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 5, // Ver.5
										   134,  108,   86,   62,   46,
										   1,  30,   0,   0,   0,   0,   0,
										   1, 134, 108,
										   2,  67,  43,
										   2,  33,  15,
										   2,  33,  11,
										   0,   0,   0,
										   0,   0,   0,
										   2,  34,  16,
										   2,  34,  12},
										 { 6, // Ver.6
										   172,  136,  108,   76,   60,
										   1,  34,   0,   0,   0,   0,   0,
										   2,  86,  68,
										   4,  43,  27,
										   4,  43,  19,
										   4,  43,  15,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 7, // Ver.7
										   196,  156,  124,   88,   66,
										   2,  22,  38,   0,   0,   0,   0,
										   2,  98,  78,
										   4,  49,  31,
										   2,  32,  14,
										   4,  39,  13,
										   0,   0,   0,
										   0,   0,   0,
										   4,  33,  15,
										   1,  40,  14},
										 { 8, // Ver.8
										   242,  194,  154,  110,   86,
										   2,  24,  42,   0,   0,   0,   0,
										   2, 121,  97,
										   2,  60,  38,
										   4,  40,  18,
										   4,  40,  14,
										   0,   0,   0,
										   2,  61,  39,
										   2,  41,  19,
										   2,  41,  15},
										 { 9, // Ver.9
										   292,  232,  182,  132,  100,
										   2,  26,  46,   0,   0,   0,   0,
										   2, 146, 116,
										   3,  58,  36,
										   4,  36,  16,
										   4,  36,  12,
										   0,   0,   0,
										   2,  59,  37,
										   4,  37,  17,
										   4,  37,  13},
										 {10, // Ver.10
										   346,  274,  216,  154,  122,
										   2,  28,  50,   0,   0,   0,   0,
										   2,  86,  68,
										   4,  69,  43,
										   6,  43,  19,
										   6,  43,  15,
										   2,  87,  69,
										   1,  70,  44,
										   2,  44,  20,
										   2,  44,  16},
										 {11, // Ver.11
										   404,  324,  254,  180,  140,
										   2,  30,  54,   0,   0,   0,   0,
										   4, 101,  81,
										   1,  80,  50,
										   4,  50,  22,
										   3,  36,  12,
										   0,   0,   0,
										   4,  81,  51,
										   4,  51,  23,
										   8,  37,  13},
										 {12, // Ver.12
										   466,  370,  290,  206,  158,
										   2,  32,  58,   0,   0,   0,   0,
										   2, 116,  92,
										   6,  58,  36,
										   4,  46,  20,
										   7,  42,  14,
										   2, 117,  93,
										   2,  59,  37,
										   6,  47,  21,
										   4,  43,  15},
										 {13, // Ver.13
										   532,  428,  334,  244,  180,
										   2,  34,  62,   0,   0,   0,   0,
										   4, 133, 107,
										   8,  59,  37,
										   8,  44,  20,
										  12,  33,  11,
										   0,   0,   0,
										   1,  60,  38,
										   4,  45,  21,
										   4,  34,  12},
										 {14, // Ver.14
										   581,  461,  365,  261,  197,
										   3,  26,  46,  66,   0,   0,   0,
										   3, 145, 115,
										   4,  64,  40,
										  11,  36,  16,
										  11,  36,  12,
										   1, 146, 116,
										   5,  65,  41,
										   5,  37,  17,
										   5,  37,  13},
										 {15, // Ver.15
										   655,  523,  415,  295,  223,
										   3,  26,  48,  70,   0,   0,   0,
										   5, 109,  87,
										   5,  65,  41,
										   5,  54,  24,
										  11,  36,  12,
										   1, 110,  88,
										   5,  66,  42,
										   7,  55,  25,
										   7,  37,  13},
										 {16, // Ver.16
										   733,  589,  453,  325,  253,
										   3,  26,  50,  74,   0,   0,   0,
										   5, 122,  98,
										   7,  73,  45,
										  15,  43,  19,
										   3,  45,  15,
										   1, 123,  99,
										   3,  74,  46,
										   2,  44,  20,
										  13,  46,  16},
										 {17, // Ver.17
										   815,  647,  507,  367,  283,
										   3,  30,  54,  78,   0,   0,   0,
										   1, 135, 107,
										  10,  74,  46,
										   1,  50,  22,
										   2,  42,  14,
										   5, 136, 108,
										   1,  75,  47,
										  15,  51,  23,
										  17,  43,  15},
										 {18, // Ver.18
										   901,  721,  563,  397,  313,
										   3,  30,  56,  82,   0,   0,   0,
										   5, 150, 120,
										   9,  69,  43,
										  17,  50,  22,
										   2,  42,  14,
										   1, 151, 121,
										   4,  70,  44,
										   1,  51,  23,
										  19,  43,  15},
										 {19, // Ver.19
										   991,  795,  627,  445,  341,
										   3,  30,  58,  86,   0,   0,   0,
										   3, 141, 113,
										   3,  70,  44,
										  17,  47,  21,
										   9,  39,  13,
										   4, 142, 114,
										  11,  71,  45,
										   4,  48,  22,
										  16,  40,  14},
										 {20, // Ver.20
										  1085,  861,  669,  485,  385,
										   3,  34,  62,  90,   0,   0,   0,
										   3, 135, 107,
										   3,  67,  41,
										  15,  54,  24,
										  15,  43,  15,
										   5, 136, 108,
										  13,  68,  42,
										   5,  55,  25,
										  10,  44,  16},
										 {21, // Ver.21
										  1156,  932,  714,  512,  406,
										   4,  28,  50,  72,  94,   0,   0,
										   4, 144, 116,
										  17,  68,  42,
										  17,  50,  22,
										  19,  46,  16,
										   4, 145, 117,
										   0,   0,   0,
										   6,  51,  23,
										   6,  47,  17},
										 {22, // Ver.22
										  1258, 1006,  782,  568,  442,
										   4,  26,  50,  74,  98,   0,   0,
										   2, 139, 111,
										  17,  74,  46,
										   7,  54,  24,
										  34,  37,  13,
										   7, 140, 112,
										   0,   0,   0,
										  16,  55,  25,
										   0,   0,   0},
										 {23, // Ver.23
										  1364, 1094,  860,  614,  464,
										   4,  30,  54,  78, 102,   0,   0,
										   4, 151, 121,
										   4,  75,  47,
										  11,  54,  24,
										  16,  45,  15,
										   5, 152, 122,
										  14,  76,  48,
										  14,  55,  25,
										  14,  46,  16},
										 {24, // Ver.24
										  1474, 1174,  914,  664,  514,
										   4,  28,  54,  80, 106,   0,   0,
										   6, 147, 117,
										   6,  73,  45,
										  11,  54,  24,
										  30,  46,  16,
										   4, 148, 118,
										  14,  74,  46,
										  16,  55,  25,
										   2,  47,  17},
										 {25, // Ver.25
										  1588, 1276, 1000,  718,  538,
										   4,  32,  58,  84, 110,   0,   0,
										   8, 132, 106,
										   8,  75,  47,
										   7,  54,  24,
										  22,  45,  15,
										   4, 133, 107,
										  13,  76,  48,
										  22,  55,  25,
										  13,  46,  16},
										 {26, // Ver.26
										  1706, 1370, 1062,  754,  596,
										   4,  30,  58,  86, 114,   0,   0,
										  10, 142, 114,
										  19,  74,  46,
										  28,  50,  22,
										  33,  46,  16,
										   2, 143, 115,
										   4,  75,  47,
										   6,  51,  23,
										   4,  47,  17},
										 {27, // Ver.27
										  1828, 1468, 1128,  808,  628,
										   4,  34,  62,  90, 118,   0,   0,
										   8, 152, 122,
										  22,  73,  45,
										   8,  53,  23,
										  12,  45,  15,
										   4, 153, 123,
										   3,  74,  46,
										  26,  54,  24,
										  28,  46,  16},
										 {28, // Ver.28
										  1921, 1531, 1193,  871,  661,
										   5,  26,  50,  74,  98, 122,   0,
										   3, 147, 117,
										   3,  73,  45,
										   4,  54,  24,
										  11,  45,  15,
										  10, 148, 118,
										  23,  74,  46,
										  31,  55,  25,
										  31,  46,  16},
										 {29, // Ver.29
										  2051, 1631, 1267,  911,  701,
										   5,  30,  54,  78, 102, 126,   0,
										   7, 146, 116,
										  21,  73,  45,
										   1,  53,  23,
										  19,  45,  15,
										   7, 147, 117,
										   7,  74,  46,
										  37,  54,  24,
										  26,  46,  16},
										 {30, // Ver.30
										  2185, 1735, 1373,  985,  745,
										   5,  26,  52,  78, 104, 130,   0,
										   5, 145, 115,
										  19,  75,  47,
										  15,  54,  24,
										  23,  45,  15,
										  10, 146, 116,
										  10,  76,  48,
										  25,  55,  25,
										  25,  46,  16},
										 {31, // Ver.31
										  2323, 1843, 1455, 1033,  793,
										   5,  30,  56,  82, 108, 134,   0,
										  13, 145, 115,
										   2,  74,  46,
										  42,  54,  24,
										  23,  45,  15,
										   3, 146, 116,
										  29,  75,  47,
										   1,  55,  25,
										  28,  46,  16},
										 {32, // Ver.32
										  2465, 1955, 1541, 1115,  845,
										   5,  34,  60,  86, 112, 138,   0,
										  17, 145, 115,
										  10,  74,  46,
										  10,  54,  24,
										  19,  45,  15,
										   0,   0,   0,
										  23,  75,  47,
										  35,  55,  25,
										  35,  46,  16},
										 {33, // Ver.33
										  2611, 2071, 1631, 1171,  901,
										   5,  30,  58,  86, 114, 142,   0,
										  17, 145, 115,
										  14,  74,  46,
										  29,  54,  24,
										  11,  45,  15,
										   1, 146, 116,
										  21,  75,  47,
										  19,  55,  25,
										  46,  46,  16},
										 {34, // Ver.34
										  2761, 2191, 1725, 1231,  961,
										   5,  34,  62,  90, 118, 146,   0,
										  13, 145, 115,
										  14,  74,  46,
										  44,  54,  24,
										  59,  46,  16,
										   6, 146, 116,
										  23,  75,  47,
										   7,  55,  25,
										   1,  47,  17},
										 {35, // Ver.35
										  2876, 2306, 1812, 1286,  986,
										   6,  30,  54,  78, 102, 126, 150,
										  12, 151, 121,
										  12,  75,  47,
										  39,  54,  24,
										  22,  45,  15,
										   7, 152, 122,
										  26,  76,  48,
										  14,  55,  25,
										  41,  46,  16},
										 {36, // Ver.36
										  3034, 2434, 1914, 1354, 1054,
										   6,  24,  50,  76, 102, 128, 154,
										   6, 151, 121,
										   6,  75,  47,
										  46,  54,  24,
										   2,  45,  15,
										  14, 152, 122,
										  34,  76,  48,
										  10,  55,  25,
										  64,  46,  16},
										 {37, // Ver.37
										  3196, 2566, 1992, 1426, 1096,
										   6,  28,  54,  80, 106, 132, 158,
										  17, 152, 122,
										  29,  74,  46,
										  49,  54,  24,
										  24,  45,  15,
										   4, 153, 123,
										  14,  75,  47,
										  10,  55,  25,
										  46,  46,  16},
										 {38, // Ver.38
										  3362, 2702, 2102, 1502, 1142,
										   6,  32,  58,  84, 110, 136, 162,
										   4, 152, 122,
										  13,  74,  46,
										  48,  54,  24,
										  42,  45,  15,
										  18, 153, 123,
										  32,  75,  47,
										  14,  55,  25,
										  32,  46,  16},
										 {39, // Ver.39
										  3532, 2812, 2216, 1582, 1222,
										   6,  26,  54,  82, 110, 138, 166,
										  20, 147, 117,
										  40,  75,  47,
										  43,  54,  24,
										  10,  45,  15,
										   4, 148, 118,
										   7,  76,  48,
										  22,  55,  25,
										  67,  46,  16},
										 {40, // Ver.40
										  3706, 2956, 2334, 1666, 1276,
										   6,  30,  58,  86, 114, 142, 170,
										  19, 148, 118,
										  18,  75,  47,
										  34,  54,  24,
										  20,  45,  15,
										   6, 149, 119,
										  31,  76,  48,
										  34,  55,  25,
										  61,  46,  16}
										};


/////////////////////////////////////////////////////////////////////////////
// GF(2^8)ƒ¿w”¨®”•ÏŠ·ƒe[ƒuƒ‹
static BYTE byExpToInt[] = {  1,   2,   4,   8,  16,  32,  64, 128,  29,  58, 116, 232, 205, 135,  19,  38,
							 76, 152,  45,  90, 180, 117, 234, 201, 143,   3,   6,  12,  24,  48,  96, 192,
							157,  39,  78, 156,  37,  74, 148,  53, 106, 212, 181, 119, 238, 193, 159,  35,
							 70, 140,   5,  10,  20,  40,  80, 160,  93, 186, 105, 210, 185, 111, 222, 161,
							 95, 190,  97, 194, 153,  47,  94, 188, 101, 202, 137,  15,  30,  60, 120, 240,
							253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163,  91, 182, 113, 226,
							217, 175,  67, 134,  17,  34,  68, 136,  13,  26,  52, 104, 208, 189, 103, 206,
							129,  31,  62, 124, 248, 237, 199, 147,  59, 118, 236, 197, 151,  51, 102, 204,
							133,  23,  46,  92, 184, 109, 218, 169,  79, 158,  33,  66, 132,  21,  42,  84,
							168,  77, 154,  41,  82, 164,  85, 170,  73, 146,  57, 114, 228, 213, 183, 115,
							230, 209, 191,  99, 198, 145,  63, 126, 252, 229, 215, 179, 123, 246, 241, 255,
							227, 219, 171,  75, 150,  49,  98, 196, 149,  55, 110, 220, 165,  87, 174,  65,
							130,  25,  50, 100, 200, 141,   7,  14,  28,  56, 112, 224, 221, 167,  83, 166,
							 81, 162,  89, 178, 121, 242, 249, 239, 195, 155,  43,  86, 172,  69, 138,   9,
							 18,  36,  72, 144,  61, 122, 244, 245, 247, 243, 251, 235, 203, 139,  11,  22,
							 44,  88, 176, 125, 250, 233, 207, 131,  27,  54, 108, 216, 173,  71, 142,   1};


/////////////////////////////////////////////////////////////////////////////
// GF(2^8)ƒ¿®”¨w”•ÏŠ·ƒe[ƒuƒ‹
static BYTE byIntToExp[] = {  0,   0,   1,  25,   2,  50,  26, 198,   3, 223,  51, 238,  27, 104, 199,  75,
							  4, 100, 224,  14,  52, 141, 239, 129,  28, 193, 105, 248, 200,   8,  76, 113,
							  5, 138, 101,  47, 225,  36,  15,  33,  53, 147, 142, 218, 240,  18, 130,  69,
							 29, 181, 194, 125, 106,  39, 249, 185, 201, 154,   9, 120,  77, 228, 114, 166,
							  6, 191, 139,  98, 102, 221,  48, 253, 226, 152,  37, 179,  16, 145,  34, 136,
							 54, 208, 148, 206, 143, 150, 219, 189, 241, 210,  19,  92, 131,  56,  70,  64,
							 30,  66, 182, 163, 195,  72, 126, 110, 107,  58,  40,  84, 250, 133, 186,  61,
							202,  94, 155, 159,  10,  21, 121,  43,  78, 212, 229, 172, 115, 243, 167,  87,
							  7, 112, 192, 247, 140, 128,  99,  13, 103,  74, 222, 237,  49, 197, 254,  24,
							227, 165, 153, 119,  38, 184, 180, 124,  17,  68, 146, 217,  35,  32, 137,  46,
							 55,  63, 209,  91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190,  97,
							242,  86, 211, 171,  20,  42,  93, 158, 132,  60,  57,  83,  71, 109,  65, 162,
							 31,  45,  67, 216, 183, 123, 164, 118, 196,  23,  73, 236, 127,  12, 111, 246,
							108, 161,  59,  82,  41, 157,  85, 170, 251,  96, 134, 177, 187, 204,  62,  90,
							203,  89,  95, 176, 156, 169, 160,  81,  11, 245,  22, 235, 122, 117,  44, 215,
							 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168,  80,  88, 175};


/////////////////////////////////////////////////////////////////////////////
// Œë‚è’ù³¶¬‘½€®ƒ¿ŒW”
static BYTE byRSExp7[]  = {87, 229, 146, 149, 238, 102,  21};
static BYTE byRSExp10[] = {251,  67,  46,  61, 118,  70,  64,  94,  32,  45};
static BYTE byRSExp13[] = { 74, 152, 176, 100,  86, 100, 106, 104, 130, 218, 206, 140,  78};
static BYTE byRSExp15[] = {  8, 183,  61,  91, 202,  37,  51,  58,  58, 237, 140, 124,   5,  99, 105};
static BYTE byRSExp16[] = {120, 104, 107, 109, 102, 161,  76,   3,  91, 191, 147, 169, 182, 194, 225, 120};
static BYTE byRSExp17[] = { 43, 139, 206,  78,  43, 239, 123, 206, 214, 147,  24,  99, 150,  39, 243, 163, 136};
static BYTE byRSExp18[] = {215, 234, 158,  94, 184,  97, 118, 170,  79, 187, 152, 148, 252, 179,   5,  98,  96, 153};
static BYTE byRSExp20[] = { 17,  60,  79,  50,  61, 163,  26, 187, 202, 180, 221, 225,  83, 239, 156, 164, 212, 212, 188, 190};
static BYTE byRSExp22[] = {210, 171, 247, 242,  93, 230,  14, 109, 221,  53, 200,  74,   8, 172,  98,  80, 219, 134, 160, 105,
						   165, 231};
static BYTE byRSExp24[] = {229, 121, 135,  48, 211, 117, 251, 126, 159, 180, 169, 152, 192, 226, 228, 218, 111,   0, 117, 232,
						    87,  96, 227,  21};
static BYTE byRSExp26[] = {173, 125, 158,   2, 103, 182, 118,  17, 145, 201, 111,  28, 165,  53, 161,  21, 245, 142,  13, 102,
						    48, 227, 153, 145, 218,  70};
static BYTE byRSExp28[] = {168, 223, 200, 104, 224, 234, 108, 180, 110, 190, 195, 147, 205,  27, 232, 201,  21,  43, 245,  87,
						    42, 195, 212, 119, 242,  37,   9, 123};
static BYTE byRSExp30[] = { 41, 173, 145, 152, 216,  31, 179, 182,  50,  48, 110,  86, 239,  96, 222, 125,  42, 173, 226, 193,
						   224, 130, 156,  37, 251, 216, 238,  40, 192, 180};
static BYTE byRSExp32[] = { 10,   6, 106, 190, 249, 167,   4,  67, 209, 138, 138,  32, 242, 123,  89,  27, 120, 185,  80, 156,
						    38,  69, 171,  60,  28, 222,  80,  52, 254, 185, 220, 241};
static BYTE byRSExp34[] = {111,  77, 146,  94,  26,  21, 108,  19, 105,  94, 113, 193,  86, 140, 163, 125,  58, 158, 229, 239,
						   218, 103,  56,  70, 114,  61, 183, 129, 167,  13,  98,  62, 129,  51};
static BYTE byRSExp36[] = {200, 183,  98,  16, 172,  31, 246, 234,  60, 152, 115,   0, 167, 152, 113, 248, 238, 107,  18,  63,
						   218,  37,  87, 210, 105, 177, 120,  74, 121, 196, 117, 251, 113, 233,  30, 120};
static BYTE byRSExp38[] = {159,  34,  38, 228, 230,  59, 243,  95,  49, 218, 176, 164,  20,  65,  45, 111,  39,  81,  49, 118,
						   113, 222, 193, 250, 242, 168, 217,  41, 164, 247, 177,  30, 238,  18, 120, 153,  60, 193};
static BYTE byRSExp40[] = { 59, 116,  79, 161, 252,  98, 128, 205, 128, 161, 247,  57, 163,  56, 235, 106,  53,  26, 187, 174,
						   226, 104, 170,   7, 175,  35, 181, 114,  88,  41,  47, 163, 125, 134,  72,  20, 232,  53,  35,  15};
static BYTE byRSExp42[] = {250, 103, 221, 230,  25,  18, 137, 231,   0,   3,  58, 242, 221, 191, 110,  84, 230,   8, 188, 106,
						    96, 147,  15, 131, 139,  34, 101, 223,  39, 101, 213, 199, 237, 254, 201, 123, 171, 162, 194, 117,
						    50,  96};
static BYTE byRSExp44[] = {190,   7,  61, 121,  71, 246,  69,  55, 168, 188,  89, 243, 191,  25,  72, 123,   9, 145,  14, 247,
						     1, 238,  44,  78, 143,  62, 224, 126, 118, 114,  68, 163,  52, 194, 217, 147, 204, 169,  37, 130,
						   113, 102,  73, 181};
static BYTE byRSExp46[] = {112,  94,  88, 112, 253, 224, 202, 115, 187,  99,  89,   5,  54, 113, 129,  44,  58,  16, 135, 216,
						   169, 211,  36,   1,   4,  96,  60, 241,  73, 104, 234,   8, 249, 245, 119, 174,  52,  25, 157, 224,
						    43, 202, 223,  19,  82,  15};
static BYTE byRSExp48[] = {228,  25, 196, 130, 211, 146,  60,  24, 251,  90,  39, 102, 240,  61, 178,  63,  46, 123, 115,  18,
						   221, 111, 135, 160, 182, 205, 107, 206,  95, 150, 120, 184,  91,  21, 247, 156, 140, 238, 191,  11,
						    94, 227,  84,  50, 163,  39,  34, 108};
static BYTE byRSExp50[] = {232, 125, 157, 161, 164,   9, 118,  46, 209,  99, 203, 193,  35,   3, 209, 111, 195, 242, 203, 225,
						    46,  13,  32, 160, 126, 209, 130, 160, 242, 215, 242,  75,  77,  42, 189,  32, 113,  65, 124,  69,
						   228, 114, 235, 175, 124, 170, 215, 232, 133, 205};
static BYTE byRSExp52[] = {116,  50,  86, 186,  50, 220, 251,  89, 192,  46,  86, 127, 124,  19, 184, 233, 151, 215,  22,  14,
						    59, 145,  37, 242, 203, 134, 254,  89, 190,  94,  59,  65, 124, 113, 100, 233, 235, 121,  22,  76,
						    86,  97,  39, 242, 200, 220, 101,  33, 239, 254, 116,  51};
static BYTE byRSExp54[] = {183,  26, 201,  87, 210, 221, 113,  21,  46,  65,  45,  50, 238, 184, 249, 225, 102,  58, 209, 218,
						   109, 165,  26,  95, 184, 192,  52, 245,  35, 254, 238, 175, 172,  79, 123,  25, 122,  43, 120, 108,
						   215,  80, 128, 201, 235,   8, 153,  59, 101,  31, 198,  76,  31, 156};
static BYTE byRSExp56[] = {106, 120, 107, 157, 164, 216, 112, 116,   2,  91, 248, 163,  36, 201, 202, 229,   6, 144, 254, 155,
						   135, 208, 170, 209,  12, 139, 127, 142, 182, 249, 177, 174, 190,  28,  10,  85, 239, 184, 101, 124,
						   152, 206,  96,  23, 163,  61,  27, 196, 247, 151, 154, 202, 207,  20,  61,  10};
static BYTE byRSExp58[] = { 82, 116,  26, 247,  66,  27,  62, 107, 252, 182, 200, 185, 235,  55, 251, 242, 210, 144, 154, 237,
						   176, 141, 192, 248, 152, 249, 206,  85, 253, 142,  65, 165, 125,  23,  24,  30, 122, 240, 214,   6,
						   129, 218,  29, 145, 127, 134, 206, 245, 117,  29,  41,  63, 159, 142, 233, 125, 148, 123};
static BYTE byRSExp60[] = {107, 140,  26,  12,   9, 141, 243, 197, 226, 197, 219,  45, 211, 101, 219, 120,  28, 181, 127,   6,
						   100, 247,   2, 205, 198,  57, 115, 219, 101, 109, 160,  82,  37,  38, 238,  49, 160, 209, 121,  86,
						    11, 124,  30, 181,  84,  25, 194,  87,  65, 102, 190, 220,  70,  27, 209,  16,  89,   7,  33, 240};
static BYTE byRSExp62[] = { 65, 202, 113,  98,  71, 223, 248, 118, 214,  94,   0, 122,  37,  23,   2, 228,  58, 121,   7, 105,
						   135,  78, 243, 118,  70,  76, 223,  89,  72,  50,  70, 111, 194,  17, 212, 126, 181,  35, 221, 117,
						   235,  11, 229, 149, 147, 123, 213,  40, 115,   6, 200, 100,  26, 246, 182, 218, 127, 215,  36, 186,
						   110, 106};
static BYTE byRSExp64[] = { 45,  51, 175,   9,   7, 158, 159,  49,  68, 119,  92, 123, 177, 204, 187, 254, 200,  78, 141, 149,
						   119,  26, 127,  53, 160,  93, 199, 212,  29,  24, 145, 156, 208, 150, 218, 209,   4, 216,  91,  47,
						   184, 146,  47, 140, 195, 195, 125, 242, 238,  63,  99, 108, 140, 230, 242,  31, 204,  11, 178, 243,
						   217, 156, 213, 231};
static BYTE byRSExp66[] = {  5, 118, 222, 180, 136, 136, 162,  51,  46, 117,  13, 215,  81,  17, 139, 247, 197, 171,  95, 173,
						    65, 137, 178,  68, 111,  95, 101,  41,  72, 214, 169, 197,  95,   7,  44, 154,  77, 111, 236,  40,
						   121, 143,  63,  87,  80, 253, 240, 126, 217,  77,  34, 232, 106,  50, 168,  82,  76, 146,  67, 106,
						   171,  25, 132,  93,  45, 105};
static BYTE byRSExp68[] = {247, 159, 223,  33, 224,  93,  77,  70,  90, 160,  32, 254,  43, 150,  84, 101, 190, 205, 133,  52,
						    60, 202, 165, 220, 203, 151,  93,  84,  15,  84, 253, 173, 160,  89, 227,  52, 199,  97,  95, 231,
						    52, 177,  41, 125, 137, 241, 166, 225, 118,   2,  54,  32,  82, 215, 175, 198,  43, 238, 235,  27,
						   101, 184, 127,   3,   5,   8, 163, 238};

static LPBYTE  byRSExp[] = {NULL,      NULL,      NULL,      NULL,      NULL,      NULL,      NULL,      byRSExp7,  NULL,      NULL,
							byRSExp10, NULL,      NULL,      byRSExp13, NULL,      byRSExp15, byRSExp16, byRSExp17, byRSExp18, NULL,
							byRSExp20, NULL,      byRSExp22, NULL,      byRSExp24, NULL,      byRSExp26, NULL,      byRSExp28, NULL,
							byRSExp30, NULL,      byRSExp32, NULL,      byRSExp34, NULL,      byRSExp36, NULL,      byRSExp38, NULL,
							byRSExp40, NULL,      byRSExp42, NULL,      byRSExp44, NULL,      byRSExp46, NULL,      byRSExp48, NULL,
							byRSExp50, NULL,      byRSExp52, NULL,      byRSExp54, NULL,      byRSExp56, NULL,      byRSExp58, NULL,
							byRSExp60, NULL,      byRSExp62, NULL,      byRSExp64, NULL,      byRSExp66, NULL,      byRSExp68};

// •¶š”ƒCƒ“ƒWƒP[ƒ^ƒrƒbƒg’·(ƒo[ƒWƒ‡ƒ“ƒOƒ‹[ƒv•Ê, {S, M, L})
static const int nIndicatorLenNumeral[]  = {10, 12, 14};
static const int nIndicatorLenAlphabet[] = { 9, 11, 13};
static const int nIndicatorLen8Bit[]	   = { 8, 16, 16};
static const int nIndicatorLenKanji[]	   = { 8, 10, 12};


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsNumeralData
// —p  “rF”šƒ‚[ƒhŠY“–ƒ`ƒFƒbƒN
// ˆø  ”F’²¸•¶š
// –ß‚è’lFŠY“–=true

#define IsNumeralData(c) ((c) >= '0' && (c) <= '9')


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsAlphabetData
// —p  “rF‰p”šƒ‚[ƒhŠY“–ƒ`ƒFƒbƒN
// ˆø  ”F’²¸•¶š
// –ß‚è’lFŠY“–=true

#define IsAlphabetData(c) \
( \
       ((c) >= '0' && (c) <= '9') \
    || ((c) >= 'A' && (c) <= 'Z') \
	|| ((c) == ' ' || (c) == '$' || (c) == '%' || (c) == '*' || \
        (c) == '+' || (c) == '-' || (c) == '.' || (c) == '/' || (c) == ':') \
)


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsKanjiData
// —p  “rFŠ¿šƒ‚[ƒhŠY“–ƒ`ƒFƒbƒN
// ˆø  ”F’²¸•¶ši16ƒrƒbƒg•¶šj
// –ß‚è’lFŠY“–=true
// ”õ  lFEBBFh ˆÈ~‚Ì S-JIS ‚Í‘ÎÛŠO
static BOOL CQR_Encode_IsKanjiData(unsigned char c1, unsigned char c2)
{
       if (((c1 >= 0x81 && c1 <= 0x9f) || (c1 >= 0xe0 && c1 <= 0xeb)) && (c2 >= 0x40))
       {
               if ((c1 == 0x9f && c2 > 0xfc) || (c1 == 0xeb && c2 > 0xbf))
                       return NO;

               return YES;
       }

       return NO;
}

/////////////////////////////////////////////////////////////////////////////
// QR_Encode ƒNƒ‰ƒX‚Ì\’z/Á–Å

#define min(x, y) (((x) < (y)) ? (x) : (y))

/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::EncodeData
// —p  “rFƒf[ƒ^ƒGƒ“ƒR[ƒh
// ˆø  ”FŒë‚è’ù³ƒŒƒxƒ‹AŒ^”Ô(0=©“®)AŒ^”Ô©“®Šg’£ƒtƒ‰ƒOAƒ}ƒXƒLƒ“ƒO”Ô†(-1=©“®)AƒGƒ“ƒR[ƒhƒf[ƒ^AƒGƒ“ƒR[ƒhƒf[ƒ^’·
// –ß‚è’lFƒGƒ“ƒR[ƒh¬Œ÷=trueAƒf[ƒ^‚È‚µA‚Ü‚½‚Í—e—ÊƒI[ƒo[=false

typedef struct tagCQR_EncodePrivateData_Struct {
    int m_ncDataCodeWordBit; // <83>f<81>[<83>^<83>R<81>[<83>h<83><8f><81>[<83>h<83>r<83>b<83>g<92>·
    BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // <93>ü<97>Í<83>f<81>[<83>^<83>G<83><93><83>R<81>[<83>h<83>G<83><8a><83>A

    int m_ncDataBlock;
    BYTE m_byBlockMode[MAX_DATACODEWORD];
    int m_nBlockLength[MAX_DATACODEWORD];

    int m_ncAllCodeWord; // <91><8d><83>R<81>[<83>h<83><8f><81>[<83>h<90><94>(<82>q<82>r<8c>ë<82>è<92>ù<90>³<83>f<81>[<83>^<82>ğ<8a>Ü<82>Ş)
    BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // <91><8d><83>R<81>[<83>h<83><8f><81>[<83>h<8e>Z<8f>o<83>G<83><8a><83>A
    BYTE m_byRSWork[MAX_CODEBLOCK]; // <82>q<82>r<83>R<81>[<83>h<83><8f><81>[<83>h<8e>Z<8f>o<83><8f><81>[<83>N
} CQR_EncodePrivateData_Struct;


#define PRIVATE(var) (((CQR_EncodePrivateData_Struct*)(data->privateData))->var)
#define COMBINED_DATA_SIZE (sizeof(CQR_EncodePrivateData_Struct) + sizeof(CQR_Encode_Struct))

// malloc'ed memory layout
// ==============================================
// private 0 to sizeof(CQR_EncodePrivateData_Struct)-1
// public  sizeof(CQR_EncodePrivateData_Struct) to COMBINED_DATA_SIZE-1


CQR_Encode_Struct* CQR_Encode_Init(void) {
    CQR_Encode_Struct *result = NULL;
    void *data = malloc(COMBINED_DATA_SIZE);
    if (! data) { // handle malloc() failure
        return NULL;
    }
    memset(data, 0, COMBINED_DATA_SIZE);
    if (data) {
        result = (CQR_Encode_Struct *) &((char*)data)[sizeof(CQR_EncodePrivateData_Struct)];
        result->privateData = (CQR_EncodePrivateData_Struct*)(data);
    }
    return result;
}

void CQR_Encode_Free(CQR_Encode_Struct *data) {
    if (data) {
        return;
    }
    memset(data->privateData, 0, sizeof(COMBINED_DATA_SIZE));
    free(data);
}

BOOL CQR_Encode_EncodeDataSource(CQR_Encode_Struct *data, int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource) {
	int i, j;

	data->m_nLevel = nLevel;
	data->m_nMaskingNo = nMaskingNo;

	// ƒf[ƒ^’·‚ªw’è‚³‚ê‚Ä‚¢‚È‚¢ê‡‚Í lstrlen ‚É‚æ‚Á‚Äæ“¾
	int ncLength = (ncSource > 0) ? ncSource : strlen(lpsSource);

	if (ncLength == 0) {
		return NO; // ƒf[ƒ^‚È‚µ
    }

	// ƒo[ƒWƒ‡ƒ“(Œ^”Ô)ƒ`ƒFƒbƒN
	int nEncodeVersion = CQR_Encode_GetEncodeVersion(data, nVersion, lpsSource, ncLength);

	if (nEncodeVersion == 0) {
		return NO; // —e—ÊƒI[ƒo[
    }

	if (nVersion == 0) {
		// Œ^”Ô©“®
		data->m_nVersion = nEncodeVersion;
	} else {
		if (nEncodeVersion <= nVersion) {
			data->m_nVersion = nVersion;
		} else {
			if (bAutoExtent) {
				data->m_nVersion = nEncodeVersion; // ƒo[ƒWƒ‡ƒ“(Œ^”Ô)©“®Šg’£
            } else {
				return NO; // —e—ÊƒI[ƒo[
            }
		}
	}

	// ƒ^[ƒ~ƒl[ƒ^ƒR[ƒh"0000"•t‰Á
	int ncDataCodeWord = QR_VersonInfo[data->m_nVersion].ncDataCodeWord[nLevel];

	int ncTerminater = min(4, (ncDataCodeWord * 8) - PRIVATE(m_ncDataCodeWordBit));

	if (ncTerminater > 0) {
		PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), 0, ncTerminater);
    }

	// ƒpƒfƒBƒ“ƒOƒR[ƒh"11101100, 00010001"•t‰Á
	BYTE byPaddingCode = 0xec;

	for (i = (PRIVATE(m_ncDataCodeWordBit) + 7) / 8; i < ncDataCodeWord; ++i) {
		PRIVATE(m_byDataCodeWord)[i] = byPaddingCode;

		byPaddingCode = (BYTE)((byPaddingCode == 0xec) ? 0x11 : 0xec);
	}

	// ‘ƒR[ƒhƒ[ƒhZoƒGƒŠƒAƒNƒŠƒA
	PRIVATE(m_ncAllCodeWord) = QR_VersonInfo[data->m_nVersion].ncAllCodeWord;
	memset(PRIVATE(m_byAllCodeWord), 0, PRIVATE(m_ncAllCodeWord));

	int nDataCwIndex = 0; // ƒf[ƒ^ƒR[ƒhƒ[ƒhˆ—ˆÊ’u

	// ƒf[ƒ^ƒuƒƒbƒN•ªŠ„”
	int ncBlock1 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo1[nLevel].ncRSBlock;
	int ncBlock2 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo2[nLevel].ncRSBlock;
	int ncBlockSum = ncBlock1 + ncBlock2;

	int nBlockNo = 0; // ˆ—’†ƒuƒƒbƒN”Ô†

	// ƒuƒƒbƒN•Êƒf[ƒ^ƒR[ƒhƒ[ƒh”
	int ncDataCw1 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo1[nLevel].ncDataCodeWord;
	int ncDataCw2 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo2[nLevel].ncDataCodeWord;

	// ƒf[ƒ^ƒR[ƒhƒ[ƒhƒCƒ“ƒ^[ƒŠ[ƒu”z’u
	for (i = 0; i < ncBlock1; ++i) {
		for (j = 0; j < ncDataCw1; ++j) {
			PRIVATE(m_byAllCodeWord)[(ncBlockSum * j) + nBlockNo] = PRIVATE(m_byDataCodeWord)[nDataCwIndex++];
		}

		++nBlockNo;
	}

	for (i = 0; i < ncBlock2; ++i) {
		for (j = 0; j < ncDataCw2; ++j) {
			if (j < ncDataCw1) {
				PRIVATE(m_byAllCodeWord)[(ncBlockSum * j) + nBlockNo] = PRIVATE(m_byDataCodeWord)[nDataCwIndex++];
			} else {
				// ‚Qí–ÚƒuƒƒbƒN’[”•ª”z’u
				PRIVATE(m_byAllCodeWord)[(ncBlockSum * ncDataCw1) + i]  = PRIVATE(m_byDataCodeWord)[nDataCwIndex++];
			}	
		}

		++nBlockNo;
	}

	// ƒuƒƒbƒN•Ê‚q‚rƒR[ƒhƒ[ƒh”(¦Œ»ó‚Å‚Í“¯”)
	int ncRSCw1 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo1[nLevel].ncAllCodeWord - ncDataCw1;
	int ncRSCw2 = QR_VersonInfo[data->m_nVersion].RS_BlockInfo2[nLevel].ncAllCodeWord - ncDataCw2;

	/////////////////////////////////////////////////////////////////////////
	// ‚q‚rƒR[ƒhƒ[ƒhZo

	nDataCwIndex = 0;
	nBlockNo = 0;

	for (i = 0; i < ncBlock1; ++i) {
		memset(PRIVATE(m_byRSWork), 0, sizeof(PRIVATE(m_byRSWork)));

		memmove(PRIVATE(m_byRSWork), PRIVATE(m_byDataCodeWord) + nDataCwIndex, ncDataCw1);

		CQR_Encode_GetRSCodeWord(data, PRIVATE(m_byRSWork), ncDataCw1, ncRSCw1);

		// ‚q‚rƒR[ƒhƒ[ƒh”z’u
		for (j = 0; j < ncRSCw1; ++j) {
			PRIVATE(m_byAllCodeWord)[ncDataCodeWord + (ncBlockSum * j) + nBlockNo] = PRIVATE(m_byRSWork)[j];
		}

		nDataCwIndex += ncDataCw1;
		++nBlockNo;
	}

	for (i = 0; i < ncBlock2; ++i) {
		memset(PRIVATE(m_byRSWork), 0, sizeof(PRIVATE(m_byRSWork)));

		memmove(PRIVATE(m_byRSWork), PRIVATE(m_byDataCodeWord) + nDataCwIndex, ncDataCw2);

		CQR_Encode_GetRSCodeWord(data, PRIVATE(m_byRSWork), ncDataCw2, ncRSCw2);

		// ‚q‚rƒR[ƒhƒ[ƒh”z’u
		for (j = 0; j < ncRSCw2; ++j) {
			PRIVATE(m_byAllCodeWord)[ncDataCodeWord + (ncBlockSum * j) + nBlockNo] = PRIVATE(m_byRSWork)[j];
		}

		nDataCwIndex += ncDataCw2;
		++nBlockNo;
	}

	data->m_nSymbleSize = data->m_nVersion * 4 + 17;

	// ƒ‚ƒWƒ…[ƒ‹”z’u
	CQR_Encode_FormatModule(data);

	return YES;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetEncodeVersion
// —p  “rFƒGƒ“ƒR[ƒhƒo[ƒWƒ‡ƒ“(Œ^”Ô)æ“¾
// ˆø  ”F’²¸ŠJnƒo[ƒWƒ‡ƒ“AƒGƒ“ƒR[ƒhƒf[ƒ^AƒGƒ“ƒR[ƒhƒf[ƒ^’·
// –ß‚è’lFƒo[ƒWƒ‡ƒ“”Ô†i—e—ÊƒI[ƒo[=0j

static int CQR_Encode_GetEncodeVersion(CQR_Encode_Struct *data, int nVersion, LPCSTR lpsSource, int ncLength) {
	int nVerGroup = (nVersion >= 27) ? QR_VRESION_L : ((nVersion >= 10) ? QR_VRESION_M : QR_VRESION_S);
	int i, j;

	for (i = nVerGroup; i <= QR_VRESION_L; ++i) {
		if (CQR_Encode_EncodeSourceData(data, lpsSource, ncLength, i)) {
			if (i == QR_VRESION_S) {
				for (j = 1; j <= 9; ++j) {
					if ((PRIVATE(m_ncDataCodeWordBit) + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[data->m_nLevel]) {
						return j;
                    }
				}
			} else if (i == QR_VRESION_M) {
				for (j = 10; j <= 26; ++j) {
					if ((PRIVATE(m_ncDataCodeWordBit) + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[data->m_nLevel]) {
						return j;
                    }
				}
			} else if (i == QR_VRESION_L) {
				for (j = 27; j <= 40; ++j) {
					if ((PRIVATE(m_ncDataCodeWordBit) + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[data->m_nLevel]) {
						return j;
                    }
				}
			}
		}
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::EncodeSourceData
// —p  “rF“ü—Íƒf[ƒ^ƒGƒ“ƒR[ƒh
// ˆø  ”F“ü—Íƒf[ƒ^A“ü—Íƒf[ƒ^’·Aƒo[ƒWƒ‡ƒ“(Œ^”Ô)ƒOƒ‹[ƒv
// –ß‚è’lFƒGƒ“ƒR[ƒh¬Œ÷=true

static BOOL CQR_Encode_EncodeSourceData(CQR_Encode_Struct *data, LPCSTR lpsSource, int ncLength, int nVerGroup) {
	memset(PRIVATE(m_nBlockLength), 0, sizeof(PRIVATE(m_nBlockLength)));

	int i, j;

	// ‚Ç‚Ìƒ‚[ƒh‚ª‰½•¶š(ƒoƒCƒg)Œp‘±‚µ‚Ä‚¢‚é‚©‚ğ’²¸
	for (PRIVATE(m_ncDataBlock) = i = 0; i < ncLength; ++i) {
		BYTE byMode;

		if (i < ncLength - 1 && CQR_Encode_IsKanjiData(lpsSource[i], lpsSource[i + 1])) {
			byMode = QR_MODE_KANJI;
        } else if (IsNumeralData(lpsSource[i])) {
			byMode = QR_MODE_NUMERAL;
        } else if (IsAlphabetData(lpsSource[i])) {
			byMode = QR_MODE_ALPHABET;
        } else {
			byMode = QR_MODE_8BIT;
        }

		if (i == 0) {
			PRIVATE(m_byBlockMode)[0] = byMode;
        }

		if (PRIVATE(m_byBlockMode)[PRIVATE(m_ncDataBlock)] != byMode) {
			PRIVATE(m_byBlockMode)[++PRIVATE(m_ncDataBlock)] = byMode;
        }

		++PRIVATE(m_nBlockLength)[PRIVATE(m_ncDataBlock)];

		if (byMode == QR_MODE_KANJI) {
			// Š¿š‚Í•¶š”‚Å‚Í‚È‚­	”‚Å‹L˜^
			++(PRIVATE(m_nBlockLength)[PRIVATE(m_ncDataBlock)]);
			++i;
		}
	}

	++(PRIVATE(m_ncDataBlock));

	/////////////////////////////////////////////////////////////////////////
	// —×Ú‚·‚é‰p”šƒ‚[ƒhƒuƒƒbƒN‚Æ”šƒ‚[ƒhƒuƒƒbƒN‚Ì•À‚Ñ‚ğ‚ğğŒ‚É‚æ‚èŒ‹‡

	int ncSrcBits, ncDstBits; // Œ³‚Ìƒrƒbƒg’·‚Æ’Pˆê‚Ì‰p”šƒ‚[ƒhƒuƒƒbƒN‰»‚µ‚½ê‡‚Ìƒrƒbƒg’·

	int nBlock = 0;

	while (nBlock < PRIVATE(m_ncDataBlock) - 1) {
		int ncJoinFront, ncJoinBehind; // ‘OŒã‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ÆŒ‹‡‚µ‚½ê‡‚Ìƒrƒbƒg’·
		int nJoinPosition = 0; // ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚Æ‚ÌŒ‹‡F-1=‘O‚ÆŒ‹‡A0=Œ‹‡‚µ‚È‚¢A1=Œã‚ë‚ÆŒ‹‡

		// u”š|‰p”šv‚Ü‚½‚Íu‰p”š|”šv‚Ì•À‚Ñ
		if ((PRIVATE(m_byBlockMode)[nBlock] == QR_MODE_NUMERAL  && PRIVATE(m_byBlockMode)[nBlock + 1] == QR_MODE_ALPHABET) ||
			(PRIVATE(m_byBlockMode)[nBlock] == QR_MODE_ALPHABET && PRIVATE(m_byBlockMode)[nBlock + 1] == QR_MODE_NUMERAL)) {
			// Œ³‚Ìƒrƒbƒg’·‚Æ’Pˆê‚Ì‰p”šƒ‚[ƒhƒuƒƒbƒN‰»‚µ‚½ê‡‚Ìƒrƒbƒg’·‚ğ”äŠr
			ncSrcBits = CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock], PRIVATE(m_nBlockLength)[nBlock], nVerGroup) +
						CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock + 1], PRIVATE(m_nBlockLength)[nBlock + 1], nVerGroup);

			ncDstBits = CQR_Encode_GetBitLength(QR_MODE_ALPHABET, PRIVATE(m_nBlockLength)[nBlock] + PRIVATE(m_nBlockLength)[nBlock + 1], nVerGroup);

			if (ncSrcBits > ncDstBits) {
				// ‘OŒã‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ª‚ ‚éê‡A‚»‚ê‚ç‚Æ‚ÌŒ‹‡‚ª—L—˜‚©‚Ç‚¤‚©‚ğƒ`ƒFƒbƒN
				if (nBlock >= 1 && PRIVATE(m_byBlockMode)[nBlock - 1] == QR_MODE_8BIT) {
					// ‘O‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ ‚è
					ncJoinFront = CQR_Encode_GetBitLength(QR_MODE_8BIT, PRIVATE(m_nBlockLength)[nBlock - 1] + PRIVATE(m_nBlockLength)[nBlock], nVerGroup) +
								  CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock + 1], PRIVATE(m_nBlockLength)[nBlock + 1], nVerGroup);

					if (ncJoinFront > ncDstBits + CQR_Encode_GetBitLength(QR_MODE_8BIT, PRIVATE(m_nBlockLength)[nBlock - 1], nVerGroup)) {
						ncJoinFront = 0; // ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚Æ‚ÍŒ‹‡‚µ‚È‚¢
                    }
				} else {
					ncJoinFront = 0;
                }

				if (nBlock < PRIVATE(m_ncDataBlock) - 2 && PRIVATE(m_byBlockMode)[nBlock + 2] == QR_MODE_8BIT) {
					// Œã‚ë‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ ‚è
					ncJoinBehind = CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock], PRIVATE(m_nBlockLength)[nBlock], nVerGroup) +
								   CQR_Encode_GetBitLength(QR_MODE_8BIT, PRIVATE(m_nBlockLength)[nBlock + 1] + PRIVATE(m_nBlockLength)[nBlock + 2], nVerGroup);

					if (ncJoinBehind > ncDstBits + CQR_Encode_GetBitLength(QR_MODE_8BIT, PRIVATE(m_nBlockLength)[nBlock + 2], nVerGroup)) {
						ncJoinBehind = 0; // ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚Æ‚ÍŒ‹‡‚µ‚È‚¢
                    }
				} else {
					ncJoinBehind = 0;
                }

				if (ncJoinFront != 0 && ncJoinBehind != 0) {
					// ‘OŒã—¼•û‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ª‚ ‚éê‡‚Íƒf[ƒ^’·‚ª’Z‚­‚È‚é•û‚ğ—Dæ
					nJoinPosition = (ncJoinFront < ncJoinBehind) ? -1 : 1;
				} else {
					nJoinPosition = (ncJoinFront != 0) ? -1 : ((ncJoinBehind != 0) ? 1 : 0);
				}

				if (nJoinPosition != 0) {
					// ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚Æ‚ÌŒ‹‡
					if (nJoinPosition == -1) {
						PRIVATE(m_nBlockLength)[nBlock - 1] += PRIVATE(m_nBlockLength)[nBlock];

						// Œã‘±‚ğƒVƒtƒg
						for (i = nBlock; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
							PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
							PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
						}
					} else {
						PRIVATE(m_byBlockMode)[nBlock + 1] = QR_MODE_8BIT;
						PRIVATE(m_nBlockLength)[nBlock + 1] += PRIVATE(m_nBlockLength)[nBlock + 2];

						// Œã‘±‚ğƒVƒtƒg
						for (i = nBlock + 2; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
							PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
							PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
						}
					}

					--PRIVATE(m_ncDataBlock);
				} else {
					// ‰p”š‚Æ”š‚Ì•À‚Ñ‚ğ’Pˆê‚Ì‰p”šƒ‚[ƒhƒuƒƒbƒN‚É“‡

					if (nBlock < PRIVATE(m_ncDataBlock) - 2 && PRIVATE(m_byBlockMode)[nBlock + 2] == QR_MODE_ALPHABET) {
						// Œ‹‡‚µ‚æ‚¤‚Æ‚·‚éƒuƒƒbƒN‚ÌŒã‚ë‚É‘±‚­‰p”šƒ‚[ƒhƒuƒƒbƒN‚ğŒ‹‡
						PRIVATE(m_nBlockLength)[nBlock + 1] += PRIVATE(m_nBlockLength)[nBlock + 2];

						// Œã‘±‚ğƒVƒtƒg
						for (i = nBlock + 2; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
							PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
							PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
						}

						--(PRIVATE(m_ncDataBlock));
					}

					PRIVATE(m_byBlockMode)[nBlock] = QR_MODE_ALPHABET;
					PRIVATE(m_nBlockLength)[nBlock] += PRIVATE(m_nBlockLength)[nBlock + 1];

					// Œã‘±‚ğƒVƒtƒg
					for (i = nBlock + 1; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
						PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
						PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
					}

					--(PRIVATE(m_ncDataBlock));

					if (nBlock >= 1 && PRIVATE(m_byBlockMode)[nBlock - 1] == QR_MODE_ALPHABET) {
						// Œ‹‡‚µ‚½ƒuƒƒbƒN‚Ì‘O‚Ì‰p”šƒ‚[ƒhƒuƒƒbƒN‚ğŒ‹‡
						PRIVATE(m_nBlockLength)[nBlock - 1] += PRIVATE(m_nBlockLength)[nBlock];

						// Œã‘±‚ğƒVƒtƒg
						for (i = nBlock; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
							PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
							PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
						}

						--(PRIVATE(m_ncDataBlock));
					}
				}

				continue; // Œ»İˆÊ’u‚ÌƒuƒƒbƒN‚ğÄ’²¸
			}
		}

		++nBlock; // ŸƒuƒƒbƒN‚ğ’²¸
	}

	/////////////////////////////////////////////////////////////////////////
	// ˜A‘±‚·‚é’Z‚¢ƒ‚[ƒhƒuƒƒbƒN‚ğ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‰»

	nBlock = 0;

	while (nBlock < PRIVATE(m_ncDataBlock) - 1) {
		ncSrcBits = CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock], PRIVATE(m_nBlockLength)[nBlock], nVerGroup)
					+ CQR_Encode_GetBitLength(PRIVATE(m_byBlockMode)[nBlock + 1], PRIVATE(m_nBlockLength)[nBlock + 1], nVerGroup);

		ncDstBits = CQR_Encode_GetBitLength(QR_MODE_8BIT, PRIVATE(m_nBlockLength)[nBlock] + PRIVATE(m_nBlockLength)[nBlock + 1], nVerGroup);

		// ‘O‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ª‚ ‚éê‡Ad•¡‚·‚éƒCƒ“ƒWƒP[ƒ^•ª‚ğŒ¸Z
		if (nBlock >= 1 && PRIVATE(m_byBlockMode)[nBlock - 1] == QR_MODE_8BIT) {
			ncDstBits -= (4 + nIndicatorLen8Bit[nVerGroup]);
        }

		// Œã‚ë‚É‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ª‚ ‚éê‡Ad•¡‚·‚éƒCƒ“ƒWƒP[ƒ^•ª‚ğŒ¸Z
		if (nBlock < PRIVATE(m_ncDataBlock) - 2 && PRIVATE(m_byBlockMode)[nBlock + 2] == QR_MODE_8BIT) {
			ncDstBits -= (4 + nIndicatorLen8Bit[nVerGroup]);
        }

		if (ncSrcBits > ncDstBits) {
			if (nBlock >= 1 && PRIVATE(m_byBlockMode)[nBlock - 1] == QR_MODE_8BIT) {
				// Œ‹‡‚·‚éƒuƒƒbƒN‚Ì‘O‚É‚ ‚é‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ğŒ‹‡
				PRIVATE(m_nBlockLength)[nBlock - 1] += PRIVATE(m_nBlockLength)[nBlock];

				// Œã‘±‚ğƒVƒtƒg
				for (i = nBlock; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
					PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
					PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
				}

				--(PRIVATE(m_ncDataBlock));
				--nBlock;
			}

			if (nBlock < PRIVATE(m_ncDataBlock) - 2 && PRIVATE(m_byBlockMode)[nBlock + 2] == QR_MODE_8BIT) {
				// Œ‹‡‚·‚éƒuƒƒbƒN‚ÌŒã‚ë‚É‚ ‚é‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒhƒuƒƒbƒN‚ğŒ‹‡
				PRIVATE(m_nBlockLength)[nBlock + 1] += PRIVATE(m_nBlockLength)[nBlock + 2];

				// Œã‘±‚ğƒVƒtƒg
				for (i = nBlock + 2; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
					PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
					PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
				}

				--(PRIVATE(m_ncDataBlock));
			}

			PRIVATE(m_byBlockMode)[nBlock] = QR_MODE_8BIT;
			PRIVATE(m_nBlockLength)[nBlock] += PRIVATE(m_nBlockLength)[nBlock + 1];

			// Œã‘±‚ğƒVƒtƒg
			for (i = nBlock + 1; i < PRIVATE(m_ncDataBlock) - 1; ++i) {
				PRIVATE(m_byBlockMode)[i]  = PRIVATE(m_byBlockMode)[i + 1];
				PRIVATE(m_nBlockLength)[i] = PRIVATE(m_nBlockLength)[i + 1];
			}

			--(PRIVATE(m_ncDataBlock));

			// Œ‹‡‚µ‚½ƒuƒƒbƒN‚Ì‘O‚©‚çÄ’²¸
			if (nBlock >= 1) {
				--nBlock;
            }

			continue;
		}

		++nBlock; // ŸƒuƒƒbƒN‚ğ’²¸
	}

	/////////////////////////////////////////////////////////////////////////
	// ƒrƒbƒg”z—ñ‰»
	int ncComplete = 0; // ˆ—Ïƒf[ƒ^ƒJƒEƒ“ƒ^
	WORD wBinCode;

	PRIVATE(m_ncDataCodeWordBit) = 0; // ƒrƒbƒg’PˆÊˆ—ƒJƒEƒ“ƒ^

	memset(PRIVATE(m_byDataCodeWord), 0, MAX_DATACODEWORD);

	for (i = 0; i < PRIVATE(m_ncDataBlock) && PRIVATE(m_ncDataCodeWordBit) != -1; ++i) {
		if (PRIVATE(m_byBlockMode)[i] == QR_MODE_NUMERAL) {
			/////////////////////////////////////////////////////////////////
			// ”šƒ‚[ƒh

			// ƒCƒ“ƒWƒP[ƒ^(0001b)
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), 1, 4); 

			// •¶š”ƒZƒbƒg
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), (WORD)PRIVATE(m_nBlockLength)[i], nIndicatorLenNumeral[nVerGroup]);

			// ƒrƒbƒg—ñ•Û‘¶
			for (j = 0; j < PRIVATE(m_nBlockLength)[i]; j += 3) {
				if (j < PRIVATE(m_nBlockLength)[i] - 2) {
					wBinCode = (WORD)(((lpsSource[ncComplete + j]	  - '0') * 100) +
									  ((lpsSource[ncComplete + j + 1] - '0') * 10) +
									   (lpsSource[ncComplete + j + 2] - '0'));

					PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 10);
				} else if (j == PRIVATE(m_nBlockLength)[i] - 2) {
					// ’[”‚QƒoƒCƒg
					wBinCode = (WORD)(((lpsSource[ncComplete + j] - '0') * 10) +
									   (lpsSource[ncComplete + j + 1] - '0'));

					PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 7);
				} else if (j == PRIVATE(m_nBlockLength)[i] - 1) {
					// ’[”‚PƒoƒCƒg
					wBinCode = (WORD)(lpsSource[ncComplete + j] - '0');

					PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 4);
				}
			}

			ncComplete += PRIVATE(m_nBlockLength)[i];
		} else if (PRIVATE(m_byBlockMode)[i] == QR_MODE_ALPHABET) {
			/////////////////////////////////////////////////////////////////
			// ‰p”šƒ‚[ƒh

			// ƒ‚[ƒhƒCƒ“ƒWƒP[ƒ^(0010b)
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), 2, 4);

			// •¶š”ƒZƒbƒg
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), (WORD)PRIVATE(m_nBlockLength)[i], nIndicatorLenAlphabet[nVerGroup]);

			// ƒrƒbƒg—ñ•Û‘¶
			for (j = 0; j < PRIVATE(m_nBlockLength)[i]; j += 2) {
				if (j < PRIVATE(m_nBlockLength)[i] - 1) {
					wBinCode = (WORD)((CQR_Encode_AlphabetToBinaly(lpsSource[ncComplete + j]) * 45) +
									   CQR_Encode_AlphabetToBinaly(lpsSource[ncComplete + j + 1]));

					PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 11);
				} else {
					// ’[”‚PƒoƒCƒg
					wBinCode = (WORD)CQR_Encode_AlphabetToBinaly(lpsSource[ncComplete + j]);

					PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 6);
				}
			}

			ncComplete += PRIVATE(m_nBlockLength)[i];
		} else if (PRIVATE(m_byBlockMode)[i] == QR_MODE_8BIT) {
			/////////////////////////////////////////////////////////////////
			// ‚WƒrƒbƒgƒoƒCƒgƒ‚[ƒh

			// ƒ‚[ƒhƒCƒ“ƒWƒP[ƒ^(0100b)
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), 4, 4);

			// •¶š”ƒZƒbƒg
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), (WORD)PRIVATE(m_nBlockLength)[i], nIndicatorLen8Bit[nVerGroup]);

			// ƒrƒbƒg—ñ•Û‘¶
			for (j = 0; j < PRIVATE(m_nBlockLength)[i]; ++j) {
				PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), (WORD)lpsSource[ncComplete + j], 8);
			}

			ncComplete += PRIVATE(m_nBlockLength)[i];
		} else /* PRIVATE(m_byBlockMode)[i] == QR_MODE_KANJI */ {
			/////////////////////////////////////////////////////////////////
			// Š¿šƒ‚[ƒh

			// ƒ‚[ƒhƒCƒ“ƒWƒP[ƒ^(1000b)
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), 8, 4);

			// •¶š”ƒZƒbƒg
			PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), (WORD)(PRIVATE(m_nBlockLength)[i] / 2), nIndicatorLenKanji[nVerGroup]);

			// Š¿šƒ‚[ƒh‚Åƒrƒbƒg—ñ•Û‘¶
			for (j = 0; j < PRIVATE(m_nBlockLength)[i] / 2; ++j) {
				WORD wBinCode = CQR_Encode_LengthKanjiToBinaly((WORD)(((BYTE)lpsSource[ncComplete + (j * 2)] << 8) + (BYTE)lpsSource[ncComplete + (j * 2) + 1]));

				PRIVATE(m_ncDataCodeWordBit) = CQR_Encode_SetBitStream(data, PRIVATE(m_ncDataCodeWordBit), wBinCode, 13);
			}

			ncComplete += PRIVATE(m_nBlockLength)[i];
		}
	}

	return (PRIVATE(m_ncDataCodeWordBit) != -1);
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetBitLength
// —p  “rFƒrƒbƒg’·æ“¾
// ˆø  ”Fƒf[ƒ^ƒ‚[ƒhí•ÊAƒf[ƒ^’·Aƒo[ƒWƒ‡ƒ“(Œ^”Ô)ƒOƒ‹[ƒv
// –ß‚è’lFƒf[ƒ^ƒrƒbƒg’·
// ”õ  lFŠ¿šƒ‚[ƒh‚Å‚Ìƒf[ƒ^’·ˆø”‚Í•¶š”‚Å‚Í‚È‚­ƒoƒCƒg”

static int CQR_Encode_GetBitLength(BYTE nMode, int ncData, int nVerGroup) {
	int ncBits = 0;

	switch (nMode) {
	case QR_MODE_NUMERAL:
		ncBits = 4 + nIndicatorLenNumeral[nVerGroup] + (10 * (ncData / 3));
		switch (ncData % 3) {
		case 1:
			ncBits += 4;
			break;
		case 2:
			ncBits += 7;
			break;
		default: // case 0:
			break;
		}

		break;

	case QR_MODE_ALPHABET:
		ncBits = 4 + nIndicatorLenAlphabet[nVerGroup] + (11 * (ncData / 2)) + (6 * (ncData % 2));
		break;

	case QR_MODE_8BIT:
		ncBits = 4 + nIndicatorLen8Bit[nVerGroup] + (8 * ncData);
		break;

	default: // case QR_MODE_KANJI:
		ncBits = 4 + nIndicatorLenKanji[nVerGroup] + (13 * (ncData / 2));
		break;
	}

	return ncBits;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetBitStream
// —p  “rFƒrƒbƒgƒZƒbƒg
// ˆø  ”F‘}“üˆÊ’uAƒrƒbƒg”z—ñƒf[ƒ^Aƒf[ƒ^ƒrƒbƒg’·(Å‘å16)
// –ß‚è’lFŸ‰ñ‘}“üˆÊ’u(ƒoƒbƒtƒ@ƒI[ƒo[=-1)
// ”õ  lFPRIVATE(m_byDataCodeWord) ‚ÉŒ‹‰Ê‚ğƒZƒbƒg(—vƒ[ƒ‰Šú‰»)

static int CQR_Encode_SetBitStream(CQR_Encode_Struct *data, int nIndex, WORD wData, int ncData) {
	int i;

	if (nIndex == -1 || nIndex + ncData > MAX_DATACODEWORD * 8) {
		return -1;
    }

	for (i = 0; i < ncData; ++i) {
		if (wData & (1 << (ncData - i - 1))) {
			PRIVATE(m_byDataCodeWord)[(nIndex + i) / 8] |= 1 << (7 - ((nIndex + i) % 8));
		}
	}

	return nIndex + ncData;
}



/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::AlphabetToBinaly
// —p  “rF‰p”šƒ‚[ƒh•¶š‚ÌƒoƒCƒiƒŠ‰»
// ˆø  ”F‘ÎÛ•¶š
// –ß‚è’lFƒoƒCƒiƒŠ’l

BYTE CQR_Encode_AlphabetToBinaly(unsigned char c)
{
	if      (c >= '0' && c <= '9') { return (unsigned char)(c - '0'); }
    else if (c >= 'A' && c <= 'Z') { return (unsigned char)(c - 'A' + 10); }
    else if (c == ' ') { return 36; }
    else if (c == '$') { return 37; }
    else if (c == '%') { return 38; }
    else if (c == '*') { return 39; }
    else if (c == '+') { return 40; }
    else if (c == '-') { return 41; }
    else if (c == '.') { return 42; }
    else if (c == '/') { return 43; }
    else /* c == ':' */{ return 44; } 

}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::KanjiToBinaly
// —p  “rFŠ¿šƒ‚[ƒh•¶š‚ÌƒoƒCƒiƒŠ‰»
// ˆø  ”F‘ÎÛ•¶š
// –ß‚è’lFƒoƒCƒiƒŠ’l

WORD CQR_Encode_LengthKanjiToBinaly(WORD wc)
{
	if (wc >= 0x8140 && wc <= 0x9ffc) {
		wc -= 0x8140;
    } else { // (wc >= 0xe040 && wc <= 0xebbf)
		wc -= 0xc140;
    }

	return (WORD)(((wc >> 8) * 0xc0) + (wc & 0x00ff));
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetRSCodeWord
// —p  “rF‚q‚rŒë‚è’ù³ƒR[ƒhƒ[ƒhæ“¾
// ˆø  ”Fƒf[ƒ^ƒR[ƒhƒ[ƒhƒAƒhƒŒƒXAƒf[ƒ^ƒR[ƒhƒ[ƒh’·A‚q‚rƒR[ƒhƒ[ƒh’·
// ”õ  lF‘ƒR[ƒhƒ[ƒh•ª‚ÌƒGƒŠƒA‚ğŠm•Û‚µ‚Ä‚©‚çŒÄ‚Ño‚µ

static void CQR_Encode_GetRSCodeWord(CQR_Encode_Struct *data, LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord)
{
	int i, j;

	for (i = 0; i < ncDataCodeWord ; ++i) {
		if (lpbyRSWork[0] != 0) {
			BYTE nExpFirst = byIntToExp[lpbyRSWork[0]]; // ‰€ŒW”‚æ‚èæ”Zo

			for (j = 0; j < ncRSCodeWord; ++j) {
				// Še€æ”‚É‰€æ”‚ğ‰ÁZi% 255 ¨ ƒ¿^255 = 1j
				BYTE nExpElement = (BYTE)(((int)(byRSExp[ncRSCodeWord][j] + nExpFirst)) % 255);

				// ”r‘¼˜_—˜a‚É‚æ‚éè—]Zo
				lpbyRSWork[j] = (BYTE)(lpbyRSWork[j + 1] ^ byExpToInt[nExpElement]);
			}

			// c‚èŒ…‚ğƒVƒtƒg
			for (j = ncRSCodeWord; j < ncDataCodeWord + ncRSCodeWord - 1; ++j) {
				lpbyRSWork[j] = lpbyRSWork[j + 1];
            }
		}
		else
		{
			// c‚èŒ…‚ğƒVƒtƒg
			for (j = 0; j < ncDataCodeWord + ncRSCodeWord - 1; ++j) {
				lpbyRSWork[j] = lpbyRSWork[j + 1];
            }
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::FormatModule
// —p  “rFƒ‚ƒWƒ…[ƒ‹‚Ö‚Ìƒf[ƒ^”z’u
// –ß‚è’lFˆê•Ó‚Ìƒ‚ƒWƒ…[ƒ‹”

static void CQR_Encode_FormatModule(CQR_Encode_Struct *data) {
	int i, j;

	memset(data->m_byModuleData, 0, sizeof(data->m_byModuleData));

	// ‹@”\ƒ‚ƒWƒ…[ƒ‹”z’u
	CQR_Encode_SetFunctionModule(data);

	// ƒf[ƒ^ƒpƒ^[ƒ“”z’u
	CQR_Encode_SetCodeWordPattern(data);

	if (data->m_nMaskingNo == -1) {
		// Å“Kƒ}ƒXƒLƒ“ƒOƒpƒ^[ƒ“‘I‘ğ
		data->m_nMaskingNo = 0;

		CQR_Encode_SetMaskingPattern(data, data->m_nMaskingNo); // ƒ}ƒXƒLƒ“ƒO
		CQR_Encode_SetFormatInfoPattern(data, data->m_nMaskingNo); // ƒtƒH[ƒ}ƒbƒgî•ñƒpƒ^[ƒ“”z’u

		int nMinPenalty = CQR_Encode_CountPenalty(data);

		for (i = 1; i <= 7; ++i) {
			CQR_Encode_SetMaskingPattern(data, i); // ƒ}ƒXƒLƒ“ƒO
			CQR_Encode_SetFormatInfoPattern(data, i); // ƒtƒH[ƒ}ƒbƒgî•ñƒpƒ^[ƒ“”z’u

			int nPenalty = CQR_Encode_CountPenalty(data);

			if (nPenalty < nMinPenalty) {
				nMinPenalty = nPenalty;
				data->m_nMaskingNo = i;
			}
		}
	}

	CQR_Encode_SetMaskingPattern(data, data->m_nMaskingNo); // ƒ}ƒXƒLƒ“ƒO
	CQR_Encode_SetFormatInfoPattern(data, data->m_nMaskingNo); // ƒtƒH[ƒ}ƒbƒgî•ñƒpƒ^[ƒ“”z’u

	// ƒ‚ƒWƒ…[ƒ‹ƒpƒ^[ƒ“‚ğƒu[ƒ‹’l‚É•ÏŠ·
	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize; ++j) {
			data->m_byModuleData[i][j] = (BYTE)((data->m_byModuleData[i][j] & 0x11) != 0);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFunctionModule
// —p  “rF‹@”\ƒ‚ƒWƒ…[ƒ‹”z’u
// ”õ  lFƒtƒH[ƒ}ƒbƒgî•ñ‚Í‹@”\ƒ‚ƒWƒ…[ƒ‹“o˜^‚Ì‚İ(Àƒf[ƒ^‚Í‹ó”’)

static void CQR_Encode_SetFunctionModule(CQR_Encode_Struct *data) {
	int i, j;

	// ˆÊ’uŒŸoƒpƒ^[ƒ“
	CQR_Encode_SetFinderPattern(data, 0, 0);
	CQR_Encode_SetFinderPattern(data, data->m_nSymbleSize - 7, 0);
	CQR_Encode_SetFinderPattern(data, 0, data->m_nSymbleSize - 7);

	// ˆÊ’uŒŸoƒpƒ^[ƒ“ƒZƒpƒŒ[ƒ^
	for (i = 0; i < 8; ++i) {
		data->m_byModuleData[i][7] = data->m_byModuleData[7][i] = '\x20';
		data->m_byModuleData[data->m_nSymbleSize - 8][i] = data->m_byModuleData[data->m_nSymbleSize - 8 + i][7] = '\x20';
		data->m_byModuleData[i][data->m_nSymbleSize - 8] = data->m_byModuleData[7][data->m_nSymbleSize - 8 + i] = '\x20';
	}

	// ƒtƒH[ƒ}ƒbƒgî•ñ‹LqˆÊ’u‚ğ‹@”\ƒ‚ƒWƒ…[ƒ‹•”‚Æ‚µ‚Ä“o˜^
	for (i = 0; i < 9; ++i) {
		data->m_byModuleData[i][8] = data->m_byModuleData[8][i] = '\x20';
	}

	for (i = 0; i < 8; ++i) {
		data->m_byModuleData[data->m_nSymbleSize - 8 + i][8] = data->m_byModuleData[8][data->m_nSymbleSize - 8 + i] = '\x20';
	}

	// ƒo[ƒWƒ‡ƒ“î•ñƒpƒ^[ƒ“
	CQR_Encode_SetVersionPattern(data);

	// ˆÊ’u‡‚í‚¹ƒpƒ^[ƒ“
	for (i = 0; i < QR_VersonInfo[data->m_nVersion].ncAlignPoint; ++i) {
		CQR_Encode_SetAlignmentPattern(data, QR_VersonInfo[data->m_nVersion].nAlignPoint[i], 6);
		CQR_Encode_SetAlignmentPattern(data, 6, QR_VersonInfo[data->m_nVersion].nAlignPoint[i]);

		for (j = 0; j < QR_VersonInfo[data->m_nVersion].ncAlignPoint; ++j) {
			CQR_Encode_SetAlignmentPattern(data, QR_VersonInfo[data->m_nVersion].nAlignPoint[i], QR_VersonInfo[data->m_nVersion].nAlignPoint[j]);
		}
	}

	// ƒ^ƒCƒ~ƒ“ƒOƒpƒ^[ƒ“
	for (i = 8; i <= data->m_nSymbleSize - 9; ++i) {
		data->m_byModuleData[i][6] = ((i % 2) == 0) ? '\x30' : '\x20';
		data->m_byModuleData[6][i] = ((i % 2) == 0) ? '\x30' : '\x20';
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFinderPattern
// —p  “rFˆÊ’uŒŸoƒpƒ^[ƒ“”z’u
// ˆø  ”F”z’u¶ãÀ•W

static void CQR_Encode_SetFinderPattern(CQR_Encode_Struct *data, int x, int y) {
	static BYTE byPattern[] = {0x7f,  // 1111111b
							   0x41,  // 1000001b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x41,  // 1000001b
							   0x7f}; // 1111111b
	int i, j;

	for (i = 0; i < 7; ++i) {
		for (j = 0; j < 7; ++j) {
			data->m_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (6 - j))) ? '\x30' : '\x20'; 
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetAlignmentPattern
// —p  “rFˆÊ’u‡‚í‚¹ƒpƒ^[ƒ“”z’u
// ˆø  ”F”z’u’†‰›À•W

static void CQR_Encode_SetAlignmentPattern(CQR_Encode_Struct *data, int x, int y) {
	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b
	int i, j;

	if (data->m_byModuleData[x][y] & 0x20) {
		return; // ‹@”\ƒ‚ƒWƒ…[ƒ‹‚Æd•¡‚·‚é‚½‚ßœŠO
    }

	x -= 2; y -= 2; // ¶ã‹÷À•W‚É•ÏŠ·

	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			data->m_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetVersionPattern
// —p  “rFƒo[ƒWƒ‡ƒ“(Œ^”Ô)î•ñƒpƒ^[ƒ“”z’u
// ”õ  lFŠg’£‚a‚b‚g(18,6)•„†‚ğŒë‚è’ù³‚Æ‚µ‚Äg—p

static void CQR_Encode_SetVersionPattern(CQR_Encode_Struct *data) {
	int i, j;

	if (data->m_nVersion <= 6) {
		return;
    }

	int nVerData = data->m_nVersion << 12;

	// è—]ƒrƒbƒgZo
	for (i = 0; i < 6; ++i) {
		if (nVerData & (1 << (17 - i))) {
			nVerData ^= (0x1f25 << (5 - i));
		}
	}

	nVerData += data->m_nVersion << 12;

	for (i = 0; i < 6; ++i) {
		for (j = 0; j < 3; ++j) {
			data->m_byModuleData[data->m_nSymbleSize - 11 + j][i] = data->m_byModuleData[i][data->m_nSymbleSize - 11 + j] =
			(nVerData & (1 << (i * 3 + j))) ? '\x30' : '\x20';
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetCodeWordPattern
// —p  “rFƒf[ƒ^ƒpƒ^[ƒ“”z’u

static void CQR_Encode_SetCodeWordPattern(CQR_Encode_Struct *data) {
	int x = data->m_nSymbleSize;
	int y = data->m_nSymbleSize - 1;

	int nCoef_x = 1; // ‚˜²”z’uŒü‚«
	int nCoef_y = 1; // ‚™²”z’uŒü‚«

	int i, j;

	for (i = 0; i < PRIVATE(m_ncAllCodeWord); ++i) {
		for (j = 0; j < 8; ++j) {
			do {
				x += nCoef_x;
				nCoef_x *= -1;

				if (nCoef_x < 0) {
					y += nCoef_y;

					if (y < 0 || y == data->m_nSymbleSize) {
						y = (y < 0) ? 0 : data->m_nSymbleSize - 1;
						nCoef_y *= -1;

						x -= 2;

						if (x == 6) { // ƒ^ƒCƒ~ƒ“ƒOƒpƒ^[ƒ“
							--x;
                        }
					}
				}
			} while (data->m_byModuleData[x][y] & 0x20); // ‹@”\ƒ‚ƒWƒ…[ƒ‹‚ğœŠO

			data->m_byModuleData[x][y] = (PRIVATE(m_byAllCodeWord)[i] & (1 << (7 - j))) ? '\x02' : '\x00';
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetMaskingPattern
// —p  “rFƒ}ƒXƒLƒ“ƒOƒpƒ^[ƒ“”z’u
// ˆø  ”Fƒ}ƒXƒLƒ“ƒOƒpƒ^[ƒ“”Ô†

static void CQR_Encode_SetMaskingPattern(CQR_Encode_Struct *data, int nPatternNo)
{
	int i, j;

	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize; ++j) {
			if (! (data->m_byModuleData[j][i] & 0x20)) { // ‹@”\ƒ‚ƒWƒ…[ƒ‹‚ğœŠO
				BOOL bMask;

				switch (nPatternNo) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}

				data->m_byModuleData[j][i] = (BYTE)((data->m_byModuleData[j][i] & 0xfe) | (((data->m_byModuleData[j][i] & 0x02) > 1) ^ bMask));
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFormatInfoPattern
// —p  “rFƒtƒH[ƒ}ƒbƒgî•ñ”z’u
// ˆø  ”Fƒ}ƒXƒLƒ“ƒOƒpƒ^[ƒ“”Ô†

static void CQR_Encode_SetFormatInfoPattern(CQR_Encode_Struct *data, int nPatternNo) {
	int nFormatInfo;
	int i;

	switch (data->m_nLevel) {
	case QR_LEVEL_M:
		nFormatInfo = 0x00; // 00nnnb
		break;

	case QR_LEVEL_L:
		nFormatInfo = 0x08; // 01nnnb
		break;

	case QR_LEVEL_Q:
		nFormatInfo = 0x18; // 11nnnb
		break;

	default: // case QR_LEVEL_H:
		nFormatInfo = 0x10; // 10nnnb
		break;
	}

	nFormatInfo += nPatternNo;

	int nFormatData = nFormatInfo << 10;

	// è—]ƒrƒbƒgZo
	for (i = 0; i < 5; ++i) {
		if (nFormatData & (1 << (14 - i))) {
			nFormatData ^= (0x0537 << (4 - i)); // 10100110111b
		}
	}

	nFormatData += nFormatInfo << 10;

	// ƒ}ƒXƒLƒ“ƒO
	nFormatData ^= 0x5412; // 101010000010010b

	// ¶ãˆÊ’uŒŸoƒpƒ^[ƒ“ü‚è”z’u
	for (i = 0; i <= 5; ++i) {
		data->m_byModuleData[8][i] = (nFormatData & (1 << i)) ? '\x30' : '\x20';
    }

	data->m_byModuleData[8][7] = (nFormatData & (1 << 6)) ? '\x30' : '\x20';
	data->m_byModuleData[8][8] = (nFormatData & (1 << 7)) ? '\x30' : '\x20';
	data->m_byModuleData[7][8] = (nFormatData & (1 << 8)) ? '\x30' : '\x20';

	for (i = 9; i <= 14; ++i) {
		data->m_byModuleData[14 - i][8] = (nFormatData & (1 << i)) ? '\x30' : '\x20';
    }

	// ‰EãˆÊ’uŒŸoƒpƒ^[ƒ“‰º”z’u
	for (i = 0; i <= 7; ++i) {
		data->m_byModuleData[data->m_nSymbleSize - 1 - i][8] = (nFormatData & (1 << i)) ? '\x30' : '\x20';
    }

	// ¶‰ºˆÊ’uŒŸoƒpƒ^[ƒ“‰E”z’u
	data->m_byModuleData[8][data->m_nSymbleSize - 8] = '\x30'; // ŒÅ’èˆÃƒ‚ƒWƒ…[ƒ‹

	for (i = 8; i <= 14; ++i) {
		data->m_byModuleData[8][data->m_nSymbleSize - 15 + i] = (nFormatData & (1 << i)) ? '\x30' : '\x20';
    }
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::CountPenalty
// —p  “rFƒ}ƒXƒNŒãƒyƒiƒ‹ƒeƒBƒXƒRƒAZo

static int CQR_Encode_CountPenalty(CQR_Encode_Struct *data) {
	int nPenalty = 0;
	int i, j, k;

	// “¯F‚Ì—ñ‚Ì—×Úƒ‚ƒWƒ…[ƒ‹
	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize - 4; ++j) {
			int nCount = 1;

			for (k = j + 1; k < data->m_nSymbleSize; k++) {
				if (((data->m_byModuleData[i][j] & 0x11) == 0) == ((data->m_byModuleData[i][k] & 0x11) == 0)) {
					++nCount;
                } else {
					break;
                }
			}

			if (nCount >= 5) {
				nPenalty += 3 + (nCount - 5);
			}

			j = k - 1;
		}
	}

	// “¯F‚Ìs‚Ì—×Úƒ‚ƒWƒ…[ƒ‹
	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize - 4; ++j) {
			int nCount = 1;

			for (k = j + 1; k < data->m_nSymbleSize; k++) {
				if (((data->m_byModuleData[j][i] & 0x11) == 0) == ((data->m_byModuleData[k][i] & 0x11) == 0)) {
					++nCount;
                } else {
					break;
                }
			}

			if (nCount >= 5) {
				nPenalty += 3 + (nCount - 5);
			}

			j = k - 1;
		}
	}

	// “¯F‚Ìƒ‚ƒWƒ…[ƒ‹ƒuƒƒbƒNi‚Q~‚Qj
	for (i = 0; i < data->m_nSymbleSize - 1; ++i) {
		for (j = 0; j < data->m_nSymbleSize - 1; ++j) {
			if ((((data->m_byModuleData[i][j] & 0x11) == 0) == ((data->m_byModuleData[i + 1][j]		& 0x11) == 0)) &&
				(((data->m_byModuleData[i][j] & 0x11) == 0) == ((data->m_byModuleData[i]	[j + 1] & 0x11) == 0)) &&
				(((data->m_byModuleData[i][j] & 0x11) == 0) == ((data->m_byModuleData[i + 1][j + 1] & 0x11) == 0))) {
				nPenalty += 3;
			}
		}
	}

	// “¯ˆê—ñ‚É‚¨‚¯‚é 1:1:3:1:1 ”ä—¦iˆÃ:–¾:ˆÃ:–¾:ˆÃj‚Ìƒpƒ^[ƒ“
	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize - 6; ++j) {
			if (((j == 0) ||				 (! (data->m_byModuleData[i][j - 1] & 0x11))) && // –¾ ‚Ü‚½‚Í ƒVƒ“ƒ{ƒ‹ŠO
											 (   data->m_byModuleData[i][j]     & 0x11)   && // ˆÃ - 1
											 (! (data->m_byModuleData[i][j + 1] & 0x11))  && // –¾ - 1
											 (   data->m_byModuleData[i][j + 2] & 0x11)   && // ˆÃ „¢
											 (   data->m_byModuleData[i][j + 3] & 0x11)   && // ˆÃ „ 3
											 (   data->m_byModuleData[i][j + 4] & 0x11)   && // ˆÃ „£
											 (! (data->m_byModuleData[i][j + 5] & 0x11))  && // –¾ - 1
											 (   data->m_byModuleData[i][j + 6] & 0x11)   && // ˆÃ - 1
				((j == data->m_nSymbleSize - 7) || (! (data->m_byModuleData[i][j + 7] & 0x11))))   // –¾ ‚Ü‚½‚Í ƒVƒ“ƒ{ƒ‹ŠO
			{
				// ‘O‚Ü‚½‚ÍŒã‚É4ˆÈã‚Ì–¾ƒpƒ^[ƒ“
				if (((j < 2 || ! (data->m_byModuleData[i][j - 2] & 0x11)) && 
					 (j < 3 || ! (data->m_byModuleData[i][j - 3] & 0x11)) &&
					 (j < 4 || ! (data->m_byModuleData[i][j - 4] & 0x11))) ||
					((j >= data->m_nSymbleSize - 8  || ! (data->m_byModuleData[i][j + 8]  & 0x11)) &&
					 (j >= data->m_nSymbleSize - 9  || ! (data->m_byModuleData[i][j + 9]  & 0x11)) &&
					 (j >= data->m_nSymbleSize - 10 || ! (data->m_byModuleData[i][j + 10] & 0x11)))) {
					nPenalty += 40;
				}
			}
		}
	}

	// “¯ˆês‚É‚¨‚¯‚é 1:1:3:1:1 ”ä—¦iˆÃ:–¾:ˆÃ:–¾:ˆÃj‚Ìƒpƒ^[ƒ“
	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize - 6; ++j) {
			if (((j == 0) ||				 (! (data->m_byModuleData[j - 1][i] & 0x11))) && // –¾ ‚Ü‚½‚Í ƒVƒ“ƒ{ƒ‹ŠO
											 (   data->m_byModuleData[j]    [i] & 0x11)   && // ˆÃ - 1
											 (! (data->m_byModuleData[j + 1][i] & 0x11))  && // –¾ - 1
											 (   data->m_byModuleData[j + 2][i] & 0x11)   && // ˆÃ „¢
											 (   data->m_byModuleData[j + 3][i] & 0x11)   && // ˆÃ „ 3
											 (   data->m_byModuleData[j + 4][i] & 0x11)   && // ˆÃ „£
											 (! (data->m_byModuleData[j + 5][i] & 0x11))  && // –¾ - 1
											 (   data->m_byModuleData[j + 6][i] & 0x11)   && // ˆÃ - 1
				((j == data->m_nSymbleSize - 7) || (! (data->m_byModuleData[j + 7][i] & 0x11))))   // –¾ ‚Ü‚½‚Í ƒVƒ“ƒ{ƒ‹ŠO
			{
				// ‘O‚Ü‚½‚ÍŒã‚É4ˆÈã‚Ì–¾ƒpƒ^[ƒ“
				if (((j < 2 || ! (data->m_byModuleData[j - 2][i] & 0x11)) && 
					 (j < 3 || ! (data->m_byModuleData[j - 3][i] & 0x11)) &&
					 (j < 4 || ! (data->m_byModuleData[j - 4][i] & 0x11))) ||
					((j >= data->m_nSymbleSize - 8  || ! (data->m_byModuleData[j + 8][i]  & 0x11)) &&
					 (j >= data->m_nSymbleSize - 9  || ! (data->m_byModuleData[j + 9][i]  & 0x11)) &&
					 (j >= data->m_nSymbleSize - 10 || ! (data->m_byModuleData[j + 10][i] & 0x11)))) {
					nPenalty += 40;
				}
			}
		}
	}

	// ‘S‘Ì‚É‘Î‚·‚éˆÃƒ‚ƒWƒ…[ƒ‹‚Ìè‚ß‚éŠ„‡
	int nCount = 0;

	for (i = 0; i < data->m_nSymbleSize; ++i) {
		for (j = 0; j < data->m_nSymbleSize; ++j) {
			if (! (data->m_byModuleData[i][j] & 0x11)) {
				++nCount;
			}
		}
	}

	nPenalty += (abs(50 - ((nCount * 100) / (data->m_nSymbleSize * data->m_nSymbleSize))) / 5) * 10;

	return nPenalty;
}
