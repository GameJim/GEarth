#######################################################################################################
#  macro for linking libraries that come from Findxxxx commands, so there is a variable that contains the
#  full path of the library name. in order to differentiate release and debug, this macro get the
#  NAME of the variables, so the macro gets as arguments the target name and the following list of parameters
#  is intended as a list of variable names each one containing  the path of the libraries to link to
#  The existence of a variable name with _DEBUG appended is tested and, in case it's value is used
#  for linking to when in debug mode
#  the content of this library for linking when in debugging
#######################################################################################################

MACRO(LINK_WITH_VARIABLES TRGTNAME)
    FOREACH(varname ${ARGN})
        IF(${varname}_DEBUG)
            IF(${varname}_RELEASE)
                TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}_RELEASE}" debug "${${varname}_DEBUG}")
            ELSE(${varname}_RELEASE)
                TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}}" debug "${${varname}_DEBUG}")
            ENDIF(${varname}_RELEASE)
        ELSE(${varname}_DEBUG)
            TARGET_LINK_LIBRARIES(${TRGTNAME} ${${varname}} )
        ENDIF(${varname}_DEBUG)
    ENDFOREACH(varname)
ENDMACRO(LINK_WITH_VARIABLES TRGTNAME)

MACRO(LINK_INTERNAL TRGTNAME)
    TARGET_LINK_LIBRARIES(${TRGTNAME} ${ARGN})
ENDMACRO(LINK_INTERNAL TRGTNAME)

MACRO(LINK_EXTERNAL TRGTNAME)
    FOREACH(LINKLIB ${ARGN})
        TARGET_LINK_LIBRARIES(${TRGTNAME} "${LINKLIB}" )
    ENDFOREACH(LINKLIB)
ENDMACRO(LINK_EXTERNAL TRGTNAME)


#######################################################################################################
#  macro for common setup of core libraries: it links OPENGL_LIBRARIES in undifferentiated mode
#######################################################################################################

MACRO(LINK_CORELIB_DEFAULT CORELIB_NAME)
    LINK_EXTERNAL(${CORELIB_NAME} ${OPENGL_LIBRARIES})
    LINK_WITH_VARIABLES(${CORELIB_NAME} OPENTHREADS_LIBRARY)
    IF(OSGEARTH_SONAMES)
      SET_TARGET_PROPERTIES(${CORELIB_NAME} PROPERTIES VERSION ${OSGEARTH_VERSION} SOVERSION ${OSGEARTH_SOVERSION})
    ENDIF(OSGEARTH_SONAMES)
ENDMACRO(LINK_CORELIB_DEFAULT CORELIB_NAME)


#######################################################################################################
#  macro for common setup of plugins, examples and applications it expect some variables to be set:
#  either within the local CMakeLists or higher in hierarchy
#  TARGET_NAME is the name of the folder and of the actually .exe or .so or .dll
#  TARGET_TARGETNAME  is the name of the target , this get buit out of a prefix, if present and TARGET_TARGETNAME
#  TARGET_SRC  are the sources of the target
#  TARGET_H are the eventual headers of the target
#  TARGET_LIBRARIES are the libraries to link to that are internal to the project and have d suffix for debug
#  TARGET_EXTERNAL_LIBRARIES are external libraries and are not differentiated with d suffix
#  TARGET_LABEL is the label IDE should show up for targets
##########################################################################################################

MACRO(SETUP_LINK_LIBRARIES)
    ######################################################################
    #
    # This set up the libraries to link to, it assumes there are two variable: one common for a group of examples or plagins
    # kept in the variable TARGET_COMMON_LIBRARIES and an example or plugin specific kept in TARGET_ADDED_LIBRARIES
    # they are combined in a single list checked for unicity
    # the suffix ${CMAKE_DEBUG_POSTFIX} is used for differentiating optimized and debug
    #
    # a second variable TARGET_EXTERNAL_LIBRARIES hold the list of  libraries not differentiated between debug and optimized
    ##################################################################################
    SET(TARGET_LIBRARIES ${TARGET_COMMON_LIBRARIES})

    FOREACH(LINKLIB ${TARGET_ADDED_LIBRARIES})
      SET(TO_INSERT TRUE)
      FOREACH (value ${TARGET_COMMON_LIBRARIES})
            IF (${value} STREQUAL ${LINKLIB})
                  SET(TO_INSERT FALSE)
            ENDIF (${value} STREQUAL ${LINKLIB})
        ENDFOREACH (value ${TARGET_COMMON_LIBRARIES})
      IF(TO_INSERT)
          LIST(APPEND TARGET_LIBRARIES ${LINKLIB})
      ENDIF(TO_INSERT)
    ENDFOREACH(LINKLIB)

#    FOREACH(LINKLIB ${TARGET_LIBRARIES})
#            TARGET_LINK_LIBRARIES(${TARGET_TARGETNAME} optimized ${LINKLIB} debug "${LINKLIB}${CMAKE_DEBUG_POSTFIX}")
#    ENDFOREACH(LINKLIB)
    LINK_INTERNAL(${TARGET_TARGETNAME} ${TARGET_LIBRARIES})

    IF(TARGET_LIBRARIES_VARS)
            LINK_WITH_VARIABLES(${TARGET_TARGETNAME} ${TARGET_LIBRARIES_VARS})
    ENDIF(TARGET_LIBRARIES_VARS)

    FOREACH(LINKLIB ${TARGET_EXTERNAL_LIBRARIES})
            TARGET_LINK_LIBRARIES(${TARGET_TARGETNAME} ${LINKLIB})
    ENDFOREACH(LINKLIB)
ENDMACRO(SETUP_LINK_LIBRARIES)


MACRO(SET_OUTPUT_DIR_PROPERTY_260 TARGET_TARGETNAME RELATIVE_OUTDIR)
    #BUILDER_VERSION_GREATER(2 8 0)
	SET(VALID_BUILDER_VERSION TRUE)
    IF(NOT VALID_BUILDER_VERSION)
        # If CMake <= 2.8.0 (Testing CMAKE_VERSION is possible in >= 2.6.4)
        IF(MSVC_IDE)
            # Using the "prefix" hack
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../${RELATIVE_OUTDIR}/")
        ELSE(MSVC_IDE)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "${RELATIVE_OUTDIR}/")
        ENDIF(MSVC_IDE)
    ELSE(NOT VALID_BUILDER_VERSION)
        # Using the output directory properties
        # Global properties (All generators but VS & Xcode)
		
        FILE(TO_CMAKE_PATH TMPVAR "CMAKE_ARCHIVE_OUTPUT_DIRECTORY/${RELATIVE_OUTDIR}")
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${TMPVAR}")
        FILE(TO_CMAKE_PATH TMPVAR "CMAKE_RUNTIME_OUTPUT_DIRECTORY/${RELATIVE_OUTDIR}")
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TMPVAR}")
        FILE(TO_CMAKE_PATH TMPVAR "CMAKE_LIBRARY_OUTPUT_DIRECTORY/${RELATIVE_OUTDIR}")
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${TMPVAR}")

        # Per-configuration property (VS, Xcode)
        FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})        # For each configuration (Debug, Release, MinSizeRel... and/or anything the user chooses)
            STRING(TOUPPER "${CONF}" CONF)                # Go uppercase (DEBUG, RELEASE...)

            # We use "FILE(TO_CMAKE_PATH", to create nice looking paths
            FILE(TO_CMAKE_PATH "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}}/${RELATIVE_OUTDIR}" TMPVAR)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY_${CONF}" "${TMPVAR}")
            FILE(TO_CMAKE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}}/${RELATIVE_OUTDIR}" TMPVAR)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${TMPVAR}")
            FILE(TO_CMAKE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}}/${RELATIVE_OUTDIR}" TMPVAR)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${TMPVAR}")
        ENDFOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})
    ENDIF(NOT VALID_BUILDER_VERSION)
ENDMACRO(SET_OUTPUT_DIR_PROPERTY_260 TARGET_TARGETNAME RELATIVE_OUTDIR)


MACRO(SETUP_LIBRARY LIB_NAME)
    IF(GLCORE_FOUND)
        INCLUDE_DIRECTORIES( ${GLCORE_INCLUDE_DIR} )
    ENDIF()

        SET(TARGET_NAME ${LIB_NAME} )
        SET(TARGET_TARGETNAME ${LIB_NAME} )
        ADD_LIBRARY(${LIB_NAME}
            ${${ROOT_NAME}_DYNAMIC_OR_STATIC}
            ${TARGET_H}
            ${TARGET_H_NO_MODULE_INSTALL}
            ${TARGET_SRC}
        )
        SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES FOLDER "OSG Core")
        IF(APPLE)
            IF(OSG_BUILD_PLATFORM_IPHONE)
                SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE ${IPHONE_ENABLE_BITCODE})
            ENDIF()
            SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_WARNING_CFLAGS "")
        ENDIF()
        IF(TARGET_LABEL)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
        ENDIF(TARGET_LABEL)

        IF(TARGET_LIBRARIES)
            LINK_INTERNAL(${LIB_NAME} ${TARGET_LIBRARIES})
        ENDIF()
        IF(TARGET_EXTERNAL_LIBRARIES)
            LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES})
        ENDIF()
        IF(TARGET_LIBRARIES_VARS)
            LINK_WITH_VARIABLES(${LIB_NAME} ${TARGET_LIBRARIES_VARS})
        ENDIF(TARGET_LIBRARIES_VARS)
        LINK_CORELIB_DEFAULT(${LIB_NAME})

    INCLUDE(ModuleInstall OPTIONAL)
ENDMACRO(SETUP_LIBRARY LIB_NAME)


MACRO(SETUP_UI_LIBRARY LIB_NAME)
   
        SET(TARGET_NAME ${LIB_NAME} )
        SET(TARGET_TARGETNAME ${LIB_NAME} )
		

		#设置QT相关操作
		set(CMAKE_AUTOMOC ON)
		set(CMAKE_AUTOUIC ON)
		set(CMAKE_AUTORCC ON)
		QT5_WRAP_UI(FORMS_UIC ${TARGET_UI})
		QT5_ADD_RESOURCES(RES_FILES RESOURCE_FILES)

		#将ui文件和生成文件整理在一个文件夹中
		SOURCE_GROUP("Form Files" FILES ${TARGET_UI})
		#将qrc资源文件整理在一个文件夹中
		SOURCE_GROUP("Resource Files" FILES ${TARGET_RESOURCE})

		source_group("moc" FILES ${MOC_SRC})
		
        ADD_LIBRARY(${LIB_NAME}
            ${${ROOT_NAME}_DYNAMIC_OR_STATIC}
            ${TARGET_H}
            ${TARGET_H_NO_MODULE_INSTALL}
            ${TARGET_SRC}
        )
        SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES FOLDER "ui")
       
        IF(TARGET_LABEL)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
        ENDIF(TARGET_LABEL)

        IF(TARGET_LIBRARIES)
            LINK_INTERNAL(${LIB_NAME} ${TARGET_LIBRARIES})
        ENDIF()
		
        IF(TARGET_EXTERNAL_LIBRARIES)
            LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES})
        ENDIF()
        IF(TARGET_LIBRARIES_VARS)
            LINK_WITH_VARIABLES(${LIB_NAME} ${TARGET_LIBRARIES_VARS})
        ENDIF(TARGET_LIBRARIES_VARS)
        LINK_CORELIB_DEFAULT(${LIB_NAME})

    INCLUDE(ModuleInstall OPTIONAL)
ENDMACRO(SETUP_UI_LIBRARY LIB_NAME)




############################################################################################
# this is the common set of command for all the plugins

#必须为dll 其是内核有定义，外部选择dll加载
MACRO(SETUP_PLUGIN PLUGIN_NAME)

    SET(TARGET_NAME ${PLUGIN_NAME} )

    #MESSAGE("in -->SETUP_PLUGIN<-- ${TARGET_NAME}-->${TARGET_SRC} <--> ${TARGET_H}<--")

    SOURCE_GROUP( "Header Files"   FILES ${TARGET_H} )
    SOURCE_GROUP( "Res Files"   FILES ${TARGET_RES} )
    SOURCe_GROUP( "Template Files" FILES ${TARGET_IN} )

    ## we have set up the target label and targetname by taking into account global prfix (osgdb_)
	IF(NOT MSVC_IDE)
		SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../bin/${OSG_PLUGINS}/")
	ELSE(NOT MSVC_IDE)
		SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../../bin/${OSG_PLUGINS}/" IMPORT_PREFIX "../")
	ENDIF(NOT MSVC_IDE) 
  

    IF(NOT TARGET_TARGETNAME)
            SET(TARGET_TARGETNAME "${TARGET_DEFAULT_PREFIX}${TARGET_NAME}")
    ENDIF(NOT TARGET_TARGETNAME)
    IF(NOT TARGET_LABEL)
            SET(TARGET_LABEL "${TARGET_DEFAULT_LABEL_PREFIX} ${TARGET_NAME}")
    ENDIF(NOT TARGET_LABEL)


	# here we use the command to generate the library
    ADD_LIBRARY(${TARGET_TARGETNAME} MODULE ${TARGET_SRC} ${TARGET_H} ${TARGET_GLSL} ${TARGET_IN})
	
	IF(MSVC)
        IF(NOT CMAKE24)
            SET_OUTPUT_DIR_PROPERTY_260(${TARGET_TARGETNAME} "${PLUGINS}")        # Sets the ouput to be /osgPlugin-X.X.X ; also ensures the /Debug /Release are removed
        ELSE(NOT CMAKE24)
			IF(NOT MSVC_IDE)
                    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../bin/${PLUGINS}/")
			ELSE(NOT MSVC_IDE)
				SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../../bin/${PLUGINS}/" IMPORT_PREFIX "../")
			ENDIF(NOT MSVC_IDE)

        ENDIF(NOT CMAKE24)
    ENDIF(MSVC)

    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")

    IF(TRACY_FOUND)
        INCLUDE_DIRECTORIES(${TRACY_INCLUDE_DIR})
        LIST(APPEND TARGET_LIBRARIES_VARS TRACY_LIBRARY)
    ENDIF(TRACY_FOUND)

    SETUP_LINK_LIBRARIES()

#the installation path are differentiated for win32 that install in bib versus other architecture that install in lib${LIB_POSTFIX}/${VPB_PLUGINS}
    IF(WIN32)
        INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib/${PLUGINS} LIBRARY DESTINATION bin/${PLUGINS} )
    ELSE(WIN32)
        INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION bin ARCHIVE DESTINATION lib${LIB_POSTFIX}/${PLUGINS} LIBRARY DESTINATION lib${LIB_POSTFIX}/${PLUGINS} )
    ENDIF(WIN32)

    IF(OSG_BUILD_PLATFORM_IPHONE)
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE ${IPHONE_ENABLE_BITCODE})
    ENDIF()

    # install the shader source files
    if(INSTALL_RES)
        INSTALL(
            FILES ${TARGET_RES}
            DESTINATION resources/res )
    endif(INSTALL_RES)

#finally, set up the solution folder -gw
    SET_PROPERTY(TARGET ${TARGET_TARGETNAME} PROPERTY FOLDER "Plugins")

ENDMACRO(SETUP_PLUGIN)





MACRO(SETUP_EXTENSION PLUGIN_NAME)

    SET(TARGET_NAME ${PLUGIN_NAME} )

    #MESSAGE("in -->SETUP_EXTENSION<-- ${TARGET_NAME}-->${TARGET_SRC} <--> ${TARGET_H}<--")

    SOURCE_GROUP( "Header Files"   FILES ${TARGET_H} )
    SOURCE_GROUP( "Shader Files"   FILES ${TARGET_GLSL} )
    SOURCe_GROUP( "Template Files" FILES ${TARGET_IN} )

    ## we have set up the target label and targetname by taking into account global prefix (osgdb_)

    IF(NOT TARGET_TARGETNAME)
            SET(TARGET_TARGETNAME "${TARGET_DEFAULT_PREFIX}${TARGET_NAME}")
    ENDIF(NOT TARGET_TARGETNAME)
    IF(NOT TARGET_LABEL)
            SET(TARGET_LABEL "${TARGET_DEFAULT_LABEL_PREFIX} ${TARGET_NAME}")
    ENDIF(NOT TARGET_LABEL)

# here we use the command to generate the library

    IF   (OSGEARTH_BUILD_SHARED_LIBS)
        ADD_LIBRARY(${TARGET_TARGETNAME} MODULE ${TARGET_SRC} ${TARGET_H} ${TARGET_GLSL} ${TARGET_IN})
    ELSE (OSGEARTH_BUILD_SHARED_LIBS)
        ADD_LIBRARY(${TARGET_TARGETNAME} STATIC ${TARGET_SRC} ${TARGET_H} ${TARGET_GLSL} ${TARGET_IN})
    ENDIF(OSGEARTH_BUILD_SHARED_LIBS)

    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")

    SETUP_LINK_LIBRARIES()

#the installation path are differentiated for win32 that install in bib versus other architecture that install in lib${LIB_POSTFIX}/${VPB_PLUGINS}
    IF(WIN32)
        INSTALL(
            TARGETS ${TARGET_TARGETNAME}
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib/${OSG_PLUGINS}
            LIBRARY DESTINATION bin/${OSG_PLUGINS} )

    ELSE(WIN32)
        INSTALL(
            TARGETS ${TARGET_TARGETNAME}
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib${LIB_POSTFIX}/${OSG_PLUGINS}
            LIBRARY DESTINATION lib${LIB_POSTFIX}/${OSG_PLUGINS} )

    ENDIF(WIN32)

    # install the shader source files
    if(OSGEARTH_INSTALL_SHADERS)
        INSTALL(
            FILES ${TARGET_GLSL}
            DESTINATION resources/shaders )
    endif(OSGEARTH_INSTALL_SHADERS)

    IF(OSG_BUILD_PLATFORM_IPHONE)
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE ${IPHONE_ENABLE_BITCODE})
    ENDIF()

#finally, set up the solution folder -gw
    SET_PROPERTY(TARGET ${TARGET_TARGETNAME} PROPERTY FOLDER "Extensions")

ENDMACRO(SETUP_EXTENSION)





#################################################################################################################
# this is the macro for example and application setup
###########################################################

MACRO(SETUP_EXE IS_COMMANDLINE_APP)
    #MESSAGE("in -->SETUP_EXE<-- ${TARGET_NAME}-->${TARGET_SRC} <--> ${TARGET_H}<--")
    IF(NOT TARGET_TARGETNAME)
            SET(TARGET_TARGETNAME "${TARGET_DEFAULT_PREFIX}${TARGET_NAME}")
    ENDIF(NOT TARGET_TARGETNAME)
    IF(NOT TARGET_LABEL)
            SET(TARGET_LABEL "${TARGET_DEFAULT_LABEL_PREFIX} ${TARGET_NAME}")
    ENDIF(NOT TARGET_LABEL)

    IF(${IS_COMMANDLINE_APP})

        ADD_EXECUTABLE(${TARGET_TARGETNAME} ${TARGET_SRC} ${TARGET_H})

    ELSE(${IS_COMMANDLINE_APP})

        IF(APPLE)
            # SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${VIRTUALPLANETBUILDER_MAJOR_VERSION}.${VIRTUALPLANETBUILDER_MINOR_VERSION}.${VIRTUALPLANETBUILDER_PATCH_VERSION}")
            # Short Version is the "marketing version". It is the version
            # the user sees in an information panel.
            SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OSGEARTH_MAJOR_VERSION}.${OSGEARTH_MINOR_VERSION}.${OSGEARTH_PATCH_VERSION}")
            # Bundle version is the version the OS looks at.
            SET(MACOSX_BUNDLE_BUNDLE_VERSION "${OSGEARTH_MAJOR_VERSION}.${OSGEARTH_MINOR_VERSION}.${OSGEARTH__PATCH_VERSION}")
            SET(MACOSX_BUNDLE_GUI_IDENTIFIER "org.osgearth.${TARGET_TARGETNAME}" )
            SET(MACOSX_BUNDLE_BUNDLE_NAME "${TARGET_NAME}" )
            # SET(MACOSX_BUNDLE_ICON_FILE "myicon.icns")
            # SET(MACOSX_BUNDLE_COPYRIGHT "")
            # SET(MACOSX_BUNDLE_INFO_STRING "Info string, localized?")
        ENDIF(APPLE)

        IF(WIN32)
            IF (REQUIRE_WINMAIN_FLAG)
                SET(PLATFORM_SPECIFIC_CONTROL WIN32)
            ENDIF(REQUIRE_WINMAIN_FLAG)
        ENDIF(WIN32)

        IF(APPLE)
            IF(VPB_BUILD_APPLICATION_BUNDLES)
                SET(PLATFORM_SPECIFIC_CONTROL MACOSX_BUNDLE)
            ENDIF(VPB_BUILD_APPLICATION_BUNDLES)
        ENDIF(APPLE)

        ADD_EXECUTABLE(${TARGET_TARGETNAME} ${PLATFORM_SPECIFIC_CONTROL} ${TARGET_SRC} ${TARGET_H})

    ENDIF(${IS_COMMANDLINE_APP})

    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES RELEASE_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELEASE_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELWITHDEBINFO_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${TARGET_NAME}${CMAKE_MINSIZEREL_POSTFIX}")

    IF(OSG_BUILD_PLATFORM_IPHONE)
        SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE ${IPHONE_ENABLE_BITCODE})
    ENDIF()

    SETUP_LINK_LIBRARIES()

ENDMACRO(SETUP_EXE)

# Taked optional second arg: APPLICATION_FOLDER
# Takes optional third arg:  (is_commandline_app?) in ARGV2
MACRO(SETUP_APPLICATION APPLICATION_NAME)

    SET(TARGET_NAME ${APPLICATION_NAME} )

    IF(TRACY_FOUND)
        INCLUDE_DIRECTORIES(${TRACY_INCLUDE_DIR})
        LIST(APPEND TARGET_LIBRARIES_VARS TRACY_LIBRARY)
    ENDIF(TRACY_FOUND)

    # 2nd arguemnt: application folder name for IDE?
    IF(${ARGC} GREATER 1)
        SET(APPLICATION_FOLDER ${ARGV1})
    ELSE(${ARGC} GREATER 1)
        SET(APPLICATION_FOLDER ${TARGET_DEFAULT_APPLICATION_FOLDER})
    ENDIF(${ARGC} GREATER 1)

    # 3rd argument: is it a command-line app?
    IF(${ARGC} GREATER 2)
        SET(IS_COMMANDLINE_APP ${ARGV2})
    ELSE(${ARGC} GREATER 2)
        SET(IS_COMMANDLINE_APP 0)
    ENDIF(${ARGC} GREATER 2)

    SETUP_EXE(${IS_COMMANDLINE_APP})

    INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION bin  )
	#Install to the OSG_DIR as well
	IF(OSGEARTH_INSTALL_TO_OSG_DIR AND OSG_DIR)
	  INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION ${OSG_DIR}/bin)
	ENDIF(OSGEARTH_INSTALL_TO_OSG_DIR AND OSG_DIR)

	IF(NOT APPLICATION_FOLDER)
	    SET(APPLICATION_FOLDER "applications")
	ENDIF(NOT APPLICATION_FOLDER)

	SET_PROPERTY(TARGET ${TARGET_TARGETNAME} PROPERTY FOLDER ${APPLICATION_FOLDER})

ENDMACRO(SETUP_APPLICATION)


# Configure an application for use with IMGUI.
# Taked optional second arg: APPLICATION_FOLDER
# Takes optional third arg:  (is_commandline_app?) in ARGV2
MACRO(SETUP_IMGUI_APPLICATION APPLICATION_NAME)
    if(OSGEARTH_ENABLE_IMGUI)
        include_directories(
            ${GLEW_INCLUDE_DIR}
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/examples
        )

        set(TARGET_LIBRARIES_VARS
            ${TARGET_LIBRARIES_VARS}
            GLEW_LIBRARIES
        )

        set(TARGET_SRC
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui.cpp
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui_demo.cpp
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui_draw.cpp
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui_widgets.cpp
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui_tables.cpp
            ${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/backends/imgui_impl_opengl3.cpp
            ${OSGEARTH_SOURCE_DIR}/src/osgEarth/ImGui/ImGui.cpp
            ${TARGET_SRC}
        )

        # define for conditional compilation (esp for imgui headers)
        if(OSGEARTHPROCEDURAL_LIBRARY)
            add_definitions(-DHAVE_OSGEARTHPROCEDURAL)
            list(APPEND TARGET_LIBRARIES_VARS OSGEARTHPROCEDURAL_LIBRARY)
        endif()

        SETUP_APPLICATION(${APPLICATION_NAME})
    else()
        message(STATUS "ImGui application ${APPLICATION_NAME} skipped")
        if (NOT GLEW_FOUND)
            message(STATUS "   ...because GLEW not found")
        elseif (NOT EXISTS "${OSGEARTH_SOURCE_DIR}/src/third_party/imgui/imgui.cpp")
            message(STATUS "   ...because ImGui git submodule not found")
        endif()
    endif()
ENDMACRO(SETUP_IMGUI_APPLICATION)


MACRO(SETUP_COMMANDLINE_APPLICATION APPLICATION_NAME)

    SETUP_APPLICATION(${APPLICATION_NAME} 1)

ENDMACRO(SETUP_COMMANDLINE_APPLICATION)

# Takes optional second argument (is_commandline_app?) in ARGV1
MACRO(SETUP_EXAMPLE EXAMPLE_NAME)

        SET(TARGET_NAME ${EXAMPLE_NAME} )

        IF(${ARGC} GREATER 1)
            SET(IS_COMMANDLINE_APP ${ARGV1})
        ELSE(${ARGC} GREATER 1)
            SET(IS_COMMANDLINE_APP 0)
        ENDIF(${ARGC} GREATER 1)

        SETUP_EXE(${IS_COMMANDLINE_APP})

    INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION share/OpenSceneGraph/bin BUNDLE DESTINATION share/OpenSceneGraph/bin  )

ENDMACRO(SETUP_EXAMPLE)


MACRO(SETUP_COMMANDLINE_EXAMPLE EXAMPLE_NAME)

    SETUP_EXAMPLE(${EXAMPLE_NAME} 1)

ENDMACRO(SETUP_COMMANDLINE_EXAMPLE)


# -----------------------------------------------------------------------
# configure_shaders -gw
#
# Bakes GLSL shaders to make into a CPP file at runtime.
# Example:
#
#   configure_shaders( MyTemplate.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/AutoGen.cpp file1.glsl file2.glsl )
#
macro(configure_shaders templateFile autoGenCppFile)

	# set up configure variables:
	set(TEMPLATE_FILE   ${templateFile} )
	set(GLSL_FILES      ${ARGN} )
	set(OUTPUT_CPP_FILE ${autoGenCppFile})

	# generate the build-time script that will create out cpp file with inline shaders:
	configure_file(
		"${CMAKE_SOURCE_DIR}/CMakeModules/ConfigureShaders.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/ConfigureShaders.cmake"
		@ONLY)

	# add the custom build-time command to run the script:
	add_custom_command(
		OUTPUT
			"${autoGenCppFile}"
		COMMAND
			"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/ConfigureShaders.cmake"
		DEPENDS
			${GLSL_FILES}
			"${TEMPLATE_FILE}"
			"${CMAKE_SOURCE_DIR}/CMakeModules/ConfigureShaders.cmake.in" )

endmacro(configure_shaders)

# http://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
