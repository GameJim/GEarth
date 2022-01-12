#include "SymbolSystem/basic_shapes.h"
#include "common/type_size.h"
namespace symbol
{
   
    void CRect::Serialize(CByte& data)
    {
        data << m_xy << m_rxy << m_size;
    }

    void CRect::Deserialize(CByte& data)
    {
        data >> m_xy >> m_rxy >> m_size;
    }

    size_t CRect::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_xy) * 3;
    }

    bool CRect::IsClosed()
    {
        return true;
    }


    CRect::CRect()
        :CShape(CShape::EnShapType::SHAP_TYPE_RECT)
    {

    }

    void CCircle::Serialize(CByte& data)
    {
        data << m_cneter << m_r;
    }

    void CCircle::Deserialize(CByte& data)
    {
        data >> m_cneter >> m_r;
    }

    size_t CCircle::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_cneter) + DOUBLE_SIZE;
    }

    bool CCircle::IsClosed()
    {
        return true;
    }



    CCircle::CCircle()
        :CShape(CShape::EnShapType::SHAP_TYPE_CIRCLE)
    {

    }

    void CEllipse::Serialize(CByte& data)
    {
        data << m_cneter << m_rxy;
    }

    void CEllipse::Deserialize(CByte& data)
    {
        data >> m_cneter >> m_rxy;
    }

    size_t CEllipse::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_cneter) * 2;
    }

    bool CEllipse::IsClosed()
    {
        return true;
    }


    CEllipse::CEllipse()
        :CShape(CShape::EnShapType::SHAP_TYPE_ELLIPSE)
    {

    }

    void CLine::Serialize(CByte& data)
    {
        data << m_staPos << m_endPos;
    }

    void CLine::Deserialize(CByte& data)
    {
        data >> m_staPos >> m_endPos;
    }

    size_t CLine::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_staPos) * 2;
    }

    bool CLine::IsClosed()
    {
        return false;
    }



    CLine::CLine()
        :CShape(CShape::EnShapType::SHAP_TYPE_LINE)
    {

    }

    void CPolyline::Serialize(CByte& data)
    {
        unsigned nSize = m_Poss.size();
        data << nSize;

        for (int i = 0;i < nSize;i++)
        {
            data << m_Poss[i];
        }
    }

    void CPolyline::Deserialize(CByte& data)
    {
        unsigned nSize;
        data >> nSize;

        m_Poss.resize(nSize);
        for (int i = 0; i < nSize; i++)
        {
            data >> m_Poss[i];
        }
    }

    size_t CPolyline::GetSize()
    {
        size_t nSize = 4;
        if (m_Poss.size() > 0)
            nSize += (m_Poss.size()* CTypeSize::Instance().GetSize(m_Poss[0]));

        return nSize;
    }

    bool CPolyline::IsClosed()
    {
        if (m_Poss.size() > 3)
        {
            return m_Poss[0] == m_Poss[m_Poss.size() - 1];
        }
        return false;
    }


    CPolyline::CPolyline()
        :CShape(CShape::EnShapType::SHAP_TYPE_POLYLINE)
    {

    }

    void CPolygon::Serialize(CByte& data)
    {
        unsigned nSize = m_Poss.size();
        data << nSize;

        for (unsigned i = 0; i < nSize; i++)
        {
            data << m_Poss[i];
        }
    }

    void CPolygon::Deserialize(CByte& data)
    {
        unsigned nSize;
        data >> nSize;

        m_Poss.resize(nSize);
        for (unsigned i = 0; i < nSize; i++)
        {
            data >> m_Poss[i];
        }
    }

    size_t CPolygon::GetSize()
    {
        size_t nSize = 4;
        if (m_Poss.size() > 0)
            nSize += (m_Poss.size()* CTypeSize::Instance().GetSize(m_Poss[0]));

        return nSize;
    }

    bool CPolygon::IsClosed()
    {
        return true;
    }

    CPolygon::CPolygon()
        :CShape(CShape::EnShapType::SHAP_TYPE_POLYGON)
    {

    }

}