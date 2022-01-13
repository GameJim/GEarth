#include "SymbolSystem/Symbol.h"
#include "Common/register.h"
#include "common/type_size.h"
namespace symbol
{
    REGISTER_OBJECT(CSymbolProperty)

    CSymbol::EnSymbolType CSymbol::SymbolType() const
    {
        return m_eSymbolType;
    }


    void CSymbol::SetProperty(std::unique_ptr<CSymbolProperty> pProperty)
    {
        m_pProperty = std::move(pProperty);
    }

    CSymbolProperty* CSymbol::Property() const
    {
        return m_pProperty.get();
    }

    void CSymbol::Serialize(CByte& data)
    {
        data << m_MaxOpacity;
        bool hasObject = false;
        if (m_pProperty)
        {
            hasObject = true;
            data << hasObject;
            m_pProperty->Serialize(data);
        }
        else
        {
            data << hasObject;
        }
    }

    void CSymbol::Deserialize(CByte& data)
    {

        bool hasObject = false;
        data >> m_MaxOpacity >>  hasObject;
        if (hasObject)
        {
            //获取文件名称
            std::string sClassName;
            data >> sClassName;
            //获取对应的类型
            CSymbolProperty* pSymbolProperty = static_cast<CSymbolProperty*>(ClassFactory::get_instance(sClassName.c_str()));
            //序列化初始化
            pSymbolProperty->Deserialize(data);
            m_pProperty.reset(pSymbolProperty);
        }
    }

    size_t CSymbol::GetSize()
    {
        size_t size = 0;
        size = 1 + FLOAT_SIZE; //类型、是否存在属性
        if (m_pProperty)
        {
            size += m_pProperty->GetSize();
        }

        return size;
    }

    void CSymbol::SetMaxOpacity(const float& fOpacity)
    {
        m_MaxOpacity = fOpacity;
        m_MaxOpacity = m_MaxOpacity < 0.0f ? 0.0f : m_MaxOpacity;
        m_MaxOpacity = m_MaxOpacity > 1.0f ? 1.0f : m_MaxOpacity;

    }

    float CSymbol::GetMaxOpacity() const
    {
        return m_MaxOpacity;
    }

    CSymbol::CSymbol(const EnSymbolType& symbolType)
        :m_eSymbolType(symbolType), m_pProperty(std::make_unique<CSymbolProperty>())
        , m_MaxOpacity(1.0f)
    {

    }

    CSymbol::~CSymbol()
    {

        m_pProperty = nullptr;
    }

    void CSymbolProperty::Serialize(CByte& data)
    {
        data << m_sClassName << m_sName;
    }

    void CSymbolProperty::Deserialize(CByte& data)
    {
        data >> m_sName;
    }

    size_t CSymbolProperty::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_sName) + CTypeSize::Instance().GetSize(m_sClassName);
    }

}
