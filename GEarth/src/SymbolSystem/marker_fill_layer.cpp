#include "SymbolSystem/marker_fill_layer.h"
#include "common/type_size.h"

namespace symbol
{

    CMarkerFillLayer::CMarkerFillLayer()
        :CFillLayer(CFillLayer::EnFillLayerType::Fill_LAYER_TYPE_MARKER)
        , m_pMarkerSymbol(std::make_unique<CMarkerSymbol>())
        , m_Direction(1.0f,0.0f,0.0f)
    {

    }

    void CMarkerFillLayer::Serialize(CByte& data)
    {
        CFillLayer::Serialize(data);

        if (m_pMarkerSymbol)
        {
            data << true;
            m_pMarkerSymbol->Serialize(data);
        }
        else
        {
            data << false;
        }

        data << m_Step << m_Direction;
    }

    void CMarkerFillLayer::Deserialize(CByte& data)
    {
        CFillLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pMarkerSymbol = std::make_unique<CMarkerSymbol>();
            m_pMarkerSymbol->Deserialize(data);
        }

        data >> m_Step >> m_Direction;

    }

    size_t CMarkerFillLayer::GetSize()
    {
        size_t nSize = 1 + CFillLayer::GetSize();
        if (m_pMarkerSymbol)
        {
            nSize += m_pMarkerSymbol->GetSize();
        }

       
        nSize += CTypeSize::Instance().GetSize(m_Step) + CTypeSize::Instance().GetSize(m_Direction);

        return nSize;
    }

    CMarkerSymbol* CMarkerFillLayer::GetMarkerSymbol()
    {
        return m_pMarkerSymbol.get();
    }

    void CMarkerFillLayer::SetMarkerSymbol(std::unique_ptr<CMarkerSymbol> pColor)
    {
        m_pMarkerSymbol = std::move(pColor);
    }

    

    void CMarkerFillLayer::SetStep(const CVec3d& step)
    {
        m_Step = step;
    }

    CVec3d CMarkerFillLayer::GetStep()
    {
        return m_Step;
    }

    void CMarkerFillLayer::SetXDirection(const CVec3f& vec)
    {
        m_Direction =vec ;
    }

    CVec3f CMarkerFillLayer::GetXDirection()
    {
        return m_Direction;
    }

}
