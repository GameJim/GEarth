#include "SymbolSystem/shape_marker_layer.h"
#include "SymbolSystem/basic_shapes.h"
#include "SymbolSystem/path.h"
namespace symbol
{

    CShapeMarkerLayer::CShapeMarkerLayer()
        :CMarkerLayer(SYMBOL_TYPE_SHAPE), m_pFillSymbol(nullptr)
    {
        m_vShapes = new std::vector<std::unique_ptr<CShape>>();
    }

    CShapeMarkerLayer::~CShapeMarkerLayer()
    {
        delete m_vShapes;
    }
    CBox3d CShapeMarkerLayer::GetBoundBox()
    {
        CBox3d box;
        return box;
    }

    void CShapeMarkerLayer::Serialize(CByte& data)
    {
        unsigned nSize = m_vShapes->size();
        data << nSize;
        for (auto& shap : *m_vShapes)
        {
            uint8_t nType = shap->m_eShapType;
            data << nType;
            shap->Serialize(data);
        }

        if (m_pFillSymbol)
        {
            data << true;
            m_pFillSymbol->Serialize(data);
        }

        data << false;
    }

    void CShapeMarkerLayer::Deserialize(CByte& data)
    {
        unsigned nSize;
        data >> nSize;

        for (unsigned i = 0;i < nSize;++i)
        {
            uint8_t nType;
            data >> nType;
            CShape::EnShapType eShapType = (CShape::EnShapType)nType;

            std::unique_ptr<CShape> pShape;
            switch (eShapType)
            {
            case symbol::CShape::SHAP_TYPE_RECT:
                pShape = std::make_unique<CRect>();
                break;
            case symbol::CShape::SHAP_TYPE_CIRCLE:
                pShape = std::make_unique<CCircle>();
                break;
            case symbol::CShape::SHAP_TYPE_ELLIPSE:
                pShape = std::make_unique<CEllipse>();
                break;
            case symbol::CShape::SHAP_TYPE_LINE:
                pShape = std::make_unique<CLine>();
                break;
            case symbol::CShape::SHAP_TYPE_POLYLINE:
                pShape = std::make_unique<CPolyline>();
                break;
            case symbol::CShape::SHAP_TYPE_POLYGON:
                pShape = std::make_unique<CPolygon>();
                break;
            case symbol::CShape::SHAP_TYPE_PATH:
                pShape = std::make_unique<CPath>();
                break;
            case symbol::CShape::SHAP_TYPE_CUSTOM:
            {
                std::string sClassName;
                data >> sClassName;
                CShape* pTemp = static_cast<CShape*>(ClassFactory::get_instance(sClassName.c_str()));
                pShape.reset(pTemp);
            }
                break;
            default:
                break;
            }
            pShape->Deserialize(data);
            m_vShapes->push_back(std::move(pShape));
        }
    }

    size_t CShapeMarkerLayer::GetSize()
    {
        size_t nSize = 4 + 1 + m_vShapes->size();
        for (auto& shap : *m_vShapes)
        {
            nSize += shap->GetSize();
        }

        if (m_pFillSymbol)
        {
            nSize += m_pFillSymbol->GetSize();
        }

        return nSize;
    }

    bool CShapeMarkerLayer::IsEnable() const
    {
        return true;
    }
}
