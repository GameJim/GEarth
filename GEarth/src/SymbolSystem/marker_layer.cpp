#include "symbolSystem/marker_layer.h"


namespace symbol
{
    CMarkerLayer::EnMarkerLayerType CMarkerLayer::MarkerLayerType() const
    {
        return m_eMarkerLayerType;
    }

    void CMarkerLayer::Serialize(CByte& data)
    {

    }

    void CMarkerLayer::Deserialize(CByte& data)
    {

    }

    size_t CMarkerLayer::GetSize()
    {
        return 1 + 0;
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

    CMarkerLayer::CMarkerLayer(const EnMarkerLayerType& type)
        :m_IsEnable(true), m_eMarkerLayerType(type), m_pos(0.0, 0.0, 0.0)
    {

    }

}
