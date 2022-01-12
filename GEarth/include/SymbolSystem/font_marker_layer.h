/*!
*@File    font_marker_layer.h
*@Brief   字体库
*@Date    2021/12/30
*/
#pragma once
#include "SymbolSystem/marker_layer.h"
#include "common/font_library.h"

namespace symbol
{
    class CORE_PUBLIC CFontMarkerLayer : public CMarkerLayer
    {
    public:
        CFontMarkerLayer();
       virtual ~CFontMarkerLayer();
        CFontMarkerLayer(CFont& pFont);

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



        CFont& GetFont();
    protected:
        CFont&  m_pFont;
    };

}

