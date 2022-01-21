cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_USE_RELATIVE_PATHS true)         #开启相对路径
set(GEARTH_DIR ${PROJECT_SOURCE_DIR}/../)  # 父路径

project(EarthCore)                    # 工程名称  必填
set(PROJECT_NAME EarthCore PARENT_SCOPE)
set(ThirdParty ${GEARTH_DIR}/3dParty)

set(CMAKE_CXX_STANDARD 11)   #C++ 11
set(CMAKE_DEBUG_POSTFIX "d") #增添debug后缀
set(CMAKE_INCLUDE_CURRENT_DIR ON) #当前文件路径

#预编译宏定义
add_definitions(-DCOMMON_EXPORTS)


#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/bin)


# 设置库包含目录（头文件目录）
include_directories(${GEARTH_DIR}/include) # 其他人头文件
include_directories(${GEARTH_DIR}/include/${PROJECT_NAME}) #自己的头文件

#设置库目录
link_directories(${GEARTH_DIR}/lib)


#当前文件信息
FILE(GLOB_RECURSE HEADER_FILES *.h *.hpp)
FILE(GLOB_RECURSE SRC_FILES *.cpp *.c)
FILE(GLOB_RECURSE FORMS *.ui)
FILE(GLOB_RECURSE RESOURCE_FILES *.qrc *.rc)

#头文件部分包含，不然VS类图无法生成类视图
FILE(GLOB_RECURSE EXTERN_HEADER_FILES "${GEARTH_DIR}/include/${PROJECT_NAME}/*.*h" "${GEARTH_DIR}/include/${PROJECT_NAME}/*.*hpp")


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

# 生成可执行文件
#add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES} ${FORMS} ${RESOURCE_FILES})
# 生成可执行文件SHARED STATIC 
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SRC_FILES} ${FORMS} ${RESOURCE_FILES} ${EXTERN_HEADER_FILES})

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


#链接本地个人库
target_link_libraries(${PROJECT_NAME} debug commond.lib)
target_link_libraries(${PROJECT_NAME} optimized common.lib)