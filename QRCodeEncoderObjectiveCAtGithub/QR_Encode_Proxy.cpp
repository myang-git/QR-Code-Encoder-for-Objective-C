#include "QR_Encode.h"

CQR_Encode_Proxy::CQR_Encode_Proxy(CQR_Encode_Struct *_data) : 
    data          (_data),
    m_nLevel      (_data->m_nLevel      ),
    m_nVersion    (_data->m_nVersion    ),
    m_bAutoExtent (_data->m_bAutoExtent ),
    m_nMaskingNo  (_data->m_nMaskingNo  ),
    m_nSymbleSize (_data->m_nSymbleSize ),
    m_byModuleData(_data->m_byModuleData) {
}

CQR_Encode_Proxy::~CQR_Encode_Proxy() {
    if (NULL != data) {
        CQR_Encode_Free(data);
    }
    data = NULL;
}
