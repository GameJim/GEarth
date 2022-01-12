/*!
*@File    size.h
*@Brief   文件描述:下次一定
*@Date    2022/1/7
*/
#pragma once
#include <common/apiDescription.h>
#include <common/type.h>
#include <array>

namespace COMMON_NAMESPACE
{

    template<class T, size_t N>
    class CORE_PUBLIC CSize {};

     template<class T>
     class CORE_PUBLIC CSize<T, 2> : public CVec2<T>
     {
     public:
         CSize() :CVec2<T>() {};
         CSize(const T& _X, const T& _Y)
         {
             x = _X;
             y = _Y;
         }

         CSize(const CVec2<T>& data)
         {
             CVec2::operator=(data);
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
    class CORE_PUBLIC CSize<T, 3> : public CVec3<T>
    {
    public:
        CSize() : CVec3<T>{};
        CSize(const T& _X, const T& _Y)
        {
            x = _X;
            y = _Y;
        }

        CSize(const CVec3<T>& data)
        {
            CVec3::operator=(data);
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

