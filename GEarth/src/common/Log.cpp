#include "Common/Log.h"
#include <iostream>


void CLogManager::Log(const EnLogLevel& level, const std::string& log)
{
	if (!m_IsEnable)
		return;
	
	//锁，保证只有一个线程访问
	std::unique_lock<std::mutex> lock(m_Mutex);
	switch (level)
	{
	case LOG_LEVEL_INFO:
		std::cout << "info:" << log << std::endl;
		break;
	case LOG_LEVEL_DEBUG:
#ifdef _DEBUG
		std::cout << "debug:" << log << std::endl;
#endif
		break;
	case LOG_LEVEL_ERROR:
		std::cout << "error:" << log << std::endl;
		break;
	default:
		break;
	}

	
	
}

bool CLogManager::IsEanble() const
{
	return m_IsEnable;
}

void CLogManager::SetEnable(const bool& enable)
{
	m_IsEnable = enable;
}
