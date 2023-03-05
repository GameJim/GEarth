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
		strrchr:�������ܣ�����һ���ַ�c����һ���ַ���str��ĩ�γ��ֵ�λ�ã�Ҳ���Ǵ�str���Ҳ࿪ʼ�����ַ�c�״γ��ֵ�λ�ã���
		���������λ�õĵ�ַ�����δ���ҵ�ָ���ַ�����ô����������NULL��
		ʹ�������ַ���ش����һ���ַ�c��strĩβ���ַ�����
		*/
		(strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�//
		std::string path = szFilePath;
		return path;
#elif
#endif
		return "";
	}

}

