/*!
*@File    path.h
*@Brief   文件描述:下次一定
*@Date    2022/1/12
*/
#pragma once



#include "SymbolSystem/shape.h"

#include <vector>
#include <memory>
#include <stdint.h>

using namespace common;
namespace symbol
{
    
    class CORE_PUBLIC CPathCommand : public CObject
    {
    public:
       
        enum EnPathCommandType : uint8_t
        {
            COMMAND_MOVETO = 0,
            COMMAND_LINETO,
            COMMAND_HLINETO,
            COMMAND_VLINETO,
            COMMAND_ARCTO,
            COMMAND_CURVETO,
            COMMAND_SCURVETO,
            COMMAND_QBCURVETO,
            COMMAND_TBCURVETO,
            COMMAND_CLOSED,
        };
    public:
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

     protected:
        CPathCommand(const EnPathCommandType& type);
    public:
        virtual ~CPathCommand() = default;
    
        const EnPathCommandType m_enCommandType;
        bool m_IsRelative  = false;
    };

    class CORE_PUBLIC CMoveTo :public CPathCommand
    {
    public:
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
    public:
        CMoveTo();
        virtual ~CMoveTo() = default;
        CVec2d  m_Pos;
    };

    class CORE_PUBLIC CClosed :public CPathCommand
    {
    public:
        CClosed();
        virtual ~CClosed() = default;

    };

    class CORE_PUBLIC CLineTo :public CPathCommand
    {
    public:
        CLineTo();
        virtual ~CLineTo() = default;
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        CVec2d  m_Pos;
    };

    class CORE_PUBLIC CHLineTo :public CPathCommand
    {
    public:
        CHLineTo();
        virtual ~CHLineTo() = default;
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        double m_x;
    };

    class CORE_PUBLIC CVLineTo :public CPathCommand
    {
    public:
        CVLineTo();
        virtual ~CVLineTo() = default;
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        double m_y;
    };

    class CORE_PUBLIC CArcTo :public CPathCommand
    {
    public:
        CArcTo();
        virtual ~CArcTo() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        CVec2d m_radio;
        CVec2d m_endPos;   //相对值
        double m_xRotation;
        bool m_IsLargeFlag;
        bool m_IsClockwise;
    };

    class CORE_PUBLIC CCurveTo :public CPathCommand
    {
    public:
        CCurveTo();
        virtual ~CCurveTo() = default;

        virtual void Serialize(CByte& data)override ;
        virtual void Deserialize(CByte& data)override;
        virtual size_t GetSize() override;

        CVec2d m_staPos;
        CVec2d m_endPos;  
        CVec2d m_finalPos;
    };

    class CORE_PUBLIC CSCurveTo :public CPathCommand
    {
    public:
        CSCurveTo();
        virtual ~CSCurveTo() = default;
        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        CVec2d m_endPos;
        CVec2d m_finalPos;
    };

    class CORE_PUBLIC CQBCurveTo :public CPathCommand
    {
    public:
        CQBCurveTo();
        virtual ~CQBCurveTo() = default;

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;
        CVec2d m_staPos;
        CVec2d m_finalPos;
    };

    class CORE_PUBLIC CTBCurveTo :public CPathCommand
    {
    public:
        CTBCurveTo();

        virtual void Serialize(CByte& data) override;
        virtual void Deserialize(CByte& data) override;
        virtual size_t GetSize() override;

        virtual ~CTBCurveTo() = default;
        CVec2d m_finalPos;
    };

    class CORE_PUBLIC CPath : public CShape
    {
    public:
        CPath();
        virtual ~CPath();
        void AddPathCommand(std::unique_ptr<CPathCommand> pCommand);
        void RemovePathCommand(const unsigned& index);
        CPathCommand* GetPathCommand(const unsigned& index);

        /*!
        /* @Brief:     序列化
        /* @Date:      2021/12/21
        /* @Parameter: CByte* data
        /* @Return     void
        */
        virtual void Serialize(CByte& data) ;

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

       virtual bool IsClosed() override;
    protected:

    protected:
        std::vector<std::unique_ptr<CPathCommand>>*  m_vPathCommand;
    };
   
}
