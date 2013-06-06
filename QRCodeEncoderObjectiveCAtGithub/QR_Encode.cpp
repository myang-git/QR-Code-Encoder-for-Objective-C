#include "QR_Encode.h"

// Always return something
static inline CQR_Encode_Struct *CQR_Encode_Init_CPP() {
    CQR_Encode_Struct *result = CQR_Encode_Init();
    if (NULL == result) {
        throw "CQR_Encode_Init malloc failed.  Probable low memory condition.";
    }
    return result;
}

CQR_Encode::CQR_Encode() : CQR_Encode_Proxy(CQR_Encode_Init_CPP()) {
}

CQR_Encode::~CQR_Encode() {
}

BOOL CQR_Encode::EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource) {
    return CQR_Encode_EncodeDataSource(data, nLevel, nVersion, bAutoExtent, nMaskingNo, lpsSource, ncSource);
}
