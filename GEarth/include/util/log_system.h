#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/singleton_plugin.h"
#include <string>
#include <atomic>
#define LOG_INFO(log) util::LogSystem::GetInstance()->Log(util::LogSystem::EnLogLevel::LOG_LEVEL_INFO,log);
#define LOG_DEBUG(log) util::LogSystem::GetInstance()->Log(util::LogSystem::EnLogLevel::LOG_LEVEL_DEBUG,log);
#define LOG_ERROR(log) util::LogSystem::GetInstance()->Log(util::LogSystem::EnLogLevel::LOG_LEVEL_ERROR,log);
namespace util
{
	class UTIL_API LogSystem : public SingletonPlugin
	{
	public:
		enum EnLogLevel
		{
			LOG_LEVEL_INFO = 0,
			LOG_LEVEL_DEBUG,
			LOG_LEVEL_ERROR
		};
		DEFINE_SINGLETON_PLUGIN(LogSystem)
		DEFINE_OPTION(EnLogLevel,logLevel)
	public:
		
		void Log(const EnLogLevel& level, const std::string& log);
		virtual void LogImpl(const EnLogLevel& level, const std::string& log) = 0;
	protected:

	};

}



