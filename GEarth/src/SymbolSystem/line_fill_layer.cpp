#include "SymbolSystem/line_fill_layer.h"
#include "common/type_size.h"

namespace symbol
{

    CLineFillLayer::CLineFillLayer()
        :CFillLayer(CFillLayer::EnFillLayerType::Fill_LAYER_TYPE_LINE)
        , m_pLineSymbol(std::make_unique<CLineSymbol>())
        , m_Direction(1.0f, 0.0f, 0.0f)
    {

    }

    void CLineFillLayer::Serialize(CByte& data)
    {
        CFillLayer::Serialize(data);

        if (m_pLineSymbol)
        {
            data << true;
            m_pLineSymbol->Serialize(data);
        }
        else
        {
            data << false;
        }

        data << m_Step << m_Direction;
    }

    void CLineFillLayer::Deserialize(CByte& data)
    {
        CFillLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pLineSymbol = std::make_unique<CLineSymbol>();
            m_pLineSymbol->Deserialize(data);
        }

        data >> m_Step >> m_Direction;

    }

    size_t CLineFillLayer::GetSize()
    {
        size_t nSize = 1 + CFillLayer::GetSize();
        if (m_pLineSymbol)
        {
            nSize += m_pLineSymbol->GetSize();
        }


        nSize += CTypeSize::Instance().GetSize(m_Step) + CTypeSize::Instance().GetSize(m_Direction);

        return nSize;
    }

    CLineSymbol* CLineFillLayer::GetLineSymbol()
    {
        return m_pLineSymbol.get();
    }

    void CLineFillLayer::SetLineSymbol(std::unique_ptr<CLineSymbol> pColor)
    {
        m_pLineSymbol = std::move(pColor);
    }



    void CLineFillLayer::SetStep(const CVec3d& step)
    {
        m_Step = step;
    }

    CVec3d CLineFillLayer::GetStep()
    {
        return m_Step;
    }

    void CLineFillLayer::SetXDirection(const CVec3f& vec)
    {
        m_Direction = vec;
    }

    CVec3f CLineFillLayer::GetXDirection()
    {
        return m_Direction;
    }

}
