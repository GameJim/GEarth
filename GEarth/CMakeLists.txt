cmake_minimum_required(VERSION 3.5)
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(GEarh)

set(CMAKE_USE_RELATIVE_PATHS true)         #开启相对路径
set(GEARTH_DIR ${PROJECT_SOURCE_DIR})  # 父路径
set(GEARTH_INCLUDE ${PROJECT_SOURCE_DIR}/include)  # 包含目录
set(GEARTH_LIB ${PROJECT_SOURCE_DIR}/lib)  # 库目录

#第三方库目录
IF (WIN32)
	MESSAGE(STATUS "Now is windows")
	if(CMAKE_CL_64)
		find_file(THIRDPATH 3dParty/win/x64 ${GEARTH_DIR})
	else()
		find_path(THIRDPATH 3dParty/win/x86 ${GEARTH_DIR})
	endif()
	
	
	set(CMAKE_CXX_STANDARD 11)   #C++ 11
	set(CMAKE_DEBUG_POSTFIX "d") #增添debug后缀

	#链接三方库,将其拷贝之输出目录
	function(INCLUDE_THIRDPARTY NAME)
	#头文件
	string(TOUPPER ${NAME} NAME) 
	string(CONCAT INCLUDE_PATH ${NAME}_ "INCLUDE_DIR")
	find_file(${INCLUDE_PATH} include  ${THIRDPATH}/${NAME} NO_DEFAULT_PATH)
	include_directories(${${INCLUDE_PATH}}) 

	#LIB路径
	# string(CONCAT LIB_PATH ${NAME}_ "LIB_DIR")
	# find_file(${LIB_PATH} lib ${THIRDPATH}/${NAME} NO_DEFAULT_PATH)
	# link_directories(${${LIB_PATH}}) 
	
	string(CONCAT DLL_PATH ${NAME}_ "DLL_DIR")
	
	find_file(${DLL_PATH} bin ${THIRDPATH}/${NAME} NO_DEFAULT_PATH)
	
	#link_directories(${${DLL_PATH}}) 
	endfunction()


	function(LINK_THIRDPARTY NAME LIB_NAME LIB_NAME_DEBUG)
		string(TOUPPER ${NAME} NAME) 
		if(LIB_NAME_DEBUG MATCHES "NONE")
			string(CONCAT LIB_RELEASE ${NAME}_ "LIB_RELEASE")
			find_file(${LIB_RELEASE} ${LIB_NAME}.lib ${THIRDPATH}/${NAME}/lib)
			target_link_libraries(${PROJECT_NAME} ${${LIB_RELEASE}})
		else()
		   string(CONCAT LIB_DEBUG ${NAME}_ "LIB_DEBUG")
		   find_file(${LIB_DEBUG} ${LIB_NAME_DEBUG}.lib ${THIRDPATH}/${NAME}/lib)
		   target_link_libraries(${PROJECT_NAME} optimized ${LIB_DEBUG})
		   
		   string(CONCAT LIB_RELEASE ${NAME}_ "LIB_RELEASE")
		   find_file(${LIB_RELEASE} ${LIB_NAME}.lib  ${THIRDPATH}/${NAME}/lib)
		   target_link_libraries(${PROJECT_NAME} optimized ${LIB_RELEASE})
		endif()
		
		
		# file(GLOB_RECURSE LIBS_NAMES ${THIRDPATH}/${NAME}/lib/*.lib)
		# foreach(LIB_NAME ${LIBS_NAMES})
			# target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
		# endforeach()
		
		string(CONCAT DLL_PATH ${NAME}_ "DLL_DIR")
		file(GLOB_RECURSE DLL_NAMES ${${DLL_PATH}}/*.dll)
		foreach(DLL_NAME ${DLL_NAMES})
			#dll拷贝直输出目录
			add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD        # Adds a post-build event to MyTest
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
					${DLL_NAME}                   # <--this is in-file
					$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		endforeach()
	endfunction()

	function(LINK_THIRDPARTYS NAME )
		string(TOUPPER ${NAME} NAME) 
		string(CONCAT LIB_PATH ${NAME}_ "LIB_DIR")
		
		find_file(${LIB_PATH} lib ${THIRDPATH}/${NAME} NO_DEFAULT_PATH)
	
		file(GLOB_RECURSE LIBS_NAMES ${${LIB_PATH}}/*.lib)
		foreach(LIB_NAME ${LIBS_NAMES})
			target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
		endforeach()
		
		string(CONCAT DLL_PATH ${NAME}_ "DLL_DIR")
		file(GLOB_RECURSE DLL_NAMES ${${DLL_PATH}}/*.dll)
		foreach(DLL_NAME ${DLL_NAMES})
			#dll拷贝直输出目录
			add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD        # Adds a post-build event to MyTest
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
					${DLL_NAME}                   # <--this is in-file
					$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		endforeach()
	endfunction()



	#链接本地库
	function(LINK_SRC_LIB NAME)
		string(CONCAT RELEASE_LIB ${NAME}_ "RELEASE_LIB")
		target_link_libraries(${PROJECT_NAME} debug ${NAME}d.lib)
		target_link_libraries(${PROJECT_NAME} optimized ${NAME}.lib)
	endfunction()


	option(USE_QT_UI "QTITAN" ON)
	if(USE_QT_UI)
	#UI QTITAN
		find_file(QTITAN_INCLUDE Qtitan/include ${THIRDPATH} NO_DEFAULT_PATH)
		find_file(QTITAN_DLL Qtitan/lib/X64 ${THIRDPATH} NO_DEFAULT_PATH)
		find_file(QTITAN_LIB Qtitan/lib/X64 ${THIRDPATH} NO_DEFAULT_PATH)
		
		if(CMAKE_CL_64)
			set(QT_DIR $ENV{QTDIR_X64})
		else()
			set(QT_DIR $ENV{QTDIR})
		endif()
	else()
	endif()

	function(INCLUDE_QT)

		
		
		if(POLICY CMP0990)
		  cmake_policy(SET CMP0990 NEW)
		endif()
		
		
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

		include_directories(${QTITAN_INCLUDE}) 
		link_directories(${QTITAN_LIB}) 
	endfunction()

	function(COPY_DLL NAME DLL_PATH)
	#dll拷贝直输出目录
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD         # Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
			${${DLL_PATH}}/${NAME}.dll                   # <--this is in-file
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
	endfunction()


	function(LINK_QT)
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
		
		COPY_DLL(qtnribbond4 QTITAN_LIB)
		COPY_DLL(qtnribbon4 QTITAN_LIB)
		
		#拷贝资源
	endfunction()
ELSEIF (APPLE)
	MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()






# 子文件
add_subdirectory(plugs)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(ui)

