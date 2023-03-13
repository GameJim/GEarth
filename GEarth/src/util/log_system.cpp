#include "util/log_system.h"

namespace util
{
	DEFINE_SINGLETON_PLUGIN_CPP(LogSystem)


	LogSystem::NullStream::NullStream()
		: std::ostream(new LogSystem::NullStreamBuffer())
	{
		_buffer = static_cast<LogSystem::NullStreamBuffer *>(rdbuf());
	}

	LogSystem::NullStream::~NullStream()
	{
		rdbuf(0);
		delete _buffer;
	}

	static LogSystem::NullStream nullStream;

	LogSystem::LogSystem()
	{
		name() = "LogSystem";


		if (CMAKE_INTDIR == "RelWithDebInfo")
		{
			logLevel() = EnLogLevel::LOG_LEVEL_INFO;
		}
		else if (CMAKE_INTDIR == "Release")
		{
			logLevel() = EnLogLevel::LOG_LEVEL_WARNING;
		}
		else
		{
			logLevel() = EnLogLevel::LOG_LEVEL_DEBUG;
		}

		async() = true;
		enable() = true;
		logFile() = "log.log";
		isFileLog() = false;
	}


	std::ostream& LogSystem::notify(const EnLogLevel& level)
	{

		if (!LogSystem::isNotifyEnabled(level))
			return nullStream;

		return m_pInstance->logImpl(level);
	}

	bool LogSystem::isNotifyEnabled(const EnLogLevel& level)
	{
		if (!m_pInstance || !m_pInstance->_enable || level < m_pInstance->logLevel())
			return false;

		return true;
	}

	std::streamsize LogSystem::NullStreamBuffer::xsputn(const std::streambuf::char_type * /*str*/, std::streamsize n)
	{
		return n;
	}



}

