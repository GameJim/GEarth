#include "util/file_system.h"

#ifdef __APPLE__
#include <AvailabilityMacros.h>
#if !defined(MAC_OS_X_VERSION_10_3) || (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_3)
#define APPLE_PRE_10_3
#endif
#endif

#if defined(WIN32) && !defined(__CYGWIN__)
#include <io.h>
#include <windows.h>
#include <winbase.h>
#elif defined(__APPLE__) && defined(APPLE_PRE_10_3)
#include <mach-o/dyld.h>
#else // all other unix
#include <unistd.h>
#ifdef __hpux
// Although HP-UX has dlopen() it is broken! We therefore need to stick
// to shl_load()/shl_unload()/shl_findsym()
#include <dl.h>
#include <errno.h>
#else
#include <dlfcn.h>
#endif
#endif
namespace util
{
	DEFINE_SINGLETON_PLUGIN_CPP(FileSystem)

	FileSystem::FileSystem()
	{
		name() = "FileSystem";
	}

	std::string FileSystem::GetModulePath()
	{
#if defined(WIN32) && !defined(__CYGWIN__)
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		/*
		strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
		并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
		使用这个地址返回从最后一个字符c到str末尾的字符串。
		*/
		(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
		std::string path = szFilePath;
		return path;
#elif
#endif
		return "";
	}

}

