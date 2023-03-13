#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/file_system.h"
#include "util/register_helper.h"
namespace util
{
	namespace plugin
	{
		class BoostFileSystem : public FileSystem
		{
		public:
			BoostFileSystem();
			virtual ~BoostFileSystem();
		public:
			virtual bool fileExists(const std::string& sPath);
			virtual std::string fileStem(const std::string& sPath);
			virtual std::string fileName(const std::string& sPath);
			virtual std::string fileExtension(const std::string& sPath);
			virtual std::string parentPath(const std::string& sPath);
			virtual bool isAbsPath(const std::string& sPath);
			virtual std::string searchFile(std::vector<std::string>& sRoots, const std::string& sRelatviePath);
		protected:
		};
		REGISTER_DEFINE_SINGLETON_PLUGIN(FileSystem, BoostFileSystem)
	}
}


