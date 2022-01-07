/*!
*@File    font_marker_layer.h
*@Brief   字体库
*@Date    2021/12/30
*/
#pragma once
#include "SymbolSystem/marker_layer.h"
#include "common/font_library.h"

namespace symbol
{
    class CFontMarkerLayer : public CMarkerLayer
    {
    public:
        CFontMarkerLayer();
        ~CFontMarkerLayer() = default;
        CFontMarkerLayer(std::shared_ptr<CFont> pFont);
    protected:
        std::shared_ptr<CFont>  m_pFont;
    };

}

