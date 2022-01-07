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
        data << m_IsEnable;
        data << m_pos.x << m_pos.y << m_pos.z;

        if (m_pColor)
        {
            data << true << m_pColor->r << m_pColor->g << m_pColor->b << m_pColor->a;
        }
        else
        {
            data << false;
        }
    }

    void CMarkerLayer::Deserialize(CByte& data)
    {
        data >> m_IsEnable;
        data >> m_pos.x >> m_pos.y >> m_pos.z;

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pColor = std::make_unique<CColor>();
            data >> m_pColor->r >> m_pColor->g >> m_pColor->b >> m_pColor->a;
        }
       
    }

    size_t CMarkerLayer::GetSize()
    {
        size_t nSize = 1 + 3 * FLOAT_SIZE;
        if (m_pColor)
        {
            nSize + 1 + 4 * FLOAT_SIZE;
        }
        return nSize;
    }

    bool CMarkerLayer::IsEnable() const
    {
        return m_IsEnable;
    }

    CVec3f CMarkerLayer::GetPos() const
    {
        return m_pos;
    }

    void CMarkerLayer::SetPos(const CVec3f& pos)
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

    CMarkerLayer::CMarkerLayer(const EnMarkerLayerType& type)
        :m_IsEnable(true), m_eMarkerLayerType(type), m_pos(0.0f, 0.0f, 0.0f), m_pColor(nullptr)
    {

    }

}
