/*!
*@File    Mutexable.h
*@Brief   多线程支持
*@Date    2021/12/21
*/
#pragma once
#include "common/nonCopyable.h"
#include <mutex>

namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CMutexable : public CNonCopyable
    {
    protected:
        CMutexable() = default;
        virtual ~CMutexable() = default;
        std::mutex m_Mutex;
    };
}

