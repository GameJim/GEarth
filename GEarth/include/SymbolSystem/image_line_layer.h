/*!
*@File    image_line_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/13
*/
#pragma once




#include "SymbolSystem/line_layer.h"
#include "common/image.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CImageLineLayer : public CLineLayer
    {
    public:

        CImageLineLayer();
        virtual ~CImageLineLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CImage* GetImage();
        virtual void SetImage(std::unique_ptr<CImage> pImage);

    protected:
        std::unique_ptr<CImage> m_pImage;
    };

}
