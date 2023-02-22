# Locate Qtitan.
# This module defines
# Qtitan_LIBRARY
# Qtitan_LIBRARY_DEBUG
# Qtitan_FOUND, if false, do not try to link to geos
# Qtitan_INCLUDE_DIR, where to find the headers

SET(Qtitan_DIR "" CACHE PATH "Root directory of Qtitan distribution")

FIND_PATH(Qtitan_INCLUDE_DIR QtnRibbonDef.h
  PATHS
    ${Qtitan_DIR}
    $ENV{Qtitan_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES include
)

FIND_PATH(Qtitan_INCLUDE_DIR QtnRibbonDef.h
  PATHS
    /usr/local/include/geos
    /usr/local/include/Qtitan
    /usr/local/include
    /usr/include/geos
    /usr/include/Qtitan
    /usr/include
    ~/Library/Frameworks/geos/Headers
    /Library/Frameworks/geos/Headers
    /sw/include/geos
    /sw/include/Qtitan
    /sw/include # Fink
    /opt/local/include/geos
    /opt/local/include/Qtitan
    /opt/local/include # DarwinPorts
    /opt/csw/include/geos
    /opt/csw/include/Qtitan
    /opt/csw/include # Blastwave
    /opt/include/geos
    /opt/include/Qtitan
    /opt/include
    e:/devel/geos-3.1.1/source/headers
)

FIND_LIBRARY(Qtitan_LIBRARY
  NAMES qtnribbon4
  PATHS
    ${Qtitan_DIR}/lib/X64
    $ENV{Qtitan_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)


FIND_LIBRARY(Qtitan_LIBRARY_DEBUG
  NAMES qtnribbond4
  PATHS
    ${Qtitan_DIR}/lib/X64
    $ENV{Qtitan_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib64 lib
)


SET(Qtitan_FOUND "NO")
IF(Qtitan_LIBRARY AND Qtitan_INCLUDE_DIR)
  SET(Qtitan_FOUND "YES")
ENDIF(Qtitan_LIBRARY AND Qtitan_INCLUDE_DIR)
