#pragma once
#include "Common/NonCopyable.h"

//工厂类,单例模式
template<class T>
class CORE_PUBLIC CFactory : public CNonCopyable
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

