# This module defines

# BOOST_LIBRARY
# BOOST_FOUND, if false, do not try to link to osg
# BOOST_INCLUDE_DIRS, where to find the headers
# BOOST_INCLUDE_DIR, where to find the source headers
# BOOST_GEN_INCLUDE_DIR, where to find the generated headers

###### headers ######

SET(BOOST_DIR "${CONAN_BOOST_ROOT}" CACHE PATH "Set to base OpenSceneGraph install path")
SET(BOOST_LIB_PRE_SUFFIXE libboost_)
FIND_PATH(BOOST_INCLUDE_DIR boost
	PATHS
        ${BOOST_DIR}
		${CONAN_BOOST_ROOT}
        $ENV{BOOST_SOURCE_DIR}
        $ENV{BOOST_ROOT}
		$ENV{Boost_DIR}
        $ENV{BOOST_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;BOOST_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include
)


FIND_PATH(BOOST_LIB_DIRS libboost_atomic.lib
	PATHS
        ${BOOST_DIR}
		${CONAN_BOOST_ROOT}
        $ENV{BOOST_SOURCE_DIR}
        $ENV{BOOST_ROOT}
		$ENV{Boost_DIR}
        $ENV{BOOST_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;BOOST_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
		/debug/lib/
		/release/lib
)

###### libraries ######

MACRO( FIND_BOOST_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${BOOST_LIB_PRE_SUFFIXE}${MYLIBRARYNAME}
    PATHS
        ${BOOST_DIR}
        $ENV{BOOST_BUILD_DIR}
        $ENV{BOOST_DIR}
        $ENV{OSGDIR}
        $ENV{BOOST_ROOT}
        $ENV{OSG}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;BOOST_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
		/debug/lib/
		/release/lib
     )

ENDMACRO(FIND_BOOST_LIBRARY LIBRARY LIBRARYNAME)




# ATOMIC_HEADERS 
# CHRONO_HEADERS 
# CONTAINER_HEADE
# CONTRACT_HEADER
# CONTEXT_HEADERS
# COROUTINE_HEADE
# DATE_TIME_HEADE
# EXCEPTION_HEADE
# FIBER_HEADERS  
# FILESYSTEM_HEAD
# GRAPH_HEADERS  
# GRAPH_PARALLEL_
# IOSTREAMS_HEADE
# LOCALE_HEADERS 
# LOG_HEADERS    
# LOG_SETUP_HEADE
# MATH_HEADERS   
# MATH_C99_HEADER
# MATH_C99F_HEADE
# MATH_C99L_HEADE
# MATH_TR1_HEADER
# MATH_TR1F_HEADE
# MATH_TR1L_HEADE
# MPI_HEADERS    
# MPI_PYTHON_HEAD
# NUMPY_HEADERS  
# PRG_EXEC_MONITO
# PROGRAM_OPTIONS
# PYTHON_HEADERS 
# RANDOM_HEADERS 
# REGEX_HEADERS  
# SERIALIZATION_H
# SIGNALS_HEADERS
# SYSTEM_HEADERS 
# TEST_EXEC_MONIT
# THREAD_HEADERS 
# TIMER_HEADERS  
# TYPE_ERASURE_HE
# UNIT_TEST_FRAME
# WAVE_HEADERS   
# WSERIALIZATION_


#目前仅罗列出常用的库
FIND_BOOST_LIBRARY( BOOST_ATOMIC_LIBRARY atomic)
FIND_BOOST_LIBRARY( BOOST_ATOMIC_LIBRARY_DEBUG atomicd)

FIND_BOOST_LIBRARY( BOOST_LOG_LIBRARY log)
FIND_BOOST_LIBRARY( BOOST_LOG_LIBRARY_DEBUG logd)

FIND_BOOST_LIBRARY( BOOST_LOG_SETUP_LIBRARY log_setup)
FIND_BOOST_LIBRARY( BOOST_LOG_SETUP_LIBRARY_DEBUG log_setupd)

FIND_BOOST_LIBRARY( BOOST_DATE_TIME_LIBRARY date_time )
FIND_BOOST_LIBRARY( BOOST_FDATE_TIME_LIBRARY_DEBUG date_timed)

FIND_BOOST_LIBRARY( BOOST_THREAD_LIBRARY thread )
FIND_BOOST_LIBRARY( BOOST_THREAD_LIBRARY_DEBUG threadd)

FIND_BOOST_LIBRARY( BOOST_SYSTEM_LIBRARY system )
FIND_BOOST_LIBRARY( BOOST_SYSTEM_LIBRARY_DEBUG systemd)

FIND_BOOST_LIBRARY( BOOST_FILESYSTEM_LIBRARY filesystem )
FIND_BOOST_LIBRARY( BOOST_FILESYSTEM_LIBRARY_DEBUG filesystemd)

FIND_BOOST_LIBRARY( BOOST_CHRONO_LIBRARY chrono )
FIND_BOOST_LIBRARY( BOOST_CHRONO_LIBRARY_DEBUG systemd)

FIND_BOOST_LIBRARY( BOOST_REGEX_LIBRARY regex )
FIND_BOOST_LIBRARY( BOOST_REGEX_LIBRARY_DEBUG regexd)

FIND_BOOST_LIBRARY( BOOST_TIMER_LIBRARY timer )
FIND_BOOST_LIBRARY( BOOST_TIMER_LIBRARY_DEBUG timerd)


#log_setup date_time system filesystem thread regex chrono

SET( BOOST_FOUND "NO" )
IF( BOOST_LOG_LIBRARY AND BOOST_INCLUDE_DIR )
    SET( BOOST_FOUND "YES" )
    SET( BOOST_INCLUDE_DIRS ${BOOST_INCLUDE_DIR} ${BOOST_GEN_INCLUDE_DIR} )
    GET_FILENAME_COMPONENT( BOOST_LIBRARIES_DIR ${BOOST_LOG_LIBRARY} PATH )
ENDIF( BOOST_LOG_LIBRARY AND BOOST_INCLUDE_DIR )

