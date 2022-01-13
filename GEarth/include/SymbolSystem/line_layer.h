/*!
*@File    line_layer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/13
*/
#pragma once
#include <common/object.h>
#include "common/nonCopyable.h"
#include "common/color.h"
#include "common/unit.h"
namespace symbol
{
    using namespace common;
    class CORE_PUBLIC CLineLayer : public CObject, public CNonCopyable
    {
    public:
        using CLineWidth = CUnitValue<float>;                     //符号大小
        enum EnLineLayerType : uint8_t
        {
            LINE_LAYER_TYPE_SIMPLE = 0,   //简单线型
            LINE_LAYER_TYPE_MARKER = 1,   //装饰线
            LINE_LAYER_TYPE_IMAGE = 2,  //纹理线
            LINE_LAYER_TYPE_CUSTOM        //自定义
        };

        enum EnLineCapType : uint8_t
        {
            LINE_CAP_TYPE_BUTT = 0,
            LINE_CAP_TYPE_ROUND,
            LINE_CAP_TYPE_SQUARE,
        };

        enum EnLineJoinType : uint8_t
        {
            LINE_JOIN_TYPE_MITER = 0,
            LINE_JOIN_TYPE_ROUND,
            LINE_JOIN_TYPE_BEVEL,
        };

        virtual ~CLineLayer() = default;

        EnLineLayerType GetLineLayerType() const;

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

        virtual bool IsEnable() const;

        void SetEnable(const bool& enable);

        CLineWidth& GetLineWidth();

        void SetLineWidth(const CLineWidth& LineWidth);

        EnLineJoinType LineJoinType();
        void SetLineJionType(const EnLineJoinType& eLineJoinType);

        EnLineCapType LineCapType();
        void SetLineCapType(const EnLineCapType& eLineCapType);
    protected:
        CLineLayer(const EnLineLayerType& type);

        const EnLineLayerType m_eLineLayerType;
        //关键数据
        bool   m_IsEnable;                     //数据是否启用
        CVec3d m_offset;                       //偏移量
        CLineWidth m_lineWidth;                //线宽
        double m_miterlimit;                   //限制
        EnLineJoinType m_eLineJionType;
        EnLineCapType m_eLineCapType;
    };
}




