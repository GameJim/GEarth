/*!
*@File    type.h
*@Brief   文件描述:下次一定
*@Date    2022/1/10
*/
#pragma once
#include "common/apiDescription.h"
#include "osgEarth/Map"

namespace earth
{
   
   class CORE_PUBLIC CMap : public osgEarth::Map
   {
   public:
       CMap();
       CMap(int argc, char **);
       /*!
       /* @Brief:     添加函数描述
       /* @Date:      2022/1/21
       /* @Return     
       */
       virtual ~CMap();
   public:
       bool IsEnable();
       bool m_IsEnable;

      
   };
}


