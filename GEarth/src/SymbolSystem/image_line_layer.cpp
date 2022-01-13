#include "SymbolSystem/image_line_layer.h"
#include "common/type_size.h"
namespace symbol
{


    CImageLineLayer::CImageLineLayer()
        :CLineLayer(CLineLayer::EnLineLayerType::LINE_LAYER_TYPE_IMAGE)
        , m_pImage(std::make_unique<CImage>())
    {

    }

    void CImageLineLayer::Serialize(CByte& data)
    {
        CLineLayer::Serialize(data);

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

    void CImageLineLayer::Deserialize(CByte& data)
    {
        CLineLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pImage = std::make_unique<CImage>();
            m_pImage->Deserialize(data);
        }


    }

    size_t CImageLineLayer::GetSize()
    {
        size_t nSize = 1 + CLineLayer::GetSize();
        if (m_pImage)
        {
            nSize += m_pImage->GetSize();
        }

     
        return nSize;
    }

    CImage* CImageLineLayer::GetImage()
    {
        return m_pImage.get();
    }

    void CImageLineLayer::SetImage(std::unique_ptr<CImage> pImage)
    {
        m_pImage = std::move(pImage);
    }
}






