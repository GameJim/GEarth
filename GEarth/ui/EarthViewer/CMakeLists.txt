cmake_minimum_required(VERSION 3.5)

project(EarthViewer)                   
set(PROJECT_NAME EarthViewer PARENT_SCOPE)


set(CMAKE_INCLUDE_CURRENT_DIR ON) #当前文件路径

#预编译宏定义
add_definitions(-DCOMMON_EXPORTS)
#add_definitions(-DUNICODE -D_UNICODE)


#设置输出目录,必须放在前面
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/bin/release)


# 设置库包含目录（头文件目录）
include_directories(${GEARTH_DIR}/include) # 其他人头文件
include_directories(${GEARTH_DIR}/ui) 
include_directories(${GEARTH_DIR}/ui/${PROJECT_NAME}) #自己的头文件

#设置库目录
link_directories(${GEARTH_DIR}/lib)

#exe程序要么控制台，要么QT界面
set(QT_UI TURE)

if (QT_UI)
	#当前文件信息
	FILE(GLOB_RECURSE HEADER_FILES *.h *.hpp)
	FILE(GLOB_RECURSE SRC_FILES *.cpp *.c)
	FILE(GLOB_RECURSE FORMS *.ui)
	FILE(GLOB_RECURSE RESOURCE_FILES *.qrc *.rc)
	
	
		
	INCLUDE_QT()
	set(CMAKE_AUTOMOC ON)  #打开全局moc,设置自动生成moc文件
else()
	FILE(GLOB_RECURSE HEADER_FILES *.h *.hpp)
	FILE(GLOB_RECURSE SRC_FILES *.cpp *.c)
endif()


#设置库目录
link_directories(${GEARTH_DIR}/lib)
INCLUDE_THIRDPARTY(freetype)



if(QT_UI)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES} ${FORMS} ${RESOURCE_FILES})
LINK_QT()
else()
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES})
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
	${GEARTH_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/res/ui/${PROJECT_NAME}
	
COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
	${GEARTH_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
	${CMAKE_CURRENT_BINARY_DIR}/res/ui/${PROJECT_NAME})                 # <--this is out-file path

LINK_SRC_LIB(common)
LINK_SRC_LIB(SymbolSystem)
LINK_SRC_LIB(EarthCore)
LINK_THIRDPARTY(freetype freetype NONE)


