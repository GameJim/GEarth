/*!
*@File    box.h
*@Brief   包围盒
*@Date    2022/1/6
*/
#pragma once
#include "common/size.h"


namespace COMMON_NAMESPACE
{
    template<class T, size_t N>
    class CBox
    {
    public:
        CBox()
        {

        }

        CBox(const glm::vec<N, T, glm::qualifier::defaultp>& min, const glm::vec<N, T, glm::qualifier::defaultp>& max)
            :m_min(min),m_max(max)
        {

        }

        bool IsValid()
        {
            for (size_t i = 0; i < N; i++)
            {
                if (m_min[i] > m_max[i])
                    return false;
            }

            return true;
        };

        void Contain(const CBox<T, N>& _Left)
        {
            for (size_t i = 0; i < N; i++)
            {
                m_min[i] = std::min(m_min[i], _Left.m_min[i]);
                m_max[i] = std::max(m_max[i], _Left.m_max[i]);
            }
        };

        CBox<T,N> Merge(const CBox<T, N>& _Left)
        {
            CBox<T, N> result = *this;
            result.Contain(_Left);
            return result;
        }

        CSize<T, N> GetSize()
        {
            return m_max - m_min;
        };

    public:
        glm::vec<N, T, glm::qualifier::defaultp>   m_min;
        glm::vec<N, T, glm::qualifier::defaultp>   m_max;
    };
   
    using CVec3d = glm::vec<3, double, glm::qualifier::defaultp>;

    using CBox2i = CBox<int, 2>;
    using CBox2d = CBox<double, 2>;
    using CBox3d = CBox<double, 3>;
    using CBox3i = CBox<int, 3>;
    using CBox3f = CBox<float, 3>;
    using CBox3d = CBox<double, 3>;
   
}