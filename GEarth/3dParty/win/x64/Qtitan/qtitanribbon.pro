TEMPLATE = subdirs

include(src/shared/platform.pri)

CONFIG += ordered

QTN_BUILD_SOURCE = 0
QTN_COMPONENT_VERSION = $$fromfile($$PWD/src/ribbon/ribbon.pri, QTN_COMPONENT_VERSION)

exists($$PWD/src/shared/ribbon/ribbon.pro) {
    !isEmpty(QTN_COMPONENT_VERSION):QTN_BUILD_SOURCE = 1
}

equals(QTN_BUILD_SOURCE, 1) {
    message(QtitanRibbon source code being prepared for building...)
    SUBDIRS = \
              src/shared/ribbon \
              src/ribbondsgn
}

SUBDIRS += \
          demos/ribbon/RibbonControls \
          demos/ribbon/RibbonMDIDemo \
          demos/ribbon/RibbonToolBarControlDemo \
          demos/ribbon/RibbonApplication \
          demos/ribbon/RibbonGalleriesDemo \
          demos/ribbon/RibbonBackstageView \
          demos/ribbon/RibbonSystemMenu \
          demos/ribbon/RibbonDesignerDemo \
          demos/ribbon/RibbonGroupScrollingFeature \
          demos/ribbon/RibbonInsideDialogDemo \
          demos/ribbon/RibbonContextualTabs \
          demos/styles/OfficeStylePopup \
          demos/styles/OfficeStyleWidgets

exists($$PWD/doc/qdocs.pri) {
    include($$PWD/doc/qdocs.pri)
}
