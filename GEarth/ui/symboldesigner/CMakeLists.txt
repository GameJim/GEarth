cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_USE_RELATIVE_PATHS true)         #开启相对路径
set(GEARTH_DIR ${PROJECT_SOURCE_DIR}/../)  # 父路径

project(SymbolDesigner)                    # 工程名称  必填
set(PROJECT_NAME SymbolDesigner PARENT_SCOPE)

set(CMAKE_CXX_STANDARD 11)   #C++ 11
set(CMAKE_DEBUG_POSTFIX "d") #增添debug后缀
set(CMAKE_INCLUDE_CURRENT_DIR ON) #当前文件路径

#预编译宏定义
add_definitions(-DCOMMON_EXPORTS)
#add_definitions(-DUNICODE -D_UNICODE)




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



#是否开启QT模块
option(USE_QT "option for debug" ON)
if (USE_QT)
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


#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/bin)