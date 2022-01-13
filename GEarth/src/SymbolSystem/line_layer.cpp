#include "SymbolSystem/line_layer.h"
#include "common/type_size.h"


namespace symbol
{

    CLineLayer::EnLineLayerType CLineLayer::GetLineLayerType() const
    {
        return m_eLineLayerType;
    }

    void CLineLayer::Serialize(CByte& data)
    {
        data << m_IsEnable << m_offset;
        
        uint8_t nType;
        data << nType;
        m_eLineCapType = (EnLineCapType)nType;

        data << nType;
        m_eLineJionType = (EnLineJoinType)nType;

        data << m_miterlimit;

        m_lineWidth.Serialize(data);
    }

    void CLineLayer::Deserialize(CByte& data)
    {
        data >> m_IsEnable >> m_offset;
        uint8_t nType = m_eLineCapType;
        data >> nType;

        nType = m_eLineJionType;
        data >> nType;

        data >> m_miterlimit;

        m_lineWidth.Deserialize(data);
    }

    size_t CLineLayer::GetSize()
    {
        size_t nSize;
        nSize = 1 + CTypeSize::Instance().GetSize(m_offset) + m_lineWidth.GetSize() + 2 + DOUBLE_SIZE;

        return nSize;
    }

    bool CLineLayer::IsEnable() const
    {
        return m_IsEnable;
    }

    void CLineLayer::SetEnable(const bool& enable)
    {
        m_IsEnable = enable;
    }

    CLineLayer::CLineWidth& CLineLayer::GetLineWidth()
    {
        return m_lineWidth;
    }

    void CLineLayer::SetLineWidth(const CLineWidth& lineWidth)
    {
        m_lineWidth = lineWidth;
    }

    CLineLayer::EnLineJoinType CLineLayer::LineJoinType()
    {
        return m_eLineJionType;
    }

    void CLineLayer::SetLineJionType(const EnLineJoinType& eLineJoinType)
    {
        m_eLineJionType = eLineJoinType;
    }

   CLineLayer::EnLineCapType CLineLayer::LineCapType()
    {
       return m_eLineCapType;
    }

    void CLineLayer::SetLineCapType(const EnLineCapType& eLineCapType)
    {
        m_eLineCapType = eLineCapType;
    }

    CLineLayer::CLineLayer(const EnLineLayerType& type)
        :m_eLineLayerType(type), m_lineWidth(1.0),m_miterlimit(4.0)
        , m_eLineCapType(LINE_CAP_TYPE_BUTT)
        , m_eLineJionType(LINE_JOIN_TYPE_MITER)
    {

    }

}