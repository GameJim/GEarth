#include "SymbolSystem/marker_line_layer.h"
#include "common/type_size.h"
namespace symbol
{


    CMarkerLineLayer::CMarkerLineLayer()
        :CLineLayer(CLineLayer::EnLineLayerType::LINE_LAYER_TYPE_MARKER)
        , m_pMarkerSymbol(std::make_unique<CMarkerSymbol>())
    {

    }

    void CMarkerLineLayer::Serialize(CByte& data)
    {
        CLineLayer::Serialize(data);

        if (m_pMarkerSymbol)
        {
            data << true;
            m_pMarkerSymbol->Serialize(data);
        }
        else
        {
            data << false;
        }

        unsigned nSize = m_stepArry.size();
        data << nSize;

        //读取数据
        data.WriteArry(&m_stepArry[0], nSize);
    }

    void CMarkerLineLayer::Deserialize(CByte& data)
    {
        CLineLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pMarkerSymbol = std::make_unique<CMarkerSymbol>();
            m_pMarkerSymbol->Deserialize(data);
        }

        unsigned nSize;
        data >> nSize;
        m_stepArry.resize(nSize);

        //读取数据
        data.ReadArry(&m_stepArry[0], nSize);

    }

    size_t CMarkerLineLayer::GetSize()
    {
        size_t nSize = 1 + CLineLayer::GetSize();
        if (m_pMarkerSymbol)
        {
            nSize += m_pMarkerSymbol->GetSize();
        }

        nSize += 4;
        nSize += DOUBLE_SIZE  * m_stepArry.size();

        return nSize;
    }

    CMarkerSymbol* CMarkerLineLayer::GetMarkerSymbol()
    {
        return m_pMarkerSymbol.get();
    }

    void CMarkerLineLayer::SetMarkerSymbol(std::unique_ptr<CMarkerSymbol> pColor)
    {
        m_pMarkerSymbol = std::move(pColor);
    }

    void CMarkerLineLayer::AddStep(const double& dStep)
    {
        m_stepArry.push_back(dStep);
    }
   

    void CMarkerLineLayer::RemoveStep(const unsigned& index)
    {
        if (m_stepArry.size() > index)
        {
            m_stepArry.erase(m_stepArry.begin() + index);
            
        }
    }

    std::vector<double> CMarkerLineLayer::GetStep()
    {
        return m_stepArry;
    }

}




