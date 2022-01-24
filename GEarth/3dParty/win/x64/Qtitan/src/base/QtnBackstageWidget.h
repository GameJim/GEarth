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

#ifndef QTN_BACKSTAGEWIDGET_H
#define QTN_BACKSTAGEWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#endif

#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE


class BackstageWidget : public QGraphicsScene
{
public:
    BackstageWidget(QWidget* parentWidget);
    virtual ~BackstageWidget();
    QWidget* parentWidget();
    void setParentWidget(QWidget* widget);
    void setWidget(QWidget* widget, const QRect& geometry);
    QWidget* widget() const;
    QGraphicsProxyWidget* widgetItem() const;
    void activate();
    void deactivate();
    void setMargins(int left, int top, int right, int bottom);
    void setMargins(const QMargins& margins);
    const QMargins& margins() const;

    void paintContent(QPainter& painter);

    /* mouse events */
    void sendMouseMove(QMouseEvent* e);
    void sendMousePress(QMouseEvent* e);
    void sendMouseRelease(QMouseEvent* e);
    void sendMouseDoubleClick(QMouseEvent* e);
    void sendMouseOver(const QPoint& pos);
    void sendMouseOut(const QPoint& pos);
    void sendWheel(QWheelEvent *);
    /* keyboard events */
    void sendKeyPress(QKeyEvent* e);
    void sendKeyRelease(QKeyEvent* e);
    void sendShortcutOverride(QKeyEvent* e);
    /* focus events */
    void sendFocusIn(QFocusEvent *);
    void sendFocusOut(QFocusEvent *);
    void sendShowContextMenu(QContextMenuEvent* event);
public:
    QCursor cursor() const;
    bool hasCursor() const;
    void setGeometry(const QRect& geometry, bool force = false);
    QRect geometry() const;
private:
    void setInternalWinPos(const QPoint& globalPos) const;
    void connectWidget(const QRect& geometry);
    void disconnectWidget();
    void checkCursor(const QPoint& p);
    QWidget* m_parentWidget;
    QPointF m_mousePressPosition;
    QPoint m_mousePressScreenPosition;
    Qt::MouseButton m_mousePressButton;
    QPointF m_lastMouseMoveScenePosition;
    QPoint m_lastMouseMoveScreenPosition;

    QGraphicsProxyWidget* m_proxy;
    QWidget* m_widget;
    QMargins m_margins;
    QWidget* m_cursorWidget;
};

QTITAN_END_NAMESPACE

#endif //QTN_BACKSTAGEWIDGET_H

