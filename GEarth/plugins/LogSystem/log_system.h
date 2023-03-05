#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/log_system.h"
#include "util/register_helper.h"


namespace util
{
	namespace plugin
	{
		class BoostLogSystem : public LogSystem
		{
		public:
			class NotifyStream;
			
			BoostLogSystem();
			virtual ~BoostLogSystem();
		public:
			virtual std::ostream& logImpl(const EnLogLevel& level);
		protected:
			NotifyStream* m_pNotifyStream;
		};
		REGISTER_DEFINE_SINGLETON_PLUGIN(LogSystem, BoostLogSystem)
	}
}


