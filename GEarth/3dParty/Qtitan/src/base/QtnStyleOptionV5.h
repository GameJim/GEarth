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
#ifndef QTN_STYLEOPTIONV5_H
#define QTN_STYLEOPTIONV5_H

#include <QStyleOption>
#include <QtnCommonStyleV5.h>
#include "QtitanDef.h"

class QTextDocument;

QTITAN_BEGIN_NAMESPACE
/* StyleOptionFrameV5 */
class QTITAN_EXPORT StyleOptionFrameV5 : public QStyleOption
{
public:
    StyleOptionFrameV5();
    StyleOptionFrameV5(const StyleOptionFrameV5& other);
    ~StyleOptionFrameV5();
public:
    enum TabBarPosition { TabBarTopPosition = 1, TabBarBottomPosition };
    void* hdc;
    int  frameBorder;
    int  titleBarSize;
    int  tabBarSize;
    int  statusHeight;
    bool active;
    bool hasStatusBar;
    bool titleVisible;
    bool maximizeFlags;
    bool isBackstageVisible;
    TabBarPosition tabBarPosition;
    QRect clientRect;
    QRect tabBarRect;
};

/* StyleOptionBackstageButtonV5 */
class QTITAN_EXPORT StyleOptionBackstageButtonV5 : public QStyleOptionToolButton
{
public:
    StyleOptionBackstageButtonV5();
    StyleOptionBackstageButtonV5(const StyleOptionBackstageButtonV5& other);
public:
    bool tabStyle;
    QTextDocument* document;
};

/* StyleOptionRibbonBackstageV5 */
class QTITAN_EXPORT StyleOptionRibbonBackstageV5 : public QStyleOption
{
public:
    StyleOptionRibbonBackstageV5();
    StyleOptionRibbonBackstageV5(const StyleOptionRibbonBackstageV5& other);
public:
    int menuWidth;
};

class OfficeFrameHelper;
/* StyleOptionRibbonV5 */
class QTITAN_EXPORT StyleOptionRibbonV5 : public QStyleOption
{
public:
    StyleOptionRibbonV5();
    StyleOptionRibbonV5(const StyleOptionRibbonV5& other);
    ~StyleOptionRibbonV5();
public:
    QRect rectTabBar;
    QRect rectCurrentTab;
    OfficeFrameHelper* frameHelper;
    int titleBarHeight;
    bool minimized;
    bool isBackstageVisible;
    StyleOptionFrameV5::TabBarPosition tabBarPosition;
    CommonStyleV5::ContextColor pageColor;
    QRegion airRegion;
    QPixmap pixTitleBackground;
};

class ContextHeader;
/* StyleOptionTitleBarV5 */
class QTITAN_EXPORT StyleOptionTitleBarV5 : public QStyleOptionTitleBar
{
public:
    StyleOptionTitleBarV5();
    StyleOptionTitleBarV5(const StyleOptionTitleBarV5 &other);
    struct ContextData
    {
        ContextData()
            : rect(QRect())
            , strTitle("")
            , color(CommonStyleV5::ContextColorNone) {}
        ContextData(const QRect& _rect, const QString& _strTitle, CommonStyleV5::ContextColor _color)
            : rect(_rect)
            , strTitle(_strTitle)
            , color(_color) {}
        QRect rect;
        QString strTitle;
        CommonStyleV5::ContextColor color;
    };
public:
    OfficeFrameHelper* frameHelper;
    QRect rcTitleText;
    QRect rectTabBar;
    QRect rcRibbonClient;
    QRect rcQuickAccess;
    bool drawIcon;
    int quickAccessVisibleCount;
    bool quickAccessOnTop;
    bool quickAccessVisible;
    bool existSysButton;
    bool normalSysButton;
    bool isBackstageVisible;
    uint alignment;
    QVector<ContextData> listContextHeaders;
    QRegion airRegion;
    QColor clrText;
    QFont fontTextTitle;
protected:
    StyleOptionTitleBarV5(int version);
};

/* StyleCaptionButtonV5 */
class QTITAN_EXPORT StyleCaptionButtonV5 : public QStyleOptionToolButton
{
public:
    StyleCaptionButtonV5();
    StyleCaptionButtonV5(const StyleCaptionButtonV5& other);
};

/* StyleOptionFrameV5 */
class QTITAN_EXPORT StyleSystemToolButtonV5 : public QStyleOptionToolButton
{
public:
    StyleSystemToolButtonV5();
    StyleSystemToolButtonV5(const StyleSystemToolButtonV5& other);
};

/* StyleHintReturnThemeColorV5 */
class QTITAN_EXPORT StyleHintReturnThemeColorV5 : public QStyleHintReturn
{
public:
    StyleHintReturnThemeColorV5(const QString& secName, const QString& kName, QColor col = QColor());
    ~StyleHintReturnThemeColorV5();
public:
    QString sectionName;
    QString keyName;
    QColor color;
    bool isValid;
};

/* StyleHintReturnThemeColorV5 */
class QTITAN_EXPORT StyleRibbonQuickAccessBarV5 : public QStyleOption
{
public:
    StyleRibbonQuickAccessBarV5();
    StyleRibbonQuickAccessBarV5(const StyleRibbonQuickAccessBarV5& other);
public:
    enum QuickAccessBarPosition { QATopPosition = 1, QABottomPosition };
    QuickAccessBarPosition quickAccessBarPosition;
};

/* StyleRibbonOptionHeaderV5 */
class QTITAN_EXPORT StyleRibbonOptionHeaderV5 : public QStyleOptionHeader
{
public:
    StyleRibbonOptionHeaderV5();
    StyleRibbonOptionHeaderV5(const StyleRibbonOptionHeaderV5& other);
public:
    bool firstTab;
    bool lastTab;
    QString contextText;
    CommonStyleV5::ContextColor pageColor;
    StyleOptionFrameV5::TabBarPosition tabBarPosition;
};

/* StyleRibbonOptionGroupV5 */
class QTITAN_EXPORT StyleRibbonOptionGroupV5 : public QStyleOption
{
public:
    StyleRibbonOptionGroupV5();
    StyleRibbonOptionGroupV5(const StyleRibbonOptionGroupV5& other);
public:
    int widthOptButton;
    int heightCaption;
    QString text;
    Qt::Alignment textAlignment;
    Qt::TextElideMode textElideMode;
};

/* StyleRibbonSysFrameMenuV5 */
class QTITAN_EXPORT StyleRibbonSysFrameMenuV5 : public QStyleOption
{
public:
    StyleRibbonSysFrameMenuV5();
    StyleRibbonSysFrameMenuV5(const StyleRibbonSysFrameMenuV5& other);
public:
    bool isAdditionalPages;
    int actionWidth;
    QMargins margins;
};

/* StyleRibbonGalleryItemV5 */
class QTITAN_EXPORT StyleRibbonGalleryItemV5 : public QStyleOption
{
public:
    StyleRibbonGalleryItemV5();
    StyleRibbonGalleryItemV5(const StyleRibbonGalleryItemV5& other);
public:
    bool separator;
    QRect rectItem;
    QString caption;
    QIcon icon;
};

/* StyleHintReturnThemeColorV5 */
class QTITAN_EXPORT StyleHintReturnSysFrameMenuV5 : public QStyleHintReturn
{
public:
    StyleHintReturnSysFrameMenuV5(const QMargins& mrgs);
    ~StyleHintReturnSysFrameMenuV5();
public:
    QMargins margins;
};

/* AutoHideWindowStyleOptionV5 */
class QTITAN_EXPORT AutoHideWindowStyleOptionV5 : public QStyleOption
{
public:
    AutoHideWindowStyleOptionV5();
    AutoHideWindowStyleOptionV5(const AutoHideWindowStyleOptionV5& other) : QStyleOption(other) { *this = other; }
public:
    QInternal::DockPosition position;
protected:
    AutoHideWindowStyleOptionV5(int version);
};

/* DockTitleBarStyleOptionV5 */
class DockTitleBarStyleOptionV5 : public QStyleOption
{
public:
    DockTitleBarStyleOptionV5();
    DockTitleBarStyleOptionV5(const DockTitleBarStyleOptionV5& other) : QStyleOption(other) 
    { *this = other; }
public:
    QString title;
    QRect rcText;
    bool closable;
    bool movable;
    bool floatable;
    bool autohide;
    bool menu;
    bool verticalTitleBar;
    int lineWidth;
protected:
    DockTitleBarStyleOptionV5(int version);
};

class DockPaneStyleOptionV5
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    : public QStyleOptionDockWidget
#else
    : public QStyleOptionDockWidgetV2
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
{
public:
    DockPaneStyleOptionV5();
    DockPaneStyleOptionV5(const DockPaneStyleOptionV5& other)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        : QStyleOptionDockWidget(other)
#else
        : QStyleOptionDockWidgetV2(other)
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    {
        *this = other;
    }
public:
    bool autohide;
    bool menu;
protected:
    DockPaneStyleOptionV5(int version);
};

QTITAN_END_NAMESPACE

#endif // QTN_STYLEOPTIONV5_H
