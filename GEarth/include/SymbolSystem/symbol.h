#pragma once
#include <string>
#include "common/register.h"
#include "common/object.h"
#include <memory>
/*!
*@class   Symbol
*@Brief   符号属性类，基类
*@Date    2021/12/21
*/

namespace symbol
{
    using namespace common;
    class CORE_PUBLIC CSymbolProperty : public CObject
    {
        REGISTER_CLASS(CSymbolProperty);
    public:
   
        virtual ~CSymbolProperty() = default;

        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
        virtual void Serialize(CByte& data) ;

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
        CSymbolProperty() = default;
    protected:
        std::string m_sName = "";  //符号名称
    };


    /*!
    *@class   Symbol
    *@Brief   符号的基类
    *@Date    2021/12/21
    */

    class CORE_PUBLIC CSymbol : public CObject
    {
    public:
        virtual ~CSymbol();
        enum EnSymbolType : uint8_t
        {
            SYMBOL_TYPE_MARKER = 0,     //点符号
            SYMBOL_TYPE_LINE = 1,       //线符号
            SYMBOL_TYPE_FILL = 2,       //填充符号 
            SYMBOL_TYPE_ANIMATION = 3,  //动画符号（动态符号）
            SYMBOL_TYPE_CUSTOM          //自定义符号
        };

        /*!
        /* @Brief:     符号类型
        /* @Date:      2021/12/21
        /* @Return     Symbol::EnSymbolType
        */
        EnSymbolType SymbolType() const;


        /*!
        /* @Brief:     添加符号属性
        /* @Date:      2021/12/21
        /* @Parameter: SymbolProperty * pProperty
        /* @Return     void
        */
        void SetProperty(std::unique_ptr<CSymbolProperty> pProperty);


        CSymbolProperty* Property() const;
        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
        virtual void Serialize(CByte& data);    //序列化

                                                /*!
                                                /* @Brief:     反序列化
                                                /* @Date:      2021/12/21
                                                /* @Parameter: CByte * data
                                                /* @Return     void
                                                */
        virtual void Deserialize(CByte& data);  //反序列化

                                                /*!
                                                /* @Brief:     获取大小
                                                /* @Date:      2021/12/21
                                                /* @Return     size_t
                                                */
        virtual size_t GetSize();                       // 文件的总字节数
       
    public:
       
    protected:
        CSymbol(const EnSymbolType& symbolType);
        EnSymbolType m_eSymbolType;                   //符号类型
        std::unique_ptr<CSymbolProperty> m_pProperty;
    };

}

