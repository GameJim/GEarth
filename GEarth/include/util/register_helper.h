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
			//ͬʱ���ǰ
			BaseName::setCurrent(dynamic_cast<BaseName*>(_plugin.get()));
		}

		~RegisterSingletonPluginProxy()
		{
			if (BaseName::Instance() == _plugin.get())
			{
				//�����л��õĲ�����˴��ݲ�����
				BaseName::setCurrent(nullptr);
			}
			SingletonPluginManager::Instance().removePlugin(_plugin.get());
		}

		T* get() { return _plugin.get(); }

	protected:
		std::unique_ptr<SingletonPlugin> _plugin;
	};

}
