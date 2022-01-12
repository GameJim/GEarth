/*!
*@File    font_factory.h
*@Brief   管理字体库
*@Date    2022/1/7
*/
#pragma once
#include "common/factory.h"
#include "font_library.h"
#include <memory>
#include <unordered_map>
namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CFontFactory : public CFactory<CFontFactory>
    {
    public:
        CFont& CreatFont(const std::string& sPath,const std::string sContext);
        CFont& CreatFont(const std::string sContext, std::unique_ptr<CFontLibrary> pFontLibrary);

        bool RemoveFont(CFont& font);

        CFont& CreateDefaultFont();
    protected:
        std::unordered_map<uint64_t, std::unique_ptr<CFont>>  m_pFonts;
    };

}



