/*!
*@File    symbol_factory.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once

#include "common/factory.h"
#include "SymbolSystem/marker_symbol.h"

namespace symbol
{
    class CORE_PUBLIC CSymbolFactory : public CFactory<CSymbolFactory>
    {
    public:
        std::unique_ptr<CMarkerSymbol> GetDefaultMarkerSymbol();
    };

}



