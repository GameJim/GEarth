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
#ifndef QTN_COMMONSTYLEV5_H
#define QTN_COMMONSTYLEV5_H

#include <QProxyStyle>
#include <QStyleOption>

#include "QtnWindowTitleBar.h"
#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE

enum IconSymbol
{
    Symbol_None,
    Symbol_ChromeClose,
    Symbol_ChromeMinimize,
    Symbol_ChromeMaximize,
    Symbol_ChromeRestore,
    Symbol_Pin,
    Symbol_PinFill,
    Symbol_Pinned,
    Symbol_PinnedFill,
    Symbol_StockDown,
    Symbol_FlickUp,
    Symbol_ChevronDown,
    NumSymbols
};

/* SymbolInfo */
struct SymbolInfo
{
    IconSymbol symbol;
    const QChar code1;
    const QChar code2;
};


class CommonStyleV5Private;
/* CommonStyleV5 */
class QTITAN_EXPORT CommonStyleV5 : public QProxyStyle
{
    Q_OBJECT
public:
    enum ContextColor
    {
        ContextColorNone,
        ContextColorGreen,
        ContextColorBlue,
        ContextColorRed,
        ContextColorYellow,
        ContextColorCyan,
        ContextColorPurple,
        ContextColorOrange,
    };

    /*! \internal */
    enum QtnPixelMetric
    {
        PM_RibbonReducedGroupWidth = PM_CustomBase + 1,
        PM_RibbonHeightCaptionGroup,
        PM_RibbonVMarginCaptionSysMenu,
        PM_RibbonHMarginCaptionSysMenu,
        PM_RibbonPopupMenuGripHeight,
        PM_RibbonItemHeight,
    };

    /*! \internal */
    enum QtnPrimitiveElement
    {
        // PE_CustomBase = 0xf0000000 - base
        PE_PanelRibbonBar = PE_CustomBase + 1,
        PE_RibbonFrameGroups,
        PE_RibbonFrameGroup,
        PE_RibbonFrameGallery,
        PE_RibbonSysFrameMenu,
        PE_RibbonSysFramePagePopupList,
        PE_RibbonSysFramePagePopup,
        PE_RibbonPanelButton,
        PE_RibbonOptionButton,
        PE_RibbonQuickAccessButton,
        PE_RibbonBackstageFrameMenu,
        PE_RibbonBackstagePanelButton,
        PE_DockAutoHideFrameWindow,
    };

    /*! \internal */
    enum QtnControlElement
    {
        CE_RibbonTabShapeLabel = CE_CustomBase + 1,
        CE_RibbonTab,
        CE_RibbonTabBar,
        CE_RibbonGroupLabel,
        CE_RibbonSysMenuItem,
        CE_RibbonBackstageMenuItem,
        CE_RibbonBackstageButtonLabel,
        CE_RibbonSysRecentFileItem,
        CE_RibbonButtonLabel,
        CE_RibbonGalleryItem,
        CE_RibbonGripBar,
    };

    /*! \internal */
    enum QtnComplexControl
    {
        CC_RibbonFileButton = CC_CustomBase + 1,
        CC_RibbonContextBar,
        CC_RibbonGalleryButton,
        CC_RibbonBackstageButton,
        CC_RibbonButton,
    };

    /*! \internal */
    enum QtnSubElement
    {
        SE_RibbonSysHeaderLabelPopupList = SE_CustomBase + 1,
    };

    /*! \internal */
    enum QtnSubControl
    {
        SC_TitleBarPinlButton = SC_CustomBase + 1,
        SC_TitleBarPinlFillButton,
        SC_TitleBarPinnedButton,
        SC_TitleBarPinnedFillButton,
    };

    /*! \internal */
    enum QtnStyleHint
    {
        SH_QtitanBegin = SH_CustomBase + 1,
        SH_RibbonBegin = SH_QtitanBegin,
        SH_RibbonSysFrameMenu = SH_RibbonBegin,
        SH_RecentFileListBackground,
        SH_RibbonBackstageHideTabs,
        SH_RibbonEnd = SH_RibbonBackstageHideTabs,
    };

    /*! \internal */
    enum QtnStandardPixmap
    {
        SP_RibbonButtonPopupMenu = SP_CustomBase + 1,
        SP_RibbonButtonScrollUp,
        SP_RibbonButtonScrollDown,
        SP_RibbonButtonGroupOption,
        SP_RibbonShadedArrowUp,
        SP_RibbonShadedArrowDown,
        SP_TitleBarPinButton,
        SP_TitleBarPinFillButton,
        SP_TitleBarPinnedButton,
        SP_TitleBarPinnedFillButton,

    };

public:
    virtual ~CommonStyleV5();
    static void createCommonProxyStyle();
protected:
    CommonStyleV5(QStyle* style = Q_NULL);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    CommonStyleV5(const QString& key);
#endif
public:
    virtual QPalette standardPalette() const;
    virtual void polish(QPalette& palette);
    virtual void polish(QApplication* app);
    virtual void polish(QWidget* widget);
    virtual void unpolish(QApplication* app);
    virtual void unpolish(QWidget* widget);
public:
    virtual QRect itemTextRect(const QFontMetrics& fm, const QRect& r, int flags, bool enabled, const QString& text) const;
    virtual QRect itemPixmapRect(const QRect& r, int flags, const QPixmap& pixmap) const;

    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w = 0) const;
    virtual void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w = 0) const;
    virtual void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* w = 0) const;
    virtual QRect subElementRect(SubElement r, const QStyleOption* opt, const QWidget* widget = Q_NULL) const;
    virtual SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, const QPoint& pt, const QWidget* w = Q_NULL) const;
    virtual QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* w = Q_NULL) const;
    virtual QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget = Q_NULL) const;

    virtual int pixelMetric(PixelMetric m, const QStyleOption* opt = Q_NULL, const QWidget* widget = Q_NULL) const;
    virtual int styleHint(StyleHint sh, const QStyleOption* opt = Q_NULL, const QWidget* w = Q_NULL, QStyleHintReturn* shret = Q_NULL) const;
    virtual QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* opt = Q_NULL, const QWidget* widget = Q_NULL) const;
    virtual QPixmap standardPixmap(StandardPixmap sp, const QStyleOption* opt = Q_NULL, const QWidget* widget = Q_NULL) const;
    virtual QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap& pixmap, const QStyleOption* opt) const;

    virtual int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, 
        const QStyleOption *option = Q_NULL, const QWidget *widget = Q_NULL) const;
public:
    void setStyleConfig(const QString& fileName);

    static QPixmap qtn_getSymbolToPixmap(IconSymbol symbol, const QSize& sizeIcon, const QColor& color = Qt::black);
    static QColor  qtn_getColorToRGB(CommonStyleV5::ContextColor color);
protected:
    CommonStyleV5(CommonStyleV5Private& dd);
protected:
    QTN_DECLARE_PRIVATE(CommonStyleV5)
private:
    Q_DISABLE_COPY(CommonStyleV5)
};

QTITAN_END_NAMESPACE

#endif // QTN_COMMONSTYLE_H
