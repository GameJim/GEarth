/*!
*@File    marker_line_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/13
*/
#pragma once

#include "SymbolSystem/line_layer.h"
#include "SymbolSystem/marker_symbol.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CMarkerLineLayer : public CLineLayer
    {
    public:

        CMarkerLineLayer();
        virtual ~CMarkerLineLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CMarkerSymbol* GetMarkerSymbol();
        virtual void SetMarkerSymbol(std::unique_ptr<CMarkerSymbol> pColor);

        virtual void AddStep(const double& dStep);
        virtual void RemoveStep(const unsigned& index);
        virtual std::vector<double> GetStep();

    protected:
        std::unique_ptr<CMarkerSymbol> m_pMarkerSymbol;
        std::vector<double> m_stepArry;  //每间距多少个像素一个点符号,如果为0则平铺
    };

}


