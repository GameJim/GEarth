#include "common/type_size.h"
#include "common/byte.h"

namespace COMMON_NAMESPACE
{

    template<>
    size_t CTypeSize::GetSize(const CMatrix4d&)
    {
        return 16 * DOUBLE_SIZE;
    };

    template<>
    size_t CTypeSize::GetSize(const CMatrix4f&)
    {
        return 16 * FLOAT_SIZE;
    };


    template<>
    size_t CTypeSize::GetSize(const CVec3f&)
    {
        return 3 * FLOAT_SIZE;
    };

    template<>
    size_t CTypeSize::GetSize(const CVec3d&)
    {
        return 3 * DOUBLE_SIZE;
    };


    template<>
    size_t CTypeSize::GetSize(const CVec2f&)
    {
        return 2 * FLOAT_SIZE;
    };

    template<>
    size_t CTypeSize::GetSize(const CVec2d&)
    {
        return 2 * DOUBLE_SIZE;
    };
   

    //template<class T>
    //size_t CTypeSize::GetSize(const T& s)
    //{
    //    /*return sizeof(T);*/
    //}
}