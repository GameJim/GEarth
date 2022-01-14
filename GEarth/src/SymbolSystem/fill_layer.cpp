#include "SymbolSystem/fill_layer.h"

namespace symbol
{

    CFillLayer::CFillLayer(const EnFillLayerType& type)
        :m_eFillLayerType(type),m_pLineSymbol(nullptr)
    {

    }


    CFillLayer::EnFillLayerType CFillLayer::GetFillLayerType() const
    {
        return m_eFillLayerType;
    }

    void CFillLayer::Serialize(CByte& data)
    {
        data << m_IsEnable;


        if (m_pLineSymbol)
        {
            data << true;
            m_pLineSymbol->Serialize(data);
        }
        else
        {
            data << false;
        }

      
    }

    void CFillLayer::Deserialize(CByte& data)
    {
        data >> m_IsEnable;

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pLineSymbol = std::make_unique<CLineSymbol>();
            m_pLineSymbol->Deserialize(data);
        }

  
    }

    size_t CFillLayer::GetSize()
    {
        size_t nSize = 2;
        if (m_pLineSymbol)
        {
            nSize = nSize + m_pLineSymbol->GetSize();
        }
        return nSize;
    }

    bool CFillLayer::IsEnable() const
    {
        return m_IsEnable;
    }

    void CFillLayer::SetEnable(const bool& enable)
    {
        m_IsEnable = enable;
    }

    void CFillLayer::SetLineSymbol(std::unique_ptr<CLineSymbol> pLineSymbol)
    {
        m_pLineSymbol = std::move(pLineSymbol);
    }

    CLineSymbol* CFillLayer::GetLineSymbol()
    {
        return m_pLineSymbol.get();
    }

}


