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
#include "QtnCommonStyleV5.h"
#include "QtnCommonStyleV5Private.h"
#include "QtnStyleOptionV5.h"
#include "QtnPlatform.h"

#include <QMenu>
#include <QScreen>
#include <QFileInfo>
#include <QMetaEnum>
#include <QColormap>
#include <QStyleFactory>
#include <QApplication>
#include <QStatusBar>
#include <QDockWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <qdrawutil.h>

#include <QTextDocument>
#include <QTextFrameFormat>
#include <QAbstractTextDocumentLayout>
#include <QTextFrame>

#ifdef Q_OS_MAC
//#define QTN_MAC_BUTTONS
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#endif

static const int windowsSepHeight    = 9;    // height of the separator
static const int windowsItemFrame    = 2;    // menu item frame width
static const int windowsCheckMarkWidth  = 16;   // checkmarks width on windows
static const int windowsArrowHMargin = 6;    // arrow horizontal margin
static const int windowsRightBorder  = 15;   // right border on windows
static const int windowsItemVMargin  = 8;    // menu item ver text margin
static const int windowsItemHMargin  = 3;    // menu item hor text margin

static const int systemPopupBorderLeft = 1;
static const int systemPopupBorderRight = 1;
//static const int systemPopupBorderTop = 18;
//static const int windowsTabSpacing   = 12;   // space between text and tab
static const int systemPopupBorderBottom = 29;

static const QString iconNameGroupOption = QString("group_option");
static const QString iconNameRibbonButtonPopupMenu = QString("qaccess_button");
static const QString iconNameShadedArrowDown = QString("indicator_shadedarrowdown");
static const QString iconNameShadedArrowUp = QString("indicator_shadedarrowup");


namespace QtnPrivate
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    static QWindow* qtn_getWindow(const QWidget* widget)
    {
        return widget ? widget->window()->windowHandle() : 0;
    }
#endif

    bool qtn_isExistWindowTitleBar(const QWidget* widget)
    {
        return widget != Q_NULL && widget->isTopLevel() && widget->findChild<WindowTitleBar*>() != Q_NULL;
    }

    static inline bool qtn_isAncestor(const QObject* child, const char* strParent)
    {
        while (child)
        {
            if (child && child->inherits(strParent))
                return true;
            child = child->parent();
        }
        return false;
    }

    static QPixmap qtn_ColorizedImage(const QPixmap& px, const QColor& clrSrc, const QColor& clrDest)
    {
        if (!clrDest.isValid())
            return px;

        QImage img = px.toImage();
        for (int y = 0; y < img.height(); ++y)
        {
            QRgb *scanLine = (QRgb*)img.scanLine(y);
            for (int x = 0; x < img.width(); ++x)
            {
                QRgb pixel = *scanLine;
                if (pixel == clrSrc.rgb())
                    *scanLine = clrDest.rgb();
                ++scanLine;
            }
        }
        return QPixmap::fromImage(img);
    }

    static QLatin1String qtn_bestImageSize(QSize* sz, double* pdblScale)
    {
        QLatin1String fileSuffix("9x9.png");
        *sz = QSize(9, 9);
        qreal dblScale = static_cast<qreal>(CommonStyleV5Private::defaultDpiX()) / 100.0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        int depth = QApplication::primaryScreen() ? QApplication::primaryScreen()->depth() : QColormap::instance().depth();
#else
        int depth = QColormap::instance().depth();
#endif
        if (depth >= 32)
        {
            if (dblScale > 1.51)
            {
                fileSuffix = QLatin1String("18x18.png");
                dblScale = dblScale / 2.0;
                *sz = QSize(18, 18);
            }
            else if (dblScale > 1.26)
            {
                fileSuffix = QLatin1String("13x13.png");
                dblScale = dblScale / 1.5;
                *sz = QSize(13, 13);
            }
            else if (dblScale > 1.1)
            {
                fileSuffix = QLatin1String("11x11.png");
                dblScale = dblScale / 1.25;
                *sz = QSize(11, 11);
            }
        }
        *pdblScale = dblScale;
        return fileSuffix;
    }
/*
    static void draw3DRect(QPainter& pnt, const QColor& col, int x, int y, int w, int h, bool up)
    {
        QColor light = col.light(135);
        QColor dark = col.dark(140);
        pnt.setPen(up ? light : dark);
        pnt.drawLine(x, y, x + w, y);
        pnt.drawLine(x, y, x, y + h);
        pnt.setPen(up ? dark : light);
        pnt.drawLine(x, y + h, x + w, y + h);
        pnt.drawLine(x + w, y, x + w, y + h);
        pnt.setPen(col);
        pnt.drawPoint(x + w, y);
        pnt.drawPoint(x, y + h);
    }

    static void draw3DRect(QPainter& pnt, const QColor& colLight, const QColor& colDark, int x, int y, int w, int h, bool up)
    {
        QColor light = colLight;
        QColor dark = colDark;
        pnt.setPen(up ? light : dark);
        pnt.drawLine(x, y, x + w, y);
        pnt.drawLine(x, y, x, y + h);

        pnt.setPen(up ? dark : light);
        pnt.drawLine(x, y + h, x + w, y + h);
        pnt.drawLine(x + w, y, x + w, y + h);

        pnt.setPen(colLight);
        pnt.drawPoint(x + w, y);
        pnt.drawPoint(x, y + h);
    }
*/
    static void draw3dRect(QPainter& p, int x, int y, int cx, int cy, const QColor& clrTopLeft, const QColor& clrBottomRight)
    {
        p.fillRect(x, y, cx - 1, 1, clrTopLeft);
        p.fillRect(x, y, 1, cy - 1, clrTopLeft);
        p.fillRect(x + cx, y, -1, cy, clrBottomRight);
        p.fillRect(x, y + cy, cx, -1, clrBottomRight);
    }

    /*! \internal */
    static void drawArrow(const QStyle* style, const QStyleOptionToolButton* toolbutton,
        const QRect& rect, QPainter* painter, const QWidget* widget = 0)
    {
        QStyle::PrimitiveElement pe;

        switch (toolbutton->arrowType)
        {
        case Qt::LeftArrow:
            pe = QStyle::PE_IndicatorArrowLeft;
            break;
        case Qt::RightArrow:
            pe = QStyle::PE_IndicatorArrowRight;
            break;
        case Qt::UpArrow:
            pe = QStyle::PE_IndicatorArrowUp;
            break;
        case Qt::DownArrow:
            pe = QStyle::PE_IndicatorArrowDown;
            break;
        default:
            return;
        }

        QStyleOptionToolButton arrowOpt;
        arrowOpt.rect = rect;
        arrowOpt.palette = toolbutton->palette;
        arrowOpt.state = toolbutton->state;
        style->drawPrimitive(pe, &arrowOpt, painter, widget);
    }

    static QPixmap qtn_Pixmap(const QIcon& icon, const QRect& rect, const QSize& iconSize, QStyle::State state, const QWidget* widget)
    {
        if (!icon.isNull())
        {
            QSize pmSize = iconSize;
            QIcon::State stateIcon = state & QStyle::State_On ? QIcon::On : QIcon::Off;
            QIcon::Mode mode;
            if (!(state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if ((state & QStyle::State_MouseOver) && (state & QStyle::State_AutoRaise))
                mode = QIcon::Active;
            else
                mode = QIcon::Normal;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QPixmap pm = widget ? icon.pixmap(QtnPrivate::qtn_getWindow(widget), rect.size().boundedTo(iconSize), mode, stateIcon) :
                icon.pixmap(rect.size().boundedTo(iconSize), mode, stateIcon);
#else
            Q_UNUSED(widget);
            QPixmap pm = icon.pixmap(rect.size().boundedTo(iconSize), mode, stateIcon);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            pmSize = pm.size() / pm.devicePixelRatio();
#else
            pmSize = pm.size();
#endif
            if (pmSize.width() * pmSize.height() < iconSize.width() * iconSize.height())
                pm = pm.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            return pm;
        }
        return QPixmap();
    }

#ifdef Q_OS_MAC
    static QLinearGradient qtn_titleBarGradientActive()
    {
        static QLinearGradient gradient;
        if (gradient == QLinearGradient()) {
            gradient.setColorAt(0, QColor(235, 235, 235));
            gradient.setColorAt(0.5, QColor(210, 210, 210));
            gradient.setColorAt(0.75, QColor(195, 195, 195));
            gradient.setColorAt(1, QColor(180, 180, 180));
        }
        return gradient;
    }

    static QLinearGradient qtn_titleBarGradientInactive()
    {
        static QLinearGradient gradient;
        if (gradient == QLinearGradient()) {
            gradient.setColorAt(0, QColor(250, 250, 250));
            gradient.setColorAt(1, QColor(225, 225, 225));
        }
        return gradient;
    }
#endif
}
QTITAN_USE_NAMESPACE


/* QtitanAnimationV5 */
QtitanAnimationV5::~QtitanAnimationV5()
{
    setWidget(Q_NULL);
}

void QtitanAnimationV5::setWidget(QWidget* widget)
{
    if (widget)
        widget->installEventFilter(this);
    else if (m_widget)
        m_widget->removeEventFilter(this);

    m_widget = widget;
}

void QtitanAnimationV5::paint(QPainter *painter, const QStyleOption *option)
{
    Q_UNUSED(option);
    Q_UNUSED(painter);
}

void QtitanAnimationV5::drawBlendedImage(QPainter* painter, QRect rect, float alpha)
{
    if (m_secondaryImage.isNull() || m_primaryImage.isNull())
        return;

    if (m_tempImage.isNull())
        m_tempImage = m_secondaryImage;

    const int a = qRound(alpha * 256);
    const int ia = 256 - a;
    const int sw = m_primaryImage.width();
    const int sh = m_primaryImage.height();
    const int bpl = m_primaryImage.bytesPerLine();
    switch (m_primaryImage.depth())
    {
    case 32:
    {
        uchar *mixed_data = m_tempImage.bits();
        const uchar *back_data = m_primaryImage.bits();
        const uchar *front_data = m_secondaryImage.bits();
        for (int sy = 0; sy < sh; sy++)
        {
            quint32* mixed = (quint32*)mixed_data;
            const quint32* back = (const quint32*)back_data;
            const quint32* front = (const quint32*)front_data;
            for (int sx = 0; sx < sw; sx++)
            {
                quint32 bp = back[sx];
                quint32 fp = front[sx];
                mixed[sx] = qRgba((qRed(bp)*ia + qRed(fp)*a) >> 8,
                    (qGreen(bp)*ia + qGreen(fp)*a) >> 8,
                    (qBlue(bp)*ia + qBlue(fp)*a) >> 8,
                    (qAlpha(bp)*ia + qAlpha(fp)*a) >> 8);
            }
            mixed_data += bpl;
            back_data += bpl;
            front_data += bpl;
        }
    }
    default:
        break;
    }
    painter->drawPixmap(rect, QPixmap::fromImage(m_tempImage, Qt::AutoColor));
}

bool QtitanAnimationV5::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_widget && event->type() == QEvent::Hide)
        setWidget(Q_NULL);

    return QObject::eventFilter(obj, event);
}

QtitanTransitionV5::QtitanTransitionV5()
    : QtitanAnimationV5()
{
}

QtitanTransitionV5::~QtitanTransitionV5()
{
}

void QtitanTransitionV5::paint(QPainter* painter, const QStyleOption* option)
{
    float alpha = 1.0;
    if (m_duration > 0)
    {
        QTime current = QTime::currentTime();

        if (m_startTime > current)
            m_startTime = current;

        int timeDiff = m_startTime.msecsTo(current);
        alpha = timeDiff / (float)m_duration;
        if (timeDiff > m_duration)
        {
            m_running = false;
            alpha = 1.0;
        }
    }
    else
    {
        m_running = false;
    }
    drawBlendedImage(painter, option->rect, alpha);
}

static bool toBool(const QLatin1String& str)
{
    return str == "true" ? true : false;
}

static QPalette::ColorGroup qtn_stringToGroup(const QLatin1String& str)
{
    if (str == QLatin1String("Normal"))             return QPalette::Normal;
    else if (str == QLatin1String("Active"))        return QPalette::Active;
    else if (str == QLatin1String("Disabled"))      return QPalette::Disabled;
    else if (str == QLatin1String("Inactive"))      return QPalette::Inactive;
    else if (str == QLatin1String("NColorGroups"))  return QPalette::NColorGroups;
    else if (str == QLatin1String("Current"))       return QPalette::Current;
    else if (str == QLatin1String("All"))           return QPalette::All;
    return QPalette::Normal;
}

static QPalette::ColorRole qtn_stringToRole(const QLatin1String& str)
{
    if (str == QLatin1String("WindowText"))           return QPalette::WindowText;
    else if (str == QLatin1String("Button"))          return QPalette::Button;
    else if (str == QLatin1String("Light"))           return QPalette::Light;
    else if (str == QLatin1String("Midlight"))        return QPalette::Midlight;
    else if (str == QLatin1String("Dark"))            return QPalette::Dark;
    else if (str == QLatin1String("Mid"))             return QPalette::Mid;
    else if (str == QLatin1String("Text"))            return QPalette::Text;
    else if (str == QLatin1String("BrightText"))      return QPalette::BrightText;
    else if (str == QLatin1String("ButtonText"))      return QPalette::ButtonText;
    else if (str == QLatin1String("Base"))            return QPalette::Base;
    else if (str == QLatin1String("Window"))          return QPalette::Window;
    else if (str == QLatin1String("Shadow"))          return QPalette::Shadow;
    else if (str == QLatin1String("Highlight"))       return QPalette::Highlight;
    else if (str == QLatin1String("HighlightedText")) return QPalette::HighlightedText;
    else if (str == QLatin1String("Link"))            return QPalette::Link;
    else if (str == QLatin1String("LinkVisited"))     return QPalette::LinkVisited;
    else if (str == QLatin1String("AlternateBase"))   return QPalette::AlternateBase;
    else if (str == QLatin1String("ToolTipBase"))     return QPalette::ToolTipBase;
    else if (str == QLatin1String("ToolTipText"))     return QPalette::ToolTipText;
    else if (str == QLatin1String("NColorRoles"))     return QPalette::NColorRoles;
    else if (str == QLatin1String("Foreground"))      return QPalette::Foreground;
    else if (str == QLatin1String("Background"))      return QPalette::Background;
    return QPalette::NoRole;
}

static CommonStyleV5Private::ImageState qtn_stringToState(const QLatin1String& str)
{
    if (str == QLatin1String("Black"))         return CommonStyleV5Private::Black;
    else if (str == QLatin1String("Gray"))     return CommonStyleV5Private::Gray;
    else if (str == QLatin1String("LtGray"))   return CommonStyleV5Private::LtGray;
    else if (str == QLatin1String("White"))    return CommonStyleV5Private::White;
    else if (str == QLatin1String("DkGray"))   return CommonStyleV5Private::DkGray;
    else if (str == QLatin1String("Black2"))   return CommonStyleV5Private::Black2;
    return CommonStyleV5Private::InvalidState;
}

PixelMetricData::PixelMetricData() : value(CommonStyleV5Private::InvalidMetric), scalableDpi(false) {}

/* StyleConfigReader */
StyleConfigReader::StyleConfigReader()
{
}

bool StyleConfigReader::read(QIODevice* device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) 
    {
        if (xml.name() == "STYLE" && xml.attributes().value("version") == "1.0")
            readStyle();
        else
            xml.raiseError(QObject::tr("The file is not an STYLE version 1.0 file."));
    }

    return !xml.error();
}

QString StyleConfigReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3").arg(xml.errorString()).
        arg(xml.lineNumber()).arg(xml.columnNumber());
}

void StyleConfigReader::readStyle()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("STYLE"));

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QLatin1String("widget"))
            readWidget();
        else if (xml.name() == QLatin1String("global"))
            readGlobal();
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readWidget()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("widget"));

    QString className = xml.attributes().value("className").toString();

    Q_ASSERT(className.size() > 0);
    WidgetData* widgetItem = new WidgetData;
    Q_ASSERT(!m_hashWidgetsData.contains(className));

    QString strHover = xml.attributes().value("hover").toString();
    if (!strHover.isEmpty())
        widgetItem->hover = toBool(QLatin1String(strHover.toLatin1()));

    QString strAutoFillBackground = xml.attributes().value("autoFillBackground").toString();
    if (!strAutoFillBackground.isEmpty())
        widgetItem->autoFillBackground = toBool(QLatin1String(strAutoFillBackground.toLatin1()));

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == "palette")
            readPalette(widgetItem->m_hashPalette);
        else if (xml.name() == "pixelMetric")
            readPixelMetric(m_hashWidgetMetricsData[className]);
        else if (xml.name() == "styleHint")
            readStyleHint(m_hashWidgetStyleHint[className]);
        else
            xml.skipCurrentElement();
    }
    m_hashWidgetsData.insert(className, WidgetData(*widgetItem));
    delete widgetItem;
}

void StyleConfigReader::readGlobal()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("global"));
    while (xml.readNextStartElement())
    {
        if (xml.name() == QLatin1String("standardPixmap"))
            readStandardPixmap();
        else if (xml.name() == QLatin1String("styleHint"))
            readStyleHint(m_hashGlobalStyleHint);
        else if (xml.name() == QLatin1String("pixelMetric"))
            readPixelMetric(m_hashGlobalMetricsData);
        else if (xml.name() == QLatin1String("palette"))
            readPalette(m_hashGlobalPalette);
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readStandardPixmap()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("standardPixmap"));
    while (xml.readNextStartElement())
    {
        if (xml.name() == QLatin1String("colorState"))
        {
            QString strState = xml.attributes().value("state").toString();
            CommonStyleV5Private::ImageState state = qtn_stringToState(QLatin1String(strState.toLatin1()));
            QString color = xml.readElementText();
            m_hashGlobalColorPixmap.insert(static_cast<uint>(state), QColor(color));
        }
        else if (xml.name() == QLatin1String("pixmap"))
        {
            QString strType = xml.attributes().value("number").toString();
            QString namePng = xml.readElementText();
            m_hashGlobalStandardPixmap[strType].append(namePng);
        }
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readPixelMetric(PixelMetricHash& pixelMetric)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "pixelMetric");
    while (xml.readNextStartElement())
    {
        if (xml.name() == QLatin1String("metric"))
        {
            PixelMetricData* item = new PixelMetricData;
            QString strType = xml.attributes().value("number").toString();

            item->scalableDpi = toBool(QLatin1String(xml.attributes().value("scalableDpi").toString().toLatin1()));
            item->value = xml.readElementText().toInt();
            pixelMetric.insert(strType, PixelMetricData(*item));
        }
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readStyleHint(StyleHintHash& styleHint)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("styleHint"));
    while (xml.readNextStartElement())
    {
        if (xml.name() == QLatin1String("hint"))
        {
            QString strType = xml.attributes().value("number").toString();
            styleHint[strType] = xml.readElementText().toInt();
        }
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readPalette(PaletteHash& hashPalette)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "palette");
    while (xml.readNextStartElement())
    {
        if (xml.name() == "colors")
            readColors(hashPalette);
        else
            xml.skipCurrentElement();
    }
}

void StyleConfigReader::readColors(PaletteHash& hashPalette)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "colors");

    QHash<QPalette::ColorRole, QBrush> hashBrush;
    QString colorGroup = xml.attributes().value("colorGroup").toString();
    QPalette::ColorGroup group = qtn_stringToGroup(QLatin1String(colorGroup.toLatin1()));
    while (xml.readNextStartElement())
    {
        if (xml.name() == "color")
        {
            QString colorRole = xml.attributes().value("colorRole").toString();
            QPalette::ColorRole role = qtn_stringToRole(QLatin1String(colorRole.toLatin1()));
            QString strColor = xml.readElementText();
            hashBrush[role] = QBrush(QColor(strColor));
        }
        else
            xml.skipCurrentElement();
    }
    hashPalette[group] = hashBrush;
}



/* StyleConfig */
StyleConfig::StyleConfig()
{
}

bool StyleConfig::load(const QString& fileName)
{
    m_hashGlobalColorPixmap.clear();
    m_hashGlobalStandardPixmap.clear();
    m_hashGlobalMetricsData.clear();
    m_hashWidgetMetricsData.clear();
    m_hashWidgetsData.clear();

    if (fileName.isEmpty())
        return false;

    QFileInfo fileInfo(fileName);
    if (!fileInfo.completeSuffix().isEmpty() && !fileInfo.exists())
        return false;

    QFile file(fileInfo.absoluteFilePath());
    file.open(QIODevice::ReadOnly);

    StyleConfigReader reader;
    if (!reader.read(&file))
        return false;

    m_hashGlobalPalette = reader.hashGlobalPalette();
    m_hashGlobalColorPixmap = reader.hashColorPixmap();
    m_hashGlobalStyleHint = reader.hashGlobalStyleHint();
    m_hashWidgetStyleHint = reader.hashWidgetStyleHint();
    m_hashGlobalStandardPixmap = reader.hashStandardPixmap();
    m_hashGlobalMetricsData = reader.hashGlobalMetricsData();
    m_hashWidgetMetricsData = reader.hashWidgetMetricsData();
    m_hashWidgetsData = reader.hashWidgetsData();
    return true;
}

void StyleConfig::setPaletteWidget(QWidget* wd, const WidgetData& data)
{
    wd->setAttribute(Qt::WA_Hover, data.hover);
    wd->setAutoFillBackground(data.autoFillBackground);

    QPalette pal = wd->palette();
    PaletteHash hash = data.m_hashPalette;
    for (uint group = QPalette::Active; group <= QPalette::All; ++group)
    {
        if (!hash.contains((QPalette::ColorGroup)group))
            continue;

        QHash<QPalette::ColorRole, QBrush> hashGroup = hash[(QPalette::ColorGroup)group];
        for (uint role = QPalette::WindowText; role <= QPalette::NColorRoles; ++role)
        {
            if (!hashGroup.contains((QPalette::ColorRole)role))
                continue;

            QBrush brush = hashGroup[(QPalette::ColorRole)role];
            pal.setBrush((QPalette::ColorGroup)group, (QPalette::ColorRole)role, brush);
        }
    }
    wd->setPalette(pal);
}

void StyleConfig::setConfigWidget(QWidget* wd)
{
    QStringList lstString;
    const QMetaObject* mo = wd->metaObject();
    while (mo) 
    {
        lstString.insert(0, mo->className());
        mo = mo->superClass();
    }

    foreach(QString className, lstString)
    {
        if (!m_hashWidgetsData.contains(QLatin1String(className.toLatin1())))
            continue;
        const WidgetData& data = m_hashWidgetsData.value(QLatin1String(className.toLatin1()));
        setPaletteWidget(wd, data);
    }
}

RoleBrushHash StyleConfig::globalGlobalPalette(QPalette::ColorGroup group) const
{
    if (!m_hashGlobalPalette.contains(group))
        return RoleBrushHash();
    return m_hashGlobalPalette.value(group);
}

int StyleConfig::pixelMetric(const QString& strWidget, const QString& numMetric, int defaultVal, bool& scalableDpi, bool* ok) const
{
    if (ok != 0)
        *ok = false;

    int val = defaultVal;
    if (numMetric.isEmpty())
        return val;

    PixelMetricData data;
    if (m_hashWidgetMetricsData.contains(strWidget) && m_hashWidgetMetricsData.value(strWidget).contains(numMetric))
        data = m_hashWidgetMetricsData.value(strWidget).value(numMetric);

    if (data.value == static_cast<uint>(CommonStyleV5Private::InvalidMetric) && m_hashGlobalMetricsData.contains(numMetric))
        data = m_hashGlobalMetricsData.value(numMetric);

    if (data.value != static_cast<uint>(CommonStyleV5Private::InvalidMetric))
    {
        val = data.value;
        scalableDpi = data.scalableDpi;
        if (ok != 0)
            *ok = true;
    }
    return val;
}

QString StyleConfig::standardPixmapName(const QString& numPixmap) const
{
    if (m_hashGlobalStandardPixmap.contains(numPixmap))
    {
        QSize sz; double pdblScale = 0.0;
        QString suffix = QtnPrivate::qtn_bestImageSize(&sz, &pdblScale);
        if (suffix.size() == 0)
            return QString("");

        QStringList listName = m_hashGlobalStandardPixmap.value(numPixmap);
        foreach(QString name, listName)
        {
            if (name.contains(suffix))
                return name;
        }
    }
    return QString("");
}

int StyleConfig::styleHint(const QString& strWidget, const QString& numMetric, int defaultVal) const
{
    int val = defaultVal;
    if (numMetric.isEmpty())
        return val;

    int data = CommonStyleV5Private::InvalidHint;
    if (m_hashWidgetStyleHint.contains(strWidget) && m_hashWidgetStyleHint.value(strWidget).contains(numMetric))
        data = m_hashWidgetStyleHint.value(strWidget).value(numMetric);

    if (data == CommonStyleV5Private::InvalidHint && m_hashGlobalStyleHint.contains(numMetric))
        data = m_hashGlobalStyleHint.value(numMetric);

    if (data != CommonStyleV5Private::InvalidMetric)
        val = data;
    return val;
}

QColor StyleConfig::standardColorPixmap(uint state) const
{
    if (!m_hashGlobalColorPixmap.contains(state))
        return QColor();
    return m_hashGlobalColorPixmap.value(state);
}

/*! \internal */
static const SymbolInfo knownIconSymbols[NumSymbols] =
{
    { Symbol_None,           QChar(0x0000),  QChar(0x0000) },
    { Symbol_ChromeClose,    QChar(0xEAA2),  QChar(0xE8BB) },
    { Symbol_ChromeMinimize, QChar(0xEAA5),  QChar(0xE921) },
    { Symbol_ChromeMaximize, QChar(0xEAA4),  QChar(0xE922) },
    { Symbol_ChromeRestore,  QChar(0xEAA6),  QChar(0xE923) },
    { Symbol_Pin,            QChar(0xEC6A),  QChar(0xE718) },
    { Symbol_PinFill,        QChar(0xEC6B),  QChar(0xE841) },
    { Symbol_Pinned,         QChar(0xEC6C),  QChar(0xE840) },
    { Symbol_PinnedFill,     QChar(0xEC6D),  QChar(0xE842) },
    { Symbol_StockDown,      QChar(0xED2E),  QChar(0xEB0F) },
    { Symbol_FlickUp,        QChar(0xEB33),  QChar(0xE936) },
    { Symbol_ChevronDown,    QChar(0xEA8E),  QChar(0xE70D) },
};

/*!
\class Qtitan::StandardSymbolIcon
\internal
*/
StandardSymbolIcon::StandardSymbolIcon()
{
}

/*! \internal */
IconSymbol StandardSymbolIcon::symbolStandardPixmap(QStyle::StandardPixmap sp)
{
    switch (sp) 
    {
        case QStyle::SP_TitleBarMinButton:    return Symbol_ChromeMinimize;
        case QStyle::SP_TitleBarMaxButton:    return Symbol_ChromeMaximize;
        case QStyle::SP_TitleBarCloseButton:  return Symbol_ChromeClose;
        case QStyle::SP_TitleBarNormalButton: return Symbol_ChromeRestore;
        case QStyle::SP_ArrowDown:            return Symbol_ChevronDown;
        default: break;
    }
    const CommonStyleV5::QtnStandardPixmap qtnSp = static_cast<CommonStyleV5::QtnStandardPixmap>(sp);
    switch (qtnSp)
    {
        case CommonStyleV5::SP_TitleBarPinButton:        return Symbol_Pin;
        case CommonStyleV5::SP_TitleBarPinFillButton:    return Symbol_PinFill;
        case CommonStyleV5::SP_TitleBarPinnedButton:     return Symbol_Pinned;
        case CommonStyleV5::SP_TitleBarPinnedFillButton: return Symbol_PinnedFill;
        default: break;
    }
    return Symbol_None;
}

/*! \internal */
IconSymbol StandardSymbolIcon::symbolSubControl(QStyle::SubControl sc)
{
    switch (sc)
    {
        case QStyle::SC_TitleBarMinButton:    return Symbol_ChromeMinimize;
        case QStyle::SC_TitleBarMaxButton:    return Symbol_ChromeMaximize;
        case QStyle::SC_TitleBarCloseButton:  return Symbol_ChromeClose;
        case QStyle::SC_TitleBarNormalButton: return Symbol_ChromeRestore;
        default: break;
    }
    const CommonStyleV5::QtnSubControl qtnSc = static_cast<CommonStyleV5::QtnSubControl>(sc);
    switch (qtnSc)
    {
        case CommonStyleV5::SC_TitleBarPinlButton:       return Symbol_Pin;
        case CommonStyleV5::SC_TitleBarPinlFillButton:   return Symbol_PinFill;
        case CommonStyleV5::SC_TitleBarPinnedButton:     return Symbol_Pinned;
        case CommonStyleV5::SC_TitleBarPinnedFillButton: return Symbol_PinnedFill;
        default: break;
    }
    return Symbol_None;
}

/*! \internal */
QPixmap StandardSymbolIcon::standardSymbolPixmap(QStyle::StandardPixmap sp, const QStyleOption* option) const
{
    QColor clrText = Qt::black;
    if (option)
        clrText = option->state & QStyle::State_MouseOver ? option->palette.color(QPalette::HighlightedText) : option->palette.color(QPalette::Text);
    return CommonStyleV5::qtn_getSymbolToPixmap(symbolStandardPixmap(sp), option ? option->rect.size() : 
        CommonStyleV5Private::qtn_bestSizePixmap(), clrText);
}

QColor CommonStyleV5Private::m_clrCloseButton          = QColor(199, 30, 30);
QColor CommonStyleV5Private::m_clrCloseButtonHighlight = QColor(224, 67, 67);
QFont  CommonStyleV5Private::m_iconSymbolFont          = QFont();

/*!
\class Qtitan::CommonStyleV5Private
\internal
*/
CommonStyleV5Private::CommonStyleV5Private()
    : m_ribbonItemHeight(24)
{
    CommonStyleV5Private::m_iconSymbolFont = Qtitan::qtn_getSegoeMDL2AssetsFont();
}

CommonStyleV5Private::~CommonStyleV5Private()
{
}

void CommonStyleV5Private::initialization()
{
    QFont fontRibbon = QApplication::font("QMenuBar");
    QApplication::setFont(fontRibbon, "Qtitan::RibbonBar");
    calcRibbonItemHeight();

    static QColor colorDisable = QColor(90, 90, 90);
    m_hashNormalPixmap[iconNameGroupOption]             = CommonStyleV5Private::loadIcon(iconNameGroupOption);
    m_hashDisabledPixmap[iconNameGroupOption]           = QtnPrivate::qtn_ColorizedImage(m_hashNormalPixmap[iconNameGroupOption], Qt::black, colorDisable);
    m_hashNormalPixmap[iconNameRibbonButtonPopupMenu]   = CommonStyleV5Private::loadIcon(iconNameRibbonButtonPopupMenu);
    m_hashDisabledPixmap[iconNameRibbonButtonPopupMenu] = QtnPrivate::qtn_ColorizedImage(m_hashNormalPixmap[iconNameRibbonButtonPopupMenu], Qt::black, colorDisable);
    m_hashNormalPixmap[iconNameShadedArrowDown]         = CommonStyleV5Private::loadIcon(iconNameShadedArrowDown);
    m_hashDisabledPixmap[iconNameShadedArrowDown]       = QtnPrivate::qtn_ColorizedImage(m_hashNormalPixmap[iconNameShadedArrowDown], Qt::black, colorDisable);
    m_hashNormalPixmap[iconNameShadedArrowUp]           = CommonStyleV5Private::loadIcon(iconNameShadedArrowUp);
    m_hashDisabledPixmap[iconNameShadedArrowUp]         = QtnPrivate::qtn_ColorizedImage(m_hashNormalPixmap[iconNameShadedArrowUp], Qt::black, colorDisable);
}

QPixmap CommonStyleV5Private::standardPixmap(const QString& indxIcon, ImageState state) const
{
    CommonStyleV5Private* that = (CommonStyleV5Private*)this;
    that->updateStandardPixmap(indxIcon);
    switch (state)
    {
        case Black:
            if (m_stdIconsBlackHash.contains(indxIcon))
                return m_stdIconsBlackHash.value(indxIcon);
            break;
        case Gray:
            if (m_stdIconsGrayHash.contains(indxIcon))
                return m_stdIconsGrayHash.value(indxIcon);
            break;
        case DkGray:
            if (m_stdIconsDkGrayHash.contains(indxIcon))
                return m_stdIconsDkGrayHash.value(indxIcon);
            break;
        case LtGray:
            if (m_stdIconsLtGrayHash.contains(indxIcon))
                return m_stdIconsLtGrayHash.value(indxIcon);
            break;
        case White:
            if (m_stdIconsWhiteHash.contains(indxIcon))
                return m_stdIconsWhiteHash.value(indxIcon);
            break;
        case Black2:
            if (m_stdIconsBlack2Hash.contains(indxIcon))
                return m_stdIconsBlack2Hash.value(indxIcon);
            break;
        default: break;
    };
    return QPixmap();
}

void CommonStyleV5Private::updateStyle()
{
//    QTN_P(CommonStyleV5);
//    qApp->setPalette(p.standardPalette());
    m_colorIconBlack  = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::Black));
    m_colorIconGray   = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::Gray));
    m_colorIconLtGray = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::LtGray));
    m_colorIconWhite  = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::White));
    m_colorIconDkGray = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::DkGray));
    m_colorIconBlack2 = m_config.standardColorPixmap(static_cast<uint>(CommonStyleV5Private::Black2));
}

void CommonStyleV5Private::updateStandardPixmap(const QString& indxIcon)
{
    if (m_colorIconBlack.isValid() && !m_stdIconsBlackHash.contains(indxIcon))
        m_stdIconsBlackHash.insert(indxIcon, loadIcon(indxIcon, m_colorIconBlack));

    if (m_colorIconGray.isValid() && !m_stdIconsGrayHash.contains(indxIcon))
        m_stdIconsGrayHash.insert(indxIcon, loadIcon(indxIcon, m_colorIconGray));

    if (m_colorIconDkGray.isValid() && !m_stdIconsDkGrayHash.contains(indxIcon))
        m_stdIconsDkGrayHash.insert(indxIcon, loadIcon(indxIcon, m_colorIconDkGray));

    if (m_colorIconLtGray.isValid() && !m_stdIconsLtGrayHash.contains(indxIcon))
        m_stdIconsLtGrayHash.insert(indxIcon, loadIcon(indxIcon, m_colorIconLtGray));

    if (m_colorIconWhite.isValid() && !m_stdIconsWhiteHash.contains(indxIcon))
        m_stdIconsWhiteHash.insert(indxIcon, loadIcon(indxIcon, m_colorIconWhite));

    if (m_colorIconBlack2.isValid() && !m_stdIconsBlack2Hash.contains(indxIcon))
        m_stdIconsBlack2Hash.insert(indxIcon, loadIcon(indxIcon, m_colorIconBlack2));
}

void CommonStyleV5Private::setPaletteFromGlobal(QPalette::ColorGroup gr, QPalette::ColorRole cr, QPalette& palette) const
{
    RoleBrushHash hashPalette = m_config.globalGlobalPalette(gr);
    if (!hashPalette.contains(cr))
        return;
    palette.setBrush(gr, cr, hashPalette.value(cr));
}

/*! \internal */
QSize CommonStyleV5Private::qtn_bestSizePixmap()
{
    QSize retSize = QSize(9, 9);
    qreal dblScale = static_cast<qreal>(CommonStyleV5Private::defaultDpiX()) / 100.0;
    if (dblScale > 1.51)
        retSize = QSize(18, 18);
    else if (dblScale > 1.26)
        retSize = QSize(13, 13);
    else if (dblScale > 1.1)
        retSize = QSize(11, 11);
    return retSize;
}

QPixmap CommonStyleV5Private::loadIcon(const QString& name)
{
    QSize sz; double pdblScale = 0.0;
    QString strIcon(":/res/");
    QString suffix = QtnPrivate::qtn_bestImageSize(&sz, &pdblScale);
    QString fileName = QString(strIcon + name + "-" + suffix);
    return QPixmap(fileName);
}

QPixmap CommonStyleV5Private::loadIcon(const QString& indxIcon, const QColor &color) const
{
    QPixmap icon;
    QString fileName = m_config.standardPixmapName(indxIcon);
    if (fileName.isEmpty())
        return icon;
    QString strIcon(":/res/");
    icon = QtnPrivate::qtn_ColorizedImage(QPixmap(strIcon + fileName), Qt::black, color);
    return icon;
}

bool CommonStyleV5Private::buttonVisible(const QStyle::SubControl sc, const QStyleOptionTitleBar* tb) 
{
    bool isMinimized = tb->titleBarState & Qt::WindowMinimized;
    bool isMaximized = tb->titleBarState & Qt::WindowMaximized;
    const uint flags = tb->titleBarFlags;
    bool retVal = false;
    switch (sc) {
    case QStyle::SC_TitleBarContextHelpButton:
        if (flags & Qt::WindowContextHelpButtonHint)
            retVal = true;
        break;
    case QStyle::SC_TitleBarMinButton:
        if (!isMinimized && (flags & Qt::WindowMinimizeButtonHint))
            retVal = true;
        break;
    case QStyle::SC_TitleBarNormalButton:
        if (isMinimized && (flags & Qt::WindowMinimizeButtonHint))
            retVal = true;
        else if (isMaximized && (flags & Qt::WindowMaximizeButtonHint))
            retVal = true;
        break;
    case QStyle::SC_TitleBarMaxButton:
        if (!isMaximized && (flags & Qt::WindowMaximizeButtonHint))
            retVal = true;
        break;
    case QStyle::SC_TitleBarShadeButton:
        if (!isMinimized &&  flags & Qt::WindowShadeButtonHint)
            retVal = true;
        break;
    case QStyle::SC_TitleBarUnshadeButton:
        if (isMinimized && flags & Qt::WindowShadeButtonHint)
            retVal = true;
        break;
    case QStyle::SC_TitleBarCloseButton:
        if (flags & Qt::WindowSystemMenuHint)
            retVal = true;
        break;
    case QStyle::SC_TitleBarSysMenu:
        if (flags & Qt::WindowSystemMenuHint)
            retVal = true;
        break;
    default:
        retVal = true;
    }
    return retVal;
}

#ifdef Q_OS_MAC
double CommonStyleV5Private::logPixel = 72.0;
#else /* Q_OS_MAC */
double CommonStyleV5Private::logPixel = 96.0;
#endif 

int CommonStyleV5Private::defaultDpiX()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (const QScreen* screen = QGuiApplication::primaryScreen())
        return qRound(screen->logicalDotsPerInchX());
#else // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
    HDC displayDC = GetDC(0);
    int currentlogPixel = GetDeviceCaps(displayDC, LOGPIXELSX);
    ReleaseDC(0, displayDC);
    return currentlogPixel;
#endif /* Q_OS_WIN*/
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    //PI has not been initialised, or it is being initialised. Give a default dpi
    return static_cast<int>(logPixel);
}

int CommonStyleV5Private::defaultDpiY()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (const QScreen* screen = QGuiApplication::primaryScreen())
        return qRound(screen->logicalDotsPerInchY());
#else // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
    HDC displayDC = GetDC(0);
    int currentlogPixel = GetDeviceCaps(displayDC, LOGPIXELSY);
    ReleaseDC(0, displayDC);
    return currentlogPixel;
#endif /* Q_OS_WIN*/
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    //PI has not been initialised, or it is being initialised. Give a default dpi
    return static_cast<int>(logPixel);
}

qreal CommonStyleV5Private::dpiScaled(qreal value)
{
    static const qreal scale = static_cast<qreal>(defaultDpiX()) / logPixel;
    return value * scale;
}

int CommonStyleV5Private::getDPI()
{
    return defaultDpiX();
}

int CommonStyleV5Private::getDPIToPercent()
{
    return getDPI() * 100 / logPixel;
}

// for standard controls
/*! \internal */
bool CommonStyleV5Private::drawFrameWindow(const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (const QStyleOptionFrame* frm = qstyleoption_cast<const QStyleOptionFrame *>(option))
    {
        if (widget && widget->inherits("Qtitan::DockWindow"))
        {
            qDrawWinPanel(painter, option->rect, option->palette, false, 0);
            return true;
        }
        else if (QtnPrivate::qtn_isExistWindowTitleBar(widget))
        {
            const bool isActive = frm->state & QStyle::State_Active;
            const int fwidth = frm->lineWidth + frm->midLineWidth;
            QColor frameColor = titleBarFrameColor(option->palette, isActive);
            QRect rect = QRect(option->rect.x(), option->rect.y() + fwidth, option->rect.x() + fwidth, option->rect.height() - fwidth);
            painter->fillRect(rect, frameColor);

            rect = QRect(option->rect.width() - fwidth, option->rect.y() + fwidth, fwidth, option->rect.height() - fwidth);
            painter->fillRect(rect, frameColor);

            rect = QRect(option->rect.x(), option->rect.height() - fwidth, option->rect.width(), fwidth);
            painter->fillRect(rect, frameColor);

            rect = QRect(option->rect.x(), option->rect.y(), option->rect.width(), option->rect.y() + fwidth + 3);
            painter->fillRect(rect, frameColor);
            return true;
        }
    }
    return false;
}

bool CommonStyleV5Private::drawPanelMenu(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorArrow(QStyle::PrimitiveElement /*pe*/, const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawFrameGroupBox(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorCheckBox(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorRadioButton(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorToolBarHandle(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorToolBarSeparator(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorHeaderArrow(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawIndicatorBranch(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawFrameFocusRect(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawFrameDockWidget(const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (widget && (widget->inherits("Qtitan::DockTabbedLayoutPanel") || widget->inherits("Qtitan::DockBarArea")))
    {
        if (const QStyleOptionFrame* frame = qstyleoption_cast<const QStyleOptionFrame *>(option))
        {
            painter->fillRect(frame->rect, option->palette.background());
            return true;
        }
    }
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawPanelButtonTool(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    if (!QtnPrivate::qtn_isExistWindowTitleBar(widget))
        return false;

    QBrush fill;
    const bool isHighlight = opt->state & QStyle::State_MouseOver;
    fill = opt->palette.color(QPalette::Active, QPalette::Button);
    if (isHighlight)
        fill = opt->palette.color(QPalette::Active, QPalette::Highlight);
    painter->fillRect(opt->rect, fill);
    return true;
}

bool CommonStyleV5Private::drawPanelButtonCommand(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawFrameLineEdit(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawFrameStatusBarItem(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* widget) const
{
    if (widget && QtnPrivate::qtn_isAncestor(widget, "Qtitan::RibbonStatusBar"))
        return true;
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawTitleBar(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    if (!QtnPrivate::qtn_isExistWindowTitleBar(widget))
        return false;

    QTN_P(const CommonStyleV5);
    if (const QStyleOptionTitleBar* tb = qstyleoption_cast<const QStyleOptionTitleBar *>(option))
    {
        QRect ir;
        const bool isActive = tb->state & QStyle::State_Active;
        if (option->subControls & QStyle::SC_TitleBarLabel)
        {
            const int fwidth = p.proxy()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth, 0, widget);
            QRect fullRect = option->rect.adjusted(fwidth, fwidth, -fwidth, 0);
            QColor frameColor = titleBarFrameColor(option->palette, isActive);
            QColor windowColor = titleBarWindowColor(option->palette, isActive);
#ifdef Q_OS_MAC
            Q_UNUSED(windowColor);
            QLinearGradient gradient;
            if (isActive)
                gradient = QtnPrivate::qtn_titleBarGradientActive();
            else
                gradient = QtnPrivate::qtn_titleBarGradientInactive();
            gradient.setStart(0, fullRect.top());
            gradient.setFinalStop(0, fullRect.bottom());
            painter->fillRect(fullRect, gradient);
#else
            painter->fillRect(fullRect, windowColor);
#endif
            QRect rect = QRect(option->rect.x(), option->rect.y() + fwidth, option->rect.x() + fwidth, option->rect.height() - fwidth);
            painter->fillRect(rect, frameColor);

            rect = QRect(option->rect.width() - fwidth, option->rect.y() + fwidth, fwidth, option->rect.height() - fwidth);
            painter->fillRect(rect, frameColor);

            rect = QRect(option->rect.x(), option->rect.y(), option->rect.width(), option->rect.y() + fwidth);
            painter->fillRect(rect, frameColor);

            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarLabel, widget);
            painter->setPen(isActive ? option->palette.text().color() : option->palette.color(QPalette::Disabled, QPalette::Text));
            painter->drawText(ir.x() + 2, ir.y(), ir.width() - 2, ir.height(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, tb->text);
        }
        bool down = false;
        bool highlight = false;
        QPixmap pm;
        QPalette savePalette = tb->palette;
        QStyleOptionTitleBar tool = *tb;
        if (tb->subControls & QStyle::SC_TitleBarCloseButton && tb->titleBarFlags & Qt::WindowSystemMenuHint) 
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarCloseButton, widget);
            down = (tb->activeSubControls & QStyle::SC_TitleBarCloseButton) && (option->state & QStyle::State_Sunken);
            highlight = tb->activeSubControls & QStyle::SC_TitleBarCloseButton && (option->state & QStyle::State_MouseOver);
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, m_clrCloseButton);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, m_clrCloseButtonHighlight);
            tool.palette.setColor(QPalette::Active, QPalette::Text, Qt::white);
            tool.palette.setColor(QPalette::Active, QPalette::HighlightedText, Qt::white);
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            QPixmap px = p.proxy()->standardPixmap(QStyle::SP_TitleBarCloseButton, &tool, 0);
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, px);
        }
        tool.palette = savePalette;
        if (tb->subControls & QStyle::SC_TitleBarMaxButton && tb->titleBarFlags & Qt::WindowMaximizeButtonHint && !(tb->titleBarState & Qt::WindowMaximized)) 
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarMaxButton, widget);
            down = (tb->activeSubControls & QStyle::SC_TitleBarMaxButton) && (option->state & QStyle::State_Sunken);
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarMaxButton) && (option->state & QStyle::State_MouseOver);
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            QPixmap px = p.proxy()->standardPixmap(QStyle::SP_TitleBarMaxButton, &tool, 0);
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, px);
        }

        if (tb->subControls & QStyle::SC_TitleBarMinButton && tb->titleBarFlags & Qt::WindowMinimizeButtonHint && !(tb->titleBarState & Qt::WindowMinimized))
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarMinButton, widget);
            down = (tb->activeSubControls & QStyle::SC_TitleBarMinButton) && (option->state & QStyle::State_Sunken);
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarMinButton) && (option->state & QStyle::State_MouseOver);
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            QPixmap px = p.proxy()->standardPixmap(QStyle::SP_TitleBarMinButton, &tool, 0);
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, px);
        }

        bool drawNormalButton = (tb->subControls & QStyle::SC_TitleBarNormalButton) && (((tb->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
            (tb->titleBarState & Qt::WindowMinimized)) || ((tb->titleBarFlags & Qt::WindowMaximizeButtonHint) && (tb->titleBarState & Qt::WindowMaximized)));
        if (drawNormalButton) 
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarNormalButton, widget);
            down = tb->activeSubControls & QStyle::SC_TitleBarNormalButton && (option->state & QStyle::State_Sunken);
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarNormalButton) && (option->state & QStyle::State_MouseOver);
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            QPixmap px = p.proxy()->standardPixmap(QStyle::SP_TitleBarNormalButton, &tool, 0);
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, px);
        }
        if (tb->subControls & QStyle::SC_TitleBarShadeButton && tb->titleBarFlags & Qt::WindowShadeButtonHint && !(tb->titleBarState & Qt::WindowMinimized)) 
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarShadeButton, widget);
            down = (tb->activeSubControls & QStyle::SC_TitleBarShadeButton && (option->state & QStyle::State_Sunken));
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarShadeButton) && (option->state & QStyle::State_MouseOver);
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            // TODO:
            QPixmap px = p.proxy()->standardPixmap(QStyle::SP_TitleBarNormalButton, &tool, 0);
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, px);
        }

        if (tb->subControls & QStyle::SC_TitleBarUnshadeButton && tb->titleBarFlags & Qt::WindowShadeButtonHint && tb->titleBarState & Qt::WindowMinimized)
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarUnshadeButton, widget);

            down = tb->activeSubControls & QStyle::SC_TitleBarUnshadeButton && (option->state & QStyle::State_Sunken);
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarUnshadeButton) && (option->state & QStyle::State_MouseOver);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            pm = p.proxy()->standardIcon(QStyle::SP_TitleBarUnshadeButton, &tool, widget).pixmap(QtnPrivate::qtn_getWindow(widget), QSize(10, 10));
#else
            pm = p.proxy()->standardIcon(QStyle::SP_TitleBarUnshadeButton, &tool, widget).pixmap(QSize(10, 10));
#endif
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            // TODO:
            painter->save();
            if (down)
                painter->translate(p.proxy()->pixelMetric(QStyle::PM_ButtonShiftHorizontal, tb, widget), p.proxy()->pixelMetric(QStyle::PM_ButtonShiftVertical, tb, widget));
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, pm);
            painter->restore();
        }

        if (tb->subControls & QStyle::SC_TitleBarContextHelpButton && tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarContextHelpButton, widget);

            down = tb->activeSubControls & QStyle::SC_TitleBarContextHelpButton && (option->state & QStyle::State_Sunken);
            highlight = (tb->activeSubControls & QStyle::SC_TitleBarUnshadeButton) && (option->state & QStyle::State_MouseOver);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            pm = p.proxy()->standardIcon(QStyle::SP_TitleBarContextHelpButton, &tool, widget).pixmap(QtnPrivate::qtn_getWindow(widget), QSize(10, 10));
#else
            pm = p.proxy()->standardIcon(QStyle::SP_TitleBarContextHelpButton, &tool, widget).pixmap(QSize(10, 10));
#endif
            tool.rect = ir;
            tool.state = down ? QStyle::State_Sunken : QStyle::State_Raised;
            if (isActive) tool.state |= QStyle::State_Active;
            if (highlight) tool.state |= QStyle::State_MouseOver;
            tool.palette.setColor(QPalette::Active, QPalette::Button, Qt::transparent);
            tool.palette.setColor(QPalette::Active, QPalette::Highlight, tb->palette.color(QPalette::Active, QPalette::Highlight).dark(120));
            p.proxy()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, painter, widget);
            // TODO:
            painter->save();
            if (down)
                painter->translate(p.proxy()->pixelMetric(QStyle::PM_ButtonShiftHorizontal, tb, widget), p.proxy()->pixelMetric(QStyle::PM_ButtonShiftVertical, tb, widget));
            p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, pm);
            painter->restore();
        }

        if (tb->subControls & QStyle::SC_TitleBarSysMenu && tb->titleBarFlags & Qt::WindowSystemMenuHint)
        {
            ir = p.proxy()->subControlRect(QStyle::CC_TitleBar, tb, QStyle::SC_TitleBarSysMenu, widget);
            if (!tb->icon.isNull()) 
                tb->icon.paint(painter, ir);
            else 
            {
                int iconSize = p.proxy()->pixelMetric(QStyle::PM_SmallIconSize, tb, widget);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                pm = p.proxy()->standardIcon(QStyle::SP_TitleBarMenuButton, &tool, widget).pixmap(QtnPrivate::qtn_getWindow(widget), QSize(iconSize, iconSize));
#else
                pm = p.proxy()->standardIcon(QStyle::SP_TitleBarMenuButton, &tool, widget).pixmap(QSize(iconSize, iconSize));
#endif
                tool.rect = ir;
                painter->save();
                p.proxy()->drawItemPixmap(painter, ir, Qt::AlignCenter, pm);
                painter->restore();
            }
        }
        return true;
    }
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawMdiControls(const QStyleOptionComplex* /*opt*/, QPainter* /*painter*/, const QWidget* /*widget*/) const
{
    return false;
}

bool CommonStyleV5Private::drawComboBox(const QStyleOptionComplex* /*option*/, QPainter* /*painter*/, const QWidget* /*widget*/) const
{
    return false;
}

bool CommonStyleV5Private::drawSlider(const QStyleOptionComplex* /*option*/, QPainter* /*painter*/, const QWidget* /*widget*/) const
{
    return false;
}

bool CommonStyleV5Private::drawSpinBox(const QStyleOptionComplex* /*option*/, QPainter* /*painter*/, const QWidget* /*widget*/) const
{
    return false;
}

bool CommonStyleV5Private::drawToolButton(const QStyleOptionComplex* /*option*/, QPainter* /*painter*/, const QWidget* /*widget*/) const
{
    return false;
}

bool CommonStyleV5Private::drawMenuBarItem(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawMenuItem(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawToolBoxTabShape(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawToolButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    if (!widget || !widget->inherits("Qtitan::RibbonSystemButton"))
        return false;
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionToolButton* toolbutton = qstyleoption_cast<const QStyleOptionToolButton *>(opt)) 
    {
        QRect rect = toolbutton->rect;
        int shiftX = 0;
        int shiftY = 0;
        if (toolbutton->state & (QStyle::State_Sunken | QStyle::State_On))
        {
            shiftX = p.proxy()->pixelMetric(QStyle::PM_ButtonShiftHorizontal, toolbutton, widget);
            shiftY = p.proxy()->pixelMetric(QStyle::PM_ButtonShiftVertical, toolbutton, widget);
        }
        // Arrow type always overrules and is always shown
        bool hasArrow = toolbutton->features & QStyleOptionToolButton::Arrow;
        if (((!hasArrow && toolbutton->icon.isNull()) && !toolbutton->text.isEmpty()) || toolbutton->toolButtonStyle == Qt::ToolButtonTextOnly) 
        {
            int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
            if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, widget))
                alignment |= Qt::TextHideMnemonic;
            rect.translate(shiftX, shiftY);
            painter->setFont(toolbutton->font);
            p.proxy()->drawItemText(painter, rect, alignment, toolbutton->palette, opt->state & QStyle::State_Enabled, toolbutton->text, QPalette::ButtonText);
        }
        else 
        {
            QSize pmSize = toolbutton->iconSize;
            QPixmap pm = QtnPrivate::qtn_Pixmap(toolbutton->icon, toolbutton->rect, toolbutton->iconSize, toolbutton->state, widget);
            if (!pm.isNull())
                pmSize = pm.size();

            if (toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly) 
            {
                painter->setFont(toolbutton->font);
                QRect pr = rect,
                    tr = rect;
                int alignment = Qt::TextShowMnemonic;
                if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, widget))
                    alignment |= Qt::TextHideMnemonic;

                if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon) 
                {
                    pr.setHeight(pmSize.height() + 6);
                    tr.adjust(0, pr.height() - 1, 0, -1);
                    pr.translate(shiftX, shiftY);
                    if (!hasArrow) 
                        p.proxy()->drawItemPixmap(painter, pr, Qt::AlignCenter, pm);
                    else 
                        QtnPrivate::drawArrow(p.proxy(), toolbutton, pr, painter, widget);
                    alignment |= Qt::AlignCenter;
                }
                else {
                    pr.setWidth(pmSize.width() + 8);
                    tr.adjust(pr.width(), 0, 0, 0);
                    pr.translate(shiftX, shiftY);
                    if (!hasArrow) 
                        p.proxy()->drawItemPixmap(painter, QStyle::visualRect(opt->direction, rect, pr), Qt::AlignCenter, pm);
                    else 
                        QtnPrivate::drawArrow(p.proxy(), toolbutton, pr, painter, widget);
                    alignment |= Qt::AlignLeft | Qt::AlignVCenter;
                }
                tr.translate(shiftX, shiftY);
                p.proxy()->drawItemText(painter, QStyle::visualRect(opt->direction, rect, tr), alignment, toolbutton->palette, 
                    toolbutton->state & QStyle::State_Enabled, toolbutton->text, QPalette::ButtonText);
            }
            else {
                rect.translate(shiftX, shiftY);
                if (hasArrow) 
                    QtnPrivate::drawArrow(p.proxy(), toolbutton, rect, painter, widget);
                else
                    p.proxy()->drawItemPixmap(painter, rect, Qt::AlignCenter, pm);
            }
        }
        return true;
    }
    return false;
}

bool CommonStyleV5Private::drawScrollBarAddSubLine(QStyle::ControlElement /*ce*/, const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawScrollBarAddSubPage(QStyle::ControlElement /*ce*/, const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawScrollBarSlider(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawTabBarTabShape(const QStyleOption* option, QPainter* painter, const QWidget* w) const
{
    if (w && w->inherits("Qtitan::DockAutoHideLayoutPanel"))
    {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab *>(option))
        {
            painter->fillRect(tab->rect, option->palette.background());
            return true;
        }
    }
    return false;
}

bool CommonStyleV5Private::drawHeaderSection(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawHeaderLabel(const QStyleOption* /*opt*/, QPainter* /*p*/, const QWidget* /*w*/) const
{
    return false;
}

bool CommonStyleV5Private::drawDockWidgetTitle(const QStyleOption* option, QPainter* painter, const QWidget* w) const
{
    if (w && w->inherits("Qtitan::DockTitleBar"))
    {
        if (const DockTitleBarStyleOptionV5* dwOpt = qstyleoption_cast<const DockTitleBarStyleOptionV5*>(option))
        {
            const QRect rc = dwOpt->rect;
            const bool isActive = (dwOpt->state & QStyle::State_Active);
            const QColor highlight = option->palette.highlight().color().lighter();

            painter->setBrush(isActive ? highlight : dwOpt->palette.background().color().darker(110));

            if (dwOpt->lineWidth > 0)
                painter->setPen(dwOpt->palette.background().color().darker(130));
            else
                painter->setPen(dwOpt->palette.background().color());
            painter->drawRect(rc.adjusted(2, 2, -3, -2));
            // TODO:
            // QRect titleRect = p.proxy()->subElementRect(QStyle::SE_DockWidgetTitleBarText, option, w);
            QRect titleRect = dwOpt->rcText;
            const QString text = dwOpt->fontMetrics.elidedText(dwOpt->title, Qt::ElideMiddle, titleRect.width() - 6);
            QApplication::style()->drawItemText(painter, titleRect.adjusted(6, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic,
                dwOpt->palette, true, text, QPalette::WindowText);
            return true;
        }
    }
    else if (w && w->inherits("Qtitan::DockTabbedLayoutPanel"))
    {
        if (const DockPaneStyleOptionV5* dwOpt = qstyleoption_cast<const DockPaneStyleOptionV5*>(option))
        {
            QRect rect = dwOpt->rect;
            const bool verticalTitleBar = dwOpt->verticalTitleBar;
            const bool active = (dwOpt->state & QStyle::State_Active);
            const QColor highlight = option->palette.highlight().color().lighter();

            if (verticalTitleBar)
            {
                painter->translate(rect.left() - 1, rect.top() + rect.width());
                painter->rotate(-90);
                painter->translate(-rect.left() + 1, -rect.top());
            }
            painter->setBrush(active ? highlight : dwOpt->palette.background().color().darker(110));
            painter->setPen(dwOpt->palette.background().color().darker(130));
            painter->drawRect(rect.adjusted(0, 1, -1, -3));
            return true;
        }
    }

    return false;
}

// PrimitiveElement - Ribbon
/*! \internal */
bool CommonStyleV5Private::drawRibbonBar(const QStyleOption* opt, QPainter* p, const QWidget* widget) const
{
    Q_UNUSED(widget);
    p->fillRect(opt->rect, opt->palette.base().color());
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonFrameGroups(const QStyleOption* option, QPainter* p, const QWidget* widget) const
{
    Q_UNUSED(widget);
    if (const StyleOptionRibbonV5* optGroups = qstyleoption_cast<const StyleOptionRibbonV5*>(option))
    {
        p->fillRect(optGroups->rect, optGroups->palette.button().color());

        QRect rectGroups = optGroups->rect;
        QRect rectActiveTab = optGroups->rectCurrentTab;
        if (optGroups->tabBarPosition == StyleOptionFrameV5::TabBarTopPosition)
        {
            QPainterPath path;
            path.moveTo(rectGroups.left(), rectGroups.top());
            path.lineTo(rectActiveTab.left(), rectGroups.top());
            path.moveTo(rectActiveTab.right(), rectGroups.top());
            path.lineTo(rectGroups.right(), rectGroups.top());

            QPen savePen = p->pen();
            QColor color = CommonStyleV5::qtn_getColorToRGB(optGroups->pageColor);
            p->setPen(color.isValid() ? color : option->palette.dark().color().light(135));
            p->drawPath(path);

            QPainterPath path1;
            path1.moveTo(rectGroups.right(), rectGroups.top());
            path1.lineTo(rectGroups.right(), rectGroups.bottom());
            path1.lineTo(rectGroups.left(), rectGroups.bottom());
            path1.lineTo(rectGroups.left(), rectGroups.top());

            p->setPen(option->palette.dark().color().light(135));
            p->drawPath(path1);
            p->setPen(savePen);
        }
        else if (optGroups->tabBarPosition == StyleOptionFrameV5::TabBarBottomPosition)
        {
            QPainterPath path;
            path.moveTo(rectGroups.left(), rectGroups.bottom() - 1);
            path.lineTo(rectActiveTab.left(), rectGroups.bottom() - 1);
            path.moveTo(rectActiveTab.right(), rectGroups.bottom() - 1);
            path.lineTo(rectGroups.right() - 1, rectGroups.bottom() - 1);
            path.lineTo(rectGroups.right() - 1, rectGroups.top());
            path.lineTo(rectGroups.left(), rectGroups.top());
            path.lineTo(rectGroups.left(), rectGroups.bottom());

            QPen savePen = p->pen();
            QColor color = CommonStyleV5::qtn_getColorToRGB(optGroups->pageColor);
            p->setPen(color.isValid() ? color : option->palette.dark().color().light(135));
            p->drawPath(path);
            p->setPen(savePen);
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonFrameGroup(const QStyleOption* option, QPainter* p, const QWidget* widget) const
{
    Q_UNUSED(widget);
    QRect rect = option->rect;
    QPen savePen = p->pen();
    p->setPen(option->palette.dark().color().light(140));
    p->drawLine(QPoint(rect.right() - 1, rect.top()), QPoint(rect.right() - 1, rect.bottom()));
    p->setPen(savePen);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonFrameGallery(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    if (qobject_cast<const QMenu*>(widget ? widget->parentWidget() : Q_NULL))
    {
        painter->fillRect(opt->rect, darkBackgroundLight(opt->palette).light());
        return true;
    }

    bool enabled = opt->state & QStyle::State_Enabled;
    bool selected = opt->state & QStyle::State_MouseOver;
    painter->fillRect(opt->rect, selected && enabled ? darkBackgroundLight(opt->palette).light() : darkBackgroundLight(opt->palette));
    QPen savePen = painter->pen();
    painter->setPen(outlineMidlight(opt->palette));
    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
    painter->setPen(savePen);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonSysFrameMenu(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    Q_UNUSED(widget);
    if (const StyleRibbonSysFrameMenuV5* frameMenu = qstyleoption_cast<const StyleRibbonSysFrameMenuV5*>(opt))
    {
        QRect rect = opt->rect;
        QPen savePen = painter->pen();
        painter->setPen(opt->palette.brush(QPalette::Mid).color());
        painter->fillRect(rect, darkBackground(opt->palette));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        rect -= frameMenu->margins;
#endif
        painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
        painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
        rect.setWidth(frameMenu->actionWidth);
        painter->fillRect(rect.adjusted(0, 1, 0, -1), darkBackgroundLight(frameMenu->palette));
        if (frameMenu->isAdditionalPages)
        {
            painter->setPen(outlineMidlight(opt->palette));
            painter->drawLine(rect.right(), rect.top() + 1, rect.right(), rect.bottom() - 1);
            painter->setPen(outlineLight(opt->palette));
            painter->drawLine(rect.right() + 1, rect.top() + 1, rect.right() + 1, rect.bottom() - 1);
        }
        painter->setPen(savePen);
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonSysFramePagePopupList(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    QFont saveFont = painter->font();
    QFont font = saveFont;
    font.setBold(true);
    painter->setFont(font);

    QFontMetrics fm(font);
    QRect rcCaption = p.proxy()->subElementRect((QStyle::SubElement)CommonStyleV5::SE_RibbonSysHeaderLabelPopupList, opt, widget);

    const int hMarginCaption = p.proxy()->pixelMetric((QStyle::PixelMetric)CommonStyleV5::PM_RibbonHMarginCaptionSysMenu, opt, widget);
    QRect rcText = rcCaption.adjusted(hMarginCaption, 0, 0, 0);

    QPalette pal = opt->palette;
    pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(150));
    uint alignment = Qt::AlignVCenter | Qt::TextSingleLine;
    p.proxy()->drawItemText(painter, rcText, alignment, pal, true, widget->windowTitle(), QPalette::WindowText);
    painter->setFont(saveFont);

    painter->setPen(outlineMidlight(opt->palette));
    painter->drawLine(rcCaption.left(), rcCaption.bottom(), rcCaption.right(), rcCaption.bottom());
    painter->setPen(outlineLight(opt->palette));
    painter->drawLine(rcCaption.left(), rcCaption.bottom() + 1, rcCaption.right(), rcCaption.bottom() + 1);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonSysFramePagePopup(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    QRect rect = opt->rect;
    QPen savePen = painter->pen();
    painter->setPen(outlineMidlight(opt->palette));
    painter->fillRect(rect, darkBackgroundLight(opt->palette));
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
    painter->setPen(savePen);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonPanelButton(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionToolButton* toolbutton = qstyleoption_cast<const QStyleOptionToolButton*>(opt))
    {
        bool smallSize = opt->rect.height() < 33;
        const bool enabled = opt->state & QStyle::State_Enabled;
        const bool checked = opt->state & QStyle::State_On;
        const bool highlighted = opt->state & QStyle::State_MouseOver;
        const bool mouseInSplit = opt->state & QStyle::State_MouseOver && toolbutton->activeSubControls & QStyle::SC_ToolButton;
        const bool mouseInSplitDropDown = opt->state & QStyle::State_MouseOver && toolbutton->activeSubControls & QStyle::SC_ToolButtonMenu;
        const bool pressed = opt->state & QStyle::State_Sunken;
        const bool popuped = (toolbutton->activeSubControls & QStyle::SC_ToolButtonMenu) && (opt->state & QStyle::State_Sunken);

        if (w && w->inherits("Qtitan::RibbonButton"))
        {
            if (!opt->rect.isEmpty())
            {
                int minIcon = qMin(opt->rect.height(), opt->rect.width());
                int actualArea = minIcon*minIcon;

                int index = -1;
                int curArea = 0;
                QList<QSize> lSz = toolbutton->icon.availableSizes();
                if (lSz.count() > 1)
                {
                    for (int i = 0, count = lSz.count(); count > i; i++)
                    {
                        QSize curSz = lSz[i];
                        int area = curSz.height()*curSz.width();
                        if (actualArea > area)
                        {
                            if (area > curArea)
                                index = i;
                            curArea = area;
                        }
                    }
                    int iconExtent = index != -1 ? toolbutton->icon.actualSize(lSz[index]).height() : smallSize;
                    smallSize = toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon ? iconExtent < 32 : iconExtent;
                }
            }
        }

        if (!(toolbutton->features & QStyleOptionToolButton::MenuButtonPopup))
        {
            bool autoRaise = toolbutton->state & QStyle::State_AutoRaise;
            if (!autoRaise && !qobject_cast<const QStatusBar*>(w->parentWidget()))
            {
                QStyleOptionButton tmpBtn;// = *btn;
                tmpBtn.state = toolbutton->state;
                tmpBtn.rect = toolbutton->rect;
                p.proxy()->drawPrimitive(QStyle::PE_PanelButtonCommand, &tmpBtn, painter, w);
            }
            else
            {
                QColor colorFill;
                QColor colorBorder;
                if (!enabled) colorFill = QColor();

                else if (popuped)
                {
                    colorFill = buttonHighlightColor(opt->palette, true); //highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (checked && !highlighted && !pressed)
                {
                    colorFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (pressed)
                {
                    colorFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (highlighted)
                {
                    colorFill = buttonHighlightColor(opt->palette, false); // highlight
                    colorBorder = buttonHighlightColor(opt->palette, false, true); // highlightDn
                }

                if (colorFill.isValid())
                {
                    painter->fillRect(opt->rect, colorFill);
                    painter->setPen(colorBorder);
                    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
                }
            }
            return true;
        }

        QRect rcButton = opt->rect;
        QRect popupr = p.proxy()->subControlRect(QStyle::CC_ToolButton, toolbutton, QStyle::SC_ToolButtonMenu, w);

        if (highlighted)
        {
            QPen savePen = painter->pen();
            painter->setPen(buttonHighlightColor(opt->palette, pressed, true)); // highlightDn
            painter->drawRect(rcButton.adjusted(0, 0, -1, -1));
            painter->setPen(savePen);
        }

        if (mouseInSplit || checked)
        {
            QColor colorSplitFill;
            QColor colorBorder;
            if (!enabled)
            {
                colorSplitFill = QColor();
            }
            else if (checked)
            {
                if (popuped)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (highlighted && !pressed && !checked)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, false); // highlight
                    colorBorder = buttonHighlightColor(opt->palette, false, true); // highlightDn
                }
                else if (highlighted && pressed)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (pressed || checked)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (highlighted || pressed)
                {
                    colorSplitFill = !mouseInSplit ? buttonHighlightColor(opt->palette, true) : QColor(); // highlightDn
                    colorBorder = !mouseInSplit ? buttonHighlightColor(opt->palette, true, true) : QColor(); // highlightDn
                }
            }
            else
            {
                if (popuped)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (pressed)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, true); // highlightDn
                    colorBorder = buttonHighlightColor(opt->palette, true, true); // highlightDn
                }
                else if (highlighted)
                {
                    colorSplitFill = buttonHighlightColor(opt->palette, false); // highlight
                    colorBorder = buttonHighlightColor(opt->palette, false, true); // highlightDn
                }
            }
            if (colorSplitFill.isValid())
            {
                QRect rcSplitUp = smallSize ? QRect(QPoint(rcButton.left(), rcButton.top()), QPoint(rcButton.right() - popupr.width() - 2, rcButton.bottom()))
                    : QRect(QPoint(rcButton.left(), rcButton.top()), QPoint(rcButton.right(), rcButton.bottom() - popupr.height() - 2));
                painter->fillRect(rcSplitUp, colorSplitFill);
                painter->setPen(colorBorder);
                painter->drawRect(rcSplitUp.adjusted(0, 0, -1, -1));
            }
        }

        if (mouseInSplitDropDown || popuped || checked)
        {
            if (!popuped)
            {
                QRect rcSplitDown = smallSize ? QRect(QPoint(rcButton.right() - popupr.width() - 1, rcButton.top()), QPoint(rcButton.right(), rcButton.bottom())) :
                    QRect(QPoint(rcButton.left(), rcButton.bottom() - popupr.height()), QPoint(rcButton.right(), rcButton.bottom()));
                painter->fillRect(rcSplitDown, buttonHighlightColor(opt->palette, pressed)); // highlightDn
                painter->setPen(buttonHighlightColor(opt->palette, pressed, true)); // highlightDn
                painter->drawRect(rcSplitDown.adjusted(0, 0, -1, -1));
            }
            else if (popuped || checked)
            {
                painter->fillRect(rcButton, buttonHighlightColor(opt->palette, true)); // highlightDn
                painter->setPen(buttonHighlightColor(opt->palette, true, true)); // highlightDn
                painter->drawRect(rcButton.adjusted(0, 0, -1, -1));
            }
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonOptionButton(const QStyleOption* option, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    QTN_P(const CommonStyleV5);
    drawButton(p.proxy()->standardPixmap((QStyle::StandardPixmap)CommonStyleV5::SP_RibbonButtonGroupOption, option, w), option, painter);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonQuickAccessButton(const QStyleOption* option, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    QTN_P(const CommonStyleV5);
    drawButton(p.proxy()->standardPixmap((QStyle::StandardPixmap)CommonStyleV5::SP_RibbonButtonPopupMenu, option, w), option, painter);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonBackstageFrameMenu(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    if (const StyleOptionRibbonBackstageV5* optBackstage = qstyleoption_cast<const StyleOptionRibbonBackstageV5*>(opt))
    {
        painter->fillRect(optBackstage->rect, darkBackground(optBackstage->palette));
        return true;
    }
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonBackstagePanelButton(const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    Q_UNUSED(w);
    if (const StyleOptionBackstageButtonV5* btnStyle = qstyleoption_cast<const StyleOptionBackstageButtonV5*>(opt))
    {
        const bool enabled = opt->state & QStyle::State_Enabled;
        const bool checked = opt->state & QStyle::State_On;
        const bool highlighted = opt->state & QStyle::State_MouseOver;
        const bool pressed = opt->state & QStyle::State_Sunken;
        const bool popuped = (btnStyle->activeSubControls & QStyle::SC_ToolButtonMenu) && (opt->state & QStyle::State_Sunken);

        QColor colorFill = Qt::white;
        QColor colorBorder(outlineMidlight(btnStyle->palette).dark(110));
        if (!enabled)
        {
            colorFill = QColor();
            colorBorder = outlineLight(btnStyle->palette);
        }
        else if (popuped)
        {
            colorFill = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, false/*border*/);
            colorBorder = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, true/*border*/);
        }
        else if (checked && !highlighted && !pressed)
        {
            colorFill = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, false/*border*/);
            colorBorder = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, true/*border*/);
        }
        else if (pressed)
        {
            colorFill = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, true/*border*/);
            colorBorder = buttonHighlightColor(btnStyle->palette, true/*highlightDn*/, true/*border*/);
        }
        else if (highlighted)
        {
            colorFill = buttonHighlightColor(btnStyle->palette, false/*highlightDn*/, false/*border*/);
            colorBorder = buttonHighlightColor(btnStyle->palette, false/*highlightDn*/, true/*border*/);
        }

        if (colorFill.isValid())
            p->fillRect(opt->rect.adjusted(0, 0, btnStyle->tabStyle ? -2 : -1, -1), colorFill);

        if (!btnStyle->tabStyle)
        {
            QPen savePen = p->pen();
            p->setPen(colorBorder);
            p->drawRect(btnStyle->rect.adjusted(0, 0, -1, -1));
            p->setPen(savePen);
        }
        return true;
    }
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonTabShapeLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    if (const QStyleOptionHeader* optTab = qstyleoption_cast<const QStyleOptionHeader*>(opt))
    {
        QTN_P(const CommonStyleV5);
        int flags = optTab->textAlignment;
        flags |= Qt::TextShowMnemonic;
        if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, w))
            flags |= Qt::TextHideMnemonic;

        // draw text
        p.proxy()->drawItemText(painter, optTab->rect, flags, opt->palette,
            opt->state & QStyle::State_Enabled, optTab->text, QPalette::WindowText);
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonTab(const QStyleOption* opt, QPainter* p, const QWidget* widget) const
{
    Q_UNUSED(widget);
    if (const StyleRibbonOptionHeaderV5* optTab = qstyleoption_cast<const StyleRibbonOptionHeaderV5*>(opt))
    {
        if (optTab->state & QStyle::State_Selected)
        {
            QRect rectTab = optTab->rect;
            // fill the background of the tab
            p->fillRect(rectTab, optTab->palette.button().color());

            QPainterPath path;
            if (optTab->tabBarPosition == StyleOptionFrameV5::TabBarTopPosition)
            {
                path.moveTo(rectTab.left(), rectTab.bottom());
                path.lineTo(rectTab.left(), rectTab.top());
                path.lineTo(rectTab.right(), rectTab.top());
                path.lineTo(rectTab.right(), rectTab.bottom());
            }
            else if (optTab->tabBarPosition == StyleOptionFrameV5::TabBarBottomPosition)
            {
                path.moveTo(rectTab.left(), rectTab.top());
                path.lineTo(rectTab.left(), rectTab.bottom());
                path.lineTo(rectTab.right(), rectTab.bottom());
                path.lineTo(rectTab.right(), rectTab.top());
            }
            else
                return true;

            QPen savePen = p->pen();
            QColor colorContext = CommonStyleV5::qtn_getColorToRGB(optTab->pageColor);
            p->setPen(colorContext.isValid() ? colorContext : optTab->palette.dark().color().light(135));
            p->drawPath(path);
            p->setPen(savePen);
        }
        else if (optTab->state & QStyle::State_Small)
        {
            QRect rectTab = optTab->rect;
            QPainterPath path;
            path.moveTo(rectTab.right(), rectTab.top());
            path.lineTo(rectTab.right(), rectTab.bottom());
            QPen savePen = p->pen();
            p->setPen(optTab->palette.dark().color().light(135));
            p->drawPath(path);
            p->setPen(savePen);
        }
        else
        {
            if (optTab->pageColor != CommonStyleV5::ContextColorNone && optTab->firstTab)
            {
                QRect rcSeparator(QPoint(opt->rect.left() - 1, opt->rect.top()), QPoint(opt->rect.left() + 1, opt->rect.bottom() - 2));

                QLinearGradient gradient(rcSeparator.topLeft(), rcSeparator.bottomLeft());
                gradient.setColorAt(0, optTab->palette.dark().color().light(110));
                gradient.setColorAt(1, Qt::white);

                p->setBrush(gradient);
                p->setPen(Qt::transparent);
                p->drawRect(rcSeparator.adjusted(0, 0, -1, -1));
            }

            if (optTab->pageColor != CommonStyleV5::ContextColorNone && optTab->lastTab)
            {
                QRect rcSeparator(QPoint(opt->rect.right(), opt->rect.top()), QPoint(opt->rect.right() + 1, opt->rect.bottom() - 2));

                QLinearGradient gradient(rcSeparator.topLeft(), rcSeparator.bottomLeft());
                gradient.setColorAt(0, optTab->palette.dark().color().light(110));
                gradient.setColorAt(1, Qt::white);

                p->setBrush(gradient);
                p->setPen(Qt::transparent);
                p->drawRect(rcSeparator.adjusted(0, 0, -1, -1));
            }
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonTabBar(const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    Q_UNUSED(opt);
    Q_UNUSED(p);
    Q_UNUSED(w);
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonGroupLabel(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    Q_UNUSED(widget);
    if (const StyleRibbonOptionGroupV5* optGroupBox = qstyleoption_cast<const StyleRibbonOptionGroupV5*>(opt))
    {
        int flags = optGroupBox->textAlignment;
        flags |= Qt::TextHideMnemonic;

        QRect rcEntryCaption = optGroupBox->rect;
        rcEntryCaption.setTop(rcEntryCaption.bottom() - optGroupBox->heightCaption);
        rcEntryCaption.adjust(-2, 4, -optGroupBox->widthOptButton, -2);
        // draw text
        QString text = optGroupBox->fontMetrics.elidedText(optGroupBox->text, optGroupBox->textElideMode, rcEntryCaption.width());
        if (!text.isEmpty())
        {
            QPen savePen = painter->pen();
            painter->setPen(optGroupBox->palette.light().color().light(55));
            painter->drawText(rcEntryCaption, flags, text);
            painter->setPen(savePen);
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonSysMenuItem(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionMenuItem* menuitem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt))
    {
        if (menuitem->text.count(QString(_qtn_PopupLable)) > 0)
        {
            QRect rcPopupLable = opt->rect;
            if (widget)
            {
                rcPopupLable.setX(0);
                rcPopupLable.setWidth(widget->width());
                rcPopupLable.adjust(1, 0, -1, 1);
            }
            painter->fillRect(rcPopupLable, darkBackground(menuitem->palette));
            painter->fillRect(rcPopupLable.left(), rcPopupLable.bottom() - 1, rcPopupLable.width(), 1, outlineMidlight(menuitem->palette));

            QFont fontSave = painter->font();

            QFont font = fontSave;
            font.setBold(true);
            painter->setFont(font);

            QRect rcText(menuitem->rect);
            rcText.adjust(7, 0, 0, 0);

            QPalette pal = menuitem->palette;
            pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(240));

            int flags = 0;
            flags |= Qt::TextHideMnemonic | Qt::AlignVCenter | Qt::TextSingleLine;

            QString text = menuitem->text;
            text = painter->fontMetrics().elidedText(text.remove(QString(_qtn_PopupLable)), Qt::ElideRight, rcText.adjusted(2, 0, -2, 0).width());
            // draw text
            p.proxy()->drawItemText(painter, rcText, flags, pal, opt->state & QStyle::State_Enabled, text, QPalette::WindowText);
            painter->setFont(fontSave);
            return true;
        }
        else
        {
            int x, y, w, h;
            menuitem->rect.getRect(&x, &y, &w, &h);
            int tab = menuitem->tabWidth;
            bool dis = !(menuitem->state & QStyle::State_Enabled);
            bool checked = menuitem->checkType != QStyleOptionMenuItem::NotCheckable ? menuitem->checked : false;
            bool act = menuitem->state & QStyle::State_Selected;

            // windows always has a check column, regardless whether we have an icon or not
            const int nIconSize = p.proxy()->pixelMetric(QStyle::PM_SmallIconSize, opt, widget);
            int checkcol = qMax<int>(menuitem->maxIconWidth, windowsCheckMarkWidth);

            QBrush fill;
            if (act)
            {
                painter->setPen(menuitem->palette.brush(QPalette::Highlight).color().light());
                fill = menuitem->palette.brush(QPalette::Highlight).color().light(170);
                painter->fillRect(menuitem->rect.adjusted(1, 0, -2, 0), fill);
                QRect rect = menuitem->rect.adjusted(0, 0, -2, -1);
                painter->drawRect(rect);

                if (menuitem->text.count(QString(_qtn_SplitActionPopup)) != 0)
                {
                    int dim = (h - 2 * windowsItemFrame) / 2;
                    int xpos = x + w - windowsArrowHMargin - windowsItemFrame - dim;
                    painter->drawLine(rect.left() + xpos, rect.top(), rect.left() + xpos, rect.bottom());
                }
            }
            else
            {
                fill = Qt::transparent;
                painter->fillRect(menuitem->rect.adjusted(0, 0, -1, 0), fill);
            }

            if (menuitem->menuItemType == QStyleOptionMenuItem::Separator)
            {
                int yoff = y - 1 + h / 2;
                int xIcon = nIconSize;
                if (widget && widget->inherits("Qtitan::OfficePopupMenu"))
                    xIcon = 0;
                painter->setPen(opt->palette.brush(QPalette::Midlight).color().dark(105));
                painter->drawLine(x + 2 + xIcon, yoff, x + w - 4, yoff);
                painter->setPen(opt->palette.brush(QPalette::Light).color());
                painter->drawLine(x + 2 + xIcon, yoff + 1, x + w - 4, yoff + 1);
                return true;
            }

            // On Windows Style, if we have a checkable item and an icon we
            // draw the icon recessed to indicate an item is checked. If we
            // have no icon, we draw a checkmark instead.
            if (!menuitem->icon.isNull())
            {
                QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
                if (act && !dis)
                    mode = QIcon::Active;
                QPixmap pixmap;
                if (checked)
                    pixmap = menuitem->icon.pixmap(p.proxy()->pixelMetric(QStyle::PM_SmallIconSize, opt, widget), mode, QIcon::On);
                else
                    pixmap = menuitem->icon.pixmap(p.proxy()->pixelMetric(QStyle::PM_SmallIconSize, opt, widget), mode);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                const int pixw = pixmap.width() / pixmap.devicePixelRatio();
                const int pixh = pixmap.height() / pixmap.devicePixelRatio();
#else
                const int pixw = pixmap.width();
                const int pixh = pixmap.height();
#endif
                QRect pmr(0, 0, pixw, pixh);
                QRect vCheckRect = p.visualRect(opt->direction, menuitem->rect, QRect(menuitem->rect.x(), menuitem->rect.y(), checkcol, menuitem->rect.height()));
                pmr.moveCenter(vCheckRect.center());
                painter->setPen(menuitem->palette.text().color());
                painter->drawPixmap(pmr.topLeft(), pixmap);
            }

            QPalette pal = opt->palette;
            pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(240));
            painter->setPen(pal.color(QPalette::WindowText));

            QColor discol;
            if (dis)
            {
                discol = pal.color(QPalette::Dark).dark(130);
                painter->setPen(discol);
            }

            int xm = int(windowsItemFrame) + checkcol + int(windowsItemHMargin);
            int xpos = menuitem->rect.x() + xm;

            QStringList split = menuitem->text.split(QString("\n"));
            QString s = split.count() > 0 ? split.at(0) : menuitem->text;
            if (split.count() > 1)
            {
                int height = menuitem->fontMetrics.height();
                QRect textRect(xpos, y + windowsItemVMargin + 2, w - xm - windowsRightBorder - tab + 1, height);
                QRect vTextRect = p.visualRect(opt->direction, menuitem->rect, textRect);

                QFont font = menuitem->font;
                QFont saveFont = font;
                font.setBold(true);
                painter->setFont(font);

                int t = s.indexOf(QLatin1Char('\t'));
                int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;

                if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, menuitem, widget))
                    text_flags |= Qt::TextHideMnemonic;
                text_flags |= Qt::AlignLeft;

                // draw hotkeys
                if (t >= 0)
                {
                    QRect vShortcutRect = p.visualRect(opt->direction, menuitem->rect, QRect(textRect.topRight(),
                        QPoint(menuitem->rect.right(), textRect.bottom())));

                    painter->setPen(opt->state & QStyle::State_Enabled ? pal.color(QPalette::Normal, QPalette::WindowText) : discol);
                    painter->drawText(vShortcutRect, text_flags, s.mid(t + 1));
                    s = s.left(t);
                }

                painter->setPen(opt->state & QStyle::State_Enabled ? pal.color(QPalette::Normal, QPalette::WindowText) : discol);
                painter->drawText(vTextRect, text_flags, s.left(t));
                painter->setFont(saveFont);

                if (split.count() > 1)
                {
                    text_flags |= Qt::TextWordWrap;
                    QRect textTipRect(xpos, y + vTextRect.height(), w - xm - 1, opt->rect.height());
                    QRect rc = menuitem->fontMetrics.boundingRect(textTipRect, text_flags, split.at(1));
                    QRect vTextTipRect = p.visualRect(opt->direction, menuitem->rect, rc);
                    vTextTipRect.setTop(y + vTextRect.height());
                    vTextTipRect.setWidth(textTipRect.width());
                    painter->setPen(opt->state & QStyle::State_Enabled ? pal.color(QPalette::Normal, QPalette::WindowText) : discol);
                    painter->drawText(vTextTipRect, text_flags, split.at(1));
                }
            }
            else
            {
                QRect textRect(xpos, y + windowsItemVMargin,
                    w - xm - windowsRightBorder - tab + 1, h - 2 * windowsItemVMargin);
                QRect vTextRect = p.visualRect(opt->direction, menuitem->rect, textRect);

                QString s(menuitem->text);
                s = s.remove(QString(_qtn_SplitActionPopup));
                // draw text
                if (!s.isEmpty())
                {
                    int t = s.indexOf(QLatin1Char('\t'));
                    int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
                    if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, menuitem, widget))
                        text_flags |= Qt::TextHideMnemonic;
                    text_flags |= Qt::AlignLeft;

                    if (t >= 0)
                    {
                        QRect vShortcutRect = p.visualRect(opt->direction, menuitem->rect, QRect(textRect.topRight(), QPoint(menuitem->rect.right(), textRect.bottom())));
                        const QString textToDraw = s.mid(t + 1);
                        if (dis && !act && p.proxy()->styleHint(QStyle::SH_EtchDisabledText, opt, widget))
                        {
                            painter->setPen(menuitem->palette.light().color());
                            painter->drawText(vShortcutRect.adjusted(1, 1, 1, 1), text_flags, textToDraw);
                            painter->setPen(discol);
                        }
                        painter->drawText(vShortcutRect, text_flags, textToDraw);
                        s = s.left(t);
                    }

                    QFont saveFont = menuitem->font;
                    QFont font = saveFont;
                    if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
                        font.setBold(true);
                    painter->setFont(font);

                    const QString textToDraw = s.left(t);
                    if (dis && !act && p.proxy()->styleHint(QStyle::SH_EtchDisabledText, opt, widget))
                    {
                        painter->setPen(menuitem->palette.light().color());
                        painter->drawText(vTextRect.adjusted(1, 1, 1, 1), text_flags, textToDraw);
                        painter->setPen(discol);
                    }
                    painter->drawText(vTextRect, text_flags, textToDraw);
                    painter->setFont(saveFont);
                }
            }
            // draw sub menu arrow
            if (menuitem->menuItemType == QStyleOptionMenuItem::SubMenu)
            {
                int dim = (h - 2 * windowsItemFrame) / 2;
                QStyle::PrimitiveElement arrow = (opt->direction == Qt::RightToLeft) ? QStyle::PE_IndicatorArrowLeft : QStyle::PE_IndicatorArrowRight;
                xpos = x + w - windowsArrowHMargin - windowsItemFrame - dim;
                QRect  vSubMenuRect = p.visualRect(opt->direction, menuitem->rect, QRect(xpos, y + h / 2 - dim / 2, dim, dim));
                QStyleOptionMenuItem newMI = *menuitem;
                newMI.rect = vSubMenuRect;
                newMI.state = dis ? QStyle::State_None : QStyle::State_Enabled;
                if (act)
                    newMI.palette.setColor(QPalette::ButtonText, newMI.palette.highlightedText().color());
                p.proxy()->drawPrimitive(arrow, &newMI, painter, widget);
            }
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonSysRecentFileItem(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    Q_UNUSED(widget);
    if (const QStyleOptionMenuItem* menuitem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt))
    {
        bool act = menuitem->state & QStyle::State_Selected;
        if (act)
        {
            painter->setPen(menuitem->palette.brush(QPalette::Highlight).color().light());
            QBrush fill = menuitem->palette.brush(QPalette::Highlight).color().light(170);
            painter->fillRect(menuitem->rect.adjusted(1, 0, -2, 0), fill);
            painter->drawRect(menuitem->rect.adjusted(1, 0, -2, -1));
        }

        QRect rect = menuitem->rect;
        uint alignment = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextShowMnemonic;
        rect.translate(7, 0); //move

        QPalette pal = opt->palette;
        QPen savePen = painter->pen();
        pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(240));
        painter->setPen(pal.color(QPalette::WindowText));
        QString text = menuitem->fontMetrics.elidedText(menuitem->text, Qt::ElideRight, rect.adjusted(2, 0, -2, 0).width());
        p.proxy()->drawItemText(painter, rect, alignment, pal, true, text, QPalette::WindowText);
        painter->setPen(savePen);
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionToolButton* toolbutton = qstyleoption_cast<const QStyleOptionToolButton*>(opt))
    {
        painter->setFont(toolbutton->font);

        QRect rect = toolbutton->rect;
        // Arrow type always overrules and is always shown
        bool hasArrow = toolbutton->features & QStyleOptionToolButton::Arrow;
        if (((!hasArrow && toolbutton->icon.isNull()) && !toolbutton->text.isEmpty()) || toolbutton->toolButtonStyle == Qt::ToolButtonTextOnly)
        {
            int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
            if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, w))
                alignment |= Qt::TextHideMnemonic;

            QPalette::ColorRole textRole = QPalette::ButtonText;
            if ((opt->state & QStyle::State_MouseOver || opt->state & QStyle::State_Sunken) && (toolbutton->state & QStyle::State_Enabled))
                textRole = QPalette::HighlightedText;

            QRect rectText = rect;
            QString text = toolbutton->text;
            if ((toolbutton->subControls & QStyle::SC_ToolButtonMenu) || (toolbutton->features & QStyleOptionToolButton::HasMenu))
            {
                int mbi = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, opt, w);
                rectText.adjust(0, 0, -mbi, 0);
            }
            p.proxy()->drawItemText(painter, rectText, alignment, toolbutton->palette, opt->state & QStyle::State_Enabled, text, textRole);
        }
        else
        {
            QPixmap pm;
            QSize pmSize = toolbutton->iconSize;
            if (!toolbutton->icon.isNull())
            {
                QIcon::State state = toolbutton->state & QStyle::State_On ? QIcon::On : QIcon::Off;
                QIcon::Mode mode;
                if (!(toolbutton->state & QStyle::State_Enabled))
                    mode = QIcon::Disabled;
                else if ((opt->state & QStyle::State_MouseOver) && (opt->state & QStyle::State_AutoRaise))
                    mode = QIcon::Active;
                else
                    mode = QIcon::Normal;

                pm = toolbutton->icon.pixmap(toolbutton->rect.size().boundedTo(toolbutton->iconSize), mode, state);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                pmSize = pm.size() / pm.devicePixelRatio();
#else
                pmSize = pm.size();
#endif
                if (pmSize.width() * pmSize.height() < toolbutton->iconSize.width() * toolbutton->iconSize.height())
                    pm = pm.scaled(toolbutton->iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }

            if (toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly)
            {
                painter->setFont(toolbutton->font);
                QRect pr = rect, tr = rect;
                QRect popupr = p.proxy()->subControlRect(QStyle::CC_ToolButton, toolbutton, QStyle::SC_ToolButtonMenu, w);

                int alignment = Qt::TextShowMnemonic;
                if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, w))
                    alignment |= Qt::TextHideMnemonic;

                if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
                {
                    pr.setBottom(pr.bottom() - popupr.height());
                    if (!hasArrow)
                        p.proxy()->drawItemPixmap(painter, pr, Qt::AlignCenter, pm);
                    else
                        QtnPrivate::drawArrow(&p, toolbutton, pr, painter, w);
                    alignment |= Qt::AlignHCenter | Qt::AlignTop;
                }
                else
                {
                    pr.setWidth(pmSize.width() + 8);
                    tr.adjust(pr.width(), 0, 0, 0);
                    if (!hasArrow)
                        p.proxy()->drawItemPixmap(painter, QStyle::visualRect(opt->direction, rect, pr), Qt::AlignCenter, pm);
                    else
                        QtnPrivate::drawArrow(&p, toolbutton, pr, painter, w);
                    alignment |= Qt::AlignLeft | Qt::AlignVCenter;
                }

                if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
                {
                    if (w && w->property("wordWrap").toBool())
                        alignment |= Qt::TextWordWrap;

                    const int space = 1;
                    tr.setTop(tr.bottom() - popupr.height());
                    QRect rcText(QStyle::visualRect(opt->direction, rect, tr));
                    rcText.adjust(space, 0, -space, 0);
                    p.proxy()->drawItemText(painter, rcText, alignment, toolbutton->palette,
                        toolbutton->state & QStyle::State_Enabled, toolbutton->text, QPalette::ButtonText);
                }
                else
                {
                    p.proxy()->drawItemText(painter, QStyle::visualRect(opt->direction, rect, tr), alignment, toolbutton->palette,
                        toolbutton->state & QStyle::State_Enabled, toolbutton->text, QPalette::ButtonText);
                }
            }
            else
            {
                if (hasArrow)
                {
                    QtnPrivate::drawArrow(&p, toolbutton, rect, painter, w);
                }
                else
                {
                    QRect pr = rect;
                    if ((toolbutton->subControls & QStyle::SC_ToolButtonMenu) || (toolbutton->features & QStyleOptionToolButton::HasMenu))
                    {
                        int mbi = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, opt, w);
                        pr.setWidth(pr.width() - mbi);
                    }
                    p.proxy()->drawItemPixmap(painter, pr, Qt::AlignCenter, pm);
                }
            }
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonGalleryItem(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    QTN_P(const CommonStyleV5);
    Q_UNUSED(w);
    if (const StyleRibbonGalleryItemV5* optItem = qstyleoption_cast<const StyleRibbonGalleryItemV5*>(opt))
    {
        bool enabled = opt->state & QStyle::State_Enabled;
        QRect rectItem = optItem->rectItem;
        if (optItem->separator)
        {
            painter->fillRect(rectItem, darkBackground(optItem->palette));
            painter->fillRect(rectItem.left(), rectItem.bottom() - 1, rectItem.width(), 1, outlineMidlight(optItem->palette));

            int alignment = 0;
            alignment |= Qt::TextSingleLine | Qt::AlignVCenter;

            QPalette pal = optItem->palette;
            pal.setColor(QPalette::WindowText, pal.color(QPalette::Light).dark(240));

            rectItem.adjust(10, 0, 0, 0);
            const QFont& saveFont = painter->font();
            QFont font(saveFont);
            font.setBold(true);
            painter->setFont(font);
            p.proxy()->drawItemText(painter, rectItem, alignment, pal, true, optItem->caption, QPalette::WindowText);
            painter->setFont(saveFont);
            return true;
        }
        if (!optItem->icon.isNull())
        {
            QStyleOption optTmp = *optItem;
            optTmp.rect = optItem->rectItem;
            drawHighlightItemGallery(&optTmp, painter);

            QList<QSize> lstSz = optItem->icon.availableSizes(enabled ? QIcon::Normal : QIcon::Disabled);
            QSize iconSize = lstSz.size() > 0 ? lstSz[0] : rectItem.size();

            if (QApplication::style()->property("DPIAware").toBool())
                iconSize = QSize(static_cast<int>(CommonStyleV5Private::dpiScaled(static_cast<qreal>(iconSize.width()))),
                    static_cast<int>(CommonStyleV5Private::dpiScaled(static_cast<qreal>(iconSize.height()))));

            QPixmap pm = optItem->icon.pixmap(iconSize, enabled ? QIcon::Normal : QIcon::Disabled);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QSize pmSize = pm.size() / pm.devicePixelRatio();
#else
            QSize pmSize = pm.size();
#endif
            int space = (int)CommonStyleV5Private::dpiScaled(static_cast<qreal>(4));
            rectItem.adjust(space, space, -space, -space);

            if (pmSize.width() * pmSize.height() < iconSize.width() * iconSize.height())
                pm = pm.scaled(rectItem.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            p.proxy()->drawItemPixmap(painter, rectItem, Qt::AlignCenter, pm);
        }
        else
        {
            QStyleOption optTmp = *optItem;
            optTmp.rect = optItem->rectItem;
            drawHighlightButton(&optTmp, painter);

            //            style->drawRectangle(p, rectItem, selected, false, enabled, checked, false, TypePopup, BarPopup);

            QRect rcText(rectItem);
            rcText.setLeft(rcText.left() + 3);
            int alignment = 0;
            alignment |= Qt::TextSingleLine | Qt::AlignVCenter | Qt::TextHideMnemonic;
            QPalette palette;
            //            palette.setColor(QPalette::WindowText, style->getTextColor(selected, false, enabled, checked, false, TypePopup, BarPopup));
            p.proxy()->drawItemText(painter, rcText, alignment, palette, enabled, optItem->caption, QPalette::WindowText);
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonBackstageMenuItem(const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionMenuItem* menuitem = qstyleoption_cast<const QStyleOptionMenuItem*>(opt))
    {
        const bool act = menuitem->state & QStyle::State_Selected;
        const bool focus = menuitem->state & QStyle::State_HasFocus;
        const bool dis = !(menuitem->state & QStyle::State_Enabled);

        QRect rcText(opt->rect);
        rcText.adjust(15, 0, 0, 0);
        if (menuitem->menuItemType == QStyleOptionMenuItem::Separator)
        {
            int x, y, w, h;
            menuitem->rect.getRect(&x, &y, &w, &h);
            // windows always has a check column, regardless whether we have an icon or not
            const int iconSize = p.proxy()->pixelMetric(QStyle::PM_ToolBarIconSize, opt, widget);
            int yoff = (y - 1 + h / 2);
            painter->setPen(opt->palette.brush(QPalette::Midlight).color().dark(105));
            painter->drawLine(x + 2 + iconSize, yoff, x + w - 4, yoff);
            painter->setPen(opt->palette.brush(QPalette::Light).color());
            painter->drawLine(x + 2 + iconSize, yoff + 1, x + w - 4, yoff + 1);
            return true;
        }

        if (act || focus)
        {
            painter->setPen(buttonHighlightColor(opt->palette, focus/*highlightDn*/, true));
            QBrush fill = buttonHighlightColor(opt->palette, focus/*highlightDn*/, false);
            painter->fillRect(opt->rect.adjusted(1, 0, -2, 0), fill);
            painter->drawRect(opt->rect.adjusted(1, 0, -2, -1));
        }

        int iconWidth = 0;
        int indent = menuitem->menuItemType == QStyleOptionMenuItem::SubMenu ? 6 : 0;
        if (!menuitem->icon.isNull())
        {
            int iconSize = p.proxy()->pixelMetric(QStyle::PM_LargeIconSize, opt, widget);
            if (iconSize > qMin(opt->rect.height(), opt->rect.width()))
                iconSize = p.proxy()->pixelMetric(QStyle::PM_SmallIconSize, opt, widget);

            QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
            QPixmap pixmap = menuitem->icon.pixmap(QSize(iconSize, iconSize), mode);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            const int pixw = pixmap.width() / pixmap.devicePixelRatio();
            const int pixh = pixmap.height() / pixmap.devicePixelRatio();
#else
            const int pixw = pixmap.width();
            const int pixh = pixmap.height();
#endif
            if (pixw * pixh < iconSize * iconSize)
                pixmap = pixmap.scaled(QSize(iconSize, iconSize), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPoint pmr(rcText.left() + indent, (rcText.top() + rcText.bottom() - iconSize) / 2);
            painter->drawPixmap(pmr, pixmap);
            iconWidth = iconSize;
        }

        indent += !menuitem->icon.isNull() ? 7 : 0;
        rcText.setLeft(rcText.left() + iconWidth + indent);
        rcText.setHeight(qMax(rcText.height(), iconWidth));

        if (!menuitem->text.isEmpty())
        {
            QPalette pal = opt->palette;
            pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(240));
            painter->setPen(pal.color(QPalette::WindowText));

            int flags = 0;
            flags |= Qt::TextHideMnemonic | Qt::AlignVCenter | Qt::TextSingleLine;
            int index = menuitem->text.indexOf(QLatin1String("&"));
            if (index != -1)
            {
                index = menuitem->text.indexOf(QLatin1String(" "), index);
                if (index != -1)
                    flags &= ~Qt::TextHideMnemonic;
            }
            // draw text
            p.proxy()->drawItemText(painter, rcText, flags, pal, opt->state & QStyle::State_Enabled, menuitem->text, QPalette::WindowText);
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonBackstageButtonLabel(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    if (const StyleOptionBackstageButtonV5* toolbutton = qstyleoption_cast<const StyleOptionBackstageButtonV5*>(opt))
    {
        QTN_P(const CommonStyleV5);
        QRect rect = toolbutton->rect;
        // Arrow type always overrules and is always shown
        bool hasArrow = toolbutton->features & QStyleOptionToolButton::Arrow;
        if (((!hasArrow && toolbutton->icon.isNull()) && !toolbutton->text.isEmpty()) || toolbutton->toolButtonStyle == Qt::ToolButtonTextOnly)
        {
            int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
            if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, w))
                alignment |= Qt::TextHideMnemonic;
        }
        else
        {
            QPixmap pm;
            QSize pmSize = toolbutton->iconSize;
            if (pmSize.width() > qMin(rect.width(), rect.height()))
            {
                const int iconExtent = p.proxy()->pixelMetric(QStyle::PM_SmallIconSize);
                pmSize = QSize(iconExtent, iconExtent);
            }

            if (!toolbutton->icon.isNull())
            {
                QIcon::State state = toolbutton->state & QStyle::State_On ? QIcon::On : QIcon::Off;
                QIcon::Mode mode;
                if (!(toolbutton->state & QStyle::State_Enabled))
                    mode = QIcon::Disabled;
                else if ((opt->state & QStyle::State_MouseOver) && (opt->state & QStyle::State_AutoRaise))
                    mode = QIcon::Active;
                else
                    mode = QIcon::Normal;
                pm = toolbutton->icon.pixmap(toolbutton->rect.size().boundedTo(pmSize), mode, state);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                pmSize = pm.size() / pm.devicePixelRatio();
#else
                pmSize = pm.size();
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                if (pmSize.width() * pmSize.height() < toolbutton->iconSize.width() * toolbutton->iconSize.height())
                    pm = pm.scaled(toolbutton->iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            if (toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly)
            {
                int alignment = Qt::TextShowMnemonic;
                if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, w))
                    alignment |= Qt::TextHideMnemonic;

                painter->setFont(toolbutton->font);
                QRect pr = rect;
                if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
                {
                    alignment |= Qt::AlignHCenter | Qt::TextWordWrap;
                    pr.moveTop((toolbutton->fontMetrics.height() / 2 + 2) / 2);
                    if (!hasArrow)
                        p.drawItemPixmap(painter, pr, Qt::AlignTop | Qt::AlignHCenter, pm);
                }
                else
                {
                    QRect pmRect(QPoint(4, (rect.height() - pmSize.height()) / 2), pmSize);
                    if (!hasArrow)
                        p.QStyle::drawItemPixmap(painter, QStyle::visualRect(opt->direction, rect, pmRect), Qt::AlignCenter, pm);

                    alignment |= Qt::AlignLeft | Qt::AlignVCenter;

                    QSize sizeText = toolbutton->fontMetrics.size(Qt::TextSingleLine, toolbutton->text);
                    QRect tcText(QPoint(pmRect.right() + 4, (rect.height() - sizeText.height()) / 2), sizeText);
                    p.proxy()->drawItemText(painter, tcText, alignment, toolbutton->palette, /*toolbutton->state & State_Enabled*/true, toolbutton->text, QPalette::ButtonText);
                }
            }
            else
            {
                QRect pr = rect;
                if ((toolbutton->subControls & QStyle::SC_ToolButtonMenu) || (toolbutton->features & QStyleOptionToolButton::HasMenu))
                {
                    int mbi = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, opt, w);
                    pr.setWidth(pr.width() - mbi);
                }
                p.QStyle::drawItemPixmap(painter, pr, Qt::AlignCenter, pm);
            }
        }
        return true;
    }
    return false;
}

/*! \internal */
bool CommonStyleV5Private::drawOfficePopupSizeGrip(const QStyleOption* opt, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    if (const QStyleOptionSizeGrip* sizeGrip = qstyleoption_cast<const QStyleOptionSizeGrip*>(opt))
    {
        painter->fillRect(sizeGrip->rect, darkBackground(opt->palette).dark());
        QPoint pt(sizeGrip->rect.right() - 3, sizeGrip->rect.bottom() - 3);
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 3 - y; x++)
                painter->fillRect(QRect(QPoint(pt.x() + 0 - x * 4, pt.y() + 0 - y * 4), QSize(2, 2)), outlineMidlight(opt->palette));
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonFileButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    if (const StyleSystemToolButtonV5* optButton = qstyleoption_cast<const StyleSystemToolButtonV5*>(opt))
    {
        const bool popuped = (optButton->activeSubControls & QStyle::SC_ToolButtonMenu) && (optButton->state & QStyle::State_Sunken);
        const bool isHighlighted = optButton->state & QStyle::State_MouseOver;
//        const bool enabled = widget->isEnabled();

        QColor fillColor = QColor(0x19, 0x79, 0xCA);
        if (isHighlighted && !popuped)
            fillColor = fillColor.light(125);
        else if (isHighlighted && popuped)
            fillColor = fillColor.light(70);

        painter->fillRect(optButton->rect, fillColor);

        if (optButton->toolButtonStyle != Qt::ToolButtonIconOnly)
        {
            int alignment = Qt::TextShowMnemonic;
            if (!p.proxy()->styleHint(QStyle::SH_UnderlineShortcut, optButton, widget))
                alignment |= Qt::TextHideMnemonic;
            alignment |= Qt::AlignCenter;
            alignment |= Qt::TextSingleLine;
            p.proxy()->drawItemText(painter, optButton->rect, alignment, optButton->palette,
                optButton->state & QStyle::State_Enabled, optButton->text, QPalette::ButtonText);
        }
    }
    return true;
}

static bool qtn_canWordWrap(const QRect& rect, const QFontMetrics& fm, const QString& str, int* heightLine = 0)
{
    int alignment = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextHideMnemonic;
    QSize sz = fm.boundingRect(rect, alignment, str).size();
    if (heightLine)
        *heightLine = fm.height();
    return  (sz.height() / fm.height()) == 2;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    if (const QStyleOptionToolButton* toolbutton = qstyleoption_cast<const QStyleOptionToolButton*>(opt))
    {
        QRect button, menuarea;
        button = p.proxy()->subControlRect(QStyle::CC_ToolButton, toolbutton, QStyle::SC_ToolButton, widget);
        menuarea = p.proxy()->subControlRect(QStyle::CC_ToolButton, toolbutton, QStyle::SC_ToolButtonMenu, widget);

        QStyle::State flags = toolbutton->state & ~QStyle::State_Sunken;

        if (flags & QStyle::State_AutoRaise)
        {
            if (!(flags & QStyle::State_MouseOver) || !(flags & QStyle::State_Enabled))
                flags &= ~QStyle::State_Raised;
        }
        QStyle::State mflags = flags;
        if (toolbutton->state & QStyle::State_Sunken)
        {
            if (toolbutton->activeSubControls & QStyle::SC_ToolButton)
                flags |= QStyle::State_Sunken;
            mflags |= QStyle::State_Sunken;
        }

        p.proxy()->drawPrimitive((QStyle::PrimitiveElement)CommonStyleV5::PE_RibbonPanelButton, toolbutton, painter, widget);

        QStyleOptionToolButton label = *toolbutton;
        label.state = flags;
        p.proxy()->drawControl((QStyle::ControlElement)CommonStyleV5::CE_RibbonButtonLabel, &label, painter, widget);

        QPixmap pm;
        QSize pmSize = toolbutton->iconSize;
        if (pmSize.width() > qMin(toolbutton->rect.width(), toolbutton->rect.height()))
        {
            const int iconExtent = p.proxy()->pixelMetric(QStyle::PM_SmallIconSize);
            pmSize = QSize(iconExtent, iconExtent);
        }

        if (!toolbutton->icon.isNull())
        {
            QIcon::State state = toolbutton->state & QStyle::State_On ? QIcon::On : QIcon::Off;
            QIcon::Mode mode;
            if (!(toolbutton->state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if ((opt->state & QStyle::State_MouseOver) && (opt->state & QStyle::State_AutoRaise))
                mode = QIcon::Active;
            else
                mode = QIcon::Normal;
            pm = toolbutton->icon.pixmap(toolbutton->rect.size().boundedTo(pmSize), mode, state);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            pmSize = pm.size() / pm.devicePixelRatio();
#else
            pmSize = pm.size();
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        }

        if (toolbutton->subControls & QStyle::SC_ToolButtonMenu || toolbutton->features & QStyleOptionToolButton::HasMenu)
        {
            QStyleOption tool(0);
            tool.palette = toolbutton->palette;
            QRect ir = menuarea, rcArrow;
            int mbi = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, toolbutton, widget) - 5;
            if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
            {
                const int heightFont = toolbutton->fontMetrics.height();
                if (qtn_canWordWrap(toolbutton->rect, toolbutton->fontMetrics, toolbutton->text))
                    rcArrow = QRect(QPoint(ir.right() - (mbi + mbi / 2), ir.top() + heightFont + (heightFont / 2 - mbi / 2)), QSize(mbi, mbi));
                else
                    rcArrow = QRect(QPoint(ir.width() / 2 - mbi / 2, ir.top() + heightFont + (heightFont / 2 - mbi / 2)), QSize(mbi, mbi));
            }
            else
                rcArrow = QRect(QPoint((ir.left() + (ir.width() / 2 - mbi / 2)), ir.y() + ir.height() / 2 - mbi / 2), QSize(mbi, mbi));

            tool.rect = rcArrow;
            tool.state = mflags;
            p.proxy()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &tool, painter, widget);
        }
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonTitleBarWidget(const QStyleOptionComplex* option, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    QTN_P(const CommonStyleV5);
    if (const StyleOptionTitleBarV5* optTitleBar = qstyleoption_cast<const StyleOptionTitleBarV5*>(option))
    {
        for (QVector<StyleOptionTitleBarV5::ContextData>::ConstIterator it = optTitleBar->listContextHeaders.begin(); it != optTitleBar->listContextHeaders.end(); ++it)
        {
            QRect rc = it->rect;
            QColor colorContext = CommonStyleV5::qtn_getColorToRGB(it->color);
            painter->fillRect(rc, colorContext);

            QPalette pal = option->palette;
            painter->setPen(pal.color(QPalette::Dark).dark(240));
            painter->drawText(rc, Qt::AlignCenter | Qt::TextSingleLine, painter->fontMetrics().elidedText(it->strTitle, Qt::ElideRight, rc.width()));
        }
        painter->setFont(optTitleBar->fontTextTitle);
        QString text = QFontMetrics(optTitleBar->fontTextTitle).elidedText(optTitleBar->text, Qt::ElideRight, optTitleBar->rcTitleText.width());
        p.proxy()->drawItemText(painter, optTitleBar->rcTitleText, optTitleBar->alignment, optTitleBar->palette, true/*enabled*/, text, QPalette::WindowText);
    }
    return true;
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonGalleryButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    if (widget == Q_NULL)
        return false;

    if (const QStyleOptionToolButton* toolbutton = qstyleoption_cast<const QStyleOptionToolButton*>(opt))
    {
        QPixmap px;
        if (widget->property(_qtn_PopupButtonGallery).toBool())
            px = p.proxy()->standardPixmap((QStyle::StandardPixmap)CommonStyleV5::SP_RibbonButtonPopupMenu, opt);
        else if (widget->property(_qtn_ScrollUpButtonGallery).toBool())
            px = p.proxy()->standardPixmap((QStyle::StandardPixmap)CommonStyleV5::SP_RibbonShadedArrowUp, opt);
        else if (widget->property(_qtn_ScrollDownButtonGallery).toBool())
            px = p.proxy()->standardPixmap((QStyle::StandardPixmap)CommonStyleV5::SP_RibbonShadedArrowDown, opt);

        if (!px.isNull())
        {
            painter->fillRect(opt->rect, darkBackground(toolbutton->palette));
            painter->setPen(outlineMidlight(toolbutton->palette));
            painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
            drawButton(px, opt, painter);
            return true;
        }
    }
    return false;
}

static void qtn_QTextDocument_draw(QPainter& painter, QTextDocument* document, const QPalette& palette, const QRect& rect)
{
    QPalette pal = palette;
    pal.setColor(QPalette::Text, pal.color(QPalette::Dark).dark(240));

    QAbstractTextDocumentLayout::PaintContext ctx;
    painter.translate(rect.left(), rect.top());
    ctx.clip = QRect(0, 0, rect.width(), rect.height());
    painter.setClipRect(ctx.clip, Qt::IntersectClip);
    ctx.palette = pal;
    document->documentLayout()->draw(&painter, ctx);
}

/*! \internal */
bool CommonStyleV5Private::drawRibbonBackstageButton(const QStyleOptionComplex* opt, QPainter* painter, const QWidget* widget) const
{
    if (const StyleOptionBackstageButtonV5* toolbutton = qstyleoption_cast<const StyleOptionBackstageButtonV5*>(opt))
    {
        QTN_P(const CommonStyleV5);
        QSize iconSize = toolbutton->iconSize;
        if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            if (!toolbutton->icon.isNull())
            {
                QSize actualSize = toolbutton->icon.actualSize(toolbutton->iconSize, QIcon::Normal, QIcon::Off);
                int ret = actualSize.width();// int(CommonStyleV5Private::dpiScaled(actualSize.width()));
                iconSize = QSize(ret, ret);
            }
        }

        QStyle::State flags = toolbutton->state & ~QStyle::State_Sunken;

        if (flags & QStyle::State_AutoRaise)
        {
            if (!(flags & QStyle::State_MouseOver) || !(flags & QStyle::State_Enabled))
                flags &= ~QStyle::State_Raised;
        }
        QStyle::State mflags = flags;
        if (toolbutton->state & QStyle::State_Sunken)
        {
            if (toolbutton->activeSubControls & QStyle::SC_ToolButton)
                flags |= QStyle::State_Sunken;
            mflags |= QStyle::State_Sunken;
        }

        StyleOptionBackstageButtonV5 thisOpt = *toolbutton;
        thisOpt.iconSize = iconSize;
        p.proxy()->drawPrimitive((QStyle::PrimitiveElement)CommonStyleV5::PE_RibbonBackstagePanelButton, &thisOpt, painter, widget);

        QStyleOptionToolButton label = *toolbutton;
        label.state = flags;
        p.proxy()->drawControl((QStyle::ControlElement)CommonStyleV5::CE_RibbonBackstageButtonLabel, &label, painter, widget);

        if (thisOpt.toolButtonStyle == Qt::ToolButtonTextUnderIcon ||
            thisOpt.toolButtonStyle == Qt::ToolButtonTextOnly)
        {
            QRect rcText = thisOpt.rect;
            const int indentation = thisOpt.icon.isNull() ? 0 :
                thisOpt.iconSize.height() + (opt->fontMetrics.height() / 2 + 2) / 2;
            rcText.adjust(0, indentation, 0, 0);
            rcText.setHeight(thisOpt.rect.height() - indentation);

            thisOpt.palette.setColor(QPalette::Text, thisOpt.palette.color(QPalette::ButtonText));
            thisOpt.document->setTextWidth(static_cast<qreal>(rcText.width()));
            QSize szDoc = thisOpt.document->size().toSize();
            QRect rcTxt = rcText;

            rcTxt.translate(0, (rcText.height() - szDoc.height()) / 2);
            rcTxt.setHeight(rcTxt.height() - (rcText.height() - szDoc.height()) / 2);
            ::qtn_QTextDocument_draw(*painter, thisOpt.document, thisOpt.palette, rcTxt);

            if (thisOpt.features & QStyleOptionToolButton::HasMenu)
            {
                QTextBlock block = thisOpt.document->firstBlock();
                if (block.isValid())
                {
                    const QTextLayout* tl = block.layout();
                    int count = tl ? tl->lineCount() : 0;
                    if (count > 0)
                    {
                        QTextLine tline = tl->lineAt(count - 1);
                        if (tline.isValid())
                        {
                            QRect rectRow = rcTxt;
                            const int minWidth = tl->minimumWidth();
                            rectRow.translate((thisOpt.rect.width() - minWidth) / 2, 0);
                            rectRow.adjust(0, thisOpt.fontMetrics.height() * (count - 1), 0, 0);
                            rectRow.setWidth(minWidth);
                            const int retArrow = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, toolbutton, widget) - 5;
                            if (count == 1)
                                thisOpt.rect = QRect(QPoint(rectRow.center().x() - retArrow / 2,
                                    rectRow.center().y() + szDoc.height() / 2 - retArrow / 2), QSize(retArrow, retArrow));
                            else
                                thisOpt.rect = QRect(QPoint(rectRow.right() + thisOpt.fontMetrics.width('x') - retArrow / 2,
                                    rectRow.center().y() - retArrow / 2), QSize(retArrow, retArrow));
                            p.proxy()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &thisOpt, painter, widget);
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}


/*! \internal */
QRect CommonStyleV5Private::ribbonSubControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    QRect retRect;
    if (!(widget && widget->inherits("Qtitan::RibbonButton")))
        return retRect;

    switch (cc)
    {
        case QStyle::CC_ToolButton:
            if (const QStyleOptionToolButton* tb = qstyleoption_cast<const QStyleOptionToolButton*>(opt))
            {
                int mbi = p.proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator, tb, widget);
                retRect = tb->rect;
                switch (sc)
                {
                    case QStyle::SC_ToolButton:
                        if ((tb->features& (QStyleOptionToolButton::MenuButtonPopup | QStyleOptionToolButton::PopupDelay)) == QStyleOptionToolButton::MenuButtonPopup)
                        {
                            if (tb->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
                            {
                                QRect popupr = p.proxy()->subControlRect(cc, opt, QStyle::SC_ToolButtonMenu, widget);
                                retRect.adjust(0, 0, 0, -popupr.height());
                            }
                            else
                                retRect.adjust(0, 0, -mbi, 0);
                            break;
                        }
                    case QStyle::SC_ToolButtonMenu:
                    {
                        if (widget && !widget->inherits("Qtitan::RibbonGroupPopupButton"))
                        {
                            if (tb->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
                            {
                                int alignment = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextHideMnemonic;
                                QRect rect = opt->rect;
                                rect.setWidth(tb->fontMetrics.width("XXXXXX"));
                                QSize textSize = tb->fontMetrics.boundingRect(rect, alignment, "XXXXXX XXXXXX").size();
                                retRect.adjust(0, opt->rect.bottom() - textSize.height(), 0, 0);
                                retRect.setWidth(qMax(textSize.width(), opt->rect.width()));
                            }
                            else
                                retRect.adjust(retRect.width() - mbi, 0, 0, 0);
                        }
                        break;
                    }
                    default:
                        break;
                }
                retRect = p.visualRect(tb->direction, tb->rect, retRect);
            }
            break;
        default:
            break;
    }
    return retRect;
}

/*! \internal */
QSize CommonStyleV5Private::ribbonSizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& csz, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    QSize sz = csz;
    switch (ct)
    {
        case QStyle::CT_MenuItem:
        {
            bool ourMenu = false;
            if (widget && (!widget->inherits("Qtitan::RibbonSystemPopupBar") && widget->inherits("Qtitan::RibbonPageSystemPopup")))
            {
                if (const QStyleOptionMenuItem* mi = qstyleoption_cast<const QStyleOptionMenuItem*>(opt))
                {
                    if (mi->text.count(QString(_qtn_PopupLable)) > 0)
                    {
                        sz = p.QProxyStyle::sizeFromContents(ct, opt, csz, widget);
                        sz.setWidth(1);
                        break;
                    }
                }
                ourMenu = true;
            }
            if (ourMenu)
            {
                if (const QStyleOptionMenuItem* mi = qstyleoption_cast<const QStyleOptionMenuItem*>(opt))
                {
                    int w = sz.width();
                    sz = p.QProxyStyle::sizeFromContents(ct, opt, csz, widget);
                    if (mi->menuItemType == QStyleOptionMenuItem::Separator)
                    {
                        sz = QSize(10, windowsSepHeight);
                    }
                    else if (mi->icon.isNull())
                    {
                        sz.setHeight(qMax(sz.height() - 2, p.proxy()->pixelMetric(QStyle::PM_LargeIconSize, opt, widget)));
                        w -= 6;
                    }

                    if (mi->menuItemType != QStyleOptionMenuItem::Separator && !mi->icon.isNull())
                    {
                        int iconExtent = p.proxy()->pixelMetric(QStyle::PM_LargeIconSize, opt, widget);
                        sz.setHeight(qMax(sz.height(), iconExtent + 2 * windowsItemFrame));
                    }
                    int maxpmw = mi->maxIconWidth;
                    int tabSpacing = 20;//use2000style ? 20 : windowsTabSpacing;
                    if (mi->text.contains(QLatin1Char('\t')))
                        w += tabSpacing;
                    else if (mi->menuItemType == QStyleOptionMenuItem::SubMenu)
                        w += 2 * windowsArrowHMargin;
                    else if (mi->menuItemType == QStyleOptionMenuItem::DefaultItem)
                    {
                        // adjust the font and add the difference in size.
                        // it would be better if the font could be adjusted in the initStyleOption qmenu func!!
                        QFontMetrics fm(mi->font);
                        QFont fontBold = mi->font;
                        fontBold.setBold(true);
                        QFontMetrics fmBold(fontBold);
                        w += fmBold.width(mi->text) - fm.width(mi->text);
                    }
                    // Windows always shows a check column
                    int checkcol = qMax(maxpmw, windowsCheckMarkWidth);
                    w += checkcol;
                    w += windowsRightBorder + 12;
                    sz.setWidth(w);

                    if (mi->menuItemType != QStyleOptionMenuItem::Separator && (widget && widget->inherits("Qtitan::RibbonPageSystemPopup")))
                    {
                        QFontMetrics fm(mi->font);
                        sz.setHeight(qMax(sz.height(), fm.height() * 3 + fm.height() / 2 + windowsItemVMargin * 2));
                    }
                }
            }
            else if (widget && widget->inherits("Qtitan::OfficePopupMenu"))
            {
                if (const QStyleOptionMenuItem* mi = qstyleoption_cast<const QStyleOptionMenuItem *>(opt))
                {
                    sz = p.QProxyStyle::sizeFromContents(ct, opt, csz, widget);
                    if (mi->menuItemType == QStyleOptionMenuItem::Separator)
                        sz.setHeight(csz.height());
                }
            }
            else
                sz = p.QProxyStyle::sizeFromContents(ct, opt, csz, widget);
        }
        break;
        default:
            sz = QSize();
            break;
    };
    return sz;
}

/*! \internal */
QRect CommonStyleV5Private::ribbonSubElementRect(QStyle::SubElement se, const QStyleOption* option, const QWidget* widget) const
{
    QTN_P(const CommonStyleV5);
    QRect retRect;
    switch ((CommonStyleV5::QtnSubElement)se)
    {
        case CommonStyleV5::SE_RibbonSysHeaderLabelPopupList:
        {
            QFontMetrics fm = option->fontMetrics;
            QRect rcTextMetrics = fm.boundingRect("XXX");
            const int vMarginCaption = p.proxy()->pixelMetric((QStyle::PixelMetric)CommonStyleV5::PM_RibbonVMarginCaptionSysMenu, option, widget);
            retRect = QRect(option->rect.left(), option->rect.top(), option->rect.right(), (option->rect.top() + rcTextMetrics.height() + vMarginCaption * 2));
        }
        break;
        default:
            break;
    }
    return retRect;
}

/*! \internal */
int CommonStyleV5Private::ribbonPixelMetric(QStyle::PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    int val = CommonStyleV5Private::InvalidMetric;
    switch (metric)
    {
        case QStyle::PM_MenuPanelWidth:
            if (widget && (widget->inherits("Qtitan::RibbonSystemPopupBar") || widget->inherits("Qtitan::RibbonPageSystemPopup")))
                val = 0;
            else if ((widget && widget->inherits("Qtitan::OfficePopupMenu")) || (QApplication::activePopupWidget() && QApplication::activePopupWidget()->inherits("Qtitan::OfficePopupMenu")))
                val = 1;
            break;
        case QStyle::PM_MenuHMargin:
            if (widget && (widget->inherits("Qtitan::RibbonSystemPopupBar") || widget->inherits("Qtitan::RibbonPageSystemPopup")))
                val = 0;
            break;
        case QStyle::PM_MenuVMargin:
            if (widget && (widget->inherits("Qtitan::RibbonSystemPopupBar") || widget->inherits("Qtitan::RibbonPageSystemPopup")))
                val = 0;
            break;
        case QStyle::PM_ToolBarIconSize:
            if (widget && (widget->inherits("Qtitan::RibbonSystemPopupBar") || widget->inherits("Qtitan::RibbonPageSystemPopup")))
                val = 32;
            break;
        case QStyle::PM_SmallIconSize:
            if (widget && (widget->inherits("Qtitan::RibbonSystemPopupBar") || widget->inherits("Qtitan::RibbonPageSystemPopup")))
                val = 32;
            break;
        case QStyle::PM_ScrollBarExtent:
            if (widget && widget->inherits("Qtitan::RibbonGallery"))
                val = int(CommonStyleV5Private::dpiScaled(14.));
            break;
        default:
            break;
    };

    if (val == CommonStyleV5Private::InvalidMetric)
    {
        switch ((CommonStyleV5::QtnPixelMetric)metric)
        {
            case CommonStyleV5::PM_RibbonItemHeight:
                val = m_ribbonItemHeight;
                break;
            case CommonStyleV5::PM_RibbonPopupMenuGripHeight:
                val = 12;
                break;
            case CommonStyleV5::PM_RibbonVMarginCaptionSysMenu:
                val = CommonStyleV5Private::dpiScaled(3.0);
                break;
            case CommonStyleV5::PM_RibbonHMarginCaptionSysMenu:
                val = CommonStyleV5Private::dpiScaled(6.0);
                break;
            case CommonStyleV5::PM_RibbonHeightCaptionGroup:
                if (const StyleRibbonOptionGroupV5* optGroup = qstyleoption_cast<const StyleRibbonOptionGroupV5*>(option))
                {
                    const bool titleGroupsVisible = widget ? widget->property(_qtn_TitleGroupsVisible).toBool() : false;
                    val = titleGroupsVisible ? optGroup->fontMetrics.height() + (optGroup->fontMetrics.descent() + optGroup->fontMetrics.ascent()) / 3 : 0;
                }
                break;
            default:
                break;
        };
    }
    return val;
}

// pixelMetric
/*! \internal */
int CommonStyleV5Private::ribbonStyleHint(QStyle::StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const
{
    QTN_P(const CommonStyleV5);
    int ret = 0;
    switch ((CommonStyleV5::QtnStyleHint)stylehint)
    {
        case CommonStyleV5::SH_RecentFileListBackground:
            return opt ? static_cast<int>(darkBackground(opt->palette).rgb()) : 0;
        case CommonStyleV5::SH_RibbonBackstageHideTabs:
            return false;
        case CommonStyleV5::SH_RibbonSysFrameMenu:
            if (StyleHintReturnSysFrameMenuV5* frameMenu = qstyleoption_cast<StyleHintReturnSysFrameMenuV5*>(returnData))
            {
                const int sysPopupBorderTop = p.proxy()->pixelMetric((QStyle::PixelMetric)CommonStyleV5::PM_RibbonItemHeight, opt, widget);
                frameMenu->margins = QMargins(systemPopupBorderLeft, sysPopupBorderTop + 2/*systemPopupBorderTop*/, systemPopupBorderRight, systemPopupBorderBottom);
                return 1;
            }
        default:
            break;
    }
    return ret;
}

// standardPixmap
/*! \internal */
QPixmap CommonStyleV5Private::ribbonStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const
{
    Q_UNUSED(widget);
    QPixmap px;
    switch ((CommonStyleV5::QtnStandardPixmap)sp)
    {
        case CommonStyleV5::SP_RibbonButtonPopupMenu:
            px = (opt->state & QStyle::State_Enabled) ? m_hashNormalPixmap[iconNameRibbonButtonPopupMenu] : m_hashDisabledPixmap[iconNameRibbonButtonPopupMenu];
            break;
        case CommonStyleV5::SP_RibbonButtonGroupOption:
            px = (opt->state & QStyle::State_Enabled) ? m_hashNormalPixmap[iconNameGroupOption] : m_hashDisabledPixmap[iconNameGroupOption];
            break;
        case CommonStyleV5::SP_RibbonShadedArrowUp:
            px = (opt->state & QStyle::State_Enabled) ? m_hashNormalPixmap[iconNameShadedArrowUp] : m_hashDisabledPixmap[iconNameShadedArrowUp];
            break;
        case CommonStyleV5::SP_RibbonShadedArrowDown:
            px = (opt->state & QStyle::State_Enabled) ? m_hashNormalPixmap[iconNameShadedArrowDown] : m_hashDisabledPixmap[iconNameShadedArrowDown];
            break;
        default:
            break;
    };
    return px;
}

/*! \internal */
bool CommonStyleV5Private::drawDockAutoHideFrameWindow(const QStyleOption* option, QPainter* painter, const QWidget* w) const
{
    Q_UNUSED(w);
    if (const AutoHideWindowStyleOptionV5* dwOpt = qstyleoption_cast<const AutoHideWindowStyleOptionV5*>(option))
    {
        QRect rc = option->rect;
        painter->fillRect(option->rect, dwOpt->palette.background());

        switch (dwOpt->position)
        {
            case QInternal::LeftDock:
                QtnPrivate::draw3dRect(*painter, rc.right() - 1, rc.top(), 1, rc.height(), dwOpt->palette.shadow().color(), dwOpt->palette.shadow().color());
                QtnPrivate::draw3dRect(*painter, rc.right() - 2, rc.top(), 1, rc.height(), dwOpt->palette.dark().color(), dwOpt->palette.dark().color());
                break;
            case QInternal::TopDock:
                QtnPrivate::draw3dRect(*painter, rc.left(), rc.bottom() - 1, rc.width(), 1, dwOpt->palette.shadow().color(), dwOpt->palette.shadow().color());
                QtnPrivate::draw3dRect(*painter, rc.left(), rc.bottom() - 2, rc.width(), 1, dwOpt->palette.dark().color(), dwOpt->palette.dark().color());
                break;
            case QInternal::RightDock:
                QtnPrivate::draw3dRect(*painter, rc.left() + 1, rc.top(), 1, rc.height(), dwOpt->palette.light().color(), dwOpt->palette.light().color());
                break;
            case QInternal::BottomDock:
                QtnPrivate::draw3dRect(*painter, rc.left(), rc.top() + 1, rc.width(), 1, dwOpt->palette.light().color(), dwOpt->palette.light().color());
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

/*! \internal */
QPixmap CommonStyleV5Private::dockStandardPixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const
{
    Q_UNUSED(widget);
    QPixmap px;
    switch ((CommonStyleV5::QtnStandardPixmap)sp)
    {
        case CommonStyleV5::SP_TitleBarPinButton:
            px = m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        case CommonStyleV5::SP_TitleBarPinnedButton:
            px = m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        default:
            break;
    };
    return px;
}

/*! \internal */
void CommonStyleV5Private::drawIcon(const QPixmap& px, QPainter* painter, const QRect& rect) const
{
    QTN_P(const CommonStyleV5);
    if (p.property("DPIAware").toBool())
    {
        QRect rectImage = rect;
        rectImage.setWidth(static_cast<int>(CommonStyleV5Private::dpiScaled(static_cast<qreal>(rectImage.width()))));
        rectImage.setHeight(static_cast<int>(CommonStyleV5Private::dpiScaled(static_cast<qreal>(rectImage.height()))));
        //        p->setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter->drawPixmap(rectImage, px, rectImage);
    }
    else
        painter->drawPixmap(rect.topLeft(), px);
}

/*! \internal */
void CommonStyleV5Private::drawHighlightButton(const QStyleOption* option, QPainter* painter) const
{
    const bool highlighted = option->state & QStyle::State_MouseOver;
    const bool pressed = option->state & QStyle::State_Sunken;
    const bool enabled = option->state & QStyle::State_Enabled;
    if (enabled && (highlighted || pressed))
    {
        QPen savePen = painter->pen();
        painter->setPen(buttonHighlightColor(option->palette, pressed/*highlightDn*/, true));
        QBrush fill = buttonHighlightColor(option->palette, pressed/*highlightDn*/, false);
        painter->fillRect(option->rect.adjusted(1, 0, -2, 0), fill);
        painter->drawRect(option->rect.adjusted(1, 0, -2, -1));
        painter->setPen(savePen);
    }
}

/*! \internal */
void CommonStyleV5Private::drawHighlightItemGallery(const QStyleOption* option, QPainter* painter) const
{
    const bool highlighted = option->state & QStyle::State_MouseOver;
    const bool pressed = option->state & QStyle::State_Sunken;
    const bool enabled = option->state & QStyle::State_Enabled;
    if (enabled && (highlighted || pressed))
    {
        QPen savePen = painter->pen();
        painter->setPen(buttonHighlightColor(option->palette, pressed/*highlightDn*/, true));
        QBrush fill = buttonHighlightColor(option->palette, pressed/*highlightDn*/, false);
        painter->fillRect(option->rect.adjusted(1, 0, -2, 0), fill);
        painter->drawRect(option->rect.adjusted(1, 0, -2, -1));
        painter->setPen(savePen);
    }
    else
        painter->fillRect(option->rect.adjusted(1, 0, -2, 0), darkBackgroundLight(option->palette));
}

/*! \internal */
void CommonStyleV5Private::drawButton(const QPixmap& px, const QStyleOption* option, QPainter* painter) const
{
    drawHighlightButton(option, painter);
    QPixmap pixmap = px;
    if (!pixmap.isNull())
    {
        QIcon icon(pixmap);
        QSize szImage = pixmap.size();
        const QRect rect = option->rect;
        pixmap = icon.pixmap(szImage, option->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
        QRect rc(QPoint(rect.width() / 2 - szImage.width() / 2, rect.height() / 2 - szImage.height() / 2), szImage);
        drawIcon(pixmap, painter, rc);
    }
}

/*! \internal */
QSize CommonStyleV5Private::bestImageSize() const
{
    double pdblScale = 1.0;
    QSize sz(9, 9);
    QtnPrivate::qtn_bestImageSize(&sz, &pdblScale);
    return sz;
}

/*! \internal */
void CommonStyleV5Private::calcRibbonItemHeight()
{
    QFont fontRegular = QApplication::font("Qtitan::RibbonBar");
    QFontMetrics fm(fontRegular);
    m_ribbonItemHeight = qMax(fm.height() + ((fm.descent() + fm.ascent()) / 3), 24) + 1;

    QLineEdit ed;
    ed.setAttribute(Qt::WA_MacSmallSize);
    ed.setFont(fontRegular);
    QSize sizeHint = ed.sizeHint();
    m_ribbonItemHeight = qMax(sizeHint.height(), m_ribbonItemHeight);

    QComboBox box1;
    box1.setAttribute(Qt::WA_MacSmallSize);
    box1.setFont(fontRegular);
    sizeHint = box1.sizeHint();
    m_ribbonItemHeight = qMax(sizeHint.height(), m_ribbonItemHeight);

    QComboBox box2;
    box2.setAttribute(Qt::WA_MacSmallSize);
    box2.setEditable(true); //Affects on MacOSX widget
    box2.setFont(fontRegular);
    sizeHint = box2.sizeHint();
    m_ribbonItemHeight = qMax(sizeHint.height(), m_ribbonItemHeight);

    QSpinBox box3;
    box3.setAttribute(Qt::WA_MacSmallSize);
    box3.setFont(fontRegular);
    sizeHint = box3.sizeHint();
    m_ribbonItemHeight = qMax(sizeHint.height(), m_ribbonItemHeight);
}

/*! \internal */
QColor CommonStyleV5Private::darkBackground(const QPalette& pal) const
{
    return pal.background().color();
}

/*! \internal */
QColor CommonStyleV5Private::darkBackgroundLight(const QPalette& pal) const
{
    return pal.color(QPalette::Base).light(99);
}

/*! \internal */
QColor CommonStyleV5Private::outlineMidlight(const QPalette& pal) const
{
    return pal.brush(QPalette::Midlight).color().dark(105);
}

/*! \internal */
QColor CommonStyleV5Private::outlineLight(const QPalette& pal) const
{
    return pal.brush(QPalette::Light).color();
}

/*! \internal */
QColor CommonStyleV5Private::buttonHighlightColor(const QPalette& pal, bool highlightDn, bool border) const
{
    if (border)
        return highlightDn ? pal.brush(QPalette::Highlight).color().light(135) : pal.brush(QPalette::Highlight).color().light(155);
    return highlightDn ? pal.brush(QPalette::Highlight).color().light(155) : pal.brush(QPalette::Highlight).color().light(170);
}

/*! \internal */
QColor CommonStyleV5Private::titleBarWindowColor(const QPalette& pal, bool isActive) const
{
    return pal.color(isActive ? QPalette::Active : QPalette::Inactive, QPalette::Window);
}

/*! \internal */
QColor CommonStyleV5Private::titleBarFrameColor(const QPalette& pal, bool isActive) const
{
    return pal.color(isActive ? QPalette::Active : QPalette::Inactive, QPalette::Shadow);
}



/* 
\class CommonStyleV5
\inmodule QtitanRibbon
*/

/*!
Constuctor of CommonStyleV5 class. You can't use this class directly. 
\sa OfficeStyle, RibbonStyle
*/
CommonStyleV5::CommonStyleV5(QStyle* style)
    : QProxyStyle(style)
{
    QTN_INIT_PRIVATE(CommonStyleV5);
    QTN_D(CommonStyleV5);
    d.initialization();
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
CommonStyleV5::CommonStyleV5(const QString& key)
    : QProxyStyle(key)
{
    QTN_INIT_PRIVATE(CommonStyleV5);
    QTN_D(CommonStyleV5);
    d.initialization();
}
#endif

CommonStyleV5::CommonStyleV5(CommonStyleV5Private& d)
{
    qtn_d_ptr = &d; 
    qtn_d_ptr->setPublic(this);
    d.initialization();
}

CommonStyleV5::~CommonStyleV5()
{
    QTN_FINI_PRIVATE();
}

void CommonStyleV5::createCommonProxyStyle()
{
    QStyle* style = QApplication::style();
    if (style && !qobject_cast<const CommonStyleV5*>(style))
    {
        CommonStyleV5* proxyStyle = new CommonStyleV5(style);
        proxyStyle->setObjectName(style->objectName());
        QApplication::setStyle(proxyStyle);
    }
}

/*! \reimp */
QPalette CommonStyleV5::standardPalette() const
{
    QTN_D(const CommonStyleV5);
    QPalette palette = QProxyStyle::standardPalette();
    // QPalette::Active
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Button, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::WindowText, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Background, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Window, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Base, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::AlternateBase, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Text, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::BrightText, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::ButtonText, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Light, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Midlight, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Dark, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Mid, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Shadow, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::Highlight, palette);
    d.setPaletteFromGlobal(QPalette::Active, QPalette::HighlightedText, palette);
    // QPalette::Inactive
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Button, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::WindowText, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Background, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Window, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Base, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::AlternateBase, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Text, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::BrightText, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::ButtonText, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Light, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Midlight, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Dark, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Mid, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Shadow, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::Highlight, palette);
    d.setPaletteFromGlobal(QPalette::Inactive, QPalette::HighlightedText, palette);
    // QPalette::Disabled
    d.setPaletteFromGlobal(QPalette::Disabled, QPalette::Button, palette);
    d.setPaletteFromGlobal(QPalette::Disabled, QPalette::WindowText, palette);
    d.setPaletteFromGlobal(QPalette::Disabled, QPalette::Background, palette);
    d.setPaletteFromGlobal(QPalette::Disabled, QPalette::Window, palette);

//    palette.setBrush(QPalette::Inactive, QPalette::Base, Qt::red);

    return palette;
}

void CommonStyleV5::polish(QPalette& palette)
{
    QProxyStyle::polish(palette);
    palette = standardPalette();
}

/*! \reimp */
void CommonStyleV5::polish(QApplication* app)
{
    QProxyStyle::polish(app);
}

/*! \reimp */
void CommonStyleV5::polish(QWidget* widget)
{
    QTN_D(CommonStyleV5);
    QProxyStyle::polish(widget);
    if (widget == Q_NULL)
        return;

    d.m_config.setConfigWidget(widget);

    if (qobject_cast<QMenu*>(widget) && QtnPrivate::qtn_isAncestor(widget, "Qtitan::RibbonBar"))
        widget->setFont(QApplication::font("Qtitan::RibbonBar"));

    if (widget->inherits("Qtitan::RibbonSystemButton"))
    {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::ButtonText, Qt::white);
        widget->setPalette(pal);
    }
    else if (qobject_cast<QMenu*>(widget) && (QtnPrivate::qtn_isAncestor(widget, "Qtitan::RibbonBar") || widget->inherits("Qtitan::OfficePopupMenu")))
    {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::Background, d.darkBackgroundLight(pal));
        pal.setColor(QPalette::Button, d.darkBackgroundLight(pal));
        widget->setPalette(pal);
    }
    else if (QtnPrivate::qtn_isAncestor(widget, "Qtitan::RibbonBar"))
    {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::WindowText, pal.color(QPalette::Dark).dark(240));
        pal.setColor(QPalette::ButtonText, pal.color(QPalette::Dark).dark(240));
        pal.setColor(QPalette::HighlightedText, pal.color(QPalette::Dark).dark(240));
        widget->setPalette(pal);
    }
}

/*! \reimp */
void CommonStyleV5::unpolish(QApplication* app)
{
    QProxyStyle::unpolish(app);
}

/*! \reimp */
void CommonStyleV5::unpolish(QWidget* widget)
{
    QProxyStyle::unpolish(widget);
}

/*! \reimp */
void CommonStyleV5::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    QTN_D(const CommonStyleV5);

    bool draw = false;
    switch ((CommonStyleV5::QtnPrimitiveElement)pe)
    {
        case CommonStyleV5::PE_PanelRibbonBar:        draw = d.drawRibbonBar(opt, p, w); break;
        case CommonStyleV5::PE_RibbonFrameGroups:     draw = d.drawRibbonFrameGroups(opt, p, w); break;
        case CommonStyleV5::PE_RibbonFrameGroup:      draw = d.drawRibbonFrameGroup(opt, p, w); break;
        case CommonStyleV5::PE_RibbonFrameGallery:    draw = d.drawRibbonFrameGallery(opt, p, w); break;
        case CommonStyleV5::PE_RibbonSysFrameMenu:    draw = d.drawRibbonSysFrameMenu(opt, p, w); break;
        case CommonStyleV5::PE_RibbonSysFramePagePopupList: draw = d.drawRibbonSysFramePagePopupList(opt, p, w); break;
        case CommonStyleV5::PE_RibbonSysFramePagePopup:    draw = d.drawRibbonSysFramePagePopup(opt, p, w); break;
        case CommonStyleV5::PE_RibbonPanelButton:          draw = d.drawRibbonPanelButton(opt, p, w); break;
        case CommonStyleV5::PE_RibbonOptionButton:         draw = d.drawRibbonOptionButton(opt, p, w); break;
        case CommonStyleV5::PE_RibbonQuickAccessButton:    draw = d.drawRibbonQuickAccessButton(opt, p, w); break;
        case CommonStyleV5::PE_RibbonBackstageFrameMenu:   draw = d.drawRibbonBackstageFrameMenu(opt, p, w); break;
        case CommonStyleV5::PE_RibbonBackstagePanelButton: draw = d.drawRibbonBackstagePanelButton(opt, p, w); break;
        case CommonStyleV5::PE_DockAutoHideFrameWindow:    draw = d.drawDockAutoHideFrameWindow(opt, p, w); break;
        default: draw = false; break;
    }
    if (!draw)
    {
        switch (pe)
        {
            case PE_FrameWindow:            draw = d.drawFrameWindow(opt, p, w); break;
            case PE_FrameDockWidget:        draw = d.drawFrameDockWidget(opt, p, w); break;
            case PE_PanelMenu:              draw = d.drawPanelMenu(opt, p, w); break;
            case PE_IndicatorArrowUp:
            case PE_IndicatorArrowDown:
            case PE_IndicatorArrowRight:
            case PE_IndicatorArrowLeft:     draw = d.drawIndicatorArrow(pe, opt, p, w); break;
            case PE_FrameGroupBox:          draw = d.drawFrameGroupBox(opt, p, w); break;
            case PE_IndicatorCheckBox:      draw = d.drawIndicatorCheckBox(opt, p, w); break;
            case PE_IndicatorRadioButton:   draw = d.drawIndicatorRadioButton(opt, p, w); break;
            case PE_IndicatorToolBarHandle: draw = d.drawIndicatorToolBarHandle(opt, p, w); break;
            case PE_IndicatorToolBarSeparator: draw = d.drawIndicatorToolBarSeparator(opt, p, w); break;
            case PE_IndicatorHeaderArrow:   draw = d.drawIndicatorHeaderArrow(opt, p, w); break;
            case PE_IndicatorBranch:        draw = d.drawIndicatorBranch(opt, p, w); break;
            case PE_FrameFocusRect:         draw = d.drawFrameFocusRect(opt, p, w); break;
            case PE_PanelButtonTool:        draw = d.drawPanelButtonTool(opt, p, w); break;
            case PE_PanelButtonCommand:     draw = d.drawPanelButtonCommand(opt, p, w); break;
            case PE_FrameLineEdit:          draw = d.drawFrameLineEdit(opt, p, w); break;
            case PE_FrameStatusBarItem:     draw = d.drawFrameStatusBarItem(opt, p, w); break;
            default: draw = false; break;
        }
    }
    if (!draw)
        QProxyStyle::drawPrimitive(pe, opt, p, w);
}

/*! \reimp */
void CommonStyleV5::drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    QTN_D(const CommonStyleV5);
    bool draw = false;
    switch ((CommonStyleV5::QtnControlElement)element)
    {
        case CommonStyleV5::CE_RibbonTabShapeLabel: draw = d.drawRibbonTabShapeLabel(opt, p, w); break;
        case CommonStyleV5::CE_RibbonTab:           draw = d.drawRibbonTab(opt, p, w); break;
        case CommonStyleV5::CE_RibbonTabBar:        draw = d.drawRibbonTabBar(opt, p, w); break;
        case CommonStyleV5::CE_RibbonGroupLabel:    draw = d.drawRibbonGroupLabel(opt, p, w); break;
        case CommonStyleV5::CE_RibbonSysMenuItem:   draw = d.drawRibbonSysMenuItem(opt, p, w); break;
        case CommonStyleV5::CE_RibbonSysRecentFileItem: draw = d.drawRibbonSysRecentFileItem(opt, p, w); break;
        case CommonStyleV5::CE_RibbonButtonLabel:   draw = d.drawRibbonButtonLabel(opt, p, w); break;
        case CommonStyleV5::CE_RibbonGalleryItem:   draw = d.drawRibbonGalleryItem(opt, p, w); break;
        case CommonStyleV5::CE_RibbonBackstageMenuItem: draw = d.drawRibbonBackstageMenuItem(opt, p, w); break;
        case CommonStyleV5::CE_RibbonBackstageButtonLabel: draw = d.drawRibbonBackstageButtonLabel(opt, p, w); break;
        case CommonStyleV5::CE_RibbonGripBar:       draw = d.drawOfficePopupSizeGrip(opt, p, w); break;
        default: draw = false; break;
    }
    if (!draw)
    {
        switch (element)
        {
            case CE_MenuBarItem:        draw = d.drawMenuBarItem(opt, p, w); break;
            case CE_MenuItem:           draw = d.drawMenuItem(opt, p, w); break;
            case CE_ScrollBarSubLine:
            case CE_ScrollBarAddLine:   draw = d.drawScrollBarAddSubLine(element, opt, p, w); break;
            case CE_ScrollBarAddPage:
            case CE_ScrollBarSubPage:   draw = d.drawScrollBarAddSubPage(element, opt, p, w); break;
            case CE_ScrollBarSlider:    draw = d.drawScrollBarSlider(opt, p, w); break;
            case CE_TabBarTabShape:     draw = d.drawTabBarTabShape(opt, p, w); break;
            case CE_HeaderSection:      draw = d.drawHeaderSection(opt, p, w); break;
            case CE_HeaderLabel:        draw = d.drawHeaderLabel(opt, p, w); break;
            case CE_DockWidgetTitle:    draw = d.drawDockWidgetTitle(opt, p, w); break;
            case CE_ToolBoxTabShape:    draw = d.drawToolBoxTabShape(opt, p, w); break;
            case CE_ToolButtonLabel:    draw = d.drawToolButtonLabel(opt, p, w); break;
            default:                    draw = false; break;
        }
    }
    if (!draw)
        QProxyStyle::drawControl(element, opt, p, w);
}

/*! \reimp */
void CommonStyleV5::drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const
{
    QTN_D(const CommonStyleV5);
    bool draw = false;
    switch ((CommonStyleV5::QtnComplexControl)cc)
    {
        case CommonStyleV5::CC_RibbonFileButton:      draw = d.drawRibbonFileButton(opt, p, w); break;
        case CommonStyleV5::CC_RibbonButton:          draw = d.drawRibbonButton(opt, p, w); break;
        case CommonStyleV5::CC_RibbonContextBar:      draw = d.drawRibbonTitleBarWidget(opt, p, w); break;
        case CommonStyleV5::CC_RibbonGalleryButton:   draw = d.drawRibbonGalleryButton(opt, p, w); break;
        case CommonStyleV5::CC_RibbonBackstageButton: draw = d.drawRibbonBackstageButton(opt, p, w); break;
        default:                                    draw = false; break;
    }
    if (!draw)
    {
        switch (cc)
        {
            case CC_TitleBar:    draw = d.drawTitleBar(opt, p, w); break;
            case CC_MdiControls: draw = d.drawMdiControls(opt, p, w); break;
            case CC_ComboBox:    draw = d.drawComboBox(opt, p, w); break;
            case CC_Slider:      draw = d.drawSlider(opt, p, w); break;
            case CC_SpinBox:     draw = d.drawSpinBox(opt, p, w); break;
            case CC_ToolButton:  draw = d.drawToolButton(opt, p, w); break;
            default:             draw = false; break;
        }
    }
    if (!draw)
        QProxyStyle::drawComplexControl(cc, opt, p, w);
}

/*! \reimp */
QRect CommonStyleV5::itemTextRect(const QFontMetrics& fm, const QRect& r, int flags, bool enabled, const QString& text) const
{
    return QProxyStyle::itemTextRect(fm, r, flags, enabled, text);
}

/*! \reimp */
QRect CommonStyleV5::itemPixmapRect(const QRect& r, int flags, const QPixmap& pixmap) const
{
    return QProxyStyle::itemPixmapRect(r, flags, pixmap);
}

/*! \reimp */
QRect CommonStyleV5::subElementRect(SubElement se, const QStyleOption* option, const QWidget* widget) const
{
    QTN_D(const CommonStyleV5);
    QRect retRect = d.ribbonSubElementRect(se, option, widget);
/*
    switch (se)
    {
        case SE_DockWidgetCloseButton:
            break;
        default: break;
    }
*/
    if (retRect.isEmpty())
        retRect = QProxyStyle::subElementRect(se, option, widget);
    return retRect;
}

/*! \reimp */
QStyle::SubControl CommonStyleV5::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, const QPoint& pt, const QWidget* w) const
{
    return QProxyStyle::hitTestComplexControl(cc, opt, pt, w);
}

/*! \reimp */
QRect CommonStyleV5::subControlRect(ComplexControl control, const QStyleOptionComplex* option, SubControl subControl, const QWidget* widget) const
{
    QTN_D(const CommonStyleV5);
    bool done = false;
    QRect rect = d.ribbonSubControlRect(control, option, subControl, widget); 
    done = !rect.isEmpty();
    if (!done)
    {
        switch (control)
        {
            case CC_TitleBar:
                if (!QtnPrivate::qtn_isExistWindowTitleBar(widget))
                    break;
                if (const QStyleOptionTitleBar* tb = qstyleoption_cast<const QStyleOptionTitleBar *>(option))
                {
                    if (!CommonStyleV5Private::buttonVisible(subControl, tb))
                        return rect;
                    const bool isToolTitle = false;
                    const int frameWidth = proxy()->pixelMetric(PM_MdiSubWindowFrameWidth, option, widget);
                    const int height = tb->rect.height() - frameWidth;
                    const int width = tb->rect.width();
                    int buttonHeight = height;
                    int buttonWidth = height + (height * 3 / 4);
                    const int delta = buttonWidth;
                    const bool sysmenuHint = (tb->titleBarFlags & Qt::WindowSystemMenuHint) != 0;
                    const bool minimizeHint = (tb->titleBarFlags & Qt::WindowMinimizeButtonHint) != 0;
                    const bool maximizeHint = (tb->titleBarFlags & Qt::WindowMaximizeButtonHint) != 0;
                    const bool contextHint = (tb->titleBarFlags & Qt::WindowContextHelpButtonHint) != 0;
                    const bool shadeHint = (tb->titleBarFlags & Qt::WindowShadeButtonHint) != 0;
                    bool isMinimized = tb->titleBarState & Qt::WindowMinimized;
                    bool isMaximized = tb->titleBarState & Qt::WindowMaximized;
                    int offset = 0;

                    switch (subControl) 
                    {
                        case SC_TitleBarLabel:
                            rect = QRect(frameWidth, 0, width - (buttonWidth + frameWidth + 10), height);
                            if (isToolTitle) {
                                if (sysmenuHint) {
                                    rect.adjust(0, 0, -buttonWidth - 3, 0);
                                }
                                if (minimizeHint || maximizeHint)
                                    rect.adjust(0, 0, -buttonWidth - 2, 0);
                            }
                            else {
                                if (sysmenuHint) 
                                {
                                    const int leftOffset = height - 8;
                                    rect.adjust(leftOffset, 0, 0, 0);
                                }
                                if (minimizeHint)
                                    rect.adjust(0, 0, -buttonWidth - 2, 0);
                                if (maximizeHint)
                                    rect.adjust(0, 0, -buttonWidth - 2, 0);
                                if (contextHint)
                                    rect.adjust(0, 0, -buttonWidth - 2, 0);
                                if (shadeHint)
                                    rect.adjust(0, 0, -buttonWidth - 2, 0);
                            }
                            break;

                        case SC_TitleBarContextHelpButton:
                            if (tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
                                offset += delta;
                            //fall through
                        case SC_TitleBarMinButton:
                            if (!isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
                                offset += delta;
                            else if (subControl == SC_TitleBarMinButton)
                                break;
                            //fall through
                        case SC_TitleBarNormalButton:
                            if (isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
                                offset += delta;
                            else if (isMaximized && (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
                                offset += delta;
                            else if (subControl == SC_TitleBarNormalButton)
                                break;
                            //fall through
                        case SC_TitleBarMaxButton:
                            if (!isMaximized && (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
                                offset += delta;
                            else if (subControl == SC_TitleBarMaxButton)
                                break;
                            //fall through
                        case SC_TitleBarShadeButton:
                            if (!isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
                                offset += delta;
                            else if (subControl == SC_TitleBarShadeButton)
                                break;
                            //fall through
                        case SC_TitleBarUnshadeButton:
                            if (isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
                                offset += delta;
                            else if (subControl == SC_TitleBarUnshadeButton)
                                break;
                            //fall through
                        case SC_TitleBarCloseButton:
                            if (tb->titleBarFlags & Qt::WindowSystemMenuHint)
                                offset += delta;
                            else if (subControl == SC_TitleBarCloseButton)
                                break;
                            rect.setRect(width - offset - frameWidth, frameWidth, buttonWidth, buttonHeight);
                            break;

                        case SC_TitleBarSysMenu:
                        {
                            const int controlTop = 6;
                            const int controlHeight = height - controlTop - 3;
                            const int iconExtent = proxy()->pixelMetric(PM_SmallIconSize);
                            QSize iconSize = tb->icon.actualSize(QSize(iconExtent, iconExtent));
                            if (tb->icon.isNull())
                                iconSize = QSize(controlHeight, controlHeight);
                            int hPad = (controlHeight - iconSize.height()) / 2;
                            int vPad = (controlHeight - iconSize.width()) / 2;
                            rect = QRect(frameWidth + hPad, controlTop + vPad, iconSize.width(), iconSize.height());
                        }
                        break;
                        default:
                            break;
                    }
                    done = true;
                }
                break;
            default:
                break;
        }
    }
    if (!done)
        rect = QProxyStyle::subControlRect(control, option, subControl, widget);
    return rect;
}

/*! \reimp */
QSize CommonStyleV5::sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const
{
    QTN_D(const CommonStyleV5);
    QSize sz = d.ribbonSizeFromContents(ct, opt, contentsSize, widget);
    if (sz.isEmpty())
    {
        sz = contentsSize;
        switch (ct)
        {
            case CT_MenuBarItem:
                if (!sz.isEmpty())
                    sz += QSize(windowsItemHMargin * 5 + 1, 5);
                return sz;
                break;
            default:
                sz = QSize();
                break;
        }
    }
    if (sz.isEmpty())
        return QProxyStyle::sizeFromContents(ct, opt, contentsSize, widget);
    return sz;
}

/*! \reimp */
int CommonStyleV5::pixelMetric(PixelMetric metric, const QStyleOption* opt, const QWidget* widget) const
{
    QTN_D(const CommonStyleV5);
    int val = CommonStyleV5Private::pixelMetricFromSystemDp(metric, opt, widget);
    if (val != CommonStyleV5Private::InvalidMetric)
        return val;

    val = d.ribbonPixelMetric(metric, opt, widget);
    if (val != CommonStyleV5Private::InvalidMetric)
        return val;

    int defaultValue = QProxyStyle::pixelMetric(metric, opt, widget);

    bool ok = false;
    bool scalableDpi = true;
    switch (metric) 
    {
        case PM_SliderTickmarkOffset:
            val = d.m_config.pixelMetric("QSlider", "SliderTickmarkOffset", defaultValue, scalableDpi, &ok);
            break;
        case PM_ButtonShiftHorizontal:
            val = d.m_config.pixelMetric("QToolButton", "ButtonShiftHorizontal", defaultValue, scalableDpi, &ok);
            break;
        case PM_ButtonShiftVertical:
            val = d.m_config.pixelMetric("QToolButton", "ButtonShiftVertical", defaultValue, scalableDpi, &ok);
            break;
        case PM_SliderThickness:
            val = d.m_config.pixelMetric("QSlider", "SliderThickness", defaultValue, scalableDpi, &ok);
            break;
        case PM_SliderLength:
            val = d.m_config.pixelMetric("QSlider", "SliderLength", defaultValue, scalableDpi, &ok);
            break;
        case PM_DockWidgetTitleMargin:
            val = d.m_config.pixelMetric("QDockWidget", "DockWidgetTitleMargin", 1, scalableDpi, &ok);
            ok = true;
            break;
        case PM_MenuPanelWidth:
            val = d.m_config.pixelMetric("QMenu", "MenuPanelWidth", defaultValue, scalableDpi, &ok);
            break;
        case PM_MenuBarItemSpacing:
            val = d.m_config.pixelMetric("QMenuBar", "MenuBarItemSpacing", defaultValue, scalableDpi, &ok);
            break;
        case PM_MenuBarVMargin:
            val = d.m_config.pixelMetric("QMenuBar", "MenuBarVMargin", defaultValue, scalableDpi, &ok);
            break;
        case PM_MenuBarHMargin:
            val = d.m_config.pixelMetric("QMenuBar", "MenuBarHMargin", defaultValue, scalableDpi, &ok);
            break;
        case PM_MenuBarPanelWidth:
            val = d.m_config.pixelMetric("QMenuBar", "MenuBarPanelWidth", defaultValue, scalableDpi, &ok);
            break;
        case PM_ToolBarHandleExtent:
            val = d.m_config.pixelMetric("QToolBar", "ToolBarHandleExtent", defaultValue, scalableDpi, &ok);
            break;
        case PM_ToolBarItemSpacing:
            val = d.m_config.pixelMetric("QToolBar", "ToolBarItemMargin", defaultValue, scalableDpi, &ok);
            break;
        case PM_ToolBarFrameWidth:
            val = d.m_config.pixelMetric("QToolBar", "ToolBarFrameWidth", defaultValue, scalableDpi, &ok);
            break;
        case PM_ToolBarItemMargin:
            val = d.m_config.pixelMetric("QToolBar", "ToolBarItemMargin", defaultValue, scalableDpi, &ok);
            break;
        case PM_DockWidgetTitleBarButtonMargin:
            val = d.m_config.pixelMetric("QDockWidget", "DockWidgetTitleBarButtonMargin", 5, scalableDpi, &ok);
            ok = true;
            break;
        case PM_ToolBarIconSize:
            val = d.m_config.pixelMetric("QToolBar", "ToolBarIconSize", defaultValue, scalableDpi, &ok);
            break;
        case PM_SubMenuOverlap:
            val = d.m_config.pixelMetric("QMenu", "SubMenuOverlap", defaultValue, scalableDpi, &ok);
//            val = -1;
            break;
        case PM_MdiSubWindowFrameWidth: // PM_MDIFrameWidth
            {
                if (widget && widget->inherits("QMdiSubWindow"))
                {
                    val = d.m_config.pixelMetric("QMdiSubWindow", "MdiSubWindowFrameWidth", defaultValue, scalableDpi, &ok);
                }
                else
                {
                    if (QtnPrivate::qtn_isExistWindowTitleBar(widget))
                    {
#ifdef Q_OS_MAC
                        const int defaultWidth = 0;
#else
                        const int defaultWidth = 1;
#endif
                        val = d.m_config.pixelMetric("QMainWindow", "MdiSubWindowFrameWidth", defaultWidth, scalableDpi, &ok);
                    }
                    else
                        val = d.m_config.pixelMetric("QMainWindow", "MdiSubWindowFrameWidth", defaultValue, scalableDpi, &ok);
                }
            }
            break;
        default:
            val = defaultValue;
            break;
    }

    if (val != CommonStyleV5Private::InvalidMetric && ok)
        return scalableDpi ? CommonStyleV5Private::dpiScaled(val) : val;
    return val;
}

/*! \reimp */
int CommonStyleV5::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    QTN_D(const CommonStyleV5);
    if (static_cast<QtnStyleHint>(hint) >= CommonStyleV5::SH_RibbonBegin && static_cast<QtnStyleHint>(hint) <= CommonStyleV5::SH_RibbonEnd)
        return d.ribbonStyleHint(hint, option, widget, returnData);

    int defaultValue = QProxyStyle::styleHint(hint, option, widget, returnData);
    int val = CommonStyleV5Private::InvalidMetric;
    switch (hint) 
    {
        case SH_DitherDisabledText:
            val = d.m_config.styleHint("", "DitherDisabledText", defaultValue);
            break;
        case SH_UnderlineShortcut:
            val = d.m_config.styleHint("", "UnderlineShortcut", defaultValue);
            break;
        case SH_Slider_SnapToValue:
        case SH_PrintDialog_RightAlignButtons:
        case SH_FontDialog_SelectAssociatedText:
        case SH_MenuBar_AltKeyNavigation:
        case SH_ComboBox_ListMouseTracking:
        case SH_ScrollBar_StopMouseOverSlider:
        case SH_ScrollBar_MiddleClickAbsolutePosition:
        case SH_EtchDisabledText:
            val = d.m_config.styleHint("", "EtchDisabledText", defaultValue);
            break;
        case SH_MenuBar_MouseTracking:
            val = d.m_config.styleHint("QMenuBar", "MenuBar_MouseTracking", defaultValue);
            break;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        case SH_Menu_SupportsSections:
            val = d.m_config.styleHint("QMenu", "Menu_SupportsSections", defaultValue);
            break;
#endif
        case SH_Menu_MouseTracking:
            val = d.m_config.styleHint("QMenu", "Menu_MouseTracking", defaultValue);
#if defined(Q_OS_IOS)
        case SH_ComboBox_UseNativePopup:
            val = 1;
            break;
#endif
        default:
            val = defaultValue;
        break;
    }
    return val;
}

/*! \reimp */
QIcon CommonStyleV5::standardIcon(StandardPixmap standardIcon, const QStyleOption* opt, const QWidget* widget) const
{
    QPixmap px = standardPixmap(standardIcon, opt, widget);
    if (px.isNull())
        return QProxyStyle::standardIcon(standardIcon, opt, widget);
    return px;
}

/*! \reimp */
QPixmap CommonStyleV5::standardPixmap(StandardPixmap sp, const QStyleOption* opt, const QWidget* widget) const
{
    QTN_D(const CommonStyleV5);
    QPixmap px = d.ribbonStandardPixmap(sp, opt, widget);
    if (!px.isNull())
        return px;

    px = d.dockStandardPixmap(sp, opt, widget);
    if (!px.isNull())
        return px;

    switch (sp)
    {
        case SP_TitleBarCloseButton:
#ifdef QTN_MAC_BUTTONS
            if (const QStyleOptionTitleBar *titlebar = qstyleoption_cast<const QStyleOptionTitleBar *>(opt))
                px = qtn_titlebar_standard_pixmap(QStyle::SP_TitleBarCloseButton, titlebar, widget);
            else
#endif
                px = d.m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        case SP_TitleBarMinButton:
#ifdef QTN_MAC_BUTTONS
            if (const QStyleOptionTitleBar *titlebar = qstyleoption_cast<const QStyleOptionTitleBar *>(opt))
                px = qtn_titlebar_standard_pixmap(QStyle::SP_TitleBarMinButton, titlebar, widget);
            else
#endif
                px = d.m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        case SP_TitleBarMaxButton:
#ifdef QTN_MAC_BUTTONS
            if (const QStyleOptionTitleBar *titlebar = qstyleoption_cast<const QStyleOptionTitleBar *>(opt))
                px = qtn_titlebar_standard_pixmap(QStyle::SP_TitleBarMaxButton, titlebar, widget);
            else
#endif
                px = d.m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        case SP_TitleBarNormalButton:
#ifdef QTN_MAC_BUTTONS
            if (const QStyleOptionTitleBar *titlebar = qstyleoption_cast<const QStyleOptionTitleBar *>(opt))
                px = qtn_titlebar_standard_pixmap(QStyle::SP_TitleBarNormalButton, titlebar, widget);
            else
#endif
                px = d.m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        case SP_ArrowUp:
            px = d.standardPixmap("Image_ArowUpMenu", (opt && !(opt->state & QStyle::State_Enabled)) ? CommonStyleV5Private::Gray : CommonStyleV5Private::Black);
            break;
        case SP_ArrowDown:
            px = d.m_standardSymbol.standardSymbolPixmap(sp, opt);
            break;
        default:
            break;
    };

    if (px.isNull())
        px = QProxyStyle::standardPixmap(sp, opt, widget);
    return px;
}

/*! \reimp */
QPixmap CommonStyleV5::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap& pixmap, const QStyleOption* opt) const
{
    return QProxyStyle::generatedIconPixmap(iconMode, pixmap, opt);
}

/*! \reimp */
int CommonStyleV5::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, 
    Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::layoutSpacing(control1, control2, orientation, option, widget);
}

void CommonStyleV5::setStyleConfig(const QString& fileName)
{
    QTN_D(CommonStyleV5);
    d.m_config.load(fileName);
    d.updateStyle();
}

QColor CommonStyleV5::qtn_getColorToRGB(CommonStyleV5::ContextColor color)
{
    switch (color)
    {
    case CommonStyleV5::ContextColorRed:
        return QColor(255, 160, 160);

    case CommonStyleV5::ContextColorOrange:
        return QColor(239, 189, 55);

    case CommonStyleV5::ContextColorYellow:
        return QColor(253, 229, 27);

    case CommonStyleV5::ContextColorGreen:
        return QColor(113, 190, 89);

    case CommonStyleV5::ContextColorBlue:
        return QColor(128, 181, 196);

    case CommonStyleV5::ContextColorCyan:
        return QColor(114, 163, 224);

    case CommonStyleV5::ContextColorPurple:
        return QColor(214, 178, 209);
    default:
        break;
    }
    return QColor();
}

QPixmap CommonStyleV5::qtn_getSymbolToPixmap(IconSymbol symbol, const QSize& sizeIcon, const QColor& textColor)
{
    if (symbol == Symbol_None)
        return  QPixmap();

    QChar code = knownIconSymbols[symbol].code1;
    QSysInfo::WinVersion winVersion = QSysInfo::windowsVersion();
    if (winVersion >= QSysInfo::WV_WINDOWS10)
        code = knownIconSymbols[symbol].code2;

    QPixmap symbolPixmap(sizeIcon.width(), sizeIcon.height());
    symbolPixmap.fill(Qt::transparent);

    QPainter p(&symbolPixmap);
    p.setPen(textColor);
    QFont f = p.font();
    f.setFamily(CommonStyleV5Private::m_iconSymbolFont.family());
    p.setFont(f);
    QTextOption textOption(Qt::AlignCenter);
    p.drawText(QRect(QPoint(0, 0), sizeIcon), code, textOption);
    return symbolPixmap;
}

