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
#include <memory>

namespace symbol
{
    class CMarkerSymbol : public CSymbol
    {
       
    public:
        using CSymbolSize = CUnitValue<float>;                     //符号大小
        CMarkerSymbol();
        virtual ~CMarkerSymbol();

        void AddMarkerLayer(std::unique_ptr<CMarkerLayer> pMarkerLayer);
        CMarkerLayer* GetMarkerLayer(const unsigned& index);
        void RemoveMarkerLayer(CMarkerLayer* pMarkerLayer);
        void RemoveMarkerLayer(const unsigned& index);

        //获取符号包围盒，此处值为像素坐标下的值
        virtual CBox3d GetBoundBox();

        
        virtual void Serialize(CByte& data) override;
      
        virtual void Deserialize(CByte& data) override;

        virtual size_t GetSize() override;
    protected:
        std::vector<std::unique_ptr<CMarkerLayer>>*  m_pLayers;   //图层
        CSymbolSize m_symbolSize;                    //符号大小
    };
}



