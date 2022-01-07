/*!
*@File    marker_layer.h
*@Brief   点符号图层位置
*@Date    2021/12/22
*/
#pragma once
#include <common/object.h>
#include "common/box.h"
namespace symbol
{
    using namespace common;
    class CMarkerLayer : public CObject
    {
    public:
        enum EnMarkerLayerType : uint8_t
        {
            SYMBOL_TYPE_GEOMETRY = 0,   //矢量
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

        virtual CBox3f GetBoundBox() = 0;
    protected:
        CMarkerLayer(const EnMarkerLayerType& type);
        EnMarkerLayerType m_eMarkerLayerType;
        CVec3d m_pos;            //数据的偏移，可以看做起点
        bool   m_IsEnable;       //数据是否启用
    };


}
