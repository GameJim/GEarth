#include "symbolSystem/fill_symbol.h"
#include "marker_fill_layer.h"
#include "line_fill_layer.h"
#include "simple_fill_layer.h"
#include "image_fill_layer.h"
#include "common/log.h"
namespace symbol
{
    CFillSymbol::CFillSymbol()
        :CSymbol(CSymbol::EnSymbolType::SYMBOL_TYPE_FILL)
    {
            m_pLayers = new std::vector<std::unique_ptr<CFillLayer>>();
    }

    CFillSymbol::~CFillSymbol()
    {
        delete m_pLayers;
    }


    void CFillSymbol::AddFillLayer(std::unique_ptr<CFillLayer> pFillLayer)
    {
        if (!pFillLayer)
        {
            LOG_ERROR("CFillLayer is error");
        }
        else
        {
            m_pLayers->push_back(std::move(pFillLayer));
        }
    }

    CFillLayer* CFillSymbol::GetFillLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CFillLayer'index  is error");
            return nullptr;
        }
        else
        {
            return (*m_pLayers)[index].get();
        }
    }

    void CFillSymbol::RemoveFillLayer(CFillLayer* pFillLayer)
    {
        for (auto itr = m_pLayers->begin(); itr != m_pLayers->end(); itr++)
        {
            if ((*itr).get() == pFillLayer)
            {
                m_pLayers->erase(itr);
                return;
            }
        }
    }

    void CFillSymbol::RemoveFillLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CFillLayer'index  is error");
        }
        else
        {
            m_pLayers->erase(m_pLayers->begin() + index);
        }
    }



    void CFillSymbol::Serialize(CByte& data)
    {
        CSymbol::Serialize(data);

        unsigned nSize = m_pLayers->size();
        data << nSize;
        for (auto& pFillLayer : *m_pLayers)
        {
            uint8_t nType = pFillLayer->GetFillLayerType();
            data << nType;
            pFillLayer->Serialize(data);
        }

    }

    void CFillSymbol::Deserialize(CByte& data)
    {
        CSymbol::Deserialize(data);

        unsigned nSize;
        data >> nSize;
        for (unsigned i = 0; i < nSize; ++i)
        {
            uint8_t nType;
            data >> nType;
            CFillLayer::EnFillLayerType eFillLayerType = (CFillLayer::EnFillLayerType)nType;

            std::unique_ptr<CFillLayer> pFillLayer;
            switch (eFillLayerType)
            {
            case symbol::CFillLayer::Fill_LAYER_TYPE_SIMPLE:
                pFillLayer = std::make_unique<CSimpleFillLayer>();
                break;
            case symbol::CFillLayer::Fill_LAYER_TYPE_MARKER:
                pFillLayer = std::make_unique<CMarkerFillLayer>();
                break;
            case symbol::CFillLayer::Fill_LAYER_TYPE_LINE:
                pFillLayer = std::make_unique<CLineFillLayer>();
                break;
            case symbol::CFillLayer::Fill_LAYER_TYPE_IMAGE:
                pFillLayer = std::make_unique<CImageFillLayer>();
                break;
            case symbol::CFillLayer::Fill_LAYER_TYPE_CUSTOM:
            {
                std::string sClassName;
                data >> sClassName;
                CFillLayer* pTemp = static_cast<CFillLayer*>(ClassFactory::get_instance(sClassName.c_str()));
                pFillLayer.reset(pTemp);
            }
            break;
            default:
                break;
            }

            pFillLayer->Deserialize(data);
            m_pLayers->push_back(std::move(pFillLayer));
        }


    }

    size_t CFillSymbol::GetSize()
    {
        size_t nSize = m_pLayers->size() + 4 + CSymbol::GetSize();
        for (auto& pFillLayer : *m_pLayers)
        {
            nSize += pFillLayer->GetSize();
        }

        return nSize;
    }
}



