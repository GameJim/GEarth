#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/singleton_plugin.h"
#include <string>
#include <atomic>
#include <sstream>
#include <iostream>


namespace util
{
	class UTIL_API LogSystem : public SingletonPlugin
	{
	public:
		enum EnLogLevel
		{
			LOG_LEVEL_TRACE,
			LOG_LEVEL_DEBUG,
			LOG_LEVEL_INFO,
			LOG_LEVEL_WARNING,
			LOG_LEVEL_ERROR,
			LOG_LEVEL_FATAL
		};
		DEFINE_SINGLETON_PLUGIN(LogSystem)
		DEFINE_PROPERTY(bool, enable)          //是否启用
		DEFINE_PROPERTY(EnLogLevel,logLevel)   //输出等级 LOG_LEVEL_WARNING
		DEFINE_PROPERTY(bool, async)           //是否异步 true
		DEFINE_PROPERTY(std::string, logFile)  //输出文件路径
		DEFINE_PROPERTY(bool, isFileLog)       //控制台还是文件日志
	public:
		class NullStreamBuffer : public std::streambuf
		{
		public:
			NullStreamBuffer() = default;
			virtual ~NullStreamBuffer() = default;
		protected:
			std::streamsize xsputn(const std::streambuf::char_type * /*str*/, std::streamsize n);
		};


		class NullStream : public std::ostream
		{
		public:
			NullStream();
			virtual ~NullStream();
		protected:
			LogSystem::NullStreamBuffer* _buffer;
		};

		static std::ostream& notify(const EnLogLevel& level);
		static bool isNotifyEnabled(const EnLogLevel& level);
	protected:
		virtual std::ostream& logImpl(const EnLogLevel& level) = 0;
	};
}


#define LOG_NOTIFY(logLevel) if(util::LogSystem::isNotifyEnabled(logLevel)) util::LogSystem::notify(logLevel)
#define LOG_TRACE LOG_NOTIFY(util::LogSystem::EnLogLevel::LOG_LEVEL_TRACE)
#define LOG_DEBUG LOG_NOTIFY(util::LogSystem::EnLogLevel::LOG_LEVEL_DEBUG)
#define LOG_INFO LOG_NOTIFY(util::LogSystem::EnLogLevel::LOG_LEVEL_INFO)
#define LOG_WARNING LOG_NOTIFY(util::LogSystem::EnLogLevel::LOG_LEVEL_WARNING)
#define LOG_ERROR LOG_NOTIFY(util::LogSystem::EnLogLevel::LOG_LEVEL_ERROR)