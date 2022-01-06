#pragma once
#include "common/mutexable.h"

//工厂类,支持多线程
namespace COMMON_NAMESPACE
{
    template<class T>
    class CORE_PUBLIC CFactory : public CMutexable
    {
    public:
        virtual ~CFactory() = default;
        static T& GetInstance()
        {
            //对象
            static T m_Factory;
            return m_Factory;
        }
    protected:
        CFactory() = default;
    };
}