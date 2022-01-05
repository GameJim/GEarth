/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2017 Developer Machines (http://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#include <QApplication>
#include "QtnStyleOptionV5.h"

QTITAN_USE_NAMESPACE

/*!
\class Qtitan::StyleOptionFrameV5
\internal
*/
StyleOptionFrameV5::StyleOptionFrameV5()
    : hdc(0)
    , frameBorder(4)
    , titleBarSize(30)
    , tabBarSize(24)
    , statusHeight(0)
    , active(false)
    , hasStatusBar(false)
    , titleVisible(true)
    , maximizeFlags(false)
    , isBackstageVisible(false)
    , tabBarPosition(TabBarTopPosition)
{
}

StyleOptionFrameV5::StyleOptionFrameV5(const StyleOptionFrameV5& other)
    : QStyleOption(Version, Type)
{
    *this = other;
}

StyleOptionFrameV5::~StyleOptionFrameV5()
{
}

/*!
\class Qtitan::StyleOptionBackstageButtonV5
\internal
*/
StyleOptionBackstageButtonV5::StyleOptionBackstageButtonV5()
    : tabStyle(false)
    , document(Q_NULL)
{
}

StyleOptionBackstageButtonV5::StyleOptionBackstageButtonV5(const StyleOptionBackstageButtonV5& other)
    : QStyleOptionToolButton(other)
{
    *this = other;
}

/*!
\class Qtitan::StyleOptionRibbonBackstageV5
\internal
*/
StyleOptionRibbonBackstageV5::StyleOptionRibbonBackstageV5()
    : menuWidth(132)
{
}

StyleOptionRibbonBackstageV5::StyleOptionRibbonBackstageV5(const StyleOptionRibbonBackstageV5& other)
    : QStyleOption(Version, Type)
{
    *this = other;
}

/*!
\class Qtitan::StyleOptionRibbonV5
\internal
*/
StyleOptionRibbonV5::StyleOptionRibbonV5()
    : frameHelper(Q_NULL)
    , titleBarHeight(0)
    , minimized(false)
    , isBackstageVisible(false)
    , tabBarPosition(StyleOptionFrameV5::TabBarTopPosition)
    , pageColor(CommonStyleV5::ContextColorNone)
{
}

StyleOptionRibbonV5::StyleOptionRibbonV5(const StyleOptionRibbonV5& other)
    : QStyleOption(Version, Type)
{
    *this = other;
}

StyleOptionRibbonV5::~StyleOptionRibbonV5()
{
}

/*!
\class Qtitan::StyleOptionTitleBarV5
\internal
*/
StyleOptionTitleBarV5::StyleOptionTitleBarV5()
    : frameHelper(Q_NULL)
    , drawIcon(true)
    , quickAccessVisibleCount(0)
    , quickAccessOnTop(true)
    , quickAccessVisible(true)
    , existSysButton(true)
    , normalSysButton(false)
    , isBackstageVisible(false)
    , alignment(0)
    , clrText()
    , fontTextTitle(QApplication::font("QMdiSubWindowTitleBar"))
{
}

StyleOptionTitleBarV5::StyleOptionTitleBarV5(const StyleOptionTitleBarV5 &other)
    : QStyleOptionTitleBar(other)
{
    *this = other;
}

/*!
\class Qtitan::StyleCaptionButtonV5
\internal
*/
StyleCaptionButtonV5::StyleCaptionButtonV5()
{
}

StyleCaptionButtonV5::StyleCaptionButtonV5(const StyleCaptionButtonV5& other)
    : QStyleOptionToolButton(other)
{
    *this = other;
}


/*!
\class Qtitan::StyleOptionTitleBarV5
\internal
*/
StyleSystemToolButtonV5::StyleSystemToolButtonV5()
{
}

StyleSystemToolButtonV5::StyleSystemToolButtonV5(const StyleSystemToolButtonV5& other)
    : QStyleOptionToolButton(other)
{
    *this = other;
}


/*!
\class Qtitan::StyleHintReturnThemeColorV5
\internal
*/
StyleHintReturnThemeColorV5::StyleHintReturnThemeColorV5(const QString& secName, const QString& kName, QColor col)
    : QStyleHintReturn()
    , sectionName(secName)
    , keyName(kName)
    , color(col)
    , isValid(true)
{
}

StyleHintReturnThemeColorV5::~StyleHintReturnThemeColorV5()
{
}

/*!
\class Qtitan::StyleRibbonQuickAccessBarV5
\internal
*/
StyleRibbonQuickAccessBarV5::StyleRibbonQuickAccessBarV5()
    : quickAccessBarPosition(StyleRibbonQuickAccessBarV5::QATopPosition)
{
}

StyleRibbonQuickAccessBarV5::StyleRibbonQuickAccessBarV5(const StyleRibbonQuickAccessBarV5& other)
    : QStyleOption(other)
{
    *this = other;
}

/*!
\class Qtitan::StyleRibbonOptionHeaderV5
\internal
*/
StyleRibbonOptionHeaderV5::StyleRibbonOptionHeaderV5()
    : QStyleOptionHeader()
    , firstTab(false)
    , lastTab(false)
    , contextText(QString(""))
    , pageColor(CommonStyleV5::ContextColorNone)
    , tabBarPosition(StyleOptionFrameV5::TabBarTopPosition)
{
}

StyleRibbonOptionHeaderV5::StyleRibbonOptionHeaderV5(const StyleRibbonOptionHeaderV5& other)
    : QStyleOptionHeader(other)
    , firstTab(other.firstTab)
    , lastTab(other.lastTab)
    , contextText(other.contextText)
    , pageColor(other.pageColor)
    , tabBarPosition(other.tabBarPosition)
{
    *this = other;
}

/*!
\class Qtitan::StyleRibbonOptionGroupV5
\internal
*/
StyleRibbonOptionGroupV5::StyleRibbonOptionGroupV5()
    : QStyleOption()
    , widthOptButton(0)
    , heightCaption(0)
    , text("")
    , textAlignment(Qt::AlignCenter)
    , textElideMode(Qt::ElideNone)
{
}

StyleRibbonOptionGroupV5::StyleRibbonOptionGroupV5(const StyleRibbonOptionGroupV5& other)
    : QStyleOption(other)
    , widthOptButton(other.widthOptButton)
    , heightCaption(other.heightCaption)
    , text(other.text)
    , textAlignment(other.textAlignment)
    , textElideMode(other.textElideMode)
{
    *this = other;
}

/*!
\class Qtitan::StyleRibbonSysFrameMenuV5
\internal
*/
StyleRibbonSysFrameMenuV5::StyleRibbonSysFrameMenuV5()
    : isAdditionalPages(false)
    , actionWidth(0)
    , margins()
{
}

StyleRibbonSysFrameMenuV5::StyleRibbonSysFrameMenuV5(const StyleRibbonSysFrameMenuV5& other)
    : QStyleOption(other)
    , isAdditionalPages(other.isAdditionalPages)
    , actionWidth(other.actionWidth)
    , margins(other.margins)
{
    *this = other;
}

/*!
\class Qtitan::StyleRibbonGalleryItemV5
\internal
*/
StyleRibbonGalleryItemV5::StyleRibbonGalleryItemV5()
    : separator(false)
    , rectItem(QRect())
    , caption("")
    , icon()
{
}

StyleRibbonGalleryItemV5::StyleRibbonGalleryItemV5(const StyleRibbonGalleryItemV5& other)
    : QStyleOption(other)
    , separator(other.separator)
    , rectItem(other.rectItem)
    , caption(other.caption)
    , icon(other.icon)
{
    *this = other;
}

/*!
\class Qtitan::StyleHintReturnSysFrameMenuV5
\internal
*/
StyleHintReturnSysFrameMenuV5::StyleHintReturnSysFrameMenuV5(const QMargins& mrgs)
    : QStyleHintReturn()
    , margins(mrgs)
{
}

StyleHintReturnSysFrameMenuV5::~StyleHintReturnSysFrameMenuV5()
{
}

/*!
\class Qtitan::AutoHideWindowStyleOptionV5
\internal
*/
AutoHideWindowStyleOptionV5::AutoHideWindowStyleOptionV5()
    : QStyleOption(Version)
    , position(QInternal::LeftDock)
{
}

AutoHideWindowStyleOptionV5::AutoHideWindowStyleOptionV5(int version)
    : QStyleOption(version)
    , position(QInternal::LeftDock)
{
}

/*!
\class Qtitan::DockTitleBarStyleOptionV5
\internal
*/
DockTitleBarStyleOptionV5::DockTitleBarStyleOptionV5()
    : QStyleOption(Version)
    , closable(false)
    , movable(false)
    , floatable(false)
    , autohide(false)
    , menu(false)
    , verticalTitleBar(false)
    , lineWidth(0)
{
}

DockTitleBarStyleOptionV5::DockTitleBarStyleOptionV5(int version)
    : QStyleOption(version)
    , closable(false)
    , movable(false)
    , floatable(false)
    , autohide(false)
    , menu(false)
    , verticalTitleBar(false)
    , lineWidth(0)
{
}

/*!
\class Qtitan::DockPaneStyleOptionV5
\internal
*/
DockPaneStyleOptionV5::DockPaneStyleOptionV5()
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : QStyleOptionDockWidget(Version)
#else
    : QStyleOptionDockWidgetV2(Version)
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    , autohide(false)
    , menu(false)
{
}

DockPaneStyleOptionV5::DockPaneStyleOptionV5(int version)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : QStyleOptionDockWidget(version)
#else
    : QStyleOptionDockWidgetV2(version)
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    , autohide(false)
    , menu(false)
{
}
