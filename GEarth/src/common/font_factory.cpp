#include "common/font_factory.h"


namespace COMMON_NAMESPACE
{
    CFont& CFontFactory::CreatFont(const std::string& sPath, const std::string sContext)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        std::unique_ptr<CFontLibrary> pFontLibrary = std::make_unique<CFontLibrary>(sPath);

        std::unique_ptr<CFont> pFont = std::make_unique<CFont>(sContext, std::move(pFontLibrary));

        // 
        CFont& font = *pFont.get();
        uint64_t id = uint64_t(pFont.get());
        m_pFonts.insert({ id, std::move(pFont) });

        return font;
    }

    CFont& CFontFactory::CreatFont(const std::string sContext,std::unique_ptr<CFontLibrary> pFontLibrary)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);

        std::unique_ptr<CFont> pFont = std::make_unique<CFont>(sContext, std::move(pFontLibrary));

        // 
        CFont& font = *pFont.get();
        uint64_t id = uint64_t(pFont.get());
        m_pFonts.insert({ id, std::move(pFont) });

        return font;
    }

    bool CFontFactory::RemoveFont(CFont& font)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        uint64_t id = uint64_t(&font);

        if (m_pFonts.find(id) != m_pFonts.end())
        {
            m_pFonts.erase(m_pFonts.find(id));
            return true;
        }
       

        return false;
    }

    CFont& CFontFactory::CreateDefaultFont()
    {
        std::string sPath = "";
        std::string sContext = "GEarth";

        std::unique_lock<std::mutex> lock(m_Mutex);
        std::unique_ptr<CFontLibrary> pFontLibrary = std::make_unique<CFontLibrary>(sPath);

        std::unique_ptr<CFont> pFont = std::make_unique<CFont>(sContext, std::move(pFontLibrary));

        // 
        CFont& font = *pFont.get();
        uint64_t id = uint64_t(pFont.get());
        m_pFonts.insert({ id, std::move(pFont) });

        return font;
    }

}
