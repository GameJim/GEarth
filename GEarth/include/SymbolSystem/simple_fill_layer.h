/*!
*@File    simple_fill_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/14
*/
#pragma once



#include "SymbolSystem/fill_layer.h"
#include "common/color.h"
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CSimpleFillLayer : public CFillLayer
    {
    public:

        CSimpleFillLayer();
        virtual ~CSimpleFillLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CColor* GetColor();
        virtual void SetColor(std::unique_ptr<CColor> pColor);

    protected:
        std::unique_ptr<CColor> m_pColor;
      
    };

}

