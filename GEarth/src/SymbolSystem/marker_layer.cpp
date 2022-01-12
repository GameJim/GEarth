#include "symbolSystem/marker_layer.h"
#include "common/type_size.h"

namespace symbol
{
    CMarkerLayer::EnMarkerLayerType CMarkerLayer::MarkerLayerType() const
    {
        return m_eMarkerLayerType;
    }

    void CMarkerLayer::Serialize(CByte& data)
    {
        data << m_IsEnable << m_pos << m_Matrix;
       
       
        if (m_pColor)
        {
            data << true;
            m_pColor->Serialize(data);
        }
        else
        {
            data << false;
        }
    }

    void CMarkerLayer::Deserialize(CByte& data)
    {
        data >> m_IsEnable >> m_pos >> m_Matrix;

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pColor = std::make_unique<CColor>();
            m_pColor->Deserialize(data);
        }
       
    }

    size_t CMarkerLayer::GetSize()
    {
        size_t nSize = 1 + common::CTypeSize::Instance().GetSize(m_pos) + common::CTypeSize::Instance().GetSize(m_Matrix) + 1;
        if (m_pColor)
        {
            nSize = nSize + m_pColor->GetSize();
        }
        return nSize;
    }

    bool CMarkerLayer::IsEnable() const
    {
        return m_IsEnable;
    }

    CVec3d CMarkerLayer::GetPos() const
    {
        return m_pos;
    }

    void CMarkerLayer::SetPos(const CVec3d& pos)
    {
        m_pos = pos;
    }

    void CMarkerLayer::SetEnable(const bool& enable)
    {
        m_IsEnable = enable;
    }

    void CMarkerLayer::SetColor(std::unique_ptr<CColor> pColor)
    {
        m_pColor = std::move(pColor);
    }

    CColor* CMarkerLayer::GetOrCreateColor()
    {
        if (!m_pColor)
        {
            m_pColor = std::make_unique<CColor>();
        }

        return m_pColor.get();
    }

    void CMarkerLayer::SetMatrix(const CMatrix4d& matrix)
    {
        m_Matrix = matrix;
    }

    CMatrix4d CMarkerLayer::GetMatrix() const
    {
        return m_Matrix;
    }

    CMarkerLayer::CMarkerLayer(const EnMarkerLayerType& type)
        :m_IsEnable(true), m_eMarkerLayerType(type), m_pos(0.0f, 0.0f, 0.0f), m_pColor(nullptr)
    {

    }

}
