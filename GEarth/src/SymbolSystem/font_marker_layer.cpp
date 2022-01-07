#include "SymbolSystem/font_marker_layer.h"

namespace symbol
{
    CFontMarkerLayer::CFontMarkerLayer(std::shared_ptr<CFont> pFont)
        :CMarkerLayer(SYMBOL_TYPE_FONT)
        , m_pFont(pFont)
    {

    }

    CFontMarkerLayer::CFontMarkerLayer()
        :CMarkerLayer(SYMBOL_TYPE_FONT)
        , m_pFont(nullptr)
    {

    };

}


