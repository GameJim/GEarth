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
        CFontMarkerLayer() = delete;
        ~CFontMarkerLayer();
        CFontMarkerLayer(CFont& pFont);
    protected:
        CFont&  m_pFont;
    };

}

