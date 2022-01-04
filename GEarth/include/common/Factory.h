#pragma once
#include "Common/NonCopyable.h"

//������,����ģʽ
template<class T>
class CORE_PUBLIC CFactory : public CNonCopyable
{
public:
	virtual ~CFactory() = default;
	static T& GetInstance()
	{
		//����
		static T m_Factory;
		return m_Factory;
	}
protected:
	CFactory() = default;
};

