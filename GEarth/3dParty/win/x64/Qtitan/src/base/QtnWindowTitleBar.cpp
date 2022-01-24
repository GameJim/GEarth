/****************************************************************************
**
** Qtitan Library by Developer Machines (Components for Qt.C++)
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
#include <QPainter>
#include <QAbstractScrollArea>
#include <QStyle>
#include <QStyleOptionFrame>
#include <QPaintEngine>
#include <QFontDatabase>

#ifdef QTN_PRIVATE_HEADERS
#include <qpa/qplatformtheme.h>
#include <private/qguiapplication_p.h>
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QBackingStore>
#endif

#include "QtnPlatform.h"
#include "QtnWindowTitleBarPrivate.h"

#ifdef Q_OS_WIN
#include "QtnWindowTitleBar_win.h"
#endif

QTITAN_USE_NAMESPACE

/* WindowTitleBar */
WindowTitleBar::WindowTitleBar(QWidget* window)
    : QObject(window), m_p(Q_NULL)
{
    Q_ASSERT(window->isTopLevel());
#ifdef Q_OS_WIN
    m_p = new WindowTitleBarPrivateWin();
#else
    m_p = new WindowTitleBarPrivateQt();
#endif
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(appAboutToQuit()));
}

WindowTitleBar* WindowTitleBar::get(QWidget* window)
{
    Q_ASSERT(window != Q_NULL);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    WindowTitleBar* ret = window->findChild<WindowTitleBar *>(QString(), Qt::FindDirectChildrenOnly);
#else
    WindowTitleBar* ret = window->findChild<WindowTitleBar *>(QString());
#endif
    if (ret == Q_NULL)
        ret = new WindowTitleBar(window);
    return ret;
}

WindowTitleBar::~WindowTitleBar()
{
    if (m_p != Q_NULL)
    {
        m_p->setDestroyed();
        Q_DELETE_AND_NULL(m_p);
    }
}

void WindowTitleBar::show()
{
    m_p->setWindow(qobject_cast<QWidget *>(parent()));
}

void WindowTitleBar::hide()
{
    m_p->setWindow(Q_NULL);
}

void WindowTitleBar::setWidget(QWidget* widget, TitleBarAlign align)
{
    m_p->setTitleBarWidget(widget);
    m_p->setTitleBarWidgetAlign(align);
}

QWidget* WindowTitleBar::widget() const
{
    return m_p->titleBarWidget();
}

bool WindowTitleBar::styledFrame() const
{
    return m_p->styledFrame();
}

void WindowTitleBar::setStyledFrame(bool styled)
{
    m_p->setStyledFrame(styled);
}

bool WindowTitleBar::extendViewIntoTitleBar() const
{
    return m_p->extendViewIntoTitleBar();
}

void WindowTitleBar::setExtendViewIntoTitleBar(bool extend)
{
    m_p->setExtendViewIntoTitleBar(extend);
}

/*!
Sets the \a height of the titlebar (top part of the window non-client area). 
If the value is not specified, then the height of the titlebar will be equal 
to the height of the system titlebar. If the styledFrame() property returns true, 
the height of the titlebar will be retrieved from the window style using the QStyle::PM_TitleBarHeight metric.
*/
void WindowTitleBar::setTitleHeight(int height)
{
    m_p->m_titleHeightRequested = height;
}

/*!
Returns the height of the titlebar.
*/
int WindowTitleBar::titleHeight() const
{
    return m_p->m_titleHeight;
}

/*!
Sets the frame \a thickness of the window non-client area.
If the value is not specified, then the thickness will be equal to the thickness of the system window frame.
If the styledFrame() property returns true, the border thickness will be retrieved from the window style using the QStyle::PM_MDIFrameWidth metric.
*/
void WindowTitleBar::setBorderThickness(int thickness)
{
    m_p->m_borderThicknessRequested = thickness;
}

/*!
Returns the window non-client area frame thickness.
*/
int WindowTitleBar::borderThickness() const
{
    return m_p->m_borderThickness;
}

bool WindowTitleBar::isSysMenuButtonVisible() const
{
    return m_p->isSysMenuButtonVisible();
}

void WindowTitleBar::setSysMenuButtonVisible(bool visible)
{
    m_p->setSysMenuButtonVisible(visible);
}

const QColor& WindowTitleBar::backgroundColor() const
{
    return m_p->backgroundColor();
}

void WindowTitleBar::setBackgroundColor(const QColor& color)
{
    m_p->setBackgroundColor(color);
}

void WindowTitleBar::setDestroyed()
{
    m_p->setDestroyed();
}

bool WindowTitleBar::blurWindowEnabled() const
{
    return m_p->blurWindowEnabled();
}

void WindowTitleBar::setBlurWindowEnabled(bool enabled)
{
    m_p->setBlurWindowEnabled(enabled);
}

void WindowTitleBar::appAboutToQuit()
{
    m_p->setDestroyed();
    m_p->setWindow(Q_NULL);
}


class QWidgetEx : public QWidget
{
public:
    friend class Qtitan::WindowTitleBarPrivateQt;
    friend class Qtitan::WindowTitleBarPrivate;
};

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
class QWindowEx : public QWindow
{
public:
    friend class Qtitan::WindowTitleBarPrivate;
};
#endif


/* WindowTitleBarPrivate */
WindowTitleBarPrivate::WindowTitleBarPrivate() 
    : WindowStyleHelperAbstract(),
    m_oldFlags(0), 
    m_titleBarWidget(Q_NULL),
    m_hovered(QStyle::SC_None),
    m_pressed(QStyle::SC_None),
    m_nonClientUnderMouse(false),
    m_styledFrame(true),
    m_blurWindowEnabled(false),
    m_extendViewIntoTitleBar(false),
    m_systemFrameMargins(QMargins(0, 0, 0, 0)),
    m_borderThickness(-1),
    m_titleHeight(-1),
    m_borderThicknessRequested(-1),
    m_titleHeightRequested(-1),
    m_maximized(false),
    m_fullScreen(false),
    m_sysMenuButtonVisible(true),
    m_backgroundColor(Qt::transparent),
    m_frameWindows10(QMargins(0, 0, 0, 0)),
    m_extendedView(Q_NULL),
    m_hasCursor(false)
{
}

WindowTitleBarPrivate::~WindowTitleBarPrivate()
{
    Q_DELETE_AND_NULL(m_titleBarWidget);
    Q_DELETE_AND_NULL(m_extendedView);
}

bool WindowTitleBarPrivate::underMouse() const
{
    return m_nonClientUnderMouse;
}

bool WindowTitleBarPrivate::eventFilter(QObject* object, QEvent* event)
{
    if (object == window())
    {
        switch (event->type())
        {
#if 0
        case QEvent::UpdateRequest:
        {
            if (extendViewIntoTitleBar() && m_extendedView != Q_NULL)
                m_extendedView->update();
        }
        break;
#endif
        case QEvent::StyleChange:
            styleChanged();
            break;
        case QEvent::ShortcutOverride:
        {
            if (m_titleBarWidget != Q_NULL)
            {
                m_titleBarWidget->sendShortcutOverride(static_cast<QKeyEvent *>(event));
                if (event->isAccepted())
                    return true;
            }
        }
        break;
        case QEvent::KeyPress:
        {
            if (m_titleBarWidget != Q_NULL)
            {
                m_titleBarWidget->sendKeyPress(static_cast<QKeyEvent *>(event));
                if (event->isAccepted())
                    return true;
           }
        }
        break;
        case QEvent::KeyRelease:
        {
            if (m_titleBarWidget != Q_NULL)
            {
                m_titleBarWidget->sendKeyRelease(static_cast<QKeyEvent *>(event));
                if (event->isAccepted())
                    return true;
            }
        }
        break;
        case QEvent::FocusIn:
        {
            if (m_titleBarWidget != Q_NULL)
                m_titleBarWidget->sendFocusIn(static_cast<QFocusEvent *>(event));
        }
        break;
        case QEvent::FocusOut:
        {
            if (m_titleBarWidget != Q_NULL)
                m_titleBarWidget->sendFocusOut(static_cast<QFocusEvent *>(event));
        }
        break;
        default:
        break;
        }
    }

    return QObject::eventFilter(object, event);
}

void WindowTitleBarPrivate::setTitleBarWidget(QWidget* widget)
{
    if (widget == Q_NULL)
    {
        Q_DELETE_AND_NULL(m_titleBarWidget);
        return;
    }

    if (m_titleBarWidget == Q_NULL)
    {
        m_titleBarWidget = new BackstageWidget(window());
        QObject::connect(m_titleBarWidget, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(backstageWidgetUpdate(const QList<QRectF> &)));
    }
    
    m_titleBarWidget->setWidget(widget, QRect());
    if (isActive())
    {
        updateTitleBarWidgetGeometry();
        m_titleBarWidget->activate();
    }
}

QWidget* WindowTitleBarPrivate::titleBarWidget() const
{
    return m_titleBarWidget != Q_NULL ? m_titleBarWidget->widget() : Q_NULL;
}

QString WindowTitleBarPrivate::titleBarText() const
{
    if (window() == Q_NULL)
        return QString();
    QString ret = window()->windowTitle();
    if (ret.isEmpty())
        ret = qApp->applicationName();
    return ret;
}

WindowTitleBar::TitleBarAlign WindowTitleBarPrivate::titleBarWidgetAlign() const
{
    return m_titleBarWidgetAlign;
}

void WindowTitleBarPrivate::setTitleBarWidgetAlign(WindowTitleBar::TitleBarAlign align)
{
    if (m_titleBarWidgetAlign == align)
        return;
    m_titleBarWidgetAlign = align;
    updateTitleBarWidgetGeometry();
}

#if 0
void WindowTitleBarPrivate::setTitleBarMargins(const QMargins& margins)
{
    if (m_titleBarMargins == margins)
        return;
    m_titleBarMargins = margins;
    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->setMargins(margins);
}
#endif

bool WindowTitleBarPrivate::hasFocus() const
{
    return window() != Q_NULL && window()->hasFocus();
}

QRect WindowTitleBarPrivate::getClientRect() const
{
    QRect rect(0, 0, windowSize().width(), windowSize().height());
    rect = rect.adjusted(borderThickness(), titleHeight(), -borderThickness(), -borderThickness());
    return rect;
}

QRegion WindowTitleBarPrivate::getNonClientRegion() const
{
    QRect rect = QRect(QPoint(0, 0), windowSize());
    QRegion region(rect);
    region -= rect.adjusted(borderThickness(), titleHeight(), -borderThickness(), -borderThickness());
    return region;
}

QRect WindowTitleBarPrivate::getTitleBarRect()
{
    return QRect(0, 0, windowSize().width(), titleHeight());
}

QStyle::SubControl WindowTitleBarPrivate::hitTest(const QPoint& pos, Qt::WindowFrameSection& frameSection)
{
    frameSection = checkFrameSection(pos);
    return frameSection != Qt::NoSection ? getHoverSubControl(pos) : QStyle::SC_None;
}

bool WindowTitleBarPrivate::styledFrame() const
{
#ifdef Q_OS_WIN
    return m_styledFrame;
#else
    return true; //Native title bar rendering on MacOSX and Linux is not implemented yet.
#endif
}

void WindowTitleBarPrivate::setStyledFrame(bool styled)
{
    if (m_styledFrame == styled)
        return;
    m_styledFrame = styled;
    styleChanged();
}

bool WindowTitleBarPrivate::blurWindowEnabled() const
{
    return m_blurWindowEnabled;
}

void WindowTitleBarPrivate::setBlurWindowEnabled(bool enabled)
{
    if (m_blurWindowEnabled == enabled)
        return;
    m_blurWindowEnabled = enabled;
}

bool WindowTitleBarPrivate::extendViewIntoTitleBar() const
{
    return m_extendViewIntoTitleBar;
}

void WindowTitleBarPrivate::setExtendViewIntoTitleBar(bool extend)
{
    if (m_extendViewIntoTitleBar == extend)
        return;
    m_extendViewIntoTitleBar = extend;
    styleChanged();
}

bool WindowTitleBarPrivate::isSysMenuButtonVisible() const
{
    return m_sysMenuButtonVisible;
}

void WindowTitleBarPrivate::setSysMenuButtonVisible(bool visible)
{
    if (m_sysMenuButtonVisible == visible)
        return;
    m_sysMenuButtonVisible = visible;
    styleChanged();
}

const QColor& WindowTitleBarPrivate::backgroundColor() const
{
    if (m_backgroundColor == Qt::transparent)
        return m_colorizationColor;
    else
        return m_backgroundColor;
}

void WindowTitleBarPrivate::setBackgroundColor(const QColor& color)
{
    m_backgroundColor = color;
}

const QMargins& WindowTitleBarPrivate::systemFrameMargins() const
{
    return m_systemFrameMargins;
}

int WindowTitleBarPrivate::borderThickness() const
{
    if (isFullScreen())
        return 0;
    return m_borderThickness;
}

int WindowTitleBarPrivate::titleHeight() const
{
    if (isFullScreen())
        return 0;
    return m_titleHeight;
}

bool WindowTitleBarPrivate::styledIconAndText() const
{
    return styledFrame() && m_titleBarWidget == Q_NULL && !extendViewIntoTitleBar();
}

bool WindowTitleBarPrivate::isMaximized() const
{
    return m_maximized;
}

bool WindowTitleBarPrivate::isFullScreen() const
{
    return m_fullScreen;
}

#if 0
static QRegion qtn_createRoundedRegion(const QSize& size, int radius)
{
#if 1
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, size.width(), size.height()), radius, radius);
    QRegion region(path.toFillPolygon().toPolygon());
#else
    QRegion verticalRegion(0, radius, size.width(), size.height() - 2 * radius);
    QRegion horizontalRegion(radius, 0, size.width() - 2 * radius, size.height());
    QRegion circle(0, 0, 2 * radius, 2 * radius, QRegion::Ellipse);

    QRegion region = verticalRegion.united(horizontalRegion);
    region = region.united(circle);
    region = region.united(circle.translated(size.width() - 2 * radius, 0));
    region = region.united(circle.translated(size.width() - 2 * radius, size.height() - 2 * radius));
    region = region.united(circle.translated(0, size.height() - 2 * radius));
#endif
    return region;
}
#endif

void WindowTitleBarPrivate::updateLayout()
{
    updateSysButtonsRect();
    updateExtendedViewGeometry();
    updateTitleBarWidgetGeometry();
}

void WindowTitleBarPrivate::updateFrame()
{
    updateLayout();
}

void WindowTitleBarPrivate::update(const QRect& rect)
{
   /* if (m_extendedView != Q_NULL)
        m_extendedView->update(rect);
    else */if (window() != Q_NULL)
        window()->update(rect);
}

void WindowTitleBarPrivate::update(const QRegion& region)
{
/*    if (m_extendedView != Q_NULL)
        m_extendedView->update(region);
    else */if(window() != Q_NULL)
        window()->update(region);
}

void WindowTitleBarPrivate::setCursor(const QCursor& cursor)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (window() == Q_NULL || window()->windowHandle() == Q_NULL)
        return;
    window()->windowHandle()->setCursor(cursor);
#else
    Q_UNUSED(cursor);
#endif
    m_hasCursor = true;
}

void WindowTitleBarPrivate::unsetCursor()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (window() == Q_NULL || window()->windowHandle() == Q_NULL)
        return;
    window()->windowHandle()->unsetCursor();
#endif
    m_hasCursor = false;
}

void WindowTitleBarPrivate::processExtendViewEvent(QEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (!event->isAccepted() && extendViewIntoTitleBar())
        static_cast<QWindowEx *>(window()->windowHandle())->event(event);
#else
    Q_UNUSED(event);
#endif
}

Qt::WindowFrameSection WindowTitleBarPrivate::checkFrameSection(const QPoint& pos)
{
    int border = 0;
#ifndef Q_OS_MAC
    //For MacOSX use native resize border.
    if (!isMaximized() && !isFullScreen())
    {
        border = qMax(5, borderThickness());
        border = qMin(border, 8);
    }
#endif 
    QRect windowRect(0, 0, windowSize().width(), windowSize().height());
    QRegion resizableFrame = QRegion(windowRect) - QRegion(windowRect.adjusted(border, border, -border, -border));

    if (resizableFrame.intersected(QRect(0, 0, border, border)).contains(pos))
        return Qt::TopLeftSection;
    if (resizableFrame.intersected(QRect(border, 0, windowSize().width() - border * 2, border)).contains(pos))
        return Qt::TopSection;
    if (resizableFrame.intersected(QRect(windowSize().width() - border, 0, border, border)).contains(pos))
        return Qt::TopRightSection;
    if (resizableFrame.intersected(QRect(windowSize().width() - border, border, border, windowSize().height() - border * 2)).contains(pos))
        return Qt::RightSection;
    if (resizableFrame.intersected(QRect(windowSize().width() - border, windowSize().height() - border, border, border)).contains(pos))
        return Qt::BottomRightSection;
    if (resizableFrame.intersected(QRect(border, windowSize().height() - border, windowSize().width() - border * 2, border)).contains(pos))
        return Qt::BottomSection;
    if (resizableFrame.intersected(QRect(0, windowSize().height() - border, border, border)).contains(pos))
        return Qt::BottomLeftSection;
    if (resizableFrame.intersected(QRect(0, border, border, windowSize().height() - border * 2)).contains(pos))
        return Qt::LeftSection;

    if (!containsPoint(pos))
        return Qt::NoSection;

    return Qt::TitleBarArea;
}

void WindowTitleBarPrivate::initTitleBarOption(QStyleOptionTitleBar& option)
{
    option.initFrom(window());
    option.subControls = QStyle::SC_All;
#if 0
        QStyle::SC_TitleBarSysMenu |
        QStyle::SC_TitleBarMinButton |
        QStyle::SC_TitleBarMaxButton |
        QStyle::SC_TitleBarCloseButton |
        QStyle::SC_TitleBarNormalButton |
        QStyle::SC_TitleBarShadeButton |
        QStyle::SC_TitleBarUnshadeButton |
        QStyle::SC_TitleBarContextHelpButton |
        QStyle::SC_TitleBarLabel;
#endif
    option.titleBarFlags = m_oldFlags;
    option.palette = window()->palette();

    option.titleBarState = window()->windowState();

#if 0
        Qt::CustomizeWindowHint
        | Qt::WindowTitleHint
        | Qt::WindowSystemMenuHint
        | Qt::WindowMinimizeButtonHint
        | Qt::WindowMaximizeButtonHint
        | Qt::WindowContextHelpButtonHint;
#endif

    option.activeSubControls = QStyle::SC_None;

    if (window()->isEnabled())
        option.state |= QStyle::State_Enabled;

    if (isActive())
    {
        option.state |= QStyle::State_Active;
        option.titleBarState |= QStyle::State_Active;
        option.palette.setCurrentColorGroup(QPalette::Active);
        QColor color = backgroundColor();
        option.palette.setColor(QPalette::Shadow, color.darker(150));
        if (extendViewIntoTitleBar())
            option.palette.setColor(QPalette::Window, Qt::transparent);
        else
            option.palette.setColor(QPalette::Window, color);
    }
    else
    {
        option.state &= ~QStyle::State_Active;
        option.palette.setCurrentColorGroup(QPalette::Inactive);
        QColor color = option.palette.window().color();
        option.palette.setColor(QPalette::Shadow, color.darker(150));

        if (extendViewIntoTitleBar())
            option.palette.setColor(QPalette::Window, Qt::transparent);
        else
            option.palette.setColor(QPalette::Window, color);
    }

    if (window()->isMinimized())
        option.titleBarState |= Qt::WindowMinimized;
    else if (window()->isMaximized())
        option.titleBarState |= Qt::WindowMaximized;

    option.text = titleBarText();
    option.icon = m_icon;

    if (m_hovered != QStyle::SC_None && m_hovered != QStyle::SC_TitleBarLabel)
        option.state |= QStyle::State_MouseOver;
    else
        option.state &= ~QStyle::State_MouseOver;

    if (m_pressed != QStyle::SC_None)
    {
        option.state |= QStyle::State_Sunken;
        if (m_pressed == m_hovered)
            option.activeSubControls = m_pressed;
    }
    else
    {
        option.state &= ~QStyle::State_Sunken;
        option.activeSubControls = m_hovered;
    }

    option.rect = QRect(0, 0, windowSize().width(), titleHeight());
}

void WindowTitleBarPrivate::styleChanged()
{
    if (window() == Q_NULL)
    {
        m_icon = QPixmap();
        setFrameMargins(QMargins());
        m_titleHeight = -1;
        m_borderThickness = -1;
        return;
    }

    m_borderThickness = m_borderThicknessRequested;
    m_titleHeight = m_titleHeightRequested;
    m_colorizationColor = qtn_getColorizationColor();
    if (styledFrame())
    {
        QStyleOptionTitleBar option;
        initTitleBarOption(option);
        if (m_borderThickness == -1)
            m_borderThickness = window()->style()->pixelMetric(QStyle::PM_MDIFrameWidth, &option, window());
        if (m_titleHeight == -1)
            m_titleHeight = window()->style()->pixelMetric(QStyle::PM_TitleBarHeight, &option, window());
    }
    else
    {
        QMargins systemMargins = systemFrameMargins() - m_frameWindows10;
        if (m_borderThickness == -1)
            m_borderThickness = systemMargins.left();
        if (m_titleHeight == -1)
            m_titleHeight = systemMargins.top();
    }
#if 0
    m_icon = qtn_getTitleBarIcon(window());
#else
    m_icon = window()->windowIcon();
#endif
    if (m_icon.isNull())
        m_icon =  QPixmap(":/res/qtitanlogo32x32.png");

    QMargins margins = QMargins(m_borderThickness, m_titleHeight, m_borderThickness, m_borderThickness);

    if (extendViewIntoTitleBar())
    {
        margins = QMargins(0, 0, 0, 0);

        m_icon = QPixmap();

        if (m_extendedView == Q_NULL)
        {
            m_extendedView = new ExendedView(this);
        }
    }
    else
    {
        Q_DELETE_AND_NULL(m_extendedView);
    }

    setFrameMargins(margins);
    updateFrame();
}

static bool isButtonVisible(const QStyle::SubControl sc, const QStyleOptionTitleBar *titleBarOption)
{
    bool isMinimized = titleBarOption->titleBarState & Qt::WindowMinimized;
    bool isMaximized = titleBarOption->titleBarState & Qt::WindowMaximized;
    const uint flags = titleBarOption->titleBarFlags;
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

void WindowTitleBarPrivate::updateSysButtonsRect()
{
    m_systemButtonsRect = QRect();
    if (window() == Q_NULL)
        return;

    Q_ASSERT(styledFrame());
    QStyleOptionTitleBar titleBarOption;
    initTitleBarOption(titleBarOption);

    QRegion region;
    if (isButtonVisible(QStyle::SC_TitleBarContextHelpButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarContextHelpButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarMinButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarMinButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarNormalButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarNormalButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarMaxButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarMaxButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarShadeButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarShadeButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarUnshadeButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarUnshadeButton, window());
    if (isButtonVisible(QStyle::SC_TitleBarCloseButton, &titleBarOption))
        region += window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, QStyle::SC_TitleBarCloseButton, window());

    m_systemButtonsRect = region.boundingRect().adjusted(-2, 0, 2, 0);
}

QStyle::SubControl WindowTitleBarPrivate::getHoverNativeSubControl(const QPoint& pos)
{
    Q_UNUSED(pos);
    return QStyle::SC_None;
}

QRect WindowTitleBarPrivate::getSubControlRect(QStyle::SubControl sc)
{
    const bool sysButtons = (sc != QStyle::SC_TitleBarSysMenu && sc != QStyle::SC_TitleBarLabel);
    if ((sysButtons && styledFrame()) || (!sysButtons && styledIconAndText()))
    {
        QStyleOptionTitleBar titleBarOption;
        initTitleBarOption(titleBarOption);
        return window()->style()->subControlRect(QStyle::CC_TitleBar, &titleBarOption, sc, window());
    }

    switch (sc)
    {
    case QStyle::SC_TitleBarSysMenu:
    {
        QRect r = getTitleBarRect();
        if (isMaximized())
            r.adjust(0, borderThickness(), 0, 0);

        const bool alignSysButtonsLeft = m_systemButtonsRect.left() - r.left() < r.right() - m_systemButtonsRect.right();

        if (alignSysButtonsLeft)
            r.setLeft(m_systemButtonsRect.right());

        
        int sysMenuButtonWidth = getSysMenuButtonWidth();
        r.setWidth(sysMenuButtonWidth);
        return r;
    }
    break;
    case QStyle::SC_TitleBarLabel:
    {
        QRect r = getTitleBarRect();
        if (isMaximized())
            r.adjust(0, borderThickness(), 0, 0);

        const bool alignSysButtonsLeft = m_systemButtonsRect.left() - r.left() < r.right() - m_systemButtonsRect.right();

        int sysMenuButtonWidth = getSysMenuButtonWidth();
        if (alignSysButtonsLeft)
            r.setLeft(m_systemButtonsRect.right() + sysMenuButtonWidth);
        else
        {
            r.setLeft(r.left() + sysMenuButtonWidth);
            r.setRight(m_systemButtonsRect.left());
        }
        return r;
    }
    break;
    case QStyle::SC_TitleBarMinButton:
    case QStyle::SC_TitleBarMaxButton:
    case QStyle::SC_TitleBarCloseButton:
    case QStyle::SC_TitleBarContextHelpButton:
    case QStyle::SC_TitleBarNormalButton:
    case QStyle::SC_TitleBarShadeButton:
    case QStyle::SC_TitleBarUnshadeButton:
        return QRect();
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return QRect();
}

QStyle::SubControl WindowTitleBarPrivate::getHoverSubControl(const QPoint& pos)
{
    QStyle::SubControl result = QStyle::SC_None;
    const bool sysButtons = m_systemButtonsRect.contains(pos);
    if ((sysButtons && styledFrame()) || (!sysButtons && styledIconAndText()))
    {
        QStyleOptionTitleBar titleBarOption;
        initTitleBarOption(titleBarOption);
        result = window()->style()->hitTestComplexControl(QStyle::CC_TitleBar, &titleBarOption, pos, window());
    }
    else
    {
        QRect r = getSubControlRect(QStyle::SC_TitleBarSysMenu);
        if (r.contains(pos))
            result = QStyle::SC_TitleBarSysMenu;

        if (result == QStyle::SC_None)
        {
            r = getSubControlRect(QStyle::SC_TitleBarLabel);
            if (r.contains(pos))
                result = QStyle::SC_TitleBarLabel;
        }

        if (result == QStyle::SC_None)
            result = getHoverNativeSubControl(pos);
    }

    if (result == QStyle::SC_TitleBarMaxButton && window()->isMaximized())
        result = QStyle::SC_TitleBarNormalButton;
    return result;
}

void WindowTitleBarPrivate::setSubControlPressed(QStyle::SubControl sc)
{
    if (m_pressed != QStyle::SC_None)
    {
        QRect r = getSubControlRect(m_pressed);
        update(r);
    }

    m_pressed = sc;

    if (m_pressed != QStyle::SC_None)
    {
        QRect r = getSubControlRect(m_pressed);
        update(r);
    }
}

void WindowTitleBarPrivate::subControlClicked(QMouseEvent* event)
{
    Q_UNUSED(event);
    switch (m_pressed)
    {
    case QStyle::SC_TitleBarSysMenu:
        {
            QPoint p = window()->frameGeometry().topLeft();
            p += QPoint(frameMargins().left(), frameMargins().top());
            showSystemMenu(p);
        }
        break;
    case QStyle::SC_TitleBarMinButton:
        window()->showMinimized();
        break;
    case QStyle::SC_TitleBarMaxButton:
        window()->showMaximized();
        break;
    case QStyle::SC_TitleBarCloseButton:
        window()->close();
        break;
    case QStyle::SC_TitleBarNormalButton:
        window()->showNormal();
        break;
    case QStyle::SC_TitleBarShadeButton:
        //window()->showShaded();
        break;
    case QStyle::SC_TitleBarUnshadeButton:
        window()->showNormal();
        break;
    case QStyle::SC_TitleBarContextHelpButton:
        break;
    case QStyle::SC_TitleBarLabel:
    default:
        break;
    }
}

void WindowTitleBarPrivate::updateSubControl(const QPoint& pos)
{
    QStyle::SubControl oldhovered = m_hovered;
    m_hovered = hitTest(pos, m_hoveredFrameSection);
    if (m_hovered == oldhovered)
        return;
#ifdef Q_OS_MAC
        update(getTitleBarRect());
        return;
#else
    if (oldhovered != QStyle::SC_None && oldhovered != QStyle::SC_TitleBarLabel)
    {
        QRect r = getSubControlRect(oldhovered);
        update(r);
    }

    if (m_hovered != QStyle::SC_None && m_hovered != QStyle::SC_TitleBarLabel)
    {
        QRect r = getSubControlRect(m_hovered);
        update(r);
    }
#endif
}

void WindowTitleBarPrivate::setWindow(QWidget* window)
{
    Q_DELETE_AND_NULL(m_extendedView);

    if (window != this->window())
    {
        if (this->window() != Q_NULL)
        {
            this->window()->removeEventFilter(this);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            if (this->window()->windowHandle() != Q_NULL)
                this->window()->windowHandle()->removeEventFilter(this);
#endif
            this->window()->setContentsMargins(m_origContentsMargins);
            m_origContentsMargins = QMargins();

        }
    }

    WindowStyleHelperAbstract::setWindow(window);

    if (window != Q_NULL)
    {
        m_origContentsMargins = window->contentsMargins();

        window->installEventFilter(this);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        window->winId();
        window->windowHandle()->installEventFilter(this);
#endif

        if (blurWindowEnabled() && !autoFillBackground())
            qtn_setBlurBehindWindow(window);
    }

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->setParentWidget(window);

    styleChanged();
}

void WindowTitleBarPrivate::nonClientActivateEvent(bool active)
{
    if (window() == Q_NULL)
        return;


    if (active)
    {
#ifdef Q_OS_WIN
        if (!styledFrame() && autoFillBackground())
        {
            qtn_setWindowColorization(window(), backgroundColor());
        }
#endif

        if (m_titleBarWidget != Q_NULL)
            m_titleBarWidget->activate();
    }
    else
    {
#ifdef Q_OS_WIN
        if (!styledFrame() && autoFillBackground())
        {
            qtn_setWindowColorization(window(), window()->palette().color(QPalette::Inactive, QPalette::Window));
        }
#endif

        if (m_titleBarWidget != Q_NULL)
            m_titleBarWidget->deactivate();

        setSubControlPressed(QStyle::SC_None);
    }
    window()->update(getNonClientRegion());
}

void WindowTitleBarPrivate::processContextMenuEvent(QMouseEvent* event)
{
#ifdef QTN_PRIVATE_HEADERS
    static const QEvent::Type contextMenuTrigger =
        QGuiApplicationPrivate::platformTheme()->themeHint(QPlatformTheme::ContextMenuOnMouseRelease).toBool() ?
        QEvent::MouseButtonRelease : QEvent::MouseButtonPress;
#else
    static const QEvent::Type contextMenuTrigger = QEvent::MouseButtonRelease;
#endif

    if (event->type() != contextMenuTrigger)
    {
        return;
    }

    QRegion region = getNonClientRegion();
    if (event->type() == contextMenuTrigger && event->button() == Qt::RightButton
        && region.contains(event->pos()))
    {
        QContextMenuEvent e(QContextMenuEvent::Mouse, event->pos(), event->globalPos(), event->modifiers());
        mouseContextNonClientEvent(&e);
    }
}

void WindowTitleBarPrivate::paintTitleBarIcon(QPainter* painter, const QStyleOptionTitleBar& option)
{
    if (!styledFrame())
    {
        qtn_paintTitleBarIcon(painter, option.icon, option.rect);
    }
    else
    {
        QPixmap pixmap = option.icon.pixmap(option.rect.size());
        QRect r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, pixmap.size(), option.rect);
        painter->drawPixmap(r, pixmap);
    }
}

void WindowTitleBarPrivate::paintTitleBarText(QPainter* painter, const QStyleOptionTitleBar& option)
{
    if (option.rect.width() == 0)
        return;

    if (!styledFrame())
    {
        qtn_paintTitleBarText(painter, option.text, option.rect, isActive());
    }
    else
    {
        QString text = option.text;
#ifdef Q_OS_WIN
        painter->setFont(qtn_getWindowCaptionFont(window()));
#endif
        QRect textBoundingRect = painter->fontMetrics().boundingRect(text);
        if (textBoundingRect.width() > option.rect.width())
            text = painter->fontMetrics().elidedText(text, Qt::ElideRight, option.rect.width());
        QTextOption textOption(Qt::AlignVCenter | Qt::AlignHCenter);
        textOption.setWrapMode(QTextOption::NoWrap);
        painter->drawText(option.rect, text, textOption);
    }
}

void WindowTitleBarPrivate::paintNonClientEvent(QPainter* painter)
{
    QStyleOptionTitleBar titleBarOption;
    if (styledFrame())
    {
        initTitleBarOption(titleBarOption);

        if (/*extendViewIntoTitleBar() ||*/ autoFillBackground())
            titleBarOption.subControls &= ~QStyle::SC_TitleBarLabel; //Remove title bar background.

        if (autoFillBackground())
        {
            QColor bgcolor = isActive() ? backgroundColor() : window()->palette().color(QPalette::Inactive, QPalette::Window);
            QRegion clip = painter->clipRegion();
            QVector<QRect> rects = clip.rects();
            for (int i = 0; i < rects.size(); ++i)
                painter->fillRect(rects[i], bgcolor);
        }

        if (!styledIconAndText())
        {
            titleBarOption.subControls &= ~QStyle::SC_TitleBarSysMenu; //Remove title bar icon.
            titleBarOption.icon = QIcon();
            titleBarOption.text = "";
        }

        bool hasBorder = false;
        painter->save();
#ifdef Q_OS_MAC
        if (window()->style()->metaObject()->className() == QString("QMacStyle"))
            qtn_paint_title_bar_mac(&titleBarOption, painter, window());
        else
#endif
        window()->style()->drawComplexControl(QStyle::CC_TitleBar, &titleBarOption, painter, window());
        painter->restore();
#if 0
        QRect temp = getSubControlRect(QStyle::SC_TitleBarCloseButton);
        painter->drawRect(temp);
        temp = getSubControlRect(QStyle::SC_TitleBarMinButton);
        painter->drawRect(temp);
        temp = getSubControlRect(QStyle::SC_TitleBarMaxButton);
        painter->drawRect(temp);
#endif

        if (/*!extendViewIntoTitleBar() && */!autoFillBackground())
        {
            QStyleOptionFrame frameOption;
            frameOption.initFrom(window());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            frameOption.features = QStyleOptionFrame::Flat;
            frameOption.frameShape = QFrame::StyledPanel;
#endif
            frameOption.lineWidth = window()->style()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth, 0, window());
            frameOption.midLineWidth = borderThickness() - frameOption.lineWidth;
            frameOption.rect = QRect(QPoint(0, 0), windowSize());
            frameOption.palette = titleBarOption.palette;
            if (isActive())
                frameOption.state |= QStyle::State_Active;
            else
                frameOption.state &= ~QStyle::State_Active;

            painter->save();
            if (!hasBorder)
                painter->setClipRect(frameOption.rect.adjusted(0, titleBarOption.rect.height(), 0, 0), Qt::IntersectClip);
            window()->style()->drawPrimitive(QStyle::PE_FrameWindow, &frameOption, painter, window());
            painter->restore();
        }
    }
    else
    {
        if (extendViewIntoTitleBar())
        {
            qtn_paintAirEffect(painter, m_systemButtonsRect, m_systemButtonsRect);
        }
    }

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->paintContent(*painter);

    if (!styledIconAndText())
    {
        //Draw icon.
        titleBarOption.rect = getSubControlRect(QStyle::SC_TitleBarSysMenu);
        titleBarOption.icon = m_icon;
        paintTitleBarIcon(painter, titleBarOption);

        //Draw title.
        titleBarOption.rect = m_textRect;
        titleBarOption.text = titleBarText();
        titleBarOption.titleBarFlags = Qt::WindowFlags();
        titleBarOption.titleBarState = 0;
        paintTitleBarText(painter, titleBarOption);
    }
}

void WindowTitleBarPrivate::mousePressNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    updateSubControl(event->pos());

    if (!defaultResizeHandler() && m_hovered != QStyle::SC_None)
    {
        if (m_hovered == QStyle::SC_TitleBarLabel)
        {
            if (m_titleBarWidget != Q_NULL)
            {
                if (m_titleBarWidget->mouseGrabberItem() != Q_NULL)
                {
                    QMouseEvent moveEvent(QEvent::MouseMove, event->pos(), Qt::MouseButton(), Qt::MouseButtons(), Qt::KeyboardModifier());
                    m_titleBarWidget->sendMouseMove(&moveEvent);
                }

                m_titleBarWidget->setFocus(Qt::MouseFocusReason);
                m_titleBarWidget->sendMousePress(event);
            }

            if (event->isAccepted() && m_titleBarWidget != Q_NULL && m_titleBarWidget->focusItem() != Q_NULL)
            {
                if (window() != Q_NULL)
                    window()->setFocus();
            }
        }
        else
        {
            event->accept();
        }
    }

    if (event->isAccepted() || event->button() != Qt::LeftButton)
    {
        setSubControlPressed(m_hovered);
        processContextMenuEvent(event);
        event->accept();
    }
    else
        setSubControlPressed(QStyle::SC_None);

    processExtendViewEvent(event);
}

void WindowTitleBarPrivate::mouseReleaseNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    updateSubControl(event->pos());

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendMouseRelease(event);

    if (!event->isAccepted() && m_pressed != QStyle::SC_None && m_pressed == m_hovered && event->button() == Qt::LeftButton)
        subControlClicked(event);

    processContextMenuEvent(event);
    setSubControlPressed(QStyle::SC_None);

    processExtendViewEvent(event);
}

void WindowTitleBarPrivate::mouseDoubleClickNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendMouseDoubleClick(event);

    processExtendViewEvent(event);
}

void WindowTitleBarPrivate::mouseMoveNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    updateSubControl(event->pos());

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendMouseMove(event);
    updateCursor();

    processExtendViewEvent(event);
}

void WindowTitleBarPrivate::mouseNonClientOver(const QPoint& pos)
{
    updateSubControl(pos);

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendMouseOver(pos);

    updateCursor();
}

void WindowTitleBarPrivate::mouseNonClientOut(const QPoint& pos)
{
    setSubControlPressed(QStyle::SC_None);

    updateSubControl(pos);

    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendMouseOut(pos);

    updateCursor();
}

void WindowTitleBarPrivate::mouseContextNonClientEvent(QContextMenuEvent* event)
{
    event->ignore();
    if (m_titleBarWidget != Q_NULL)
        m_titleBarWidget->sendShowContextMenu(event);

    if (!event->isAccepted())
    {
        if (m_pressed == QStyle::SC_TitleBarSysMenu || m_pressed == QStyle::SC_TitleBarLabel)
        {
            showSystemMenu(event->globalPos());
            event->accept();
        }
    }
}

void WindowTitleBarPrivate::resizeNonClientEvent(const QSize& windowSize)
{
    if (windowSize == this->windowSize())
        return;

    setWindowSize(windowSize);
    updateLayout();
}

bool WindowTitleBarPrivate::defaultResizeHandler()
{
    return m_hoveredFrameSection == Qt::LeftSection ||
        m_hoveredFrameSection == Qt::TopLeftSection ||
        m_hoveredFrameSection == Qt::TopSection ||
        m_hoveredFrameSection == Qt::TopRightSection ||
        m_hoveredFrameSection == Qt::RightSection ||
        m_hoveredFrameSection == Qt::BottomRightSection ||
        m_hoveredFrameSection == Qt::BottomSection ||
        m_hoveredFrameSection == Qt::BottomLeftSection;
}

static inline QMargins qtn_margins_from_rects(const QRect& r1, const QRect& r2)
{
    return QMargins(r2.left() - r1.left(), r2.top() - r1.top(), r1.right() - r2.right(), r1.bottom() - r2.bottom());
}

void WindowTitleBarPrivate::updateTitleBarWidgetGeometry()
{
    m_textRect = QRect();
    if (window() == Q_NULL)
        return;

    m_textRect = getSubControlRect(QStyle::SC_TitleBarLabel);

    if (m_titleBarWidget != Q_NULL)
    {
        WindowTitleBar::TitleBarAlign align = titleBarWidgetAlign();
        bool alignLeft = align == WindowTitleBar::TitleBarAlignLeft;

        const QRect titleAreaRect = m_textRect;

        QString text = titleBarText();
        QSize textSize = QFontMetrics(window()->font()).size(Qt::TextSingleLine, text);

        QRect widgetRect = alignLeft ? titleAreaRect.adjusted(0, 0, -textSize.width(), 0) : titleAreaRect.adjusted(textSize.width(), 0, 0, 0);
        widgetRect.setWidth(qMax(widgetRect.width(), m_titleBarWidget->widget()->minimumWidth()));
        widgetRect.setWidth(qMin(widgetRect.width(), m_titleBarWidget->widget()->maximumWidth()));

        QSize widgetSizeHint = QSize(m_titleBarWidget->widget()->sizeHint().width(), titleAreaRect.height());

        widgetRect = QStyle::alignedRect(Qt::LeftToRight, 
            alignLeft ? Qt::AlignLeft | Qt::AlignVCenter : Qt::AlignRight | Qt::AlignVCenter, widgetSizeHint, widgetRect);

        if (align == WindowTitleBar::TitleBarAlignClient)
        {
            widgetRect.setLeft(titleAreaRect.left());
            widgetRect.setWidth(m_textRect.width());
        }

        widgetRect = widgetRect.intersected(titleAreaRect);
        
        QRect rect = getTitleBarRect();
        m_titleBarWidget->setMargins(qtn_margins_from_rects(rect, widgetRect));
        m_titleBarWidget->setGeometry(rect);

//        textSize.rwidth() = qMin(textSize.width(), qMax(0, geometry.width() - widgetRect.width()));
        textSize.rwidth() = qMax(0, titleAreaRect.width() - widgetRect.width());

        if (alignLeft)
            m_textRect.setLeft(m_textRect.left() + (m_textRect.width() - textSize.width()));
        else
            m_textRect.setRight(m_textRect.left() + textSize.width());
    }
}


bool WindowTitleBarPrivate::containsPoint(const QPoint& pos)
{
    QRegion region = getNonClientRegion();
    return region.contains(pos);
}

void WindowTitleBarPrivate::updateExtendedViewGeometry()
{
    if (m_extendedView == Q_NULL)
        return;
    m_extendedView->setGeometry(QRect(QPoint(0, 0), windowSize()));
    m_extendedView->raise();
    m_extendedView->show();
}

void WindowTitleBarPrivate::updateCursor()
{
    if (window() == Q_NULL)
    {
        unsetCursor();
        return;
    }
    
    if (defaultResizeHandler())
    {
        setCursor(getCursorForFrameSection(m_hoveredFrameSection));
    }
    else
    {
        bool hc = m_titleBarWidget != Q_NULL && m_titleBarWidget->hasCursor();
        if (hc)
            setCursor(m_titleBarWidget->cursor());
        else
            unsetCursor();
    }
}

QCursor WindowTitleBarPrivate::getCursorForFrameSection(Qt::WindowFrameSection frameSection)
{
    Qt::CursorShape cursorShape = Qt::ArrowCursor;
    switch (frameSection)
    {
    case Qt::LeftSection:
    case Qt::RightSection:
        cursorShape = Qt::SizeHorCursor;
        break;
    case Qt::BottomSection:
    case Qt::TopSection:
        cursorShape = Qt::SizeVerCursor;
        break;
    case Qt::TopLeftSection:
    case Qt::BottomRightSection:
        cursorShape = Qt::SizeFDiagCursor;
        break;
    case Qt::TopRightSection:
    case Qt::BottomLeftSection:
        cursorShape = Qt::SizeBDiagCursor;
        break;
    case Qt::TitleBarArea:
        cursorShape = Qt::ArrowCursor;
        break;
    default:
        break;
    }
    return QCursor(cursorShape);
}

int WindowTitleBarPrivate::getSysMenuButtonWidth() const
{
    if (window() == Q_NULL || !isSysMenuButtonVisible())
        return 0;

    int ret = window()->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, window()) + 4;
#if 0
    ret = qMax(ret, !m_icon.isNull() ? m_icon.availableSizes()[0].width() + 4 : 0);
#endif
    return ret;
}

bool WindowTitleBarPrivate::autoFillBackground() const
{
    return m_backgroundColor != Qt::transparent;
}

void WindowTitleBarPrivate::backstageWidgetUpdate(const QList<QRectF>& region)
{
    if (m_titleBarWidget == Q_NULL)
        return;

    for (QList<QRectF>::const_iterator it = region.constBegin(); it != region.constEnd(); ++it)
    {
        QRect r = (*it).toRect();
        if (r.width() == 0 || r.height() == 0)
            continue;
        update(r.translated(m_titleBarWidget->sceneRect().topLeft().toPoint()));
    }

    //Dirty hack to fix the bug in QGraphicsScene - QWidget::scroll()
    QList<QAbstractScrollArea *> areas = m_titleBarWidget->widget()->findChildren<QAbstractScrollArea *>();
    for (QList<QAbstractScrollArea *>::const_iterator it = areas.constBegin(); it != areas.constEnd(); ++it)
        (*it)->viewport()->update();
}

/* WindowTitleBarPrivateQt */
WindowTitleBarPrivateQt::WindowTitleBarPrivateQt()
    : WindowTitleBarPrivate(),
      m_frameSection(Qt::NoSection), m_startDragPos(QPoint(-1, -1))
{
}

WindowTitleBarPrivateQt::~WindowTitleBarPrivateQt()
{
    setWindow(Q_NULL);
}

void WindowTitleBarPrivateQt::setWindow(QWidget* window)
{
    resetWindow();
    installWindow(window);
}

void WindowTitleBarPrivateQt::installWindow(QWidget* _window)
{
    Q_ASSERT(window() == Q_NULL);
    if (_window == Q_NULL)
        return;
    m_oldFlags = _window->windowFlags();

#ifdef Q_OS_MAC
    qtn_set_window_hook(_window);
    qtn_set_window_frameless_mac(_window);
    _window->setWindowFlags(Qt::Window);
#else
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    _window->setWindowFlags(_window->windowFlags() | Qt::FramelessWindowHint);
#else
    _window->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif
#endif

    _window->setAttribute(Qt::WA_Hover, true);
    _window->setMouseTracking(true);
    WindowTitleBarPrivate::setWindow(_window);

    _window->show();

    resizeNonClientEvent(QSize(window()->width(), window()->height()));
}

void WindowTitleBarPrivateQt::resetWindow()
{
    if (window() == Q_NULL)
        return;

    QWidget* _window = window();
#ifdef Q_OS_MAC
    qtn_unset_window_hook(_window);
#endif
    WindowTitleBarPrivate::setWindow(Q_NULL);
    resizeNonClientEvent(QSize());
    updateCursor();
    if (!isDestroyed())
    {
        _window->setWindowFlags(m_oldFlags);
        _window->setAttribute(Qt::WA_Hover, false);
        _window->clearMask();
        _window->show();
    }
    m_oldFlags = 0;
}

bool WindowTitleBarPrivateQt::isActive() const
{
    return window() != Q_NULL ? window()->isActiveWindow() : false;
}

void WindowTitleBarPrivateQt::showSystemMenu(const QPoint& pos)
{
    Q_UNUSED(pos);
}

bool WindowTitleBarPrivateQt::mouseEventFilter(QObject* object, QEvent* event)
{
    if (QWidget::mouseGrabber() != Q_NULL)
        return false;

    bool result = false;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (object == window()->windowHandle())
#else
    if (object == window())
#endif
    {
        switch (event->type())
        {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        case QEvent::Enter:
#else
        case QEvent::HoverEnter:
#endif
        {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QEnterEvent* he = static_cast<QEnterEvent *>(event);
#else
            QHoverEvent* he = static_cast<QHoverEvent *>(event);
#endif
            checkUnderMouse(true, he->pos());
        }
        break;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        case QEvent::Leave:
#else
        case QEvent::HoverLeave:
#endif
        {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QPoint pos = QPoint(-1, -1);
#else
            QPoint pos = static_cast<QHoverEvent *>(event)->pos();
#endif
            checkUnderMouse(false, pos);
        }
        break;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        case QEvent::MouseMove:
#else
        case QEvent::HoverMove:
#endif
        {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            QMouseEvent* he = static_cast<QMouseEvent *>(event);
            QMouseEvent me(QEvent::MouseMove, he->pos(), he->button(), he->buttons(), he->modifiers());
#else
            QHoverEvent* he = static_cast<QHoverEvent *>(event);
            QMouseEvent me(QEvent::MouseMove, he->pos(), Qt::NoButton, Qt::NoButton, qApp->queryKeyboardModifiers());
#endif
            checkUnderMouse(true, he->pos());

            if (underMouse())
            {
                mouseMoveNonClientEvent(&me);
                if (me.isAccepted())
                    he->accept();
                result = true;
            }
        }
        break;
        case QEvent::MouseButtonPress:
        {
            QMouseEvent* me = static_cast<QMouseEvent *>(event);
            if (underMouse())
            {
                mousePressNonClientEvent(me);
                result = event->isAccepted();
            }
        }
        break;
        case QEvent::MouseButtonDblClick:
        {
            QMouseEvent* me = static_cast<QMouseEvent *>(event);
            if (underMouse())
            {
                mouseDoubleClickNonClientEvent(me);
                result = event->isAccepted();
            }
        }
        break;
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent* me = static_cast<QMouseEvent *>(event);
            if (underMouse())
            {
                mouseReleaseNonClientEvent(me);
                result = me->isAccepted();
            }
        }
        break;
        default:
            break;
        }
    }

    return result;
}

bool WindowTitleBarPrivateQt::eventFilter(QObject* object, QEvent* event)
{
    bool result = false;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (object == window()->windowHandle())
#else
    if (object == window())
#endif
    {
        switch (event->type())
        {
        case QEvent::Resize:
        {
#ifdef Q_OS_MAC
            qtn_window_resize_mac(window());
#endif
            QResizeEvent* re = static_cast<QResizeEvent *>(event);
            resizeNonClientEvent(re->size());
            window()->update();
        }
        break;
        default:
            break;
        }
    }

    if (object == window())
    {
        switch (event->type())
        {
        case QEvent::WindowStateChange:
            updateFrame();
            break;
        case QEvent::WindowActivate:
            nonClientActivateEvent(true);
            break;
        case QEvent::WindowDeactivate:
            nonClientActivateEvent(false);
            break;
        case QEvent::Paint:
        {
            QPaintEvent* pe = static_cast<QPaintEvent *>(event);
            {
                PaintEventQt peq(*pe);
                peq.setRegion(QRegion(getClientRect()).intersected(pe->region()));
                peq.setRect(getClientRect().intersected(pe->rect()));
                static_cast<QWidgetEx *>(window())->paintEvent(&peq);
            }
            result = true;

            if (extendViewIntoTitleBar())
                break;

            QRegion clip = getNonClientRegion().intersected(pe->region());
            QPainter painter(window());
            QPainter::CompositionMode mode = painter.compositionMode();
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            QVector<QRect> rects = clip.rects();
            for (int i = 0; i < rects.count(); ++i)
            {
                QRect r = rects[i];
                painter.fillRect(r, Qt::white);
            }
            painter.setCompositionMode(mode);
            painter.setClipRegion(clip);
            paintNonClientEvent(&painter);
        }
        break;

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        case QEvent::ContextMenu:
        {
            QContextMenuEvent* ce = static_cast<QContextMenuEvent *>(event);
            QRegion region = getNonClientRegion();
            if (region.contains(ce->pos()))
            {
                mouseContextNonClientEvent(ce);
                result = true;
            }
        }
        break;
#endif
        default:
        break;
        }
    }

    if (result || mouseEventFilter(object, event))
        return true;

    return WindowTitleBarPrivate::eventFilter(object, event);
}

void WindowTitleBarPrivateQt::mousePressNonClientEvent(QMouseEvent* event)
{
    WindowTitleBarPrivate::mousePressNonClientEvent(event);

    if (!event->isAccepted() && event->button() == Qt::LeftButton)
    {
        if (m_hoveredFrameSection != Qt::NoSection)
        {
            startDrag(event->globalPos(), m_hoveredFrameSection);
            event->accept();
        }
    }

#if 0
    event->ignore();
    QRegion region = getNonClientRegion();
    if (region.contains(event->pos()))
        WindowTitleBarPrivate::mousePressNonClientEvent(event);
    if (event->isAccepted())
        return;

    if (event->button() == Qt::LeftButton)
    {
        Qt::WindowFrameSection fs = checkFrameSection(event->pos());
        if (fs != Qt::NoSection)
        {
            startDrag(event->globalPos(), fs);
            event->accept();
        }
    }
#endif
}

void WindowTitleBarPrivateQt::mouseReleaseNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    if (isDragMode())
    {
        stopDrag();
        checkUnderMouse(true, event->pos());
        event->accept();
        return;
    }

    WindowTitleBarPrivate::mouseReleaseNonClientEvent(event);
}

void WindowTitleBarPrivateQt::mouseMoveNonClientEvent(QMouseEvent* event)
{
    event->ignore();

    if (isDragMode())
    {
        dragMove(event->globalPos());
        event->accept();
        return;
    }

    WindowTitleBarPrivate::mouseMoveNonClientEvent(event);
}

bool WindowTitleBarPrivateQt::isDragMode() const
{
    return QWidget::mouseGrabber() == window();
}

void WindowTitleBarPrivateQt::startDrag(const QPoint& pos, Qt::WindowFrameSection frameSection)
{
#ifdef Q_OS_MAC
    if (qtn_window_start_native_drag_mac(window()))
        return;
#endif

    m_startDragPos = pos;
    m_frameSection = frameSection;
    m_grabGeometry = window()->geometry();
    window()->grabMouse();
    setCursor(getCursorForFrameSection(m_frameSection));
}

void WindowTitleBarPrivateQt::stopDrag()
{
    m_frameSection = Qt::NoSection;
    m_startDragPos = QPoint(-1, -1);
    window()->releaseMouse();
    unsetCursor();
}

void WindowTitleBarPrivateQt::dragMove(const QPoint& pos)
{
    QRect geometry = m_grabGeometry;

    QPoint offset = pos - m_startDragPos;

    if (m_frameSection == Qt::TitleBarArea)
    {
        geometry.moveTo(geometry.topLeft() + offset);
    }
    else
    {
        if (m_frameSection == Qt::RightSection || m_frameSection == Qt::TopRightSection || m_frameSection == Qt::BottomRightSection)
        {
            int newWidth = geometry.width() + offset.x();
            newWidth = qMin(qMax(newWidth, window()->minimumWidth()), window()->maximumWidth());
            geometry.adjust(0, 0, newWidth - geometry.width(), 0);
        }

        if (m_frameSection == Qt::LeftSection || m_frameSection == Qt::TopLeftSection || m_frameSection == Qt::BottomLeftSection)
        {
            int newWidth = geometry.width() - offset.x();
            newWidth = qMin(qMax(newWidth, window()->minimumWidth()), window()->maximumWidth());
            geometry.adjust(geometry.width() - newWidth, 0, 0, 0);
        }

        if (m_frameSection == Qt::BottomSection || m_frameSection == Qt::BottomLeftSection || m_frameSection == Qt::BottomRightSection)
        {
            int newHeight = geometry.height() + offset.y();
            newHeight = qMin(qMax(newHeight, window()->minimumHeight()), window()->maximumHeight());
            geometry.adjust(0, 0, 0, newHeight - geometry.height());
        }

        if (m_frameSection == Qt::TopSection || m_frameSection == Qt::TopLeftSection || m_frameSection == Qt::TopRightSection)
        {
            int newHeight = geometry.height() - offset.y();
            newHeight = qMin(qMax(newHeight, window()->minimumHeight()), window()->maximumHeight());
            geometry.adjust(0, geometry.height() - newHeight, 0, 0);
        }
    }
    if (window()->geometry() != geometry)
        window()->setGeometry(geometry);
}

void WindowTitleBarPrivateQt::checkUnderMouse(bool mouseOverWindow, const QPoint& pos)
{
    if (isDragMode() || (mouseOverWindow && m_pressed != QStyle::SC_None))
        return;

    bool underMouse = mouseOverWindow && checkFrameSection(pos) != Qt::NoSection;

    if (m_nonClientUnderMouse && !underMouse)
        mouseNonClientOut(pos);
    else if (!m_nonClientUnderMouse && underMouse)
        mouseNonClientOver(pos);

    m_nonClientUnderMouse = underMouse;
}

void WindowTitleBarPrivateQt::updateFrame()
{
    if (window() == Q_NULL)
        return;

    Qt::WindowStates states = window()->windowState();

    bool changed = false;
    bool m = states & Qt::WindowMaximized;
    bool fs = states & Qt::WindowFullScreen;
    if (m_maximized != m)
    {
        m_maximized = m;
        changed = true;
    }
    if (m_fullScreen != fs)
    {
        m_fullScreen = fs;
        changed = true;
    }

    QMargins contentsMargins = m_origContentsMargins;
    
    if (!isFullScreen())
        contentsMargins = frameMargins() + m_origContentsMargins;

    if (contentsMargins != window()->contentsMargins())
    {
        window()->setContentsMargins(contentsMargins);
        changed = true;
    }

    if (changed)
        WindowTitleBarPrivate::updateFrame();
}

void WindowTitleBarPrivateQt::processContextMenuEvent(QMouseEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    WindowTitleBarPrivate::processContextMenuEvent(event);
#else
    Q_UNUSED(event);
#endif
}

void WindowTitleBarPrivateQt::resizeNonClientEvent(const QSize& windowSize)
{
    WindowTitleBarPrivate::resizeNonClientEvent(windowSize);
    updateFrame();
}

/* ExendedView */
ExendedView::ExendedView(WindowTitleBarPrivate* bar)
    : QWidget(bar->window()), m_bar(bar)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ExendedView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRegion(m_bar->getNonClientRegion().intersected(event->region()));
    m_bar->paintNonClientEvent(&painter);
}

bool Qtitan::qtn_image_copy(QImage& dest, const QPoint& p, const QImage& src)
{

    if (dest.format() != src.format() ||
        (dest.format() != QImage::Format_ARGB32_Premultiplied && dest.format() != QImage::Format_ARGB32))
    {
        Q_ASSERT(false);
        return false;
    }

    int w = src.width();
    int h = src.height();
    if (w <= 0 || h <= 0)
    {
        Q_ASSERT(false);
        return false;
    }

    int x = p.x();
    int y = p.y();

    int dx = 0;
    int dy = 0;

    if (x < 0 || y < 0 || x + w > dest.width() || y + h > dest.height())
    {
        if (x < 0) {
            dx = -x;
            x = 0;
        }
        if (y < 0) {
            dy = -y;
            y = 0;
        }
    }

    int pixels_to_copy = qMax(w - dx, 0);
    if (x > dest.width())
        pixels_to_copy = 0;
    else if (pixels_to_copy > dest.width() - x)
        pixels_to_copy = dest.width() - x;
    int lines_to_copy = qMax(h - dy, 0);
    if (y > dest.height())
        lines_to_copy = 0;
    else if (lines_to_copy > dest.height() - y)
        lines_to_copy = dest.height() - y;

    bool aligned = true;
    bool changed = false;
    if (aligned)
    {
        uchar *dest_bits = dest.bits() + ((x * dest.depth()) >> 3) + y * dest.bytesPerLine();
        const uchar *src_bits = src.bits() + ((dx * src.depth()) >> 3) + dy * src.bytesPerLine();
        const int bytes_to_copy = (pixels_to_copy * dest.depth()) >> 3;

        for (int i = 0; i < lines_to_copy; ++i)
        {
            changed = changed || memcmp(dest_bits, src_bits, bytes_to_copy) != 0;
            if (changed)
                memcpy(dest_bits, src_bits, bytes_to_copy);
            dest_bits += dest.bytesPerLine();
            src_bits += src.bytesPerLine();
        }
    }
    return changed;
}

static bool origSegoeMDL2 = false;
static void qtn_initSegoeMDL2AssetsFont()
{
    static bool initSegoeMDL2Assets = false;
    if (initSegoeMDL2Assets)
        return;
    initSegoeMDL2Assets = true;

    if (QFontInfo(QFont("Segoe MDL2 Assets")).exactMatch())
    {
        origSegoeMDL2 = true;
        return;
    }
    if (QFontInfo(QFont("micon")).exactMatch())
        return;

    //Use MIT license based "Micon" font as an alternative for "Segoe MDL2 Assets"
    QFontDatabase::addApplicationFont(":/res/micon.ttf");
}

QFont Qtitan::qtn_getSegoeMDL2AssetsFont()
{
    qtn_initSegoeMDL2AssetsFont();
    if (origSegoeMDL2)
        return QFont("Segoe MDL2 Assets");
    else
        return QFont("micon");
}
