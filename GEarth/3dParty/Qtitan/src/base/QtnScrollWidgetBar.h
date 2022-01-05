/****************************************************************************
**
** Qtitan Library by Developer Machines
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

#ifndef QTN_SCROLLWIDGETBAR_H
#define QTN_SCROLLWIDGETBAR_H

#include <QWidget>
#include <QAbstractButton>
#include <QPropertyAnimation>

#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE

class ScrollWidgetBarButton;

class QTITAN_EXPORT AbstractScrollWidgetBar : public QWidget
{
  Q_OBJECT 
public:
    explicit AbstractScrollWidgetBar(QWidget* parent, Qt::Orientation orientation = Qt::Horizontal);
    virtual ~AbstractScrollWidgetBar();
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    QWidget* viewWidget() const;
    int pageIndent() const;
    void setPageIndent(int indent);
    int margin() const;
    void setMargin(int margin);
    int padding() const;
    void setPadding(int padding);
public slots:
    void movePrev();
    void moveNext();
protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void wheelEvent(QWheelEvent *);
protected:
    void setPrevButtonVisible(bool visible);
    void setNextButtonVisible(bool visible);
    virtual void updateLayout();
    virtual void updateItemsLayout() = 0;
    virtual int getColumnCount() const = 0;
    virtual int getColumnLength(int column) const = 0;
    virtual QPixmap getArrowIcon(bool prevButton) const = 0;
    virtual QColor getButtonColor() const;
    virtual int calcColumnsLength(int columnCount) const;
    int calcNextColumn(int column, bool back) const;
    QPoint getViewOffset() const;
    void startScrollAnimation();
protected slots:
    void scrollAnimationFinished();
protected:
    friend class ScrollWidgetBarButton;
    ScrollWidgetBarButton* m_prevButton;
    ScrollWidgetBarButton* m_nextButton;
    Qt::Orientation m_orientation;
    int m_columnNum;
    QWidget* m_viewWidget;
    int m_pageIndent;
    int m_margin;
    int m_padding;
    QPropertyAnimation* m_scrollAnimation;
    Q_DISABLE_COPY(AbstractScrollWidgetBar)
};

class QTITAN_EXPORT ScrollWidgetBar : public AbstractScrollWidgetBar
{
    Q_OBJECT
public:
    explicit ScrollWidgetBar(QWidget* parent, Qt::Orientation orientation = Qt::Horizontal);
    virtual ~ScrollWidgetBar();
    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
    void beginUpdate();
    void endUpdate();
    int maxViewWidgetCount() const;
    void setMaxViewWidgetCount(int count);
    virtual QSize sizeHint() const;
    QWidget* getWidget(int index) const;
    int getWidgetCount() const;
protected:
    virtual int getColumnCount() const;
    virtual int getColumnLength(int column) const;
    virtual void updateItemsLayout();
    QVector<QWidget *> m_widgets;
    int m_updateCount;
    int m_maxViewWidgetCount;
private:
    Q_DISABLE_COPY(ScrollWidgetBar)
};

class ScrollWidgetBarButton : public QAbstractButton
{
public:
    explicit ScrollWidgetBarButton(AbstractScrollWidgetBar* bar);
    virtual ~ScrollWidgetBarButton();
    virtual QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
private:
private:
    Q_DISABLE_COPY(ScrollWidgetBarButton)
};

QTITAN_END_NAMESPACE

#endif// QTN_SCROLLWIDGETBAR_H


