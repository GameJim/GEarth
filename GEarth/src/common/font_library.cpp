#include "common/font_library.h"
#include "freetype/freetype.h"

namespace COMMON_NAMESPACE
{
    CFontLibrary::CFontLibrary(const std::string& sPath)
        :m_sPath(sPath)
    {

    }


  

    CFont::CFont(const std::string& context, std::unique_ptr<CFontLibrary> fontLibrary)
        : m_sContext(context), m_pFontLibrary(std::move(fontLibrary)), m_enableUnderline(false), m_fSize(11.0), m_fSpacing(0.5)
    {

    }

    CFontLibrary& CFont::GetFontLibrary()
    {
        return *m_pFontLibrary.get();
    }

    CFont::~CFont()
    {

    }
}

