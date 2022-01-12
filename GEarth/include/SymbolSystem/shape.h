/*!
*@File    geometry.h
*@Brief   文件描述:下次一定
*@Date    2022/1/11
*/
#pragma once

#include "common/object.h"
#include <vector>
#include <stdint.h>

namespace symbol
{
    using namespace common;

    class CORE_PUBLIC CShape : public CObject
    {
    public:
        enum EnShapType : uint8_t
        {
            SHAP_TYPE_RECT = 0,
            SHAP_TYPE_CIRCLE,
            SHAP_TYPE_ELLIPSE,
            SHAP_TYPE_LINE,
            SHAP_TYPE_POLYLINE,
            SHAP_TYPE_POLYGON,
            SHAP_TYPE_PATH,
            SHAP_TYPE_CUSTOM
        };

        virtual bool IsClosed() = 0;
        virtual ~CShape() = default;
    protected:
        CShape(const EnShapType& type) :m_eShapType(type) {};
        
    public:
        const EnShapType m_eShapType;
    };
}