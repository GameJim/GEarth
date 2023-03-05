#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include <string>
#include <vector>
#include <mutex>
namespace util
{
	class UTIL_API SingletonPlugin
	{
		DEFINE_PROPERTY(std::string,name)
		DEFINE_PROPERTY(std::string,driver)
	protected:
		SingletonPlugin();
	public:
		virtual ~SingletonPlugin() = default;
		
	};


	class UTIL_API SingletonPluginManager
	{
		DEFINE_SINGLETON(SingletonPluginManager)
		DEFINE_OPTION_VECTOR(SingletonPlugin*,plugins)
	public:
		SingletonPluginManager() = default;
		~SingletonPluginManager() = default;
		bool addPlugin(SingletonPlugin* plugin);
		bool removePlugin(SingletonPlugin* plugin);
	protected:
		std::mutex m_Mutex;
	};
}