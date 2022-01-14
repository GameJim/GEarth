/*!
*@File    line_fill_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/14
*/
#pragma once
#include "SymbolSystem/fill_layer.h"
#include "SymbolSystem/line_symbol.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CLineFillLayer : public CFillLayer
    {
    public:

        CLineFillLayer();
        virtual ~CLineFillLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CLineSymbol* GetLineSymbol();
        virtual void SetLineSymbol(std::unique_ptr<CLineSymbol> pColor);

        virtual void SetStep(const CVec3d& step);
        virtual CVec3d GetStep();

        virtual void SetXDirection(const CVec3f& vec);
        virtual CVec3f GetXDirection();
    protected:
        std::unique_ptr<CLineSymbol> m_pLineSymbol;
        CVec3d   m_Step;                   //每间距多少插入一条线
        CVec3f   m_Direction;              //符号朝向，即旋转
    };

}




