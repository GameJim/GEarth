#pragma once
#include "common/apiDescription.h"
//不可拷贝类
namespace COMMON_NAMESPACE
{
    class CORE_PUBLIC CNonCopyable
    {
    public:
        CNonCopyable() = default;
        CNonCopyable& operator=(const CNonCopyable&) = delete;
        CNonCopyable(const CNonCopyable&) = delete;

    protected:
        virtual ~CNonCopyable() = default;
    };
}
