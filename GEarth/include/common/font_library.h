/*!
*@File    font_library.h
*@Brief   文件描述:下次一定
*@Date    2021/12/31
*/
#pragma once
#include "common/apiDescription.h"
#include <memory>
#include <list>

namespace COMMON_NAMESPACE
{
    class  CFont;
    class  CFontLibrary;
    class CFontFactory;

    class CORE_PUBLIC CFontLibrary
    {
        friend CFont;
        friend CFontFactory;
   
        CFontLibrary(const std::string& sPath);
    public:
        CFontLibrary() = delete;
        virtual ~CFontLibrary() = default;
       
    protected:
        std::string m_sPath;
    };


    // std::enable_shared_from_this
    class CORE_PUBLIC CFont
    {
    public:
        friend CFontFactory;
    public:
        virtual ~CFont();
        
        CFontLibrary& GetFontLibrary();


        CFont(const std::string& context, std::unique_ptr<CFontLibrary> fontLibrary);
    protected:
        std::string m_sContext;         //文本内容
        bool m_enableUnderline;        //是否有下划线
        float m_fSize;                 //字体大小
        float m_fSpacing;              //间距
        
        std::unique_ptr<CFontLibrary> m_pFontLibrary;
    private:
    };
}







