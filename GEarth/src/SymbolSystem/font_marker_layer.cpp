#include "SymbolSystem/font_marker_layer.h"
#include <common/font_factory.h>
namespace symbol
{

    CFontMarkerLayer::CFontMarkerLayer(CFont& pFont)
        :CMarkerLayer(SYMBOL_TYPE_FONT),m_pFont(pFont)
    {

    }

    CFontMarkerLayer::CFontMarkerLayer()
        : CMarkerLayer(SYMBOL_TYPE_FONT), m_pFont(CFontFactory::GetInstance().CreateDefaultFont())
    {

    }

    CBox3d CFontMarkerLayer::GetBoundBox()
    {
        CBox3d box;


        return box;
    }

    void CFontMarkerLayer::Serialize(CByte& data)
    {
        m_pFont.Serialize(data);
    }

    void CFontMarkerLayer::Deserialize(CByte& data)
    {
        m_pFont.Deserialize(data);
    }

    size_t CFontMarkerLayer::GetSize()
    {
        return m_pFont.GetSize();
    }

    CFont& CFontMarkerLayer::GetFont()
    {
        return m_pFont;
    }

    CFontMarkerLayer::~CFontMarkerLayer()
    {
      //从字体库总移除字体
        m_pFont.Free();
    }

}


