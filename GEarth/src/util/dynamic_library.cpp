#include "util/dynamic_library.h"
#include "util/log_system.h"
#include "extensions.h"
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
	DynamicLibrary::DynamicLibrary(const std::string& name, HANDLE handle)
	{
		_name = name;
		_handle = handle;
		LOG_INFO << "Opened DynamicLibrary " << _name << std::endl;
	}

	DynamicLibrary::~DynamicLibrary()
	{
		if (_handle)
		{
			LOG_INFO << "Closing DynamicLibrary " << _name << std::endl;
#if defined(WIN32) && !defined(__CYGWIN__)
			FreeLibrary((HMODULE)_handle);
#elif defined(__APPLE__) && defined(APPLE_PRE_10_3)
			NSUnLinkModule(static_cast<NSModule>(_handle), FALSE);
#elif defined(__hpux)
			// fortunately, shl_t is a pointer
			shl_unload(static_cast<shl_t>(_handle));
#else // other unix
			dlclose(_handle);
#endif
		}
	}

	DynamicLibrary* DynamicLibrary::loadLibrary(const std::string& libraryName)
	{
		HANDLE handle = NULL;
		LOG_DEBUG << "DynamicLibrary::try to load library \"" << libraryName << "\"" << std::endl;

		std::string fullLibraryName = DynamicLibraryManager::instance().findLibraryFile(libraryName);
		if (!fullLibraryName.empty()) handle = getLibraryHandle(fullLibraryName); // try the lib we have found
		else handle = getLibraryHandle(libraryName); // haven't found a lib ourselves, see if the OS can find it simply from the library name.

		if (handle) return new DynamicLibrary(libraryName, handle);

		// else no lib found so report errors.
		LOG_INFO << "DynamicLibrary::failed loading \"" << libraryName << "\"" << std::endl;

		return NULL;
	}

	DynamicLibrary::HANDLE DynamicLibrary::getLibraryHandle(const std::string& libraryName)
	{
		HANDLE handle = NULL;

#if defined(WIN32) && !defined(__CYGWIN__)
#ifdef OSG_USE_UTF8_FILENAME
		handle = LoadLibraryW(convertUTF8toUTF16(libraryName).c_str());
#else
		handle = LoadLibrary(libraryName.c_str());
#endif
#elif defined(__APPLE__) && defined(APPLE_PRE_10_3)
		NSObjectFileImage image;
		// NSModule os_handle = NULL;
		if (NSCreateObjectFileImageFromFile(libraryName.c_str(), &image) == NSObjectFileImageSuccess) {
			// os_handle = NSLinkModule(image, libraryName.c_str(), TRUE);
			handle = NSLinkModule(image, libraryName.c_str(), TRUE);
			NSDestroyObjectFileImage(image);
		}
#elif defined(__hpux)
		// BIND_FIRST is necessary for some reason
		handle = shl_load(libraryName.c_str(), BIND_DEFERRED | BIND_FIRST | BIND_VERBOSE, 0);
		return handle;
#else // other unix

#if defined(__ANDROID__)
		// Library can be found in APK/lib/armeabi-v7a etc.
		// Should not be prefaced with './'
		std::string localLibraryName = libraryName;
#else
		// dlopen will not work with files in the current directory unless
		// they are prefaced with './'  (DB - Nov 5, 2003).
		std::string localLibraryName;
		if (libraryName == osgDB::getSimpleFileName(libraryName))
			localLibraryName = "./" + libraryName;
		else
			localLibraryName = libraryName;
#endif

		handle = dlopen(localLibraryName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
		if (handle == NULL)
		{
			if (fileExists(localLibraryName))
			{
				LOG_WARN << "Warning: dynamic library '" << libraryName << "' exists, but an error occurred while trying to open it:" << std::endl;
				LOG_WARN << dlerror() << std::endl;
			}
			else
			{
				LOG_INFO << "Warning: dynamic library '" << libraryName << "' does not exist (or isn't readable):" << std::endl;
				LOG_INFO << dlerror() << std::endl;
			}
		}
#endif
		return handle;
	}

	DynamicLibrary::PROC_ADDRESS DynamicLibrary::getProcAddress(const std::string& procName)
	{
		if (_handle == NULL) return NULL;
#if defined(WIN32) && !defined(__CYGWIN__)
		return util::convertPointerType<DynamicLibrary::PROC_ADDRESS, FARPROC>(GetProcAddress((HMODULE)_handle, procName.c_str()));
#elif defined(__APPLE__) && defined(APPLE_PRE_10_3)
		std::string temp("_");
		NSSymbol symbol;
		temp += procName;   // Mac OS X prepends an underscore on function names
		symbol = NSLookupSymbolInModule(static_cast<NSModule>(_handle), temp.c_str());
		return NSAddressOfSymbol(symbol);
#elif defined(__hpux)
		void* result = NULL;
		if (shl_findsym(reinterpret_cast<shl_t*>(&_handle), procName.c_str(), TYPE_PROCEDURE, result) == 0)
		{
			return result;
		}
		else
		{
			OSG_WARN << "DynamicLibrary::failed looking up " << procName << std::endl;
			OSG_WARN << "DynamicLibrary::error " << strerror(errno) << std::endl;
			return NULL;
		}
#else // other unix
		void* sym = dlsym(_handle, procName.c_str());
		if (!sym) {
			OSG_WARN << "DynamicLibrary::failed looking up " << procName << std::endl;
			OSG_WARN << "DynamicLibrary::error " << dlerror() << std::endl;
		}
		return sym;
#endif
	}



	DynamicLibraryManager::DynamicLibraryManager()
	{
		
	}

	DynamicLibraryManager::~DynamicLibraryManager()
	{
		dynamicLibrarys().clear();
	}

	void DynamicLibraryManager::destoryInstance()
	{
		instance().dynamicLibrarys().clear();
	}

	bool DynamicLibraryManager::registerDynamicLibrary(const std::string& sName, const std::string& sPath)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		auto itor = dynamicLibraryInfo.find(sName);
		if (itor != dynamicLibraryInfo.end())
		{
			LOG_WARNING << "library " << sName << "was exist";
			return false;
		}
		dynamicLibraryInfo.insert({ sName,sPath });
		return false;
	}

	bool DynamicLibraryManager::activeDynamicLibrary(const std::string& sName)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		
		for (auto& dynamicLibrary : _dynamicLibrarys)
		{
			if (dynamicLibrary->getName() == sName)
			{
				LOG_WARNING << "library " << sName << "was actived";
				return false;
			}
		}
		lock.unlock();
		//从列表中集火
		std::shared_ptr<DynamicLibrary> pDynamicLibrary(DynamicLibrary::loadLibrary(sName));
		
		if (!pDynamicLibrary)
		{
			LOG_WARNING << "library " << sName << "was failed to actived, please check the name or path of dynamicLibrary ";
			return false;
		}

		_dynamicLibrarys.push_back(pDynamicLibrary);
		return true;
	}

	bool DynamicLibraryManager::inactiveDynamicLibrary(const std::string& sName)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		for (auto itr= _dynamicLibrarys.begin() ; itr!= _dynamicLibrarys.end();itr++)
		{
			if ((*itr)->getName() == sName)
			{
				_dynamicLibrarys.erase(itr);
				return true;
			}
		}
		LOG_WARNING << "library " << sName << "was failed to inactived";
		return false;
	}

	std::string DynamicLibraryManager::findLibraryFile(const std::string& libraryName)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		auto itor = dynamicLibraryInfo.find(libraryName);
		if (itor != dynamicLibraryInfo.end())
		{
			return itor->second;
		}
		return "";
	}

}



