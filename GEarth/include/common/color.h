/*!
*@File    color.h
*@Brief   文件描述:下次一定
*@Date    2022/1/7
*/
#pragma once
#include "common/object.h"
#include <array>

namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CColor : public CObject
    {
    public:
        CColor() = default;
        CColor(const float& r, const float& g, const float& b, const float& a = 1.0);
        virtual ~CColor() = default;

        union
        {
            std::array<float, 4> data;
            struct { float r, g, b, a; };
        };

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

    public:
        static const CColor DIMGRAY;
        static const CColor BACK;
        static const CColor RED;
        static const CColor GREEN;
        static const CColor BULUE;
        static const CColor WHITE;
    };
}




