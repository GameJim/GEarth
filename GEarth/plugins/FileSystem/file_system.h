#pragma once
#include "util/util_export.h"
#include "util/define_helper.h"
#include "util/file_system.h"

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
			virtual bool fileExist(const std::string& sPath);
			virtual std::string fileName(const std::string& sPath);
			virtual std::string fileExtension(const std::string& sPath);
			virtual std::string parentPath(const std::string& sPath);
			virtual bool isAbsPath(const std::string& sPath);
			virtual std::string appendPath(const std::string& sRoot, const std::string& sPath);
		protected:
		};

	}
}


