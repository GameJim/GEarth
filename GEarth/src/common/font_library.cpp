#include "common/font_library.h"
#include "freetype/freetype.h"

namespace COMMON_NAMESPACE
{
    CFontLibrary::CFontLibrary(const std::string& sPath)
        :m_sPath(sPath)
    {
        //初始化字体
        FT_Library library;
        FT_Error Error = FT_Init_FreeType(&library);

        //加载第n个字体库的字体字体外观
        FT_Face face;
        FT_Error error = FT_New_Face(library,
            sPath.c_str(),
            0,
            &face);
        if (error == FT_Err_Unknown_File_Format)
        {

        }
        else if (error)
        {

        }


        /*error = FT_New_Memory_Face(library,
        buffer,
        size,
        0,
        &face);
        if (error) { ... }*/
    }

    std::shared_ptr<CFont> CFontLibrary::CreateFont(const std::string& sContext)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        std::shared_ptr<CFont> pFont = std::make_shared<CFont>(sContext);
        m_fonts.push_back(pFont);

        return pFont;
    }

    void CFontLibrary::ClearUnrefFonts()
    {
        return;
    }

    void CFontLibrary::RemoveFont(const std::shared_ptr<CFont>& pFont)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        //是否为2这个值待定
        //if (pFont.use_count()==2 && m_fonts.(pFont) != m_fonts.end())
        //{
        //    //移除某个字体
        //    m_fonts.erase(pFont);
        //}
    }

    CFont::CFont(const std::string& context)
        :m_sContext(context)
    {

    }

    CFont::~CFont()
    {

    }
}

