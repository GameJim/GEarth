project(projectGenerator)                   
set(PROJECT_NAME projectGenerator PARENT_SCOPE)

set(CMAKE_INCLUDE_CURRENT_DIR ON) #当前文件路径

#设置输出目录,必须放在前面
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/lib/${ARCH}/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/lib/${ARCH}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/bin/${ARCH}/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/lib/${ARCH}/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/lib/${ARCH}/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/bin/${ARCH}/release)

add_definitions(-DAPI_EXPORTS)
# 设置库包含目录（头文件目录）
include_directories(${ROOT_DIR}/include) # 其他人头文件



#当前文件信息
FILE(GLOB_RECURSE HEADER_FILES *.h *.hpp)
FILE(GLOB_RECURSE SRC_FILES *.cpp *.c)
FILE(GLOB_RECURSE FORMS *.ui)
FILE(GLOB_RECURSE RESOURCE_FILES *.qrc *.rc)
#在include中头文件
FILE(GLOB_RECURSE EXTERN_HEADER_FILES "${ROOT_DIR_INCLUDE}/${PROJECT_NAME}/*.*h" "${ROOT_DIR_INCLUDE}/${PROJECT_NAME}/*.*hpp")

include_directories(${ROOT_DIR}/3rdparty) 

#设置库目录
link_directories(${ROOT_DIR}/lib)

#QT 环境
find_path(QTITAN_INCLUDE QtitanRibbon.h ${TDPARTY_ROOT}/Qtitan/include NO_DEFAULT_PATH)
find_path(QTITAN_DLL qtnribbon4.dll ${TDPARTY_ROOT}/Qtitan/lib/X64 NO_DEFAULT_PATH)
find_path(QTITAN_LIB qtnribbon4.lib ${TDPARTY_ROOT}/Qtitan/lib/X64 NO_DEFAULT_PATH)

if(CMAKE_CL_64)
	set(QT_DIR $ENV{QTDIR_X64})
else()
	set(QT_DIR $ENV{QTDIR})
endif()


if(POLICY CMP0990)
  cmake_policy(SET CMP0990 NEW)
endif()


if(CMAKE_CL_64)
	set(CMAKE_PREFIX_PATH $ENV{QTDIR_X64})
else()
	set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${QTITAN_INCLUDE}) 
link_directories(${QTITAN_LIB}) 

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

#include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
#add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(FORMS)
	QT5_WRAP_UI(FORMS_UIC ${FORMS})
ENDIF()

#message(MOC_FILES)
#message(${HEADER_FILES})
#将包含Q_OBJECT的头文件编译成moc_*.cpp文件
#QT5_WRAP_CPP(MOC_FILES ${HEADER_FILES})

#启用qrc资源文件
QT5_ADD_RESOURCES(RES_FILES RESOURCE_FILES)

#将ui文件和生成文件整理在一个文件夹中
SOURCE_GROUP("Form Files" FILES ${FORMS})
#将qrc资源文件整理在一个文件夹中
SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})

source_group("moc" FILES ${MOC_SRC})


add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SRC_FILES} ${FORMS} ${RESOURCE_FILES} ${EXTERN_HEADER_FILES})

#拷贝资源
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
	${ROOT_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/res/ui/${PROJECT_NAME}
	
COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
	${ROOT_DIR}/res/ui/${PROJECT_NAME}                   # <--this is in-file
	${CMAKE_CURRENT_BINARY_DIR}/res/ui/${PROJECT_NAME})                 # <--this is out-file path

#拷贝相关dll
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)

		
		#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Cored.dll                 # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Guid.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Widgetsd.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path


#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Core.dll                 # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		#拷贝对应的dll
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Gui.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QT_DIR}/bin/Qt5Widgets.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		
		

target_link_libraries(${PROJECT_NAME} debug ${QTITAN_LIB}/qtnribbond4.lib)
target_link_libraries(${PROJECT_NAME} optimized ${QTITAN_LIB}/qtnribbon4.lib)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QTITAN_LIB}/qtnribbond4.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		
		
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
		${QTITAN_LIB}/qtnribbon4.dll                # <--this is in-file
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		
		
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME} TARGETS
        LIBRARY DESTINATION ${ROOT_DIR}/install/${ARCH}/${PROJECT_NAME}/lib
        ARCHIVE DESTINATION ${ROOT_DIR}/install/${ARCH}/${PROJECT_NAME}/lib
        RUNTIME DESTINATION ${ROOT_DIR}/install/${ARCH}/${PROJECT_NAME}/bin)	
		
install(FILES ${EXTERN_HEADER_FILES} DESTINATION  ${ROOT_DIR}/install/${ARCH}/${PROJECT_NAME}/include/${PROJECT_NAME} COMPONENT devel) 

