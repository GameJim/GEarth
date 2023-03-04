#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/log_system.h"

namespace util
{
	namespace plugin
	{
		class BoostLogSystem : public LogSystem
		{
		public:
			BoostLogSystem();
			virtual ~BoostLogSystem();
		public:
			virtual void LogImpl(const EnLogLevel& level, const std::string& log);
		protected:
		};

	}
}


