/*!
*@File    size.h
*@Brief   文件描述:下次一定
*@Date    2022/1/7
*/
#pragma once
#include <common/apiDescription.h>
#include <array>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
namespace COMMON_NAMESPACE
{

    template<class T, size_t N>
    class CORE_PUBLIC CSize {};

     template<class T>
     class CORE_PUBLIC CSize<T, 2> : public glm::vec<2, T, glm::qualifier::defaultp>
     {
     public:
         CSize() :glm::vec<2, T, glm::qualifier::defaultp>() {};
         CSize(const T& _X, const T& _Y)
         {
             x = _X;
             y = _Y;
         }

         CSize(const glm::vec<2, T, glm::qualifier::defaultp>& data)
         {
             glm::vec<2, T, glm::qualifier::defaultp>::operator=(data);
         }

         T& Width()
         {
             return x;
         }

         T& Height()
         {
             return y;
         }
     };

    using CSize2i = CSize<int,2>;
    using CSize2f = CSize<float,2>;
    using CSize2d = CSize<double,2>;



    template<class T>
    class CORE_PUBLIC CSize<T, 3> : public glm::vec<3, T, glm::qualifier::defaultp>
    {
    public:
        CSize() :glm::vec<3, T, glm::qualifier::defaultp>() {};
        CSize(const T& _X, const T& _Y)
        {
            x = _X;
            y = _Y;
        }

        CSize(const glm::vec<3, T, glm::qualifier::defaultp>& data)
        {
            glm::vec<3, T, glm::qualifier::defaultp>::operator=(data);
        }

        T& Width()
        {
            return x;
        }

        T& Height()
        {
            return y;
        }
    };

    using CSize3i = CSize<int,3>;
    using CSize3f = CSize<float,3>;
    using CSize3d = CSize<double,3>;

}

