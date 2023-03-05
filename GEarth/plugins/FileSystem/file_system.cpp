#include "file_system.h"
#include "util/register_helper.h"
#include "boost/filesystem.hpp"
//using namespace boost::filesystem;
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

		
		bool BoostFileSystem::fileExists(const std::string& sPath)
		{
			return boost::filesystem::exists(sPath);
		}

		std::string BoostFileSystem::fileStem(const std::string& sPath)
		{
			boost::filesystem::path path = sPath;
			return path.stem().string();
		}

		std::string BoostFileSystem::fileName(const std::string& sPath)
		{
			boost::filesystem::path path = sPath;
			return path.extension().string();
			return path.filename().string();
		}

		std::string BoostFileSystem::fileExtension(const std::string& sPath)
		{
			boost::filesystem::path  path = sPath;
			return path.extension().string();
		}

		std::string BoostFileSystem::parentPath(const std::string& sPath)
		{
			boost::filesystem::path path = sPath;
			return path.extension().string();
			return path.parent_path().string();
		}

		bool BoostFileSystem::isAbsPath(const std::string& sPath)
		{
			boost::filesystem::path path = sPath;
			return path.is_absolute();
		}

		std::string BoostFileSystem::searchFile(std::vector<std::string>& sRoots, const std::string& sRelatviePath)
		{
			for (auto& root :sRoots)
			{
				if (fileExists(root + "/" + sRelatviePath))
				{
					return root + "/" + sRelatviePath;
				}
			}

			return "";
		}
	}
}

