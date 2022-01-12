/*!
*@File    geometry_marker_layer.h
*@Brief   文件描述:下次一定
*@Date    2021/12/30
*/
#pragma once

#include "SymbolSystem/marker_layer.h"
#include "SymbolSystem/shape.h"
#include "SymbolSystem/fill_symbol.h"
#include <vector>
#include <memory>
namespace symbol
{
   
    class CORE_PUBLIC CShapeMarkerLayer : public CMarkerLayer
    {
    public:
        CShapeMarkerLayer();
        virtual ~CShapeMarkerLayer();


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

    protected:
        //
        std::vector<std::unique_ptr<CShape>>*  m_vShapes;
        std::unique_ptr<CFillSymbol>  m_pFillSymbol;     //包含填充和线
    };

    

}

