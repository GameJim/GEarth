
INCLUDEPATH += $$quote($$PWD)

INCLUDE_HEADERS = \
    $$PWD/../../include/QtnWindowTitleBar.h \
    $$PWD/../../include/QtnAcrylicWidget.h \
    $$PWD/../../include/QtnCommonStyleV5.h

PUBLIC_HEADERS = \
    $$PWD/QtitanDef.h \
    $$PWD/QtnWindowTitleBar.h \
    $$PWD/QtnAcrylicWidget.h

PRIVATE_HEADERS = \
    $$PWD/QtitanMSVSDebug.h \
    $$PWD/QtnPlatform.h \
    $$PWD/QtnCustomLayout.h \
    $$PWD/QtnScrollWidgetBar.h \
    $$PWD/QtnBackstageWidget.h \
    $$PWD/QtnWindowTitleBarPrivate.h \
    $$PWD/QtnCommonStyleV5Private.h \
    $$PWD/QtnCommonStyleV5.h \
    $$PWD/QtnStyleOptionV5.h

ALL_SOURCES = \
    $$PWD/QtnCustomLayout.cpp \
    $$PWD/QtnScrollWidgetBar.cpp \
    $$PWD/QtnBackstageWidget.cpp \
    $$PWD/QtnWindowTitleBar.cpp \
    $$PWD/QtnAcrylicWidget.cpp \
    $$PWD/QtnCommonStyleV5.cpp \
	$$PWD/QtnStyleOptionV5.cpp

WINDOWS_HEADERS = \
    $$PWD/QtnPlatform_win.h \
    $$PWD/QtnWindowTitleBar_win.h

WINDOWS_SOURCES = \
    $$PWD/QtnPlatform_win.cpp \
    $$PWD/QtnWindowTitleBar_win.cpp \
    $$PWD/QtnCommonStyleV5_win.cpp

LINUX_HEADERS = 
LINUX_SOURCES = \ 
    $$PWD/QtnPlatform_linux.cpp \
    $$PWD/QtnCommonStyleV5_linux.cpp

MACOSX_HEADERS = 
MACOSX_SOURCES = \
    $$PWD/QtnPlatform_mac.mm \
    $$PWD/QtnCommonStyleV5_mac.mm

RESOURCES += \
    $$PWD/QtnBaseResources.qrc

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS

SOURCES += \
    $$ALL_SOURCES

win32 {
  HEADERS += $$WINDOWS_HEADERS
  SOURCES += $$WINDOWS_SOURCES

  DEFINES += _WINDOWS
  LIBS += -luser32 -lgdi32 -lshell32 -lcomctl32
}

linux* {
  HEADERS += $$LINUX_HEADERS
  SOURCES += $$LINUX_SOURCES
}

macx {
  HEADERS += $$MACOSX_HEADERS
  OBJECTIVE_SOURCES += $$MACOSX_SOURCES
  LIBS += -framework AppKit -framework Carbon
}

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

private_headers {
  QT += core-private widgets-private gui-private
}

