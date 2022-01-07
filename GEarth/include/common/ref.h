/*!
*@File    refObject.h
*@Brief   引用某个数据的指针指针
*@Date    2022/1/6
*/
#pragma once
#include <memory>

namespace COMMON_NAMESPACE
{
    template<class T>
    class CRef :public std::shared_ptr<T>, public T
    {
    public:
        CRef() = default;
       virtual ~CRef() = default;
    };

}


