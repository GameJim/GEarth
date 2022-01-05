QTITAN_ROOT = $$PWD/../..

include($$QTITAN_ROOT/src/shared/platform.pri)

QTITAN_LIB_PATH = $$QTITAN_ROOT/bin$$QTITAN_LIB_PREFIX
INCLUDEPATH += $$QTITAN_ROOT/include
QTN_COMPONENT_VERSION = $$fromfile($$QTITAN_ROOT/src/ribbon/ribbon.pri, QTN_COMPONENT_VERSION)
QTN_COMPONENT_VERSION_LIST = $$split(QTN_COMPONENT_VERSION, ".")
DEFINES += QTN_VERSION_RIBBON=$$QTN_COMPONENT_VERSION

QTN_LIB_NAME=qtnribbon

equals(QTN_STATIC_LIB, 1):CONFIG+=static_library

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    QTN_LIB_NAME = $$QTN_LIB_NAME"d"
} else {
}

win32|static_library {
    QTN_LIB_NAME = $$QTN_LIB_NAME$$member(QTN_COMPONENT_VERSION_LIST, 0)
}

static_library {
    QTN_LIB_NAME = $$QTN_LIB_NAME"_static"
    DEFINES += QTITAN_LIBRARY_STATIC
    macx:LIBS += -framework AppKit
    win32:LIBS += -luser32 -lgdi32 -ladvapi32 -lshlwapi
}

unix|win32-g++ {
    LIBS += -L$$QTITAN_LIB_PATH
    LIBS += -l$$QTN_LIB_NAME
} else {
    LIBS += $$QTITAN_LIB_PATH/$$QTN_LIB_NAME".lib"
}

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

