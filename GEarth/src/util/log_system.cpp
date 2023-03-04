#include "util/log_system.h"

namespace util
{
	DEFINE_SINGLETON_PLUGIN_CPP(LogSystem)

	LogSystem::LogSystem()
	{
		name() = "LogSystem";
		logLevel() = EnLogLevel::LOG_LEVEL_INFO;
	}

	void LogSystem::Log(const EnLogLevel& level, const std::string& log)
	{
		if (!m_pInstance)
		{
			m_pInstance->LogImpl(level, log);
		}
	}

}

