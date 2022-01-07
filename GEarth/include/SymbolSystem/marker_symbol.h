/*!
*@File    marker_symbol.h
*@Brief   点符号
*@Date    2021/12/22
*/
#pragma once
#include "common/register.h"
#include "common/unit.h"
#include "SymbolSystem/Symbol.h"
#include "SymbolSystem/marker_layer.h"
#include <vector>


namespace symbol
{
    class CMarkerSymbol : public Symbol
    {
        REGISTER_CLASS(CMarkerSymbol);
    public:
        using CFontSize = CUnitValue<float>;                     //符号大小
        CMarkerSymbol();
        virtual ~CMarkerSymbol();

        void AddMarkerLayer(CMarkerLayer* pMarkerLayer);
        CMarkerLayer* GetMarkerLayer(const unsigned& index);
        void RemoveMarkerLayer(CMarkerLayer* pMarkerLayer);
        void RemoveMarkerLayer(const unsigned& index);

        //获取符号包围盒，此处值为像素坐标下的值
        virtual CBox3f GetBoundBox();
    protected:
        std::vector<CMarkerLayer*>  m_pLayers;   //图层
        CFontSize m_fontSize;                    //符号大小
    };
}



