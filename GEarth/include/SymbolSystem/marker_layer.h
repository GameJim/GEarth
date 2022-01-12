/*!
*@File    marker_layer.h
*@Brief   点符号图层位置
*@Date    2021/12/22
*/
#pragma once
#include <common/object.h>
#include "common/nonCopyable.h"
#include "common/box.h"
#include "common/color.h"
namespace symbol
{
    using namespace common;
    class CORE_PUBLIC CMarkerLayer : public CObject, public CNonCopyable
    {
    public:
        enum EnMarkerLayerType : uint8_t
        {
            SYMBOL_TYPE_SHAPE = 0,   //矢量
            SYMBOL_TYPE_IMAGE = 1,      //栅格
            SYMBOL_TYPE_FONT = 2,      //栅格
            SYMBOL_TYPE_CUSTOM          //自定义
        };

        virtual ~CMarkerLayer() = default;

        EnMarkerLayerType MarkerLayerType() const;

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

        CVec3d GetPos() const;

        void SetPos(const CVec3d& pos);

        void SetEnable(const bool& enable);

        /*!
        /* @Brief:     每次获取包围盒都需要重新计算，因此涉及效率问题
        /* @Date:      2022/1/10
        /* @Return     COMMON_NAMESPACE::CBox3d
        */
        virtual CBox3d GetBoundBox() = 0;

        void SetColor(std::unique_ptr<CColor> pColor);

        CColor* GetOrCreateColor();

        virtual void SetMatrix(const CMatrix4d& matrix);
        CMatrix4d GetMatrix() const;
    protected:
        CMarkerLayer(const EnMarkerLayerType& type);
        EnMarkerLayerType m_eMarkerLayerType;

        //关键数据
        bool   m_IsEnable;                     //数据是否启用
        std::unique_ptr<CColor>   m_pColor;    //颜色

        CVec3d m_pos;                          //数据的偏移，可以看做起点
        CMatrix4d m_Matrix;                    //数据仿射矩阵，用于旋转放缩,注意其对数据包围盒的影响！

    };


}
