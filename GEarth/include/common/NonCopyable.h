#pragma once
#include "Common/ApiDescription.h"
//���ɿ�����
class CORE_PUBLIC CNonCopyable
{
public:
	CNonCopyable() = default;
	CNonCopyable& operator=(const CNonCopyable&) = delete;
	CNonCopyable(const CNonCopyable&) = delete;

protected:
	virtual ~CNonCopyable() = default;
};