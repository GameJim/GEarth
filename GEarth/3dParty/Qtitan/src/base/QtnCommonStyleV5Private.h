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
#ifndef QTN_COMMONSTYLEPV5RIVATE_H
#define QTN_COMMONSTYLEPV5RIVATE_H

#include <QTime>
#include <QPainter>
#include <QApplication>
#include <QXmlStreamReader>
#include "QtnCommonStyleV5.h"


QTITAN_BEGIN_NAMESPACE

#define _qtn_PopupLable                "PopupLable_"
#define _qtn_SplitActionPopup          "SplitActionPopup_"
#define _qtn_TitleGroupsVisible        "TitleGroupsVisible"
#define _qtn_PopupButtonGallery        "PopupButtonGallery"
#define _qtn_ScrollUpButtonGallery     "ScrollUpButtonGallery"
#define _qtn_ScrollDownButtonGallery   "ScrollDownButtonGallery"

/* QtitanAnimationV5 */
class QtitanAnimationV5 : public QObject
{
public:
    QtitanAnimationV5() : m_widget(0), m_running(true) {}
    virtual ~QtitanAnimationV5();
public:
    QWidget* widget() const { return m_widget; }
    bool running() const { return m_running; }
    const QTime& startTime() const { return m_startTime; }
    void setRunning(bool val) { m_running = val; }
    void setWidget(QWidget* widget);
    void setStartTime(const QTime& startTime) { m_startTime = startTime; }
public:
    virtual void paint(QPainter* painter, const QStyleOption* option);

protected:
    void drawBlendedImage(QPainter* painter, QRect rect, float value);
    virtual bool eventFilter(QObject* obj, QEvent* event);

protected:
    QTime m_startTime;
    QWidget* m_widget;
    QImage m_primaryImage;
    QImage m_secondaryImage;
    QImage m_tempImage;
    bool m_running;
};

/* QtitanTransitionV5 */
class QtitanTransitionV5 : public QtitanAnimationV5
{
public:
    QtitanTransitionV5();
    virtual ~QtitanTransitionV5();
public:
    void setDuration(int duration) { m_duration = duration; }
    void setStartImage(const QImage& image) { m_primaryImage = image; }
    void setEndImage(const QImage& image) { m_secondaryImage = image; }
    virtual void paint(QPainter* painter, const QStyleOption* option);
    int duration() const { return m_duration; }
public:
    int m_duration;
};

typedef QHash<QPalette::ColorRole, QBrush> RoleBrushHash;
typedef QHash<QPalette::ColorGroup, RoleBrushHash > PaletteHash;

class AbstractData
{
};

class PixelMetricData : public AbstractData
{
public:
    PixelMetricData();
    PixelMetricData(const PixelMetricData& other) : value(other.value), scalableDpi(other.scalableDpi) {}
public:
    uint value;
    bool scalableDpi;
};

class WidgetData : public AbstractData
{
public:
    WidgetData() : m_hashPalette(), hover(false), autoFillBackground(false){}
    WidgetData(const WidgetData& other) : m_hashPalette(other.m_hashPalette), hover(other.hover), autoFillBackground(other.autoFillBackground){}
public:
    PaletteHash m_hashPalette;
    bool hover;
    bool autoFillBackground;
};

typedef QHash<uint, QColor> ColorHash;

typedef QHash<QString, int> StyleHintHash;
typedef QHash<QString, StyleHintHash> WidgetStyleHintHash;
typedef QHash<QString, QStringList> StandardPixmapHash;
typedef QHash<QString, PixelMetricData> PixelMetricHash;
typedef QHash<QString, PixelMetricHash> WidgetPixelMetricHash;
typedef QHash<QString, WidgetData> WidgetDataHash;

/* StyleConfigReader */
class StyleConfigReader
{
public:
    StyleConfigReader();
public:
    bool read(QIODevice* device);
    QString errorString() const;

    PaletteHash hashGlobalPalette() const { return m_hashGlobalPalette; }
    ColorHash hashColorPixmap() const { return m_hashGlobalColorPixmap; }
    StyleHintHash hashGlobalStyleHint() const { return m_hashGlobalStyleHint; }
    WidgetStyleHintHash hashWidgetStyleHint() const { return m_hashWidgetStyleHint; }
    StandardPixmapHash hashStandardPixmap() const { return m_hashGlobalStandardPixmap; }
    PixelMetricHash hashGlobalMetricsData() const { return m_hashGlobalMetricsData; }
    WidgetPixelMetricHash hashWidgetMetricsData() const { return m_hashWidgetMetricsData; }
    WidgetDataHash hashWidgetsData() const { return m_hashWidgetsData; }
protected:
    void readStyle();
    void readWidget();
    void readGlobal();

    void readStandardPixmap();
    void readPixelMetric(PixelMetricHash& pixelMetric);
    void readStyleHint(StyleHintHash& styleHint);

    void readPalette(PaletteHash& hashPalette);
    void readColors(PaletteHash& hashPalette);
protected:
    QXmlStreamReader xml;

    PaletteHash m_hashGlobalPalette;
    ColorHash m_hashGlobalColorPixmap;
    StyleHintHash m_hashGlobalStyleHint;
    WidgetStyleHintHash m_hashWidgetStyleHint;
    StandardPixmapHash m_hashGlobalStandardPixmap;
    PixelMetricHash m_hashGlobalMetricsData;
    WidgetPixelMetricHash m_hashWidgetMetricsData;

    WidgetDataHash m_hashWidgetsData;
private:
    Q_DISABLE_COPY(StyleConfigReader)
};

/* StyleConfig */
class StyleConfig
{
public:
    StyleConfig();
public:
    bool load(const QString& fileName);
    void setPaletteWidget(QWidget* wd, const WidgetData& data);
    void setConfigWidget(QWidget* wd);

    RoleBrushHash globalGlobalPalette(QPalette::ColorGroup group) const;
    int pixelMetric(const QString& strWidget, const QString& numMetric, int defaultVal, bool& scalableDpi, bool* ok = 0) const;
    int styleHint(const QString& strWidget, const QString& numMetric, int defaultVal) const;

    QColor standardColorPixmap(uint satate) const;
    QString standardPixmapName(const QString& numPixmap) const;
protected:
    PaletteHash m_hashGlobalPalette;
    StyleHintHash m_hashGlobalStyleHint;
    WidgetStyleHintHash m_hashWidgetStyleHint;
    ColorHash m_hashGlobalColorPixmap;
    StandardPixmapHash m_hashGlobalStandardPixmap;
    PixelMetricHash m_hashGlobalMetricsData;
    WidgetPixelMetricHash m_hashWidgetMetricsData;

    WidgetDataHash m_hashWidgetsData;
private:
    Q_DISABLE_COPY(StyleConfig)
};

/* StandardSymbolIcon */
class StandardSymbolIcon
{
public:
    StandardSymbolIcon();
public:
    static IconSymbol symbolStandardPixmap(QStyle::StandardPixmap sp);
    static IconSymbol symbolSubControl(QStyle::SubControl sp);
public:
    QPixmap standardSymbolPixmap(QStyle::StandardPixmap sp, const QStyleOption* option) const;
};

class CommonPaintManager;
/* CommonStyleV5Private */
class CommonStyleV5Private : public QObject
{
    Q_OBJECT
public:
    QTN_DECLARE_PUBLIC(CommonStyleV5);
    enum { InvalidMetric = -23576 };
    enum { InvalidHint = -23576 };
    enum ImageState
    {
        Black,
        Gray,
        LtGray,
        White,
        DkGray,
        Black2,
        InvalidState,
    };
public:
    explicit CommonStyleV5Private();
    virtual ~CommonStyleV5Private();
public:
    void initialization();
    void updateStyle();
    void updateStandardPixmap(const QString& indxIcon);
    void setPaletteFromGlobal(QPalette::ColorGroup gr, QPalette::ColorRole cr, QPalette& palette) const;

    QPixmap standardPixmap(const QString& indxIcon, ImageState state) const;
public:
    static int pixelMetricFromSystemDp(QStyle::PixelMetric pm, const QStyleOption* option = 0, const QWidget* widget = 0);
    static QSize qtn_bestSizePixmap();
    static QPixmap loadIcon(const QString& name);
    static bool buttonVisible(const QStyle::SubControl sc, const QStyleOptionTitleBar* tb);
    static int defaultDpiX();
    static int defaultDpiY();
    static qreal dpiScaled(qreal value);
    static int getDPI();
    static int getDPIToPercent();

    QPixmap loadIcon(const QString& icon, const QColor &color) const;
public:
    // for standard controls
    // draw PrimitiveElement
    virtual bool drawFrameWindow(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawPanelMenu(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorArrow(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawFrameGroupBox(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorCheckBox(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorRadioButton(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorToolBarHandle(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorToolBarSeparator(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorHeaderArrow(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawIndicatorBranch(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawFrameFocusRect(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawFrameDockWidget(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    // PE - ButtonTool
    virtual bool drawPanelButtonTool(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawPanelButtonCommand(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawFrameLineEdit(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawFrameStatusBarItem(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    // draw ComplexControl
    virtual bool drawTitleBar(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawMdiControls(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawComboBox(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawSlider(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawSpinBox(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawToolButton(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const;

    // draw ControlElement
    virtual bool drawMenuBarItem(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawMenuItem(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawToolBoxTabShape(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawToolButtonLabel(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    // CE - draw components ScrollBar
    virtual bool drawScrollBarAddSubLine(QStyle::ControlElement ce, const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawScrollBarAddSubPage(QStyle::ControlElement ce, const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawScrollBarSlider(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawTabBarTabShape(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawHeaderSection(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawHeaderLabel(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    virtual bool drawDockWidgetTitle(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    ///////////////////////////////////////////////////////////////////////////////
    // for RibbonBar
    // draw PrimitiveElement
    virtual bool drawRibbonBar(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonFrameGroups(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonFrameGroup(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonFrameGallery(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonSysFrameMenu(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonSysFramePagePopupList(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonSysFramePagePopup(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonPanelButton(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonOptionButton(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonQuickAccessButton(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonBackstageFrameMenu(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonBackstagePanelButton(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    // draw ControlElement
    virtual bool drawRibbonTabShapeLabel(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonTab(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonTabBar(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonGroupLabel(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonSysMenuItem(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonSysRecentFileItem(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonButtonLabel(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonGalleryItem(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonBackstageMenuItem(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonBackstageButtonLabel(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawOfficePopupSizeGrip(const QStyleOption* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    // draw ComplexControl
    virtual bool drawRibbonFileButton(const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget = Q_NULL) const;
    virtual bool drawRibbonButton(const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget = Q_NULL) const;
    virtual bool drawRibbonTitleBarWidget(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonGalleryButton(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    virtual bool drawRibbonBackstageButton(const QStyleOptionComplex* opt, QPainter* p, const QWidget* w = Q_NULL) const;
    // subControlRect
    virtual QRect ribbonSubControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc, const QWidget* widget = Q_NULL) const;
    virtual QSize ribbonSizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& csz, const QWidget* widget = Q_NULL) const;
    // subControlRect
    virtual QRect ribbonSubElementRect(QStyle::SubElement se, const QStyleOption* option, const QWidget* widget = Q_NULL) const;
    // pixelMetric
    virtual int ribbonPixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget = Q_NULL) const;
    // pixelMetric
    virtual int ribbonStyleHint(QStyle::StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const;
    // standardPixmap
    virtual QPixmap ribbonStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const;
    ///////////////////////////////////////////////////////////////////////////////
    // for Docking
    // draw PrimitiveElement
    virtual bool drawDockAutoHideFrameWindow(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    // standardPixmap
    virtual QPixmap dockStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const;

protected:
    void drawIcon(const QPixmap& px, QPainter* p, const QRect& rect) const;
    void drawHighlightButton(const QStyleOption* option, QPainter* p) const;
    void drawHighlightItemGallery(const QStyleOption* option, QPainter* p) const;
    void drawButton(const QPixmap& px, const QStyleOption* option, QPainter* p) const;
protected:
    QSize bestImageSize() const;
    void calcRibbonItemHeight();
private:
    QColor darkBackground(const QPalette &pal) const;
    QColor darkBackgroundLight(const QPalette &pal) const;
    QColor outlineMidlight(const QPalette& pal) const;
    QColor outlineLight(const QPalette& pal) const;
    QColor buttonHighlightColor(const QPalette& pal, bool highlightDn, bool border = false) const;
    QColor titleBarWindowColor(const QPalette& pal, bool isActive) const;
    QColor titleBarFrameColor(const QPalette& pal, bool isActive) const;
public:
    StyleConfig m_config;
    QHash<QString, QPixmap> m_stdIconsBlackHash;
    QHash<QString, QPixmap> m_stdIconsDkGrayHash;
    QHash<QString, QPixmap> m_stdIconsGrayHash;
    QHash<QString, QPixmap> m_stdIconsLtGrayHash;
    QHash<QString, QPixmap> m_stdIconsWhiteHash;
    QHash<QString, QPixmap> m_stdIconsBlack2Hash;
protected:
    int m_ribbonItemHeight;
    QColor m_colorIconBlack;
    QColor m_colorIconDkGray;
    QColor m_colorIconGray;
    QColor m_colorIconLtGray;
    QColor m_colorIconWhite;
    QColor m_colorIconBlack2;
    QHash<QString, QPixmap> m_hashNormalPixmap;
    QHash<QString, QPixmap> m_hashDisabledPixmap;
    StandardSymbolIcon m_standardSymbol;

    static QColor m_clrCloseButton;
    static QColor m_clrCloseButtonHighlight;
    static QFont  m_iconSymbolFont;
    static double logPixel;
private:
    Q_DISABLE_COPY(CommonStyleV5Private);
    friend class StandardSymbolIcon;
};



#ifdef Q_OS_MACX
bool set_style_mask_hook(QWidget* w);
bool unset_style_mask_hook(QWidget* w);
#endif

QTITAN_END_NAMESPACE

#endif // QTN_COMMONSTYLEV5PRIVATE_H
