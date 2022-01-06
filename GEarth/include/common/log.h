/*!
*@File    Log.h
*@Brief   日志文件
*@Date    2021/12/21
*/
#pragma once
#include "Common/Factory.h"
#include "Common/Mutexable.h"
#include <string>

#define LOG_INFO(log) CLogManager::GetInstance().Log(CLogManager::EnLogLevel::LOG_LEVEL_INFO,log);
#define LOG_DEBUG(log) CLogManager::GetInstance().Log(CLogManager::EnLogLevel::LOG_LEVEL_DEBUG,log);
#define LOG_ERROR(log) CLogManager::GetInstance().Log(CLogManager::EnLogLevel::LOG_LEVEL_ERROR,log);

class CORE_PUBLIC CLogManager : public CFactory<CLogManager>, public CMutexable
{
public:
	enum EnLogLevel
	{
		LOG_LEVEL_INFO = 0,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_ERROR
	};

	void Log(const EnLogLevel& level, const std::string& log);

	bool IsEanble() const;
	void SetEnable(const bool& enable);

	CLogManager() = default;
	virtual ~CLogManager() = default;
protected:
	
	bool m_IsEnable = true;  //默认是启用的
};


