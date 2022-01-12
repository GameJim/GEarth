/*!
*@File    unit.h
*@Brief   文件描述:下次一定
*@Date    2022/1/6
*/
#pragma once

#include <stdint.h>
#include "common/object.h"
namespace COMMON_NAMESPACE
{
    enum EnUnitType :uint8_t
    {
        UNIT_PIEXL = 0,  //像素
        UNIT_MM,         //毫米
        UNIT_M,          //米
    };


    template<class T>
    class CUnitValue 
    {
    public:
        CUnitValue(const T& data)
            :m_data(data), m_eUnitType(UNIT_PIEXL)
        {

        }

        CUnitValue(const T& data, const EnUnitType& eUnitType)
            :m_data(data), m_eUnitType(eUnitType)
        {

        }

        void Serialize(CByte& data)
        {
            uint8_t nType = m_eUnitType;
            data << nType << m_data;
           
        };

        /*!
        /* @Brief:     反序列化
        /* @Date:      2021/12/22
        /* @Parameter: CByte & data
        /* @Return     void
        */
        void Deserialize(CByte& data)
        {
            uint8_t nType;
            data >> nType >> m_data;
            m_eUnitType = (EnUnitType)nType;
        }

        /*!
        /* @Brief:     获取字节大小
        /* @Date:      2021/12/22
        /* @Return     size_t
        */
         size_t GetSize() { return 1 + sizeof(T); };

        EnUnitType m_eUnitType;
        T m_data;
    };
}


