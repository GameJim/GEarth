#include "symbolSystem/animation_symbol.h"

namespace symbol
{
    REGISTER_OBJECT(CAnimationSymbol)

        CAnimationSymbol::CAnimationSymbol()
        :Symbol(Symbol::EnSymbolType::SYMBOL_TYPE_ANIMATION)
    {

    }
}
