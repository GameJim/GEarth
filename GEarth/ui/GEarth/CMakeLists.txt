cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_USE_RELATIVE_PATHS true)         #开启相对路径
set(GEARTH_DIR ${PROJECT_SOURCE_DIR}/../)  # 父路径

project(GEarth)                    # 工程名称  必填
set(PROJECT_NAME GEarth PARENT_SCOPE)

set(CMAKE_CXX_STANDARD 11)   #C++ 11
set(CMAKE_DEBUG_POSTFIX "d") #增添debug后缀
set(CMAKE_INCLUDE_CURRENT_DIR ON) #当前文件路径

#预编译宏定义
add_definitions(-DCOMMON_EXPORTS)
#add_definitions(-DUNICODE -D_UNICODE)


#设置输出目录,必须放在前面
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/bin)


# 设置库包含目录（头文件目录）
include_directories(${GEARTH_DIR}/include) # 其他人头文件
include_directories(${GEARTH_DIR}/ui) 
include_directories(${GEARTH_DIR}/ui/${PROJECT_NAME}) #自己的头文件

#设置库目录
link_directories(${GEARTH_DIR}/lib)


#当前文件信息
FILE(GLOB_RECURSE HEADER_FILES *.h *.hpp)
FILE(GLOB_RECURSE SRC_FILES *.cpp *.c)
FILE(GLOB_RECURSE FORMS *.ui)
FILE(GLOB_RECURSE RESOURCE_FILES *.qrc *.rc)

#按目录结构加入文件
# macro(create_filters source_files)
    # if(MSVC)
        # # 获取当前目录
        # set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
        # foreach(src_file ${${source_files}})
            # # 求出相对路径
            # string(REPLACE ${current_dir}/ "" rel_path_name ${src_file})
            # # 删除相对路径中的文件名部分
            # string(REGEX REPLACE "(.*)/.*" \\1 rel_path ${rel_path_name})
            # # 比较是否是当前路径下的文件
            # string(COMPARE EQUAL ${rel_path_name} ${rel_path} is_same_path)
            # # 替换成Windows平台的路径分隔符
            # string(REPLACE "/" "\\" rel_path ${rel_path})
            # if(is_same_path)
                # set(rel_path "\\")
            # endif(is_same_path)
 
            # # CMake 命令
            # source_group(${rel_path} FILES ${src_file})
        # endforeach(src_file)
    # endif(MSVC)
# endmacro(create_filters)

# create_filters(HEADER_FILES)
# create_filters(SRC_FILES)


#是否开启QT模块
option(USE_QT "option for debug" ON)
if (USE_QT)
if(POLICY CMP0990)
  cmake_policy(SET CMP0990 NEW)
endif()

set(CMAKE_AUTOMOC ON)  #打开全局moc,设置自动生成moc文件
#导入qt模块
if(CMAKE_CL_64)
	set(CMAKE_PREFIX_PATH $ENV{QTDIR_X64})
else()
	set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(FORMS)
	QT5_WRAP_UI(FORMS_UIC ${FORMS})
ENDIF()


#将包含Q_OBJECT的头文件编译成moc_*.cpp文件
QT5_WRAP_CPP(MOC_FILES ${HEADER_FILES})
 
#启用qrc资源文件
QT5_ADD_RESOURCES(RES_FILES RESOURCE_FILES)


#将ui文件和生成文件整理在一个文件夹中
SOURCE_GROUP("Form Files" FILES ${FORMS})
#将qrc资源文件整理在一个文件夹中
SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})
endif()


#链接三方库
function(import_3dParty NAME HEADER_PATH LIB_PATH)
#头文件
string(TOUPPER ${NAME} NAME) 
string(CONCAT INCLUDE_PATH ${NAME}_ "INCLUDE_DIR")
find_file(${INCLUDE_PATH} ${HEADER_PATH} ${GEARTH_DIR}/3dParty/${NAME} NO_DEFAULT_PATH)
include_directories(${${INCLUDE_PATH}}) 

#dll路径
string(CONCAT DLL_PATH ${NAME}_ "LIB_DIR")
find_file(${DLL_PATH} ${LIB_PATH} ${GEARTH_DIR}/3dParty/${NAME} NO_DEFAULT_PATH)
link_directories(${${DLL_PATH}}) 
endfunction()


#链接三方库
option(USE_QTITAN "option for debug" ON)
if(USE_QTITAN)
import_3dParty("Qtitan" include lib/X64)
else()
endif()


#引入库目录路径
link_directories(${CMAKE_BINARY_DIR}/x64)



#链接三方库
function(inlcluded_3dParty NAME HEADER_PATH LIB_PATH)
#头文件
string(TOUPPER ${NAME} NAME) 
string(CONCAT INCLUDE_PATH ${NAME}_ "INCLUDE_DIR")
find_file(${INCLUDE_PATH} ${HEADER_PATH} ${GEARTH_DIR}/3dParty/${NAME} NO_DEFAULT_PATH)
include_directories(${${INCLUDE_PATH}}) 

#dll路径
string(CONCAT DLL_PATH ${NAME}_ "LIB_DIR")
find_file(${DLL_PATH} ${LIB_PATH} ${GEARTH_DIR}/3dParty/${NAME} NO_DEFAULT_PATH)
link_directories(${${DLL_PATH}}) 
endfunction()


inlcluded_3dParty("OSG3rdParty" include  lib)
inlcluded_3dParty("OSG" include  lib)
inlcluded_3dParty("gdal" include  lib)
inlcluded_3dParty("OSGEarth" include  lib)



#lib库和bin
function(link_thirdParty NAME)
	file(GLOB_RECURSE LIBS_NAMES ${ThirdParty}/${NAME}/lib/*.lib)
	foreach(LIB_NAME ${LIBS_NAMES})
		target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
	endforeach()
	
	file(GLOB_RECURSE DLL_NAMES ${ThirdParty}/${NAME}/bin/*.dll)
	foreach(DLL_NAME ${DLL_NAMES})
		#dll拷贝直输出目录
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
				${DLL_NAME}                   # <--this is in-file
				$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
	endforeach()
endfunction()


#链接三方库
link_thirdParty(OSG3rdParty)
link_thirdParty(OSG)
link_thirdParty(gdal)
link_thirdParty(OSGEarth)




# 生成可执行文件
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES} ${FORMS} ${RESOURCE_FILES})
# 生成可执行文件SHARED STATIC 
#add_library(${PROJECT_NAME} SHARED  ${SRC_FILES} ${RC_FILES} ${UI_FILE})


#link_libraries
if (USE_QT) #QT
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)

#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Cored.dll                 # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Guid.dll                # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Widgetsd.dll                # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path


#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Core.dll                 # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Gui.dll                # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_PREFIX_PATH}/bin/Qt5Widgets.dll                # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
else()
endif()

#3dParty
function(import_3dPartylib NAME lib_debug lib_release dll_debug dll_release)
string(TOUPPER ${NAME} NAME) 
string(CONCAT DLL_PATH ${NAME}_ "LIB_DIR")
#debug
string(CONCAT LIB_DEBUG ${NAME}_ "LIB_DEBUG")
find_library(${LIB_DEBUG} ${lib_debug} ${${DLL_PATH}})
target_link_libraries(${PROJECT_NAME} debug ${${LIB_DEBUG}})

#dll拷贝直输出目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${${DLL_PATH}}/${dll_debug}.dll                   # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
#dll拷贝直输出目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${${DLL_PATH}}/${dll_debug}.pdb                   # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path


#release
string(CONCAT LIB_RELEASE ${NAME}_ "LIB_RELEASE")
find_library(${LIB_RELEASE} ${lib_release} ${${DLL_PATH}})
target_link_libraries(${PROJECT_NAME} optimized ${${LIB_RELEASE}})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${${DLL_PATH}}/${dll_release}.dll                   # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path

endfunction()

if(USE_QTITAN)
	import_3dPartylib("Qtitan" qtnribbond4.lib qtnribbon4.lib qtnribbond4 qtnribbon4)
else()
endif()



#关闭控制台
option(USE_CONSOLE_LOG "option for debug" ON)
if(USE_CONSOLE_LOG)
else()
	if(WIN32)
			if(MSVC)
					set_target_properties(${PROJECT_NAME} PROPERTIES
							WIN32_EXECUTABLE YES
							LINK_FLAGS "/ENTRY:mainCRTStartup"
					)
			elseif(CMAKE_COMPILER_IS_GNUCXX)
							SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
			else()
					message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
			endif(MSVC)
	elseif(APPLE)
			set_target_properties(${PROJECT_NAME} PROPERTIES
							MACOSX_BUNDLE YES
			)
	elseif(UNIX)
			# Nothing special required
	else()
			message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
	endif(WIN32)
endif(USE_CONSOLE_LOG)

#拷贝res
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
        ${GEARTH_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/ui/${PROJECT_NAME}
		
	COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
        ${GEARTH_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
        ${CMAKE_CURRENT_BINARY_DIR}/res/ui/${PROJECT_NAME})                 # <--this is out-file path
		
		
		#lib库和bin
function(link_thirdParty NAME)
	file(GLOB_RECURSE LIBS_NAMES ${ThirdParty}/${NAME}/lib/*.lib)
	foreach(LIB_NAME ${LIBS_NAMES})
		target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
	endforeach()
	
	file(GLOB_RECURSE DLL_NAMES ${ThirdParty}/${NAME}/bin/*.dll)
	foreach(DLL_NAME ${DLL_NAMES})
		#dll拷贝直输出目录
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
				${DLL_NAME}                   # <--this is in-file
				$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
	endforeach()
endfunction()


#链接三方库
link_thirdParty(OSG3rdParty)
link_thirdParty(OSG)
link_thirdParty(gdal)
link_thirdParty(OSGEarth)
		
target_link_libraries(${PROJECT_NAME} debug SymbolSystemd.lib)
target_link_libraries(${PROJECT_NAME} optimized SymbolSystem.lib)

target_link_libraries(${PROJECT_NAME} debug commond.lib)
target_link_libraries(${PROJECT_NAME} optimized common.lib)


target_link_libraries(${PROJECT_NAME} debug EarthCored.lib)
target_link_libraries(${PROJECT_NAME} optimized EarthCore.lib)


#构建后cmake目录
#message(${CMAKE_CURRENT_BINARY_DIR})

# 64位
# if (CMAKE_CL_64)
    # # 设置 可执行程序输出目录
	# set(publish_bin_debug 			${CMAKE_CURRENT_SOURCE_DIR}/publish/x64/bin/debug64)
	# set(publish_bin_release 		${CMAKE_CURRENT_SOURCE_DIR}/publish/x64/bin/release64)

    # # 设置库文件输出目录
	# set(publish_lib_debug 			${CMAKE_CURRENT_SOURCE_DIR}/publish/x64/lib/debug64)
	# set(publish_lib_release 		${CMAKE_CURRENT_SOURCE_DIR}/publish/x64/lib/release64)

	# set(PROJECT_NAME 				${PROJECT_NAME}64)
# # 32
# else()
 # # 设置 可执行程序输出目录
	# set(publish_bin_debug 			${CMAKE_CURRENT_SOURCE_DIR}/publish/x86/bin/debug)
	# set(publish_bin_release 		${CMAKE_CURRENT_SOURCE_DIR}/publish/x86/bin/release)

 # # 设置库文件输出目录
	# set(publish_lib_debug 			${CMAKE_CURRENT_SOURCE_DIR}/publish/x86/lib/debug)
	# set(publish_lib_release 		${CMAKE_CURRENT_SOURCE_DIR}/publish/x86/lib/release)
# endif()


# 指定可执行程序输出目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${publish_bin_debug})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${publish_bin_release})

# # 指定 库文件输出目录
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${publish_lib_debug})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE 	${publish_lib_release})
