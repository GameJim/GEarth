# Locate PTHREAD.
# This module defines
# PTHREAD_LIBRARY
# PTHREAD_LIBRARY_DEBUG
# PTHREAD_FOUND, if false, do not try to link to libconfig
# PTHREAD_INCLUDE_DIR, where to find the headers

SET(PTHREAD_DIR "${CONAN_PTHREAD_ROOT}" CACHE PATH "Root directory of PTHREAD distribution")

FIND_PATH(PTHREAD_INCLUDE_DIR pthread.h
  PATHS
    ${CONAN_PTHREAD_ROOT}
	${CONAN_PTHREADS4W_ROOT}
    ${PTHREAD_DIR}
    $ENV{PTHREAD_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES include
)



FIND_PATH(PTHREAD_INCLUDE_DIR libconfig.h
  PATHS
    /usr/local/include/libconfig
    /usr/local/include/LibConfig
    /usr/local/include
    /usr/include/libconfig
    /usr/include/PTHREAD
    /usr/include
    ~/Library/Frameworks/libconfig/Headers
    /Library/Frameworks/libconfig/Headers
    /sw/include/libconfig
    /sw/include/PTHREAD
    /sw/include # Fink
    /opt/local/include/libconfig
    /opt/local/include/PTHREAD
    /opt/local/include # DarwinPorts
    /opt/csw/include/libconfig
    /opt/csw/include/PTHREAD
    /opt/csw/include # Blastwave
    /opt/include/libconfig
    /opt/include/PTHREAD
    /opt/include
    e:/devel/libconfig-3.1.1/source/headers
)

FIND_LIBRARY(PTHREAD_LIBRARY
  NAMES libpthreadVC3 qthread 
  PATHS
    ${CONAN_PTHREAD_ROOT}
	${CONAN_PTHREADS4W_ROOT}
    ${PTHREAD_DIR}
    $ENV{PTHREAD_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)

FIND_LIBRARY(PTHREAD_LIBRARY
  NAMES libpthreadVC3 qthread
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    /usr/freeware
  PATH_SUFFIXES lib64 lib
)

FIND_LIBRARY(PTHREAD_LIBRARY_DEBUG
  NAMES libpthreadVC3d qthreadd
  PATHS
	${CONAN_PTHREAD_ROOT}
    ${PTHREAD_DIR}
    $ENV{PTHREAD_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)

FIND_LIBRARY( 
  NAMES libpthreadVC3 qthread
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    /usr/freeware
  PATH_SUFFIXES lib64 lib
)

SET(PTHREAD_FOUND "NO")
IF(PTHREAD_LIBRARY AND PTHREAD_INCLUDE_DIR)
  SET(PTHREAD_FOUND "YES")
ENDIF(PTHREAD_LIBRARY AND PTHREAD_INCLUDE_DIR)
