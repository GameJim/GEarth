#pragma once
//learn osg 
#include "util/util_export.h"
#include "util/define_helper.h"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <mutex>
namespace util
{
	class UTIL_API DynamicLibrary
	{
	public:

		typedef void*   HANDLE;
		typedef void*   PROC_ADDRESS;

		/** returns a pointer to a DynamicLibrary object on successfully
		* opening of library returns NULL on failure.
		*/
		static DynamicLibrary* loadLibrary(const std::string& libraryName);

		/** return name of library stripped of path.*/
		const std::string& getName() const { return _name; }

		/** return name of library including full path to it.*/
		const std::string& getFullName() const { return _fullName; }

		/** return handle to .dso/.dll dynamic library itself.*/
		HANDLE             getHandle() const { return _handle; }

		/** return address of function located in library.*/
		PROC_ADDRESS       getProcAddress(const std::string& procName);

		virtual ~DynamicLibrary();
	protected:

		/** get handle to library file */
		static HANDLE getLibraryHandle(const std::string& libraryName);

		/** disallow default constructor.*/
		DynamicLibrary() = default;
		/** disallow copy constructor.*/
		DynamicLibrary(const DynamicLibrary&) {};
		/** disallow copy operator.*/
		DynamicLibrary& operator = (const DynamicLibrary&) { return *this; }

		/** Disallow public construction so that users have to go
		* through loadLibrary() above which returns NULL on
		* failure, a valid DynamicLibrary object on success.
		*/
		DynamicLibrary(const std::string& name, HANDLE handle);
		

		HANDLE          _handle;
		std::string     _name;
		std::string     _fullName;
	};


	class UTIL_API DynamicLibraryManager
	{
		DEFINE_SINGLETON(DynamicLibraryManager)
		DEFINE_OPTION_VECTOR(std::shared_ptr<DynamicLibrary>, dynamicLibrarys)
	public:
		DynamicLibraryManager();
		virtual ~DynamicLibraryManager();
		bool registerDynamicLibrary(const std::string& sName, const std::string& sPath);
		bool activeDynamicLibrary(const std::string& sName);
		bool inactiveDynamicLibrary(const std::string& sName);
		
		std::string findLibraryFile(const std::string& libraryName);
	protected:
		std::mutex m_Mutex;
		std::map<std::string, std::string> dynamicLibraryInfo;  //¿âÃû³Æ£¬¿âÂ·¾¶
	};
	
}
