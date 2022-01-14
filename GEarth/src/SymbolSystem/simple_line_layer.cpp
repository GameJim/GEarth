#include "SymbolSystem/simple_line_layer.h"
#include "common/type_size.h"
namespace symbol
{

    CSimpleLineLayer::CSimpleLineLayer()
        :CLineLayer(CLineLayer::EnLineLayerType::LINE_LAYER_TYPE_SIMPLE)
        , m_pColor(std::make_unique<CColor>())
    {

    }

    void CSimpleLineLayer::Serialize(CByte& data)
    {
        CLineLayer::Serialize(data);

        if (m_pColor)
        {
            data << true;
            m_pColor->Serialize(data);
        }
        else
        {
            data << false;
        }

        unsigned nSize = m_Dasharray.size();
        data << nSize;
       
        //读取数据
        data.WriteArry(&m_Dasharray[0], nSize);
    }

    void CSimpleLineLayer::Deserialize(CByte& data)
    {
        CLineLayer::Deserialize(data);

        bool hasObject;
        data >> hasObject;
        if (hasObject)
        {
            m_pColor = std::make_unique<CColor>();
            m_pColor->Deserialize(data);
        }

        unsigned nSize;
        data >> nSize;
        m_Dasharray.resize(nSize);

        //读取数据
        data.ReadArry(&m_Dasharray[0], nSize);

    }

    size_t CSimpleLineLayer::GetSize()
    {
        size_t nSize = 1 + CLineLayer::GetSize();
        if (m_pColor)
        {
            nSize += m_pColor->GetSize();
        }

        nSize += 4;
        nSize += DOUBLE_SIZE  * m_Dasharray.size();

        return nSize;
    }

    CColor* CSimpleLineLayer::GetColor()
    {
        return m_pColor.get();
    }

    void CSimpleLineLayer::SetColor(std::unique_ptr<CColor> pColor)
    {
        m_pColor = std::move(pColor);
    }

    void CSimpleLineLayer::AddDash(const double& solid, const double& dash /*= 0.0*/)
    {
        m_Dasharray.push_back(solid);
        m_Dasharray.push_back(dash);
    }

    void CSimpleLineLayer::RemoveDash(const unsigned& index)
    {
        if (m_Dasharray.size()/2 > index)
        {
            m_Dasharray.erase(m_Dasharray.begin() + 2 * index + 1);
            m_Dasharray.erase(m_Dasharray.begin() + 2 * index );
        }
    }

    std::vector<double> CSimpleLineLayer::GetDash()
    {
        return m_Dasharray;
    }

}

