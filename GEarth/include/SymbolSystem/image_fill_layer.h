/*!
*@File    image_fill_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/14
*/
#pragma once
#include "SymbolSystem/fill_layer.h"
#include "common/image.h"
#include <vector>
#include <memory>

namespace symbol
{
    class CORE_PUBLIC CImageFillLayer : public CFillLayer
    {
    public:
        CImageFillLayer();
        virtual ~CImageFillLayer() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual CImage* GetImage();
        virtual void SetImageSymbol(std::unique_ptr<CImage> pImage);

        virtual void SetStep(const CVec3d& step);
        virtual CVec3d GetStep();

        virtual void SetXDirection(const CVec3f& vec);
        virtual CVec3f GetXDirection();
    protected:
        std::unique_ptr<CImage> m_pImage;
        CVec3d   m_Step;                   //每间距多少插入一条线
        CVec3f   m_Direction;              //符号朝向，即旋转
    };

}








