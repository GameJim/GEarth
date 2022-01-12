#include "SymbolSystem/marker_symbol.h"
#include "SymbolSystem/shape_marker_layer.h"
#include "SymbolSystem/font_marker_layer.h"
#include "symbolSystem/image_marker_layer.h"
#include "Common/Log.h"
#include "common/register.h"
namespace symbol
{

  
    CMarkerSymbol::CMarkerSymbol()
        : CSymbol(CSymbol::EnSymbolType::SYMBOL_TYPE_MARKER)
        , m_symbolSize(1.0)
    {
        m_pLayers = new std::vector<std::unique_ptr<CMarkerLayer>>();
    }


    CMarkerSymbol::~CMarkerSymbol()
    {
        delete m_pLayers;
    }

    void CMarkerSymbol::AddMarkerLayer(std::unique_ptr<CMarkerLayer> pMarkerLayer)
    {
        if (!pMarkerLayer)
        {
            LOG_ERROR("CMarkerLayer is error");
        }
        else
        {
            m_pLayers->push_back(std::move(pMarkerLayer));
        }
    }

    CMarkerLayer* CMarkerSymbol::GetMarkerLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CMarkerLayer'index  is error");
            return nullptr;
        }
        else
        {
            return (*m_pLayers)[index].get();
        }
    }

    void CMarkerSymbol::RemoveMarkerLayer(CMarkerLayer* pMarkerLayer)
    {
        for (auto itr = m_pLayers->begin(); itr != m_pLayers->end(); itr++)
        {
            if ((*itr).get() == pMarkerLayer)
            {
                m_pLayers->erase(itr);
                return;
            }
        }
    }

    void CMarkerSymbol::RemoveMarkerLayer(const unsigned& index)
    {
        if (index >= m_pLayers->size())
        {
            LOG_ERROR("CMarkerLayer'index  is error");
        }
        else
        {
            m_pLayers->erase(m_pLayers->begin() + index);
        }
    }

    CBox3d CMarkerSymbol::GetBoundBox()
    {
        CBox3d box;
        for (auto& pLayer:  *m_pLayers)  {
           box.Contain(pLayer->GetBoundBox());
        }

        return box;
    }

    void CMarkerSymbol::Serialize(CByte& data)
    {
        CSymbol::Serialize(data);

        unsigned nSize = m_pLayers->size();
        data << nSize;
        for (auto& pMakerLayer : *m_pLayers)
        {
            uint8_t nType = pMakerLayer->MarkerLayerType();
            data << nType;
            pMakerLayer->Serialize(data);
        }

        m_symbolSize.Serialize(data);
    }

    void CMarkerSymbol::Deserialize(CByte& data)
    {
        CSymbol::Deserialize(data);

        unsigned nSize;
        data >> nSize;
        for (unsigned i = 0;i < nSize;++i)
        {
            uint8_t nType;
            data >> nType;
            CMarkerLayer::EnMarkerLayerType eMarkerLayerType = (CMarkerLayer::EnMarkerLayerType)nType;

            std::unique_ptr<CMarkerLayer> pMarkerLayer;
            switch (eMarkerLayerType)
            {
            case symbol::CMarkerLayer::SYMBOL_TYPE_SHAPE:
                pMarkerLayer = std::make_unique<CShapeMarkerLayer>();
                break;
            case symbol::CMarkerLayer::SYMBOL_TYPE_IMAGE:
                pMarkerLayer = std::make_unique<CImageMarkerLayer>();
                break;
            case symbol::CMarkerLayer::SYMBOL_TYPE_FONT:
                pMarkerLayer = std::make_unique<CFontMarkerLayer>();
                break;
            case symbol::CMarkerLayer::SYMBOL_TYPE_CUSTOM:
            {
                std::string sClassName;
                data >> sClassName;
                CMarkerLayer* pTemp = static_cast<CMarkerLayer*>(ClassFactory::get_instance(sClassName.c_str()));
                pMarkerLayer.reset(pTemp);
            }
                break;
            default:
                break;
            }
            pMarkerLayer->Deserialize(data);
            m_pLayers->push_back(std::move(pMarkerLayer));
        }
       
        m_symbolSize.Deserialize(data);
    }

    size_t CMarkerSymbol::GetSize()
    {
        size_t nSize = m_pLayers->size() + 4 + CSymbol::GetSize();
        for (auto& pMakerLayer : *m_pLayers)
        {
            nSize += pMakerLayer->GetSize();
        }

        return nSize + m_symbolSize.GetSize();
    }

}