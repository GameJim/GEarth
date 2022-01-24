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
#include <QWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent> 

#include "QtnPlatform.h"
#include "QtnBackstageWidget.h"

QTITAN_USE_NAMESPACE

#ifdef _MSC_VER
static
#endif
bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event)
{
    return QCoreApplication::sendSpontaneousEvent(receiver, event);
}

/* BackstageWidget */
BackstageWidget::BackstageWidget(QWidget* parentWidget)
    : QGraphicsScene(), m_parentWidget(parentWidget), m_mousePressButton(Qt::NoButton), 
    m_proxy(Q_NULL), m_widget(Q_NULL), m_cursorWidget(Q_NULL)
{
}

BackstageWidget::~BackstageWidget()
{
    disconnectWidget();
    Q_DELETE_AND_NULL(m_widget);
}

QWidget* BackstageWidget::parentWidget()
{
    return m_parentWidget;
}

void BackstageWidget::setParentWidget(QWidget* widget)
{
    m_parentWidget = widget;
}

void BackstageWidget::setWidget(QWidget* widget, const QRect& geometry)
{
    if (m_widget == widget)
        return;
    if (m_widget != Q_NULL)
        disconnectWidget();
    m_widget = widget;
    connectWidget(geometry);
}

QWidget* BackstageWidget::widget() const
{
    return m_widget;
}

QGraphicsProxyWidget* BackstageWidget::widgetItem() const
{
    return m_proxy;
}

void BackstageWidget::connectWidget(const QRect& geometry)
{
    if (m_widget == Q_NULL)
        return;

    m_widget->setWindowFlags(m_widget->windowFlags() | Qt::BypassGraphicsProxyWidget);  //Means all toplevel/popup child windows will be created natively.
    Q_ASSERT(m_proxy == Q_NULL);
    m_proxy = addWidget(m_widget);
    m_proxy->setMinimumSize(0, 0);
    setGeometry(geometry, true);
}

void BackstageWidget::disconnectWidget()
{
    m_cursorWidget = Q_NULL;
    if (m_proxy == Q_NULL)
        return;
    m_proxy->setWidget(Q_NULL);
    Q_DELETE_AND_NULL(m_proxy);
}

void BackstageWidget::checkCursor(const QPoint& p)
{
    m_cursorWidget = Q_NULL;
    QWidget* w = m_widget != Q_NULL ? m_widget->childAt(p) : Q_NULL;
    if (w == Q_NULL && m_widget != Q_NULL && m_widget->underMouse())
        w = m_widget;

    while (w)
    {
        if (w->testAttribute(Qt::WA_SetCursor))
        {
            m_cursorWidget = w;
            break;
        }
        w = w->parentWidget();
    }
}

void BackstageWidget::activate()
{
    if (isActive())
        return;
    QEvent event(QEvent::WindowActivate);
    qt_sendSpontaneousEvent(this, &event);
}

void BackstageWidget::deactivate()
{
    if (!isActive())
        return;
    QEvent event(QEvent::WindowDeactivate);
    qt_sendSpontaneousEvent(this, &event);
}

void BackstageWidget::setMargins(int left, int top, int right, int bottom)
{
    setMargins(QMargins(left, top, right, bottom));
}

const QMargins& BackstageWidget::margins() const
{
    return m_margins;
}

void BackstageWidget::setMargins(const QMargins& margins)
{
    if (m_margins == margins)
        return;
    m_margins = margins;
    setGeometry(QRect(0, 0, width(), height()), true);
}

void BackstageWidget::paintContent(QPainter& painter)
{
    if (m_proxy == Q_NULL)
        return;

    if (painter.opacity() == 0)
        return;

    QRect rect(0, 0, width(), height());
    rect.adjust(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
    rect.translate(sceneRect().left(), sceneRect().top());

    QRect clipRect = painter.hasClipping() ? painter.clipBoundingRect().toRect() : 
        QRect(0, 0, painter.device()->width(), painter.device()->height());
    clipRect = clipRect.intersected(rect);
    if (clipRect.isEmpty())
        return;

    painter.save();

    painter.setClipRect(clipRect);
    painter.translate(rect.left(), rect.top());

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if (painter.opacity() == 1.0)
        painter.setOpacity(0.9999); //Hack to fix the flickering under Qt4.
#endif

#if 1
    QStyleOptionGraphicsItem option;
    option.exposedRect = QRect(0, 0, rect.width(), rect.height());
    m_proxy->paint(&painter, &option, Q_NULL);
#else
    Q_ASSERT(false);
    m_scene.render(&painter, paintRect, paintRect);
#endif
    painter.restore();
}

void BackstageWidget::sendMouseMove(QMouseEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent.setWidget(parentWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(e->pos() - sceneRect().topLeft());
    mouseEvent.setScreenPos(e->globalPos());
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    m_lastMouseMoveScenePosition = mouseEvent.scenePos();
    m_lastMouseMoveScreenPosition = mouseEvent.screenPos();
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(this, &mouseEvent);

    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);

    checkCursor(mouseEvent.scenePos().toPoint());
}

void BackstageWidget::sendMousePress(QMouseEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setInternalWinPos(e->globalPos() + sceneRect().topLeft().toPoint() - e->pos());

    m_mousePressButton = e->button();
    m_mousePressPosition = e->pos() - sceneRect().topLeft();
    m_mousePressScreenPosition = e->globalPos();
    m_lastMouseMoveScenePosition = m_mousePressPosition;
    m_lastMouseMoveScreenPosition = m_mousePressScreenPosition;

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent.setWidget(parentWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(m_mousePressPosition);
    mouseEvent.setScreenPos(m_mousePressScreenPosition);
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(this, &mouseEvent);

    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
}

void BackstageWidget::sendMouseRelease(QMouseEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setInternalWinPos(e->globalPos() + sceneRect().topLeft().toPoint() - e->pos());

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
    mouseEvent.setWidget(parentWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(e->pos() - sceneRect().topLeft());
    mouseEvent.setScreenPos(e->globalPos());
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(this, &mouseEvent);

    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
}

void BackstageWidget::sendMouseDoubleClick(QMouseEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setInternalWinPos(e->globalPos() + sceneRect().topLeft().toPoint() - e->pos());

    m_mousePressButton = e->button();
    m_mousePressPosition = e->pos() - sceneRect().topLeft();
    m_mousePressScreenPosition = e->globalPos();
    m_lastMouseMoveScenePosition = m_mousePressPosition;
    m_lastMouseMoveScreenPosition = m_mousePressScreenPosition;

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseDoubleClick);
    mouseEvent.setWidget(parentWidget());

    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(m_mousePressPosition);
    mouseEvent.setScreenPos(m_mousePressScreenPosition);
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(this, &mouseEvent);

    const bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
}

void BackstageWidget::sendMouseOver(const QPoint& pos)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
        return;

    QPoint screenPos = QCursor::pos();
    QGraphicsSceneHoverEvent hoverEvent(QEvent::GraphicsSceneHoverEnter);
    hoverEvent.setScenePos(pos - sceneRect().topLeft());
    hoverEvent.setScreenPos(screenPos);
    hoverEvent.setLastScenePos(hoverEvent.scenePos());
    hoverEvent.setLastScreenPos(hoverEvent.screenPos());
    hoverEvent.setWidget(parentWidget());

    qt_sendSpontaneousEvent(this, &hoverEvent);

    checkCursor(hoverEvent.scenePos().toPoint());
}

void BackstageWidget::sendMouseOut(const QPoint& pos)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
        return;

    if (mouseGrabberItem() != Q_NULL)
        mouseGrabberItem()->ungrabMouse();

    QPoint screenPos = QCursor::pos();
    QGraphicsSceneHoverEvent hoverEvent(QEvent::GraphicsSceneHoverLeave);
    hoverEvent.setScenePos(pos - sceneRect().topLeft());
    hoverEvent.setScreenPos(screenPos);
    hoverEvent.setLastScenePos(hoverEvent.scenePos());
    hoverEvent.setLastScreenPos(hoverEvent.screenPos());

    hoverEvent.setWidget(parentWidget());

    qt_sendSpontaneousEvent(this, &hoverEvent);

    checkCursor(hoverEvent.scenePos().toPoint());
}

void BackstageWidget::sendWheel(QWheelEvent* e)
{
    e->ignore();
    if (m_widget == Q_NULL || !m_widget->isEnabled())
        return;

    QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
    wheelEvent.setWidget(parentWidget());
    wheelEvent.setScenePos(e->pos() - sceneRect().topLeft());
    wheelEvent.setScreenPos(e->globalPos());
    wheelEvent.setButtons(e->buttons());
    wheelEvent.setModifiers(e->modifiers());
    wheelEvent.setDelta(e->delta());
    wheelEvent.setOrientation(e->orientation());
    wheelEvent.setAccepted(false);

    qt_sendSpontaneousEvent(this, &wheelEvent);

    e->setAccepted(wheelEvent.isAccepted());
}

void BackstageWidget::sendKeyPress(QKeyEvent* e)
{
    e->ignore();
    if (m_widget == Q_NULL || !m_widget->isEnabled())
        return;

    const bool keyTab = (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab);
    if (keyTab)
        e->ignore();
    QWidget* focus = m_widget->focusWidget();
    QApplication::sendEvent(this, e);
    if (keyTab && focus == m_widget->focusWidget())
            e->ignore(); //QGraphicsProxyWidget::focusNextPrevChild(bool next) actually returned false, 
                         //but event is accepted by default. We have to fix it. 
                         //See for details - QGraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
}

void BackstageWidget::sendKeyRelease(QKeyEvent* e)
{
    e->ignore();
    if (m_widget == Q_NULL || !m_widget->isEnabled())
        return;

    QApplication::sendEvent(this, e);
}

void BackstageWidget::sendShortcutOverride(QKeyEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    QApplication::sendEvent(this, e);
}

void BackstageWidget::sendFocusIn(QFocusEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setFocus(e->reason());
    if (focusItem() == Q_NULL)
    {
        m_widget->setFocus(e->reason());
    }
}

void BackstageWidget::sendFocusOut(QFocusEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
    setFocusItem(Q_NULL, e->reason());
}

void BackstageWidget::sendShowContextMenu(QContextMenuEvent* e)
{
    if (m_widget == Q_NULL || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
    contextEvent.setWidget(parentWidget());
    contextEvent.setScenePos(e->pos() - sceneRect().topLeft());
    contextEvent.setScreenPos(e->globalPos());
    contextEvent.setModifiers(e->modifiers());
    contextEvent.setReason((QGraphicsSceneContextMenuEvent::Reason)(e->reason()));
    contextEvent.setAccepted(e->isAccepted());

    qt_sendSpontaneousEvent(this, &contextEvent);

    e->setAccepted(contextEvent.isAccepted());
}

QCursor BackstageWidget::cursor() const
{
    if (hasCursor())
        return m_cursorWidget->cursor();
    else
        return QCursor(Qt::ForbiddenCursor);
}

bool BackstageWidget::hasCursor() const
{
    return m_cursorWidget != Q_NULL;
}

void BackstageWidget::setGeometry(const QRect& geometry, bool force)
{
    if (m_widget == Q_NULL)
        return;

    setSceneRect(geometry);
    
    QRect r = QRect(m_margins.left(), m_margins.top(), 
        geometry.size().width() - (m_margins.left() + m_margins.right()),
        geometry.size().height() - (m_margins.top() + m_margins.bottom()));

    r.setWidth(qMax(r.width(), 0));
    r.setHeight(qMax(r.height(), 0));

    if (force || m_widget->geometry() != r)
    {
        m_widget->setGeometry(r);

        if (r.width() == 0 || r.height() == 0)
        {
            //#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            m_widget->setVisible(false);
            m_widget->setAttribute(Qt::WA_Mapped, false);
            //#endif
        }
        else
        {
            //#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            m_widget->setVisible(true);
            m_widget->setAttribute(Qt::WA_Mapped, true);
            //#endif
        }
    }
}

QRect BackstageWidget::geometry() const
{
    return sceneRect().toRect();
}

void BackstageWidget::setInternalWinPos(const QPoint& globalPos) const
{
    if (m_widget == Q_NULL)
        return;
    QPoint gp = globalPos + QPoint(margins().left(), margins().top());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QWindow* w = m_widget->windowHandle();
    if (w != Q_NULL)
        w->setGeometry(QRect(gp, w->geometry().size()));
#else
    qtn_setWidgetPostion(m_widget, gp);
#endif
}
