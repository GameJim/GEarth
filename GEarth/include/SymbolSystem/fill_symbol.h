/*!
*@File    fill_symbol.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once
#include "Common/register.h"
#include "SymbolSystem/Symbol.h"
#include "SymbolSystem/fill_layer.h"
#include <vector>

namespace symbol
{
    class CFillSymbol : public CSymbol
    {
    public:
        CFillSymbol();
        virtual ~CFillSymbol();
    public:

        void AddFillLayer(std::unique_ptr<CFillLayer> pFillLayer);
        CFillLayer* GetFillLayer(const unsigned& index);
        void RemoveFillLayer(CFillLayer* pFillLayer);
        void RemoveFillLayer(const unsigned& index);

        virtual void Serialize(CByte& data) override;

        virtual void Deserialize(CByte& data) override;

        virtual size_t GetSize() override;
    protected:

        std::vector<std::unique_ptr<CFillLayer>>*  m_pLayers;   //图层
    };

}


