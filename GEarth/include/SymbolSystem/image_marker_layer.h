/*!
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
    class CImageMarkerLayer : public CMarkerLayer
    {
        CImageMarkerLayer();
        ~CImageMarkerLayer() = default;

        virtual bool IsEnable() const;

        CImage* GetImage() const;
        void SetImage(CImage* pImage);
    protected:
        CImage*  m_pImage; //纹理数据
    };
}


