/*!
*@File    line_symbol.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once

#include "SymbolSystem/Symbol.h"
#include "SymbolSystem/line_layer.h"
#include <vector>

namespace symbol
{
    class CLineSymbol : public CSymbol
    {
    
    public:
        CLineSymbol();
        virtual ~CLineSymbol();

        void AddLineLayer(std::unique_ptr<CLineLayer> pLineLayer);
        CLineLayer* GetLineLayer(const unsigned& index);
        void RemoveLineLayer(CLineLayer* pLineLayer);
        void RemoveLineLayer(const unsigned& index);

        virtual void Serialize(CByte& data) override;

        virtual void Deserialize(CByte& data) override;

        virtual size_t GetSize() override;
    protected:

        std::vector<std::unique_ptr<CLineLayer>>*  m_pLayers;   //图层
    };
}






