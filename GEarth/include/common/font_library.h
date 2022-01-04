/*!
*@File    font_library.h
*@Brief   文件描述:下次一定
*@Date    2021/12/31
*/
#pragma once
#include "Common/Mutexable.h"
#include <memory>
#include <list>
class CFont;
class CFontLibrary;

class CFontLibrary : public CMutexable
{
    friend CFont;
public:
    CFontLibrary(const std::string& sPath);
    CFontLibrary() = delete;
    virtual ~CFontLibrary() = default;
    /*!
    /* @Brief:     创建字体
    /* @Date:      2021/12/31
    /* @Parameter: const std::string & sContext
    /* @Return     std::shared_ptr<CFont>
    */
    std::shared_ptr<CFont> CreateFont(const std::string& sContext);

    /*!
    /* @Brief:     移除掉所有无效字体
    /* @Date:      2021/12/31
    /* @Return     void
    */
    void ClearUnrefFonts();
    
    //说明某个地方移除引用
    void RemoveFont(const std::shared_ptr<CFont>& pFont);
protected:
    std::list<std::weak_ptr<CFont>> m_fonts;   //字体集合
    std::string m_sPath;
};



// std::enable_shared_from_this
class CFont 
{
public:
    friend CFontLibrary;
public:
    virtual ~CFont();

    CFont(const std::string& context);
protected:
    std::string m_sContext;     //文本内容
    bool m_enableUnderline;     //是否有下划线
    float m_fSize;              //字体大小
    float m_fSpacing;           //间距
    std::shared_ptr<CFontLibrary> m_pFontLibrary; //字体库来源
private:
};


