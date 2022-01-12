#include "symbolsystem/symbol_libary.h"
#include "SymbolSystem/marker_symbol.h"
#include "SymbolSystem/line_symbol.h"
#include "SymbolSystem/fill_symbol.h"
#include "SymbolSystem/animation_symbol.h"
#include "SymbolSystem/custom_symbol.h"
#include <iostream>
#include <fstream>
#include "common/log.h"
#include "common/type_size.h"
namespace symbol
{
    CSymbolLibary::CSymbolLibary()
        :m_sVer("0.0"), m_sName("UnNamed")
    {
        m_pSymbols = new std::vector<std::unique_ptr<CSymbol>>();
    }

    CSymbolLibary::~CSymbolLibary()
    {
        delete m_pSymbols;
    }

    CSymbolLibary::CSymbolLibary(const std::string& sPath)
    {
        LoadFromFile(sPath);
    }

    bool CSymbolLibary::LoadFromFile(const std::string& sPath)
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
            file.clear();
            file.seekg(0, std::ios::end);//文件指针指向结尾

            nSize = file.tellg();
            CByte byte(nSize);
            file.seekg(0, std::ios::beg);//文件指针指向头
            file.read((char*)byte.Data(), nSize);

            //构造结果
            this->Deserialize(byte);
        }
        return true;
    }

    bool CSymbolLibary::Save2File(const std::string& sPath)
    {
        std::ofstream file(sPath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open())
        {
            LOG_ERROR("File:" + sPath + " failed to save");
            return false;
        }
        else
        {
            size_t nSize = GetSize();

            CByte byte(nSize);
            //序列化数据
            this->Serialize(byte);
            file.write((char*)byte.Data(), nSize);
            file.close();
        }

        return true;
    }

   
    void CSymbolLibary::Serialize(CByte& data)
    {
        data << m_sVer << m_sName;

        unsigned nChild = m_pSymbols->size();
        data << nChild;
        for (auto& pSymbol : *m_pSymbols)
        {
            uint8_t nType = pSymbol->SymbolType();
            data << nType;
            pSymbol->Serialize(data);
        }
    }


    void CSymbolLibary::Deserialize(CByte& data)
    {
        data >> m_sVer >> m_sName;

        unsigned nChild;
        data >> nChild;
        m_pSymbols->reserve(nChild);
        for (unsigned i = 0; i < nChild; ++i)
        {
            uint8_t nType;
            data >> nType;
            CSymbol::EnSymbolType eSymbolType = (CSymbol::EnSymbolType)nType;
          
            std::unique_ptr<CSymbol> pSymbol;
            switch (eSymbolType)
            {
            case symbol::CSymbol::SYMBOL_TYPE_MARKER:
                pSymbol = std::make_unique<CMarkerSymbol>();
                break;
            case symbol::CSymbol::SYMBOL_TYPE_LINE:
                pSymbol = std::make_unique<CLineSymbol>();
                break;
            case symbol::CSymbol::SYMBOL_TYPE_FILL:
                pSymbol = std::make_unique<CFillSymbol>();
                break;
            case symbol::CSymbol::SYMBOL_TYPE_ANIMATION:
                pSymbol = std::make_unique<CAnimationSymbol>();
                break;
            case symbol::CSymbol::SYMBOL_TYPE_CUSTOM:
            {
                std::string sClassName;
                data >> sClassName;
                CSymbol* pTemp = static_cast<CSymbol*>(ClassFactory::get_instance(sClassName.c_str()));
                pSymbol.reset(pTemp);
            }
            break;
            default:
                break;
            }
            pSymbol->Deserialize(data);
            m_pSymbols->push_back(std::move(pSymbol));
        }
    }

    size_t CSymbolLibary::GetSize()
    {
        //字符串和符号
        size_t nSize = CTypeSize::Instance().GetSize(m_sName) + CTypeSize::Instance().GetSize(m_sVer);
        nSize = nSize + 4 + m_pSymbols->size();
        for (auto& symbol : *m_pSymbols)
        {
            nSize += symbol->GetSize();
        }

        return nSize;
    }

    std::string CSymbolLibary::GetName()
    {
        return m_sName;
    }

    void CSymbolLibary::SetName(const std::string& sName)
    {
        m_sName = sName;
    }

    void CSymbolLibary::AddSymbol(std::unique_ptr<CSymbol> pSymbol)
    {
        m_pSymbols->push_back(std::move(pSymbol));
    }

}
