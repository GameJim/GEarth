#include "log_system.h"
#include "util/register_helper.h"

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/detail/format.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/algorithm/string.hpp>   


#include <iostream>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
namespace trivial = boost::log::trivial;

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(counter, "LineCounter", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "Timestamp",boost::posix_time::ptime)

namespace util
{
	namespace plugin
	{
		trivial::severity_level getLevel(const BoostLogSystem::EnLogLevel& level)
		{
			return trivial::severity_level(level);
		}



		class NotifyStreamBuffer : public std::stringbuf
		{
		public:
			DEFINE_OPTION(trivial::severity_level,level)
			NotifyStreamBuffer()
				:_level(trivial::severity_level::trace)
			{
				//BOOST_LOG_GLOBAL_LOGGER(_lg, "Timestamp", attrs::local_clock{});
			};



			void setSeverityLevel(const trivial::severity_level& level)
			{
				
				if (_level.isSet())
				{
					sync();
				}
				_level = level;
			};
		protected:
			virtual int sync()
			{
				int ret = std::stringbuf::sync();

				std::string s = str();
				str("");

				switch (_level.get())
				{
				case boost::log::v2s_mt_nt6::trivial::trace:
					BOOST_LOG_SEV(_lg, trivial::severity_level::trace) << s;
					break;
				case boost::log::v2s_mt_nt6::trivial::debug:
					BOOST_LOG_SEV(_lg, trivial::severity_level::debug) << s;
					break;
				case boost::log::v2s_mt_nt6::trivial::info:
					BOOST_LOG_SEV(_lg, trivial::severity_level::info) << s;
					break;
				case boost::log::v2s_mt_nt6::trivial::warning:
					BOOST_LOG_SEV(_lg, trivial::severity_level::warning) << s;
					break;
				case boost::log::v2s_mt_nt6::trivial::error:
					BOOST_LOG_SEV(_lg, trivial::severity_level::error) << s;
					break;
				case boost::log::v2s_mt_nt6::trivial::fatal:
					BOOST_LOG_SEV(_lg, trivial::severity_level::fatal) << s;
					break;
				default:
					break;

				}

				return ret;
			};
		private:
		protected:
			src::severity_logger<trivial::severity_level > _lg;
		};


		class BoostLogSystem::NotifyStream :  public std::iostream
		{
		public:
			NotifyStream() :
				std::iostream(new NotifyStreamBuffer)
			{
				_buffer = static_cast<NotifyStreamBuffer*>(rdbuf());
			}
				
			virtual  ~NotifyStream()
			{
				rdbuf(0);
				delete _buffer;
			}

			std::iostream& setSeverityLevel(const trivial::severity_level& level)
			{
				if (_buffer)
					_buffer->setSeverityLevel(level);

				return *this;
			}
			//fstream
		protected:
			NotifyStreamBuffer* _buffer;

		};




		BoostLogSystem::BoostLogSystem()
		{
			driver() = "boost";

			//定义全局日志属性
			logging::core::get()->add_global_attribute("Timestamp", attrs::local_clock{});
			//创建文本日志
			if (isFileLog())
			{
				boost::shared_ptr< sinks::text_file_backend > backend =
					boost::make_shared< sinks::text_file_backend >(
						keywords::file_name = logFile(),
						keywords::target_file_name = "file_%5N.log",
						keywords::rotation_size = 5 * 1024 * 1024,
						keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
						);
				//自动写入到文件日志中
				backend->auto_flush(true);
				// Wrap it into the frontend and register in the core.

				if (async())
				{
					typedef sinks::asynchronous_sink< sinks::text_file_backend > sink_t;
					boost::shared_ptr< sink_t > sink(new sink_t(backend));
					sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= getLevel(logLevel()));
					sink->set_formatter(expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " << expr::attr<trivial::severity_level>("Severity") << ": " << expr::smessage);
					logging::core::get()->add_sink(sink);
				}
				else
				{
					typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;
					boost::shared_ptr< sink_t > sink(new sink_t(backend));
					sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= getLevel(logLevel()));
					sink->set_formatter(expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " << expr::attr<trivial::severity_level>("Severity") << ": " << expr::smessage);
					logging::core::get()->add_sink(sink);
				}
			}
			//创建窗口日志
			{	
				boost::shared_ptr< sinks::text_ostream_backend > backend =
					boost::make_shared< sinks::text_ostream_backend >();

				backend->add_stream(
					boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));
				backend->auto_flush(true);
				

				if (async())
				{
					typedef sinks::asynchronous_sink< sinks::text_ostream_backend > sink_t;
					boost::shared_ptr< sink_t > sink(new sink_t(backend));
					sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= getLevel(logLevel()));
					sink->set_formatter(expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " << expr::attr<trivial::severity_level>("Severity") << ": " << expr::smessage);
					logging::core::get()->add_sink(sink);


					src::severity_logger<trivial::severity_level > _lg;
				}
				else
				{
					typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
					boost::shared_ptr< sink_t > sink(new sink_t(backend));
					sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= getLevel(logLevel()));
					sink->set_formatter(expr::stream << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << " " << expr::attr<trivial::severity_level>("Severity") << ": " << expr::smessage);
					logging::core::get()->add_sink(sink);
				}
			}

			m_pNotifyStream = new NotifyStream();
		}

		BoostLogSystem::~BoostLogSystem()
		{
			if (m_pNotifyStream)
			{
				delete m_pNotifyStream;
			}
			m_pNotifyStream = nullptr;
		}

		std::ostream& BoostLogSystem::logImpl(const EnLogLevel& level)
		{
			return m_pNotifyStream->setSeverityLevel(getLevel(level));
		}

	}

}

