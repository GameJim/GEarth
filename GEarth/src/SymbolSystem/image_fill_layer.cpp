#include "SymbolSystem/image_fill_layer.h"
#include "common/type_size.h"

namespace symbol
{

    CImageFillLayer::CImageFillLayer()
        :CFillLayer(CFillLayer::EnFillLayerType::Fill_LAYER_TYPE_IMAGE)
        , m_pImage(nullptr)
        , m_Direction(1.0f, 0.0f, 0.0f)
    {

    }

    void CImageFillLayer::Serialize(CByte& data)
    {
        CFillLayer::Serialize(data);

        if (m_pImage)
        {
            data << true;
            m_pImage->Serialize(data);
        }
        else
        {
            data << false;
        }

        data << m_Step << m_Direction;
    }

    void CImageFillLayer::Deserialize(CByte& data)
    {
        CFillLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pImage = std::make_unique<CImage>();
            m_pImage->Deserialize(data);
        }

        data >> m_Step >> m_Direction;

    }

    size_t CImageFillLayer::GetSize()
    {
        size_t nSize = 1 + CFillLayer::GetSize();
        if (m_pImage)
        {
            nSize += m_pImage->GetSize();
        }


        nSize += CTypeSize::Instance().GetSize(m_Step) + CTypeSize::Instance().GetSize(m_Direction);

        return nSize;
    }

    CImage* CImageFillLayer::GetImage()
    {
        return m_pImage.get();
    }

    void CImageFillLayer::SetImageSymbol(std::unique_ptr<CImage> pColor)
    {
        m_pImage = std::move(pColor);
    }



    void CImageFillLayer::SetStep(const CVec3d& step)
    {
        m_Step = step;
    }

    CVec3d CImageFillLayer::GetStep()
    {
        return m_Step;
    }

    void CImageFillLayer::SetXDirection(const CVec3f& vec)
    {
        m_Direction = vec;
    }

    CVec3f CImageFillLayer::GetXDirection()
    {
        return m_Direction;
    }

}



