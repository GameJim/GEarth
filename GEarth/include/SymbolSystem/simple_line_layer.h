/*!
*@File    simple_line_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/13
*/
#pragma once
#include "SymbolSystem/line_layer.h"
#include "common/color.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CSimpleLineLayer : public CLineLayer
    {
    public:
      
        CSimpleLineLayer();
        virtual ~CSimpleLineLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CColor* GetColor();
        virtual void SetColor(std::unique_ptr<CColor> pColor);

        virtual void AddDash(const double& solid, const double& dash = 0.0);
        virtual void RemoveDash(const unsigned& index);
        virtual std::vector<double> GetDash();
    protected:
        std::unique_ptr<CColor> m_pColor;
        std::vector<double> m_Dasharray;//长度是基于像素
    };

}
