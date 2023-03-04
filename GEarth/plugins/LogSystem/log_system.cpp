#include "log_system.h"
#include "util/register_helper.h"
namespace util
{
	namespace plugin
	{

		BoostLogSystem::BoostLogSystem()
		{
			driver() = "boost";
		}

		BoostLogSystem::~BoostLogSystem()
		{

		}

		void BoostLogSystem::LogImpl(const EnLogLevel& level, const std::string& log)
		{

		}
		

		REGISTER_DEFINE_SINGLETON_PLUGIN(LogSystem, BoostLogSystem)
	}

}

