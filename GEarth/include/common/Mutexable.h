/*!
*@File    Mutexable.h
*@Brief   多线程支持
*@Date    2021/12/21
*/
#pragma once
#include "Common/NonCopyable.h"
#include <mutex>
class CMutexable : public CNonCopyable
{
protected:
	CMutexable() = default;
	virtual ~CMutexable() = default;
	std::mutex m_Mutex;
};
