#include "util/file_system.h"

namespace util
{
	DEFINE_SINGLETON_PLUGIN_CPP(FileSystem)

	FileSystem::FileSystem()
	{
		name() = "FileSystem";
	}
}

