#include "symbolSystem/line_symbol.h"
#include "symbolSystem/simple_line_layer.h"
#include "symbolSystem/image_line_layer.h"
#include "symbolSystem/marker_line_layer.h"
#include "common/log.h"
namespace symbol
{

    CLineSymbol::CLineSymbol()
        :CSymbol(CSymbol::EnSymbolType::SYMBOL_TYPE_LINE)
    {

    }


    CLineSymbol::~CLineSymbol()
    {
        delete m_pLayers;
    }

    void CLineSymbol::AddLineLayer(std::unique_ptr<CLineLayer> pLineLayer)
    {
        if (!pLineLayer)
        {
            LOG_ERROR("CLineLayer is error");
        }
        else
        {
            m_pLayers->push_back(std::move(pLineLayer));
        }
    }

    CLineLayer* CLineSymbol::GetLineLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CLineLayer'index  is error");
            return nullptr;
        }
        else
        {
            return (*m_pLayers)[index].get();
        }
    }

    void CLineSymbol::RemoveLineLayer(CLineLayer* pLineLayer)
    {
        for (auto itr = m_pLayers->begin(); itr != m_pLayers->end(); itr++)
        {
            if ((*itr).get() == pLineLayer)
            {
                m_pLayers->erase(itr);
                return;
            }
        }
    }

    void CLineSymbol::RemoveLineLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CLineLayer'index  is error");
        }
        else
        {
            m_pLayers->erase(m_pLayers->begin() + index);
        }
    }

    

    void CLineSymbol::Serialize(CByte& data)
    {
        CSymbol::Serialize(data);

        unsigned nSize = m_pLayers->size();
        data << nSize;
        for (auto& pLineLayer : *m_pLayers)
        {
            uint8_t nType = pLineLayer->GetLineLayerType();
            data << nType;
            pLineLayer->Serialize(data);
        }

    }

    void CLineSymbol::Deserialize(CByte& data)
    {
        CSymbol::Deserialize(data);

        unsigned nSize;
        data >> nSize;
        for (unsigned i = 0; i < nSize; ++i)
        {
            uint8_t nType;
            data >> nType;
            CLineLayer::EnLineLayerType eLineLayerType = (CLineLayer::EnLineLayerType)nType;

            std::unique_ptr<CLineLayer> pLineLayer;
            switch (eLineLayerType)
            {
            case symbol::CLineLayer::LINE_LAYER_TYPE_SIMPLE:
                pLineLayer = std::make_unique<CSimpleLineLayer>();
                break;
            case symbol::CLineLayer::LINE_LAYER_TYPE_MARKER:
                pLineLayer = std::make_unique<CMarkerLineLayer>();
                break;
            case symbol::CLineLayer::LINE_LAYER_TYPE_IMAGE:
                pLineLayer = std::make_unique<CImageLineLayer>();
                break;
            case symbol::CLineLayer::LINE_LAYER_TYPE_CUSTOM:
            {
                std::string sClassName;
                data >> sClassName;
                CLineLayer* pTemp = static_cast<CLineLayer*>(ClassFactory::get_instance(sClassName.c_str()));
                pLineLayer.reset(pTemp);
            }
                break;
            default:
                break;
            }
           
            pLineLayer->Deserialize(data);
            m_pLayers->push_back(std::move(pLineLayer));
        }


    }

    size_t CLineSymbol::GetSize()
    {
        size_t nSize = m_pLayers->size() + 4 + CSymbol::GetSize();
        for (auto& pLineLayer : *m_pLayers)
        {
            nSize += pLineLayer->GetSize();
        }

        return nSize;
    }
}




