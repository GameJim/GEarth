/*!
*@File    font_library.h
*@Brief   文件描述:下次一定
*@Date    2021/12/31
*/
#pragma once
#include "common/object.h"
#include <memory>
#include <list>

namespace COMMON_NAMESPACE
{
    class  CFont;
    class  CFontLibrary;
    class CFontFactory;

    class CORE_PUBLIC CFontLibrary : public CObject
    {
    public:
        CFontLibrary(const std::string& sPath);
        CFontLibrary() = delete;
        virtual ~CFontLibrary() = default;
       
        virtual void Serialize(CByte& data);

        /*!
        /* @Brief:     反序列化
        /* @Date:      2021/12/22
        /* @Parameter: CByte & data
        /* @Return     void
        */
        virtual void Deserialize(CByte& data);

        /*!
        /* @Brief:     获取字节大小
        /* @Date:      2021/12/22
        /* @Return     size_t
        */
        virtual size_t GetSize();

      
    protected:
        std::string m_sPath;
    };


    // std::enable_shared_from_this
    //CFontFactory 创建字体
    class CORE_PUBLIC CFont : public CObject
    {
    public:
        friend CFontFactory;
    public:
        virtual ~CFont();
        CFontLibrary& GetFontLibrary();
        CFont(const std::string& context, std::unique_ptr<CFontLibrary> fontLibrary);


        //释放该字体时调用
        virtual void Free();

        virtual void Serialize(CByte& data);

        /*!
        /* @Brief:     反序列化
        /* @Date:      2021/12/22
        /* @Parameter: CByte & data
        /* @Return     void
        */
        virtual void Deserialize(CByte& data);

        /*!
        /* @Brief:     获取字节大小
        /* @Date:      2021/12/22
        /* @Return     size_t
        */
        virtual size_t GetSize();
    protected:
        std::string m_sContext;         //文本内容
        bool m_enableUnderline;        //是否有下划线
        float m_fSize;                 //字体大小
        float m_fSpacing;              //间距
        
        std::unique_ptr<CFontLibrary> m_pFontLibrary;

    private:
    };

  
}







