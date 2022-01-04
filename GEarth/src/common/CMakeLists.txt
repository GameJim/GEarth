cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_USE_RELATIVE_PATHS true)
# 父路径
set(GEARTH_DIR ${PROJECT_SOURCE_DIR}/../)
message("${GEARTH_DIR}")
project (common)
set(PROJECT_NAME common)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_DEBUG_POSTFIX "d") #增添debug后缀
#设置输出目录,仅支持64
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GEARTH_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEARTH_DIR}/bin)

# 添加预编译宏定义
add_definitions(-DCOMMON_EXPORTS)

# 设置库包含目录（头文件目录）
include_directories(${GEARTH_DIR}/include) # 其他人头文件
include_directories(${GEARTH_DIR}/include/${PROJECT_NAME}) #自己的头文件
include_directories(${GEARTH_DIR}/3dParty) # 第三方头文件

#文件
file(GLOB_RECURSE SRC_FILES *.cpp *.c *.h *.hpp)
file(GLOB_RECURSE RC_FILES *.rc *.qrc)
file(GLOB_RECURSE UI_FILE *.ui) #qt recource

# 生成可执行文件
#add_executable(${SRC_FILES} ${RC_FILES} ${UI_FILE})
# 生成可执行文件SHARED STATIC 
add_library(${PROJECT_NAME} SHARED  ${SRC_FILES} ${RC_FILES} ${UI_FILE})

# 设置库目录
link_directories(${GEARTH_DIR}/lib)

# 链接库
#debug
find_library(FREETYPE_LIB_DEBUG freetyped.lib ${GEARTH_DIR}/lib)
target_link_libraries(${PROJECT_NAME} debug ${FREETYPE_LIB_DEBUG})
#release
find_library(FREETYPE_LIB freetype.lib ${GEARTH_DIR}/lib)
target_link_libraries(${PROJECT_NAME} optimized ${FREETYPE_LIB})