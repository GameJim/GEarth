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

#ifndef QTN_PLATFORM_H
#define QTN_PLATFORM_H

#include "QtitanDef.h"

#include <QWidget>
#include <QMouseEvent>
#include <QStyleOption>

#ifdef Q_OS_WIN
#include "QtnPlatform_win.h"
#endif
#ifdef Q_OS_LINUX
#endif
#ifdef Q_OS_MAC
#endif

QT_BEGIN_NAMESPACE

class MouseEventQt : public QMouseEvent
{
public:
    MouseEventQt(MouseEventQt& e) : QMouseEvent(e) {}
    MouseEventQt(QMouseEvent& e) : QMouseEvent(e) {}
    void setPos(const QPoint& pos) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))        
        this->l = pos;
        this->w = pos;
#else
        this->p = pos;
#endif
    }
};

class WheelEventQt : public QWheelEvent
{
public:
    WheelEventQt(WheelEventQt& e) : QWheelEvent(e) {}
    WheelEventQt(QWheelEvent& e) : QWheelEvent(e) {}
    void setPos(const QPoint& pos) {
        this->p = pos;
    }
};

class ContextMenuEventQt : public QContextMenuEvent
{
public:
    ContextMenuEventQt(ContextMenuEventQt& e) : QContextMenuEvent(e) {}
    ContextMenuEventQt(QContextMenuEvent& e) : QContextMenuEvent(e) {}
    void setPos(const QPoint& pos) {
        this->p = pos;
    }
};

class PaintEventQt : public QPaintEvent
{
public:
    PaintEventQt(PaintEventQt& e) : QPaintEvent(e) {}
    PaintEventQt(QPaintEvent& e) : QPaintEvent(e) {}
    void setRect(const QRect& rect) { this->m_rect = rect; }
    void setRegion(const QRegion& region) { this->m_region = region; }
};

Q_GUI_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
Q_GUI_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
Q_GUI_EXPORT QImage qt_halfScaled(const QImage &source);

QT_END_NAMESPACE

QTITAN_BEGIN_NAMESPACE

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
enum HBitmapFormat
{
    HBitmapNoAlpha,
    HBitmapPremultipliedAlpha,
    HBitmapAlpha
};
#endif

#ifdef Q_OS_MAC
bool qtn_set_window_hook(QWidget* w);
bool qtn_unset_window_hook(QWidget* w);
bool qtn_set_window_frameless_mac(QWidget* w);
void qtn_paint_title_bar_mac(const QStyleOptionComplex *option, QPainter* painter, const QWidget *widget);
bool qtn_window_start_native_drag_mac(QWidget* w);
void qtn_window_resize_mac(QWidget* w);
QPixmap qtn_titlebar_standard_pixmap(QStyle::StandardPixmap sp, const QStyleOption* opt, const QWidget* widget);
#endif

void qtn_initializeDrawingResources();
void qtn_setWidgetPostion(QWidget* widget, const QPoint& position);
void qtn_paintTitleBarText(QPainter* painter, const QString& text, const QRect& rect, bool active, const QColor& color = QColor());
void qtn_paintTitleBarIcon(QPainter* painter, const QIcon& icon, const QRect& rect);
QPixmap qtn_getTitleBarIcon(QWidget* widget);
void qtn_paintAirEffect(QPainter* painter, const QRect& rect, const QRegion& clip);
bool qtn_setBlurBehindWindow(QWidget* widget);
bool qtn_image_copy(QImage& dest, const QPoint& p, const QImage& src);
QFont qtn_getSegoeMDL2AssetsFont();
QColor qtn_getColorizationColor();

QTITAN_END_NAMESPACE


#endif //QTN_PLATFORM_H

