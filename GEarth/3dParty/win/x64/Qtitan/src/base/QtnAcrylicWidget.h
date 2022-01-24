/****************************************************************************
**
** Qtitan Library by Developer Machines (Navigation UI for Qt.C++)
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
#ifndef QTN_ACRYLICWIDGET_H
#define QTN_ACRYLICWIDGET_H

#include <QWidget>
#include <QGraphicsEffect>

#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE

class QTITAN_EXPORT AcrylicWidget : public QWidget
{
    Q_OBJECT
public:
    enum AcrylicBlend
    {
        Disabled,
        InAppAcrylic,
        BackgroundAcrylic
    };
    explicit AcrylicWidget(QWidget* parent = Q_NULL, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~AcrylicWidget();
    AcrylicBlend acrylicBlend() const;
    void setAcrylicBlend(AcrylicBlend acrylicBlend);
    const QColor& tintColor() const;
    void setTintColor(const QColor& color);
    double tintOpacity() const;
    void setTintOpacity(double opacity);
protected:
    QBrush createAcrylicBrush() const;
    void paintBackground(QPainter* painter, const QRegion& clip);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
    virtual void changeEvent(QEvent * event);
    const QImage& grabBackstageToBuffer(const QRegion& clip, bool& hasChanges);
    void setBackstageToWindow(const QImage& newBackstage, const QRegion& clip);
private:
    friend class AcrylicWidgetEffect;
    AcrylicBlend m_acrylicBlend;
    double m_tintOpacity;
    QBrush m_acrylicBrush;
    QImage m_buffer;
};


class AcrylicWidgetEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    AcrylicWidgetEffect(AcrylicWidget* widget);
    virtual ~AcrylicWidgetEffect();
    virtual QRectF boundingRectFor(const QRectF &sourceRect) const;
protected:
    virtual void draw(QPainter *painter);
    virtual void sourceChanged(ChangeFlags flags);
private:
    AcrylicWidget* m_widget;
    QImage m_blurredBuffer;
};

QTITAN_END_NAMESPACE

#endif// QTN_ACRYLICWIDGET_H



