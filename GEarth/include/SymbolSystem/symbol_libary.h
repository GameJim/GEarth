/*!
*@File    symbol_libary.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once
#include "common/object.h"
#include "common/image.h"
#include "Common/object_manager.h"
#include "SymbolSystem/symbol.h"
#include <string>
#include <memory>
namespace symbol
{
    class CORE_PUBLIC CSymbolLibary : public CObject
    {
    public:
        CSymbolLibary();
        CSymbolLibary(const std::string& sPath);
        virtual ~CSymbolLibary();

        bool Save2File(const std::string& sPath);

        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
        virtual void Serialize(CByte& data);

        /*!
        /* @Brief:     反序列化
        /* @Date:      2021/12/22
        /* @Parameter: CByte & data
        /* @Return     void
        */
        virtual void Deserialize(CByte& data);

        /*!
        /* @Brief:     获取字节大小
        /* @Date:      2021/12/22
        /* @Return     size_t
        */
        virtual size_t GetSize();


        virtual std::string GetName();

        virtual void SetName(const std::string& sName);

        virtual void AddSymbol(std::unique_ptr<CSymbol> pSymbol);
    public:

        /*!
        /* @Brief:     从文件中导入符号库
        /* @Date:      2021/12/30
        /* @Parameter: const std::string & sPath
        /* @Return     bool
        */
        bool LoadFromFile(const std::string& sPath);

    protected:
        std::string m_sName; //符号库的名称
        std::string m_sVer;  //数据版本
        std::vector<std::unique_ptr<CSymbol>>*  m_pSymbols; //符号组
    };
}



