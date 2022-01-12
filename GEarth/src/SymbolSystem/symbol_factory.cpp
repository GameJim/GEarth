#include "symbolsystem/symbol_factory.h"
#include "symbolsystem/font_marker_layer.h"
#include "symbolsystem/image_marker_layer.h"
#include "symbolsystem/shape_marker_layer.h"
#include "common/font_factory.h"
namespace symbol
{
    using namespace common;
    std::unique_ptr<CMarkerSymbol> CSymbolFactory::GetDefaultMarkerSymbol()
    {
        std::unique_ptr<CMarkerSymbol> pMarkerSymbol = std::make_unique<CMarkerSymbol>();

        //
        std::unique_ptr<CFontMarkerLayer> pFontMarkerLayer = std::make_unique<CFontMarkerLayer>();
        pMarkerSymbol->AddMarkerLayer(std::move(pFontMarkerLayer));

        std::unique_ptr<CImageMarkerLayer> pImageMarkerLayer = std::make_unique<CImageMarkerLayer>();
        pMarkerSymbol->AddMarkerLayer(std::move(pImageMarkerLayer));

        std::unique_ptr<CShapeMarkerLayer> pShapeMarkerLayer = std::make_unique<CShapeMarkerLayer>();
        pMarkerSymbol->AddMarkerLayer(std::move(pShapeMarkerLayer));

        return std::move(pMarkerSymbol);
    }

}
