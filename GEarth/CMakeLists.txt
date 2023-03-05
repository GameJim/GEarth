# set to 0 when not a release candidate, non zero means that any generated
# git tags will be treated as release candidates of given number
SET(OPENSCENEGRAPH_RELEASE_CANDIDATE 0)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ON )
SET_PROPERTY( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets" )

CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1 FATAL_ERROR)

if(COMMAND cmake_policy)
    # Works around warnings libraries linked against that don't
    # have absolute paths (e.g. -lpthreads)
    cmake_policy(SET CMP0003 NEW)

    # Works around warnings about escaped quotes in ADD_DEFINITIONS
    # statements.
    cmake_policy(SET CMP0005 NEW)

    # tell CMake to prefer CMake's own CMake modules when available
    # only available from cmake-2.8.4
    if("${CMAKE_VERSION}" VERSION_GREATER 2.8.3)
        cmake_policy(SET CMP0017 NEW)
    endif()

endif()
set(ROOT_NAME GEARTH)
PROJECT(${ROOT_NAME})

IF(WIN32)
    SET(${ROOT_NAME}_WINDOWING_SYSTEM "Win32" CACHE STRING "Windowing system type for graphics window creation; options: Win32 or None.")
ELSEIF (APPLE)
	SET(${ROOT_NAME}_WINDOWING_SYSTEM "Carbon" CACHE STRING "Windowing system type for graphics window creation; options: Win32 or None.")
ENDIF()



SET(${ROOT_NAME}_DEPENDENCY_DIR "" CACHE PATH "Folder to search for dependencies (e.g., vcpkg/installed/x64-windows)")
IF(${ROOT_NAME}_DEPENDENCY_DIR)
    LIST(APPEND CMAKE_INCLUDE_PATH ${${ROOT_NAME}_DEPENDENCY_DIR}/include)
    LIST(APPEND CMAKE_LIBRARY_PATH ${${ROOT_NAME}_DEPENDENCY_DIR}/lib)
ENDIF()

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${${ROOT_NAME}_SOURCE_DIR}/CMakeModules;${${ROOT_NAME}_SOURCE_DIR}/conan/release;${${ROOT_NAME}_DEPENDENCY_DIR}")


INCLUDE(CXX11)
check_for_cxx11_compiler(CXX11_AVAILABLE)

option(${ROOT_NAME}_BUILD_PLUGINS "Build the ${ROOT_NAME} PLUGINS" ON)

SET(CMAKE_DEBUG_POSTFIX  "d" CACHE STRING "add a Debug postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a Release postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a RelWithDebInfo postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a MinSizeRel postfix, usually empty on windows")

#依赖cmake
INCLUDE(Find3rdPartyDependencies)
INCLUDE(UtilityMacros)
INCLUDE(MacroUtils)
INCLUDE(conanbuildinfo)


#输出
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/bin)
#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
IF(WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
ELSE(WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
ENDIF(WIN32)
FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})        # For each configuration (Debug, Release, MinSizeRel... and/or anything the user chooses)
	STRING(TOUPPER "${CONF}" CONF)                # Go uppercase (DEBUG, RELEASE...)
	SET("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}/${CONF}")
	SET("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}/${CONF}")
	IF(WIN32)
		SET("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}/${CONF}")
	ELSE()
		SET("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}/${CONF}")
	ENDIF()
ENDFOREACH()

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${OUTPUT_LIBDIR} )

# may set these values without needing to manipulate the environment.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} CACHE STRING "Add additional search paths here. Use ; to separate multiple paths.")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} CACHE STRING "Add additional search paths here. Use ; to separate multiple paths.")
SET(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH}  CACHE STRING "Add additional search paths here. Use ; to separate multiple paths.")

SET(PLUGIN_PREFIX "")
SET(PLUGINS plugins)

#插件输出路径
IF(MSVC AND NOT MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_BINDIR}/${OSG_PLUGINS})
ENDIF(MSVC AND NOT MSVC_IDE)

if(CMAKE_CL_64)
	set(CMAKE_PREFIX_PATH $ENV{QTDIR_X64})
else()
	set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

# Dynamic vs Static Linking
OPTION(${ROOT_NAME}_BUILD_SHARED_LIBS "ON to build shared libraries; OFF to build static libraries." ON)
IF(${ROOT_NAME}_BUILD_SHARED_LIBS)
    SET(${ROOT_NAME}_DYNAMIC_OR_STATIC "SHARED")
ELSE()
    SET(${ROOT_NAME}_DYNAMIC_OR_STATIC "STATIC")
ENDIF()
OPTION(BUILD_WITH_BOOST "ON to build with boost; OFF to build with boost." ON)
IF(BUILD_WITH_BOOST)
    find_package(boost 1.79.0 REQUIRED)
ELSE()
    SET(${ROOT_NAME}_DYNAMIC_OR_STATIC "STATIC")
ENDIF()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(ui)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(applications)