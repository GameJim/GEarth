﻿/*!
*@File    image_marker_layer.h
*@Brief   文件描述:下次一定
*@Date    2021/12/22
*/
#pragma once
#include "SymbolSystem/marker_layer.h"
#include "common/image.h"
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CImageMarkerLayer : public CMarkerLayer
    {
    public:
        CImageMarkerLayer();
        ~CImageMarkerLayer() = default;


        virtual CBox3d GetBoundBox() override;

        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
        virtual void Serialize(CByte& data) override;

        /*!
        /* @Brief:     反序列化
        /* @Date:      2021/12/22
        /* @Parameter: CByte & data
        /* @Return     void
        */
        virtual void Deserialize(CByte& data) override;

        /*!
        /* @Brief:     获取字节大小
        /* @Date:      2021/12/22
        /* @Return     size_t
        */
        virtual size_t GetSize() override;

        virtual bool IsEnable() const;

        CImage* GetImage() const;
        void SetImage(std::unique_ptr<CImage> pImage);
    protected:
        std::unique_ptr<CImage>  m_pImage; //纹理数据
    };
}


