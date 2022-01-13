#include "symbolSystem/image_marker_layer.h"


namespace symbol
{
    CImageMarkerLayer::CImageMarkerLayer() :
        CMarkerLayer(CMarkerLayer::EnMarkerLayerType::MARKER_LAYER_TYPE_IMAGE)
        ,m_pImage(nullptr)
    {

    }

   CBox3d CImageMarkerLayer::GetBoundBox()
    {
       CBox3d box;
       return box;
    }

   void CImageMarkerLayer::Serialize(CByte& data)
   {

      if (m_pImage)
      {
          data << true;
          m_pImage->Serialize(data);
      }
      else
      {
          data << false;
      }
   }

   void CImageMarkerLayer::Deserialize(CByte& data)
   {
       bool hasObject;
       data >> hasObject;
       if (hasObject)
       {
           m_pImage = std::make_unique<CImage>();
           m_pImage->Deserialize(data);
       }
     
   }

   size_t CImageMarkerLayer::GetSize()
   {
       size_t nSize = 1;
       m_pImage ? nSize += m_pImage->GetSize() :  nSize;
       return nSize;
   }

   bool CImageMarkerLayer::IsEnable() const
    {
        if (!m_pImage)
            return false;

        return CMarkerLayer::IsEnable();
    }

    CImage* CImageMarkerLayer::GetImage() const
    {
        return m_pImage.get();
    }

    void CImageMarkerLayer::SetImage(std::unique_ptr<CImage> pImage)
    {
        m_pImage = std::move(pImage);
    }
}

