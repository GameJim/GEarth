# Locate LIBCONFIG.
# This module defines
# LIBCONFIG_LIBRARY
# LIBCONFIG_LIBRARY_DEBUG
# LIBCONFIG_FOUND, if false, do not try to link to libconfig
# LIBCONFIG_INCLUDE_DIR, where to find the headers

SET(LIBCONFIG_DIR "${CONAN_LIBCONFIG_ROOT}" CACHE PATH "Root directory of LIBCONFIG distribution")

FIND_PATH(LIBCONFIG_INCLUDE_DIR libconfig.h
  PATHS
    ${CONAN_LIBCONFIG_ROOT}
    ${LIBCONFIG_DIR}
    $ENV{LIBCONFIG_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES include
)

FIND_PATH(LIBCONFIG_INCLUDE_DIR libconfig.h
  PATHS
    /usr/local/include/libconfig
    /usr/local/include/LibConfig
    /usr/local/include
    /usr/include/libconfig
    /usr/include/LIBCONFIG
    /usr/include
    ~/Library/Frameworks/libconfig/Headers
    /Library/Frameworks/libconfig/Headers
    /sw/include/libconfig
    /sw/include/LIBCONFIG
    /sw/include # Fink
    /opt/local/include/libconfig
    /opt/local/include/LIBCONFIG
    /opt/local/include # DarwinPorts
    /opt/csw/include/libconfig
    /opt/csw/include/LIBCONFIG
    /opt/csw/include # Blastwave
    /opt/include/libconfig
    /opt/include/LIBCONFIG
    /opt/include
    e:/devel/libconfig-3.1.1/source/headers
)

FIND_LIBRARY(LIBCONFIG_LIBRARY
  NAMES libconfig++ libconfig++d 
  PATHS
    ${CONAN_LIBCONFIG_ROOT}
    ${LIBCONFIG_DIR}
    $ENV{LIBCONFIG_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)

FIND_LIBRARY(LIBCONFIG_LIBRARY
  NAMES libconfig++
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

FIND_LIBRARY(LIBCONFIG_LIBRARY_DEBUG
  NAMES libconfig libconfigd libconfigd
  PATHS
	${CONAN_LIBCONFIG_ROOT}
    ${LIBCONFIG_DIR}
    $ENV{LIBCONFIG_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)

FIND_LIBRARY( 
  NAMES libconfig libconfig libconfig
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

SET(LIBCONFIG_FOUND "NO")
IF(LIBCONFIG_LIBRARY AND LIBCONFIG_INCLUDE_DIR)
  SET(LIBCONFIG_FOUND "YES")
ENDIF(LIBCONFIG_LIBRARY AND LIBCONFIG_INCLUDE_DIR)
