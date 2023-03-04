#include "util/singleton_plugin.h"

namespace util
{

	SingletonPlugin::SingletonPlugin()
		:_driver("")
		, _name("")
	{

	}

	bool SingletonPluginManager::addPlugin(SingletonPlugin* plugin)
	{
		if (!plugin) return false;
		
		std::unique_lock<std::mutex> lock(m_Mutex);
		_plugins.push_back(plugin);

		return true;
	}

	bool SingletonPluginManager::removePlugin(SingletonPlugin* plugin)
	{
		if (!plugin) return false;

		std::unique_lock<std::mutex> lock(m_Mutex);
		for (auto itor = _plugins.begin(); itor != _plugins.end(); itor++)
		{
			if (*itor == plugin)
			{
				_plugins.erase(itor);
				break;;
			}
		}

		return true;
	}

}

