/*!
*@File    type.h
*@Brief   文件描述:下次一定
*@Date    2022/1/10
*/
#pragma once

#include "common/apiDescription.h"
#include "glm/mat4x4.hpp"


namespace COMMON_NAMESPACE
{
    template <typename T> using CVec2 = glm::vec<2, T, glm::qualifier::defaultp>;
    template <typename T> using CVec3 = glm::vec<3, T, glm::qualifier::defaultp>;
    template <typename T> using CVec4 = glm::vec<4, T, glm::qualifier::defaultp>;


    using CVec2d = CVec2<double>;
    using CVec2f = CVec2<float>;
    using CVec3d = CVec3<double>;
    using CVec3f = CVec3<float>;

    template <typename T> using CMatrix = glm::mat<4, 4, T, glm::qualifier::defaultp>;
    using CMatrix4d = glm::dmat4;
    using CMatrix4f = glm::mat4;
  
 
   
}


