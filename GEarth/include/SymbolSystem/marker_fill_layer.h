/*!
*@File    marker_fill_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/14
*/
#pragma once


#include "SymbolSystem/fill_layer.h"
#include "SymbolSystem/marker_symbol.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CMarkerFillLayer : public CFillLayer
    {
    public:

        CMarkerFillLayer();
        virtual ~CMarkerFillLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CMarkerSymbol* GetMarkerSymbol();
        virtual void SetMarkerSymbol(std::unique_ptr<CMarkerSymbol> pColor);

        virtual void SetStep(const CVec3d& step);
        virtual CVec3d GetStep();

        virtual void SetXDirection(const CVec3f& vec);
        virtual CVec3f GetXDirection();
    protected:
        std::unique_ptr<CMarkerSymbol> m_pMarkerSymbol;
        CVec3d   m_Step;                   //每间距多少插入符号
        CVec3f   m_Direction;              //符号朝向，即旋转
    };

}


