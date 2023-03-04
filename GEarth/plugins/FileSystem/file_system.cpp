#include "file_system.h"
#include "util/register_helper.h"
namespace util
{
	namespace plugin
	{

		BoostFileSystem::BoostFileSystem()
		{
			driver() = "boost";
		}

		BoostFileSystem::~BoostFileSystem()
		{

		}

		
		bool BoostFileSystem::fileExist(const std::string& sPath)
		{
			return false;
		}

		std::string BoostFileSystem::fileName(const std::string& sPath)
		{
			return "";
		}

		std::string BoostFileSystem::fileExtension(const std::string& sPath)
		{
			return "";
		}

		std::string BoostFileSystem::parentPath(const std::string& sPath)
		{
			return "";
		}

		bool BoostFileSystem::isAbsPath(const std::string& sPath)
		{
			return false;
		}

		std::string BoostFileSystem::appendPath(const std::string& sRoot, const std::string& sPath)
		{
			return "";
		}


		REGISTER_DEFINE_SINGLETON_PLUGIN(FileSystem, BoostFileSystem)
	}
}

