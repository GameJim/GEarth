/*!
*@File    basic_shapes.h
*@Brief   文件描述:下次一定
*@Date    2022/1/12
*/
#pragma once
#include "SymbolSystem/shape.h"
#include "common/type.h"
#include "common/size.h"
namespace symbol
{
    class CORE_PUBLIC CRect : public CShape
    {
    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CRect();
        CVec2d m_xy;
        CSize2d m_size;
        CVec2d m_rxy;
    };


    class CORE_PUBLIC CCircle : public CShape
    {
    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CCircle();
        CVec2d m_cneter;
        double m_r;
    };

    

    class CORE_PUBLIC CEllipse : public CShape
    {
    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CEllipse();
        CVec2d m_cneter;
        CVec2d m_rxy;
    };

    class CORE_PUBLIC CLine : public CShape
    {

    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CLine();
        CVec2d m_staPos;
        CVec2d m_endPos;
    };

    class CORE_PUBLIC CPolyline : public CShape
    {
    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CPolyline();
        std::vector<CVec2d> m_Poss;
    };

    class CORE_PUBLIC CPolygon : public CShape
    {
    public:
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        virtual bool IsClosed() override;
        CPolygon();
        std::vector<CVec2d> m_Poss;
    };
}




