/*!
*@File    unit.h
*@Brief   文件描述:下次一定
*@Date    2022/1/6
*/
#pragma once

#include <stdint.h>

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

    EnUnitType m_eUnitType;
    T m_data;
};

