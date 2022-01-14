#include "SymbolSystem/simple_fill_layer.h"

namespace symbol
{
    CSimpleFillLayer::CSimpleFillLayer()
        :CFillLayer(CFillLayer::EnFillLayerType::Fill_LAYER_TYPE_SIMPLE)
        , m_pColor(std::make_unique<CColor>())
    {

    }

    void CSimpleFillLayer::Serialize(CByte& data)
    {
        CFillLayer::Serialize(data);

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

    void CSimpleFillLayer::Deserialize(CByte& data)
    {
        CFillLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pColor = std::make_unique<CColor>();
            m_pColor->Deserialize(data);
        }

    }

    size_t CSimpleFillLayer::GetSize()
    {
        size_t nSize = 1 + CFillLayer::GetSize();
        if (m_pColor)
        {
            nSize += m_pColor->GetSize();
        }


        return nSize;
    }

    CColor* CSimpleFillLayer::GetColor()
    {
        return m_pColor.get();
    }

    void CSimpleFillLayer::SetColor(std::unique_ptr<CColor> pColor)
    {
        m_pColor = std::move(pColor);
    }

}

