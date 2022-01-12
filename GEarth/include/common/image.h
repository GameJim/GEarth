/*!
*@File    Image.h
*@Brief   纹理
*@Date    2021/12/22
*/
#pragma once
#include "common/object.h"
#include <memory>
namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CImage : public CObject
    {
    public:
        //float和int都是4个字节 
        enum EnImageFormateType : uint8_t
        {
            IFT_UNKNOWN = 0,
            IFT_BINARY,
            IFT_GRAY,
            IFT_RGB,
            IFT_BGR,
            IFT_RGBA,
            IFT_BGRA,
            IFT_DEFAULT
        };

        CImage();

        CImage(std::unique_ptr<uint8_t[]> pData, unsigned nWidth, unsigned nHegith, unsigned nChannel, const EnImageFormateType& eImageFormateType = IFT_DEFAULT);
        virtual ~CImage();

        unsigned GetImageSize() const;

        CImage& operator=(const CImage& image);

        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
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

        /*!
        /* @Brief:     判断纹理是否有效
        /* @Date:      2021/12/30
        /* @Return     bool
        */
        bool IsEnable() const;

        /*!
        /* @Brief:     放缩图形
        /* @Date:      2021/12/30
        /* @Return     void
        */
        void Resize() const;   //纹理重采样
                               /*!
                               /* @Brief:     更新子区域
                               /* @Date:      2021/12/30
                               /* @Parameter: const unsigned char * data
                               /* @Parameter: const unsigned & nWidth
                               /* @Parameter: const unsigned & nHegith
                               /* @Parameter: const unsigned & x
                               /* @Parameter: const unsigned & y
                               /* @Return     void
                               */
        void UpdateSubArea(const unsigned char* data, const unsigned& nWidth, const unsigned& nHegith, const unsigned& x = 0, const unsigned& y = 0);
    protected:

        std::unique_ptr<uint8_t[]>        m_pData;
        unsigned m_nWidth;                 //图片宽度
        unsigned m_nHeigth;                //图片高度
        uint8_t  m_nChannel;              //颜色通道
        EnImageFormateType m_eImageFormateType;  //纹理数据的数据格式
    };
}



