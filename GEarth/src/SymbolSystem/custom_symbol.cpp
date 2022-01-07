#include "symbolSystem/custom_symbol.h"

namespace symbol
{
    REGISTER_OBJECT(CCustomSymbol)

        CCustomSymbol::CCustomSymbol()
        :Symbol(Symbol::EnSymbolType::SYMBOL_TYPE_CUSTOM)
    {

    }
}



