#include "symbolSystem/image_marker_layer.h"



CImageMarkerLayer::CImageMarkerLayer():
	CMarkerLayer(CMarkerLayer::EnMarkerLayerType::SYMBOL_TYPE_IMAGE)
{

}

bool CImageMarkerLayer::IsEnable() const
{
    if (!m_pImage)
        return false;
  
    return CMarkerLayer::IsEnable();
}

CImage* CImageMarkerLayer::GetImage() const
{
	return m_pImage;
}

void CImageMarkerLayer::SetImage(CImage* pImage)
{
    if (m_pImage)
    {
        delete m_pImage;
    }

    m_pImage = pImage;
}
