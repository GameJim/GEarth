#include "path.h"
#include "common/type_size.h"
namespace symbol
{

    void CPathCommand::Serialize(CByte& data)
    {
        data << m_IsRelative;
    }

    void CPathCommand::Deserialize(CByte& data)
    {
        data >> m_IsRelative;
    }

    size_t CPathCommand::GetSize()
    {
        return 1;
    }

    CPathCommand::CPathCommand(const EnPathCommandType& type)
        :m_enCommandType(type)
    {

    }

    CClosed::CClosed()
        :CPathCommand(COMMAND_CLOSED)
    {

    }


    void CMoveTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);
        data << m_Pos;
    }

    void CMoveTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_Pos;
    }

    size_t CMoveTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_Pos) + CPathCommand::GetSize();
    }


    void CLineTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);
        data << m_Pos;
    }

    void CLineTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_Pos;
    }

    size_t CLineTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_Pos) + CPathCommand::GetSize();
    }

    CMoveTo::CMoveTo()
        : CPathCommand(COMMAND_MOVETO), m_Pos(0.0, 0.0)
    {

    }

    CLineTo::CLineTo()
        : CPathCommand(COMMAND_LINETO),m_Pos(0.0,0.0)
    {

    }

    CHLineTo::CHLineTo()
        : CPathCommand(COMMAND_HLINETO), m_x(0.0)
    {

    }

    void CHLineTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);
        data << m_x;
    }

    void CHLineTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_x;
    }

    size_t CHLineTo::GetSize()
    {
        return DOUBLE_SIZE + CPathCommand::GetSize();
    }

    void CVLineTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);
        data << m_y;
    }

    void CVLineTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_y;
    }

    size_t CVLineTo::GetSize()
    {
        return DOUBLE_SIZE + CPathCommand::GetSize();
    }

    CVLineTo::CVLineTo()
        : CPathCommand(COMMAND_VLINETO), m_y(0.0)
    {

    }

    CArcTo::CArcTo()
        : CPathCommand(COMMAND_ARCTO)
        , m_radio(5.0,5.0)
        , m_endPos(0.0,0.0)
        , m_xRotation(0.0)
        , m_IsLargeFlag(false)
        , m_IsClockwise(false)
    {

    }

    void CArcTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);

        data << m_radio << m_endPos << m_IsClockwise << m_IsLargeFlag << m_xRotation;
    }

    void CArcTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);

        data >> m_radio >> m_endPos >> m_IsClockwise >> m_IsLargeFlag >> m_xRotation;
    }

    size_t CArcTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_radio) * 2 + 2 + DOUBLE_SIZE + CPathCommand::GetSize();
    }

    CCurveTo::CCurveTo()
        : CPathCommand(COMMAND_CURVETO)
    {
        
    }

    void CCurveTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);

        data << m_staPos << m_endPos << m_finalPos;
    }

    void CCurveTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_staPos >> m_endPos >> m_finalPos;

    }

    size_t CCurveTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_staPos) * 3 + CPathCommand::GetSize();
    }

    CSCurveTo::CSCurveTo()
        : CPathCommand(COMMAND_SCURVETO)
    {

    }

    void CSCurveTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);

        data << m_endPos << m_finalPos;
    }

    void CSCurveTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_endPos >> m_finalPos;

    }

    size_t CSCurveTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_endPos) * 2 + CPathCommand::GetSize();
    }


    CQBCurveTo::CQBCurveTo()
        : CPathCommand(COMMAND_QBCURVETO)
    {

    }

    void CQBCurveTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);

        data << m_staPos  << m_finalPos;
    }

    void CQBCurveTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data >> m_staPos  >> m_finalPos;

    }

    size_t CQBCurveTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_staPos) * 2 + CPathCommand::GetSize();
    }

    CTBCurveTo::CTBCurveTo()
        : CPathCommand(COMMAND_TBCURVETO)
    {

    }

    void CTBCurveTo::Serialize(CByte& data)
    {
        CPathCommand::Serialize(data);

        data  << m_finalPos;
    }

    void CTBCurveTo::Deserialize(CByte& data)
    {
        CPathCommand::Deserialize(data);
        data  >> m_finalPos;

    }

    size_t CTBCurveTo::GetSize()
    {
        return CTypeSize::Instance().GetSize(m_finalPos) + CPathCommand::GetSize();
    }

    CPath::CPath()
        :CShape(CShape::EnShapType::SHAP_TYPE_PATH)
    {
        m_vPathCommand = new std::vector<std::unique_ptr<CPathCommand>>();
    }

    CPath::~CPath()
    {
        delete m_vPathCommand;
    }

    void CPath::AddPathCommand(std::unique_ptr<CPathCommand> pCommand)
    {
        //m_vPathCommand->push_back(std::move((pCommand)));
    }

    void CPath::RemovePathCommand(const unsigned& index)
    {
        if (m_vPathCommand->size() > index)
        {
            m_vPathCommand->erase(m_vPathCommand->begin() + index);
        }
    }

    CPathCommand* CPath::GetPathCommand(const unsigned& index)
    {
        if (m_vPathCommand->size() > index)
        {
            return (*m_vPathCommand)[index].get();
        }

        return nullptr;
    }

    void CPath::Serialize(CByte& data)
    {
        unsigned nSize = m_vPathCommand->size();
        data << nSize;

        for (auto& command : *m_vPathCommand)
        {
            uint8_t nType = command->m_enCommandType;
            data << nType;
            command->Serialize(data);
        }
    }

    void CPath::Deserialize(CByte& data)
    {
        unsigned nSize;
        data >> nSize;
        for (unsigned i = 0;i <nSize;++i)
        {
            uint8_t type;
            data >> type;
            CPathCommand::EnPathCommandType ePathCommandType = (CPathCommand::EnPathCommandType)type;
            std::unique_ptr<CPathCommand> pPathCommand = nullptr;
            switch (ePathCommandType)
            {
            case symbol::CPathCommand::COMMAND_MOVETO:
                pPathCommand = std::make_unique<CMoveTo>();
                break;
            case symbol::CPathCommand::COMMAND_LINETO:
                pPathCommand = std::make_unique<CLineTo>();
                break;
            case symbol::CPathCommand::COMMAND_HLINETO:
                pPathCommand = std::make_unique<CHLineTo>();
                break;
            case symbol::CPathCommand::COMMAND_VLINETO:
                pPathCommand = std::make_unique<CVLineTo>();
                break;
            case symbol::CPathCommand::COMMAND_ARCTO:
                pPathCommand = std::make_unique<CArcTo>();
                break;
            case symbol::CPathCommand::COMMAND_CURVETO:
                pPathCommand = std::make_unique<CCurveTo>();
                break;
            case symbol::CPathCommand::COMMAND_SCURVETO:
                pPathCommand = std::make_unique<CSCurveTo>();
                break;
            case symbol::CPathCommand::COMMAND_QBCURVETO:
                pPathCommand = std::make_unique<CQBCurveTo>();
                break;
            case symbol::CPathCommand::COMMAND_TBCURVETO:
                pPathCommand = std::make_unique<CTBCurveTo>();
                break;
            case symbol::CPathCommand::COMMAND_CLOSED:
                pPathCommand = std::make_unique<CClosed>();
                break;
            default:
                break;
            }

            pPathCommand->Deserialize(data);

            m_vPathCommand->push_back(std::move(pPathCommand));
        }
       
    }

    size_t CPath::GetSize()
    {
        size_t nSize = 4;
        for (auto& command : *m_vPathCommand)
        {
            nSize = nSize + 1 + command->GetSize();  
        }

        return nSize;
    }

   
    bool CPath::IsClosed()
    {
        if (m_vPathCommand->size() > 1 && (*m_vPathCommand)[m_vPathCommand->size()-1]->m_enCommandType == CPathCommand::COMMAND_CLOSED)
        {
            return true;
        }
        return false;
    }

}

