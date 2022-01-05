TEMPLATE = app

QTITANDIR = $$quote($$(QTITANDIR))

isEmpty(QTITANDIR):QTITANDIR = %QtitanInstallPath%

include($$QTITANDIR/src/shared/qtitanribbon.pri)

DESTDIR = $$QTITANDIR/bin
DESTDIR = $$member(DESTDIR, 0)$$QTITAN_LIB_PREFIX

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}

HEADERS += %RibbonClassName:l%.%CppHeaderSuffix%

SOURCES += \
            %RibbonClassName:l%.%CppSourceSuffix% \
            main.cpp


FORMS   += %RibbonClassName:l%.ui

macx {
    CONFIG-=app_bundle
}

!lessThan(QT_VER_MAJ, 5) {
  QT += printsupport
}
