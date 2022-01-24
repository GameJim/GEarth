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


#ifndef QTN_WINDOWTITLEBAR_PRIVATE_H
#define QTN_WINDOWTITLEBAR_PRIVATE_H

#include <QWidget>
#include <QStyleOptionTitleBar>

#include "QtitanDef.h"
#include "QtnWindowTitleBar.h"
#include "QtnBackstageWidget.h"
#include "QtnAcrylicWidget.h"

QTITAN_BEGIN_NAMESPACE

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q_DECL_CONSTEXPR inline QMargins operator+(const QMargins &m1, const QMargins &m2) Q_DECL_NOTHROW
{
    return QMargins(m1.left() + m2.left(), m1.top() + m2.top(),
        m1.right() + m2.right(), m1.bottom() + m2.bottom());
}

Q_DECL_CONSTEXPR inline QMargins operator-(const QMargins &m1, const QMargins &m2) Q_DECL_NOTHROW
{
    return QMargins(m1.left() - m2.left(), m1.top() - m2.top(),
        m1.right() - m2.right(), m1.bottom() - m2.bottom());
}
#endif

class WindowStyleHelperAbstract
{
public:
    WindowStyleHelperAbstract()
        : m_window(Q_NULL), m_destroyed(false) {}
    virtual ~WindowStyleHelperAbstract() {}
public:
    const QMargins& frameMargins() const { return m_frameMargins; }
    void setFrameMargins(const QMargins& margins) { m_frameMargins = margins; }
    virtual void setWindow(QWidget* window) { m_window = window; }
    QWidget* window() const { return m_window; };
    void setWindowSize(const QSize& size) { m_windowSize = size; }
    const QSize& windowSize() const { return m_windowSize; };
    virtual void update(const QRect& rect) = 0;
    virtual void update(const QRegion& region) = 0;
    virtual bool isActive() const = 0;
    virtual bool underMouse() const = 0;
    virtual void showSystemMenu(const QPoint& pos) = 0;
    void setDestroyed() { m_destroyed = true; }
    bool isDestroyed() const { return m_destroyed; }
protected:
    virtual void nonClientActivateEvent(bool active) = 0;
    virtual void paintNonClientEvent(QPainter* painter) = 0;
    virtual void mousePressNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseReleaseNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseDoubleClickNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseMoveNonClientEvent(QMouseEvent* event) = 0;
    virtual void mouseNonClientOver(const QPoint& pos) = 0;
    virtual void mouseNonClientOut(const QPoint& pos) = 0;
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event) = 0;
    virtual void resizeNonClientEvent(const QSize& windowSize) = 0;
private:
    QWidget* m_window;
    QMargins m_frameMargins;
    QSize m_windowSize;
    bool m_destroyed;
};

class WindowTitleBarPrivate : public QObject, public WindowStyleHelperAbstract
{
    Q_OBJECT
public:
    friend class ExendedView;
    friend class WindowTitleBar;
    WindowTitleBarPrivate();
    virtual ~WindowTitleBarPrivate();
    virtual void setWindow(QWidget* window);
    virtual bool underMouse() const;
    void setTitleBarWidget(QWidget* widget);
    QWidget* titleBarWidget() const;
    QString titleBarText() const;
    WindowTitleBar::TitleBarAlign titleBarWidgetAlign() const;
    void setTitleBarWidgetAlign(WindowTitleBar::TitleBarAlign align);
    bool hasFocus() const;
    QRect getClientRect() const;
    QRegion getNonClientRegion() const;
    QRect getTitleBarRect();
    QStyle::SubControl hitTest(const QPoint& pos, Qt::WindowFrameSection& frameSection);
    bool styledFrame() const;
    void setStyledFrame(bool styled);
    bool blurWindowEnabled() const;
    void setBlurWindowEnabled(bool enabled);
    bool extendViewIntoTitleBar() const;
    void setExtendViewIntoTitleBar(bool extend);
    bool isSysMenuButtonVisible() const;
    void setSysMenuButtonVisible(bool visible);
    const QColor& backgroundColor() const;
    void setBackgroundColor(const QColor& color);
    const QMargins& systemFrameMargins() const;
    int borderThickness() const;
    int titleHeight() const;
    bool styledIconAndText() const;
    bool isMaximized() const;
    bool isFullScreen() const;
    void updateLayout();
    virtual void updateFrame();
    virtual void update(const QRect& rect);
    virtual void update(const QRegion& region);
    void setCursor(const QCursor& cursor);
    void unsetCursor();
protected:
    virtual bool eventFilter(QObject* object, QEvent* event);
protected:
    virtual void nonClientActivateEvent(bool active);
    virtual void paintNonClientEvent(QPainter* painter);
    virtual void mousePressNonClientEvent(QMouseEvent* event);
    virtual void mouseReleaseNonClientEvent(QMouseEvent* event);
    virtual void mouseDoubleClickNonClientEvent(QMouseEvent* event);
    virtual void mouseMoveNonClientEvent(QMouseEvent* event);
    virtual void mouseNonClientOver(const QPoint& pos);
    virtual void mouseNonClientOut(const QPoint& pos);
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
protected:
    virtual QRect getSubControlRect(QStyle::SubControl sc);
    virtual QStyle::SubControl getHoverSubControl(const QPoint& pos);
    virtual void styleChanged();
    virtual void updateSysButtonsRect();
    virtual QStyle::SubControl getHoverNativeSubControl(const QPoint& pos);
    virtual void paintTitleBarIcon(QPainter* painter, const QStyleOptionTitleBar& option);
    virtual void paintTitleBarText(QPainter* painter, const QStyleOptionTitleBar& option);
protected:
    void processExtendViewEvent(QEvent* event);
    Qt::WindowFrameSection checkFrameSection(const QPoint& pos);
    void initTitleBarOption(QStyleOptionTitleBar& option);
    void updateSubControl(const QPoint& pos);
    void setSubControlPressed(QStyle::SubControl sc);
    void subControlClicked(QMouseEvent* event);
    virtual void processContextMenuEvent(QMouseEvent* event);
    bool defaultResizeHandler();
    void updateTitleBarWidgetGeometry();
    bool containsPoint(const QPoint& pos);
    void updateExtendedViewGeometry();
    void updateCursor();
    QCursor getCursorForFrameSection(Qt::WindowFrameSection frameSection);
    int getSysMenuButtonWidth() const;
    bool autoFillBackground() const;
    Qt::WindowFlags m_oldFlags;
    BackstageWidget* m_titleBarWidget;
    WindowTitleBar::TitleBarAlign m_titleBarWidgetAlign;
    QRect m_textRect;
    Qt::WindowFrameSection m_hoveredFrameSection;
    QStyle::SubControl m_hovered;
    QStyle::SubControl m_pressed;
    bool m_nonClientUnderMouse;
    bool m_styledFrame;
    bool m_blurWindowEnabled;
    bool m_extendViewIntoTitleBar;
    QMargins m_systemFrameMargins;
    QRect m_systemButtonsRect;
    int m_borderThickness;
    int m_titleHeight;
    int m_borderThicknessRequested;
    int m_titleHeightRequested;
    QIcon m_icon;
    bool m_maximized;
    bool m_fullScreen;
    bool m_sysMenuButtonVisible;
    QColor m_backgroundColor;
    QColor m_colorizationColor;
    QMargins m_frameWindows10;
    QWidget* m_extendedView;
    QMargins m_origContentsMargins;
    bool m_hasCursor;
private Q_SLOTS:
    void  backstageWidgetUpdate(const QList<QRectF>& region);
};

class WindowTitleBarPrivateQt : public WindowTitleBarPrivate
{
public:
    WindowTitleBarPrivateQt();
    virtual ~WindowTitleBarPrivateQt();
    virtual void setWindow(QWidget* window);
    virtual bool isActive() const;
    virtual void showSystemMenu(const QPoint& pos);
protected:
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void mousePressNonClientEvent(QMouseEvent* event);
    virtual void mouseReleaseNonClientEvent(QMouseEvent* event);
    virtual void mouseMoveNonClientEvent(QMouseEvent* event);
    virtual void processContextMenuEvent(QMouseEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
private:
    bool mouseEventFilter(QObject* object, QEvent* event);
    void installWindow(QWidget* window);
    void resetWindow();
    bool isDragMode() const;
    void startDrag(const QPoint& pos, Qt::WindowFrameSection frameSection);
    void stopDrag();
    void dragMove(const QPoint& pos);
    virtual void checkUnderMouse(bool mouseOverWindow, const QPoint& pos);
    virtual void updateFrame();
private:
    Qt::WindowFrameSection m_frameSection;
    QPoint m_startDragPos;
    QRect m_grabGeometry;
};

class ExendedView : public QWidget
{
public:
    ExendedView(WindowTitleBarPrivate* bar);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    WindowTitleBarPrivate* m_bar;
};

QTITAN_END_NAMESPACE

#endif //QTN_WINDOWTITLEBAR_PRIVATE_H


