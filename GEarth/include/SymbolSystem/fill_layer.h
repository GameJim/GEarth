/*!
*@File    fill_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/14
*/
#pragma once
#include "common/object.h"
#include "common/nonCopyable.h"
#include "SymbolSystem/line_symbol.h"
namespace symbol
{
    using namespace common;
    class CORE_PUBLIC CFillLayer : public CObject, public CNonCopyable
    {
    public:
        
        enum EnFillLayerType : uint8_t
        {
            Fill_LAYER_TYPE_SIMPLE = 0,   //简单线型
            Fill_LAYER_TYPE_MARKER = 1,   //点符号填充
            Fill_LAYER_TYPE_LINE = 2,     //线
            Fill_LAYER_TYPE_IMAGE = 3,     //纹理填充
            Fill_LAYER_TYPE_CUSTOM        //自定义
        };

       
        virtual ~CFillLayer() = default;

        EnFillLayerType GetFillLayerType() const;

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

        virtual bool IsEnable() const;

        virtual void SetEnable(const bool& enable);

        virtual void SetLineSymbol(std::unique_ptr<CLineSymbol> pLineSymbol);
        virtual CLineSymbol* GetLineSymbol();
    protected:
        CFillLayer(const EnFillLayerType& type);

        const EnFillLayerType m_eFillLayerType;
        //关键数据
        bool   m_IsEnable;                     //数据是否启用
    
        std::unique_ptr<CLineSymbol> m_pLineSymbol;   //轮廓样式
    };
}