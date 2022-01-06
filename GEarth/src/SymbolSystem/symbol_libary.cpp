#include "symbolsystem/symbol_libary.h"
#include <iostream>
#include <fstream>
#include "Common/Log.h"

SymbolLibary::SymbolLibary()
	:m_sVer("0.0"),m_sName("UnNamed")
{

}

SymbolLibary::SymbolLibary(const std::string& sPath)
{
    LoadFromFile(sPath);
}

bool SymbolLibary::LoadFromFile(const std::string& sPath)
{
	std::ifstream file(sPath.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		LOG_ERROR("File:" + sPath + " failed to load");
		return false;
	}
	else
	{
		//获取字节大小
		unsigned nSize;
		file.seekg(0, std::ios::beg);//文件指针指向结尾

		nSize = file.tellg();
		CByte byte(nSize);
		file.read((char*)byte.Data(), nSize);

		//构造结果
		this->Deserialize(byte);
	}
    return true;
}

bool SymbolLibary::Save2File(const std::string& sPath)
{
	std::ofstream file(sPath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open())
	{
		LOG_ERROR("File:" + sPath + " failed to save");
		return false;
	}
	else
	{
		unsigned nSize = GetSize();
		
		CByte byte(nSize);
		//序列化数据
		this->Serialize(byte);
		file.write((char*)byte.Data(), nSize);
		file.close();
	}

	return true;
}

////处理符号
//#define SERIALIZE_VEC_SHARE(data,vec)   \
//{                                       \
//	unsigned nChild = vec.size();       \
//	data << nChild;                     \
//	for(auto child : vec)               \
//	{                                   \
//		child->Serialize(data);         \
//	}                                   \
//}
//	

void SymbolLibary::Serialize(CByte& data)
{
	data << m_sVer;
    data << m_sName;

    unsigned nChild = m_pSymbols.size();
    data << nChild;
    for (auto pSymbol : m_pSymbols)
    {
        pSymbol->Serialize(data);
    }

    
}

////处理符号
//#define DESERIALIZE_VEC_SHARE(data,vec,class_name)   \
//	{                                            \
//		unsigned nChild;                        \
//		std::string sClassName;                 \
//		data >> nChild;                         \
//		vec.reserve(nChild);                 \
//		for (int i = 0; i < nChild; ++i)       \
//		{                                      \
//		data >> sClassName;                 \
//		assert(sClassName == #class_name);  \
//		class_name* pTemp = static_cast<class_name*>(ClassFactory::get_instance(sClassName.c_str()));  \
//		std::shared_ptr<class_name> pChild;   \
//		pChild.reset(pTemp);                  \
//		pChild->Deserialize(data);                 \
//		vec.push_back(pChild);                  \
//		}                                       \
//	}


void SymbolLibary::Deserialize(CByte& data)
{
	data >> m_sVer;
    data >> m_sName;
	
    unsigned nChild;
    std::string sClassName;
    data >> nChild;
    m_pSymbols.reserve(nChild);
    for (int i = 0; i < nChild; ++i)
    {
        data >> sClassName;
        Symbol* pTemp = static_cast<Symbol*>(ClassFactory::get_instance(sClassName.c_str()));
        std::shared_ptr<Symbol> pChild(pTemp);
       
        pChild->Deserialize(data);
        m_pSymbols.push_back(pChild);
    }
}

size_t SymbolLibary::GetSize()
{
	//字符串和符号
	unsigned nSize = 4 + m_sVer.size() + 4 + m_sName.size();
	nSize += 4;
	for (auto symbol: m_pSymbols)
	{
		nSize += symbol->GetSize();
	}


	return nSize;
}

std::string SymbolLibary::GetName()
{
    return m_sName;
}

void SymbolLibary::SetName(const std::string& sName)
{
    m_sName = sName;
}
