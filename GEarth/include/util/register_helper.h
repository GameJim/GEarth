#pragma once
#include "util/singleton_plugin.h"

namespace util
{
	template<class T2, class T>
	class RegisterSingletonPluginProxy 
	{
		using BaseName = T2;
	public:
		RegisterSingletonPluginProxy()
		{
			_plugin = std::make_unique<T>();
			SingletonPluginManager::Instance().addPlugin(_plugin.get());
			//同时激活当前
			BaseName::setCurrent(dynamic_cast<BaseName*>(_plugin.get()));
		}

		~RegisterSingletonPluginProxy()
		{
			if (BaseName::Instance() == _plugin.get())
			{
				//启动切换久的插件，此处暂不考虑
				BaseName::setCurrent(nullptr);
			}
			SingletonPluginManager::Instance().removePlugin(_plugin.get());
		}

		T* get() { return _plugin.get(); }

	protected:
		std::unique_ptr<SingletonPlugin> _plugin;
	};

}
