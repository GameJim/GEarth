#include "SymbolSystem/Symbol.h"
#include "Common/register.h"


Symbol::EnSymbolType Symbol::SymbolType() const
{
	return m_eSymbolType;
}


void Symbol::SetProperty(SymbolProperty* pProperty)
{
	if (!pProperty)
		return;
	
	if (m_pProperty)
	{
		delete m_pProperty;
	}
	m_pProperty = pProperty;
}

SymbolProperty* Symbol::Property() const
{
	return m_pProperty;
}

void Symbol::Serialize(CByte& data)
{
	data<< m_eSymbolType;
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

void Symbol::Deserialize(CByte& data)
{
	uint8_t nType;
	data >> nType;
	m_eSymbolType = (EnSymbolType)nType;
	bool hasObject = false;
	data >> hasObject;
	if (hasObject)
	{
		//获取文件名称
		std::string sClassName;
		data >> sClassName;
		//获取对应的类型
		m_pProperty = static_cast<SymbolProperty*>(ClassFactory::get_instance(sClassName.c_str()));
		//序列化初始化
		m_pProperty->Serialize(data);
	}
}

size_t Symbol::GetSize()
{
	size_t size = 0;
	size = 1 + 1; //类型、是否存在属性
	if (m_pProperty)
	{
		size += m_pProperty->GetSize();
	}

	return size;
}

Symbol::Symbol(const EnSymbolType& symbolType)
	:m_eSymbolType(symbolType),m_pProperty(nullptr)
{

}

Symbol::~Symbol()
{
	if (m_pProperty)
	{
		delete m_pProperty;
	}
	m_pProperty = nullptr;
}