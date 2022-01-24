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

#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include "QtnScrollWidgetBar.h"

QTITAN_USE_NAMESPACE

static const int QtnScrollWidgetBarPageIndent = 25;
static const int QtnScrollWidgetBarItemIndent = 20;

AbstractScrollWidgetBar::AbstractScrollWidgetBar(QWidget* parent, Qt::Orientation orientation)
    : QWidget(parent), m_orientation(orientation), m_columnNum(0), 
    m_pageIndent(QtnScrollWidgetBarPageIndent), m_margin(QtnScrollWidgetBarItemIndent), m_padding(QtnScrollWidgetBarItemIndent)
{
    m_viewWidget = new QWidget(this);
    m_viewWidget->setAttribute(Qt::WA_NoSystemBackground, true);

    m_prevButton = new ScrollWidgetBarButton(this);
    connect(m_prevButton, SIGNAL(clicked()), this, SLOT(movePrev()));
    m_nextButton = new ScrollWidgetBarButton(this);
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(moveNext()));

    m_scrollAnimation = new QPropertyAnimation(m_viewWidget, "geometry");
    m_scrollAnimation->setDuration(150);
    m_scrollAnimation->setEasingCurve(QEasingCurve::InOutExpo /* QEasingCurve::OutCubic */);
    connect(m_scrollAnimation, SIGNAL(finished()), this, SLOT(scrollAnimationFinished()));
}

AbstractScrollWidgetBar::~AbstractScrollWidgetBar()
{
    Q_DELETE_AND_NULL(m_scrollAnimation);
    Q_DELETE_AND_NULL(m_viewWidget);
    Q_DELETE_AND_NULL(m_prevButton);
    Q_DELETE_AND_NULL(m_nextButton);
}

Qt::Orientation AbstractScrollWidgetBar::orientation() const
{
    return m_orientation;
}

void AbstractScrollWidgetBar::setOrientation(Qt::Orientation orientation)
{
    if (m_orientation == orientation)
        return;
    m_orientation = orientation;
    updateItemsLayout();
}

QWidget* AbstractScrollWidgetBar::viewWidget() const
{
    return m_viewWidget;
}

int AbstractScrollWidgetBar::pageIndent() const
{
    return m_pageIndent;
}

void AbstractScrollWidgetBar::setPageIndent(int indent)
{
    m_pageIndent = indent;
}

int AbstractScrollWidgetBar::margin() const
{
    return m_margin;
}

void AbstractScrollWidgetBar::setMargin(int margin)
{
    m_margin = margin;
}

int AbstractScrollWidgetBar::padding() const
{
    return m_padding;
}

void AbstractScrollWidgetBar::setPadding(int padding)
{
    if (m_padding == padding)
        return;
    m_padding = padding;
}

/*! \reimp */
void AbstractScrollWidgetBar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateLayout();
}

/*! \reimp */
void AbstractScrollWidgetBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
#if 0
    QRect r = QRect(0, 0, width(), height());
    painter.fillRect(r, palette().color(backgroundRole()));
#endif
}

/*! \reimp */
void AbstractScrollWidgetBar::wheelEvent(QWheelEvent *event)
{
    if (orientation() != event->orientation())
        return;
    int d = event->delta();
    if (d < 0)
        moveNext();
    else
        movePrev();
    event->accept();
}

void AbstractScrollWidgetBar::setPrevButtonVisible(bool visible)
{
    if (visible)
    {
        QSize s = m_prevButton->sizeHint();

        QRect r;
        if (orientation() == Qt::Horizontal)
        {
            s.rheight() = height();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignLeft | Qt::AlignTop, s, QRect(0, 0, this->width(), this->height()));
        }
        else
        {
            s.rwidth() = width();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignTop | Qt::AlignLeft, s, QRect(0, 0, this->width(), this->height()));
        }
        m_prevButton->setGeometry(r);
        m_prevButton->raise();
    }
    m_prevButton->setVisible(visible);
}

void AbstractScrollWidgetBar::setNextButtonVisible(bool visible)
{
    if (visible)
    {
        QSize s = m_nextButton->sizeHint();
        QRect r;
        if (orientation() == Qt::Horizontal)
        {
            s.rheight() = height();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignTop, s, QRect(0, 0, this->width(), this->height()));
        }
        else
        {
            s.rwidth() = width();
            r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignBottom | Qt::AlignLeft, s, QRect(0, 0, this->width(), this->height()));
        }
        m_nextButton->setGeometry(r);
        m_nextButton->raise();
    }
    m_nextButton->setVisible(visible);
}

void AbstractScrollWidgetBar::scrollAnimationFinished()
{
    updateLayout();
}

void AbstractScrollWidgetBar::movePrev()
{
    if (m_prevButton->isHidden() || m_scrollAnimation->state() == QAbstractAnimation::Running)
        return;

    m_columnNum = calcNextColumn(m_columnNum, true);

    startScrollAnimation();
}

void AbstractScrollWidgetBar::moveNext()
{
    if (m_nextButton->isHidden() || m_scrollAnimation->state() == QAbstractAnimation::Running)
        return;

    m_columnNum = calcNextColumn(m_columnNum, false);

    startScrollAnimation();
}

QColor AbstractScrollWidgetBar::getButtonColor() const
{
    return palette().color(backgroundRole());
}

int AbstractScrollWidgetBar::calcColumnsLength(int columnCount) const
{
    int ret = 0;
    for (int i = 0; i < columnCount; ++i)
        ret += getColumnLength(i);
    return ret;
}

int AbstractScrollWidgetBar::calcNextColumn(int column, bool back) const
{
    int w = orientation() == Qt::Horizontal ? width() - pageIndent() : height() - pageIndent();
    bool first = true;
    while (w > 0)
    {
        if (back)
        {
            if (column == 0)
                break;
        }
        else
        {
            if (column == getColumnCount())
                break;
        }

        w -= (getColumnLength(column) + padding());

        if (w < 0 && !first)
            break;

        first = false;

        if (back)
            column--;
        else
            column++;

        /*if (back)
        {
        if (column == 0)
        break;
        column--;
        }
        else
        {
        if (column == getColumnCount())
        break;
        column++;
        }*/
    }
    return column;
}

QPoint AbstractScrollWidgetBar::getViewOffset() const
{
    int columnsLen = calcColumnsLength(m_columnNum) + padding() * m_columnNum;
    QPoint offset = orientation() == Qt::Horizontal ? QPoint(-columnsLen, 0) : QPoint(0, -columnsLen);
    return offset;
}

void AbstractScrollWidgetBar::startScrollAnimation()
{
    QRect rect = m_viewWidget->geometry();
    m_scrollAnimation->setStartValue(rect);
    rect.moveTo(getViewOffset());
    m_scrollAnimation->setEndValue(rect);
    m_scrollAnimation->start();
}

void AbstractScrollWidgetBar::updateLayout()
{
    bool prevVisible = m_columnNum > 0;
    bool nextVisible = calcNextColumn(m_columnNum, false) < getColumnCount();

    QPoint offset = getViewOffset();

    int columnsLength = calcColumnsLength(getColumnCount());
    QSize s = sizeHint();
    QRect viewRect = orientation() == Qt::Horizontal ? 
        QRect(offset, QSize(pageIndent() * 2 + columnsLength + padding() * (getColumnCount() - 1), s.height())) :
        QRect(offset, QSize(s.width(), pageIndent() * 2 + columnsLength + padding() * (getColumnCount() - 1)));
    m_viewWidget->setGeometry(viewRect);

    setPrevButtonVisible(prevVisible);
    setNextButtonVisible(nextVisible);
}

/* ScrollWidgetBar */
ScrollWidgetBar::ScrollWidgetBar(QWidget* parent, Qt::Orientation orientation)
    : AbstractScrollWidgetBar(parent, orientation), m_updateCount(0), m_maxViewWidgetCount(0)
{
}

ScrollWidgetBar::~ScrollWidgetBar()
{
}

void ScrollWidgetBar::addWidget(QWidget* widget)
{
    beginUpdate();
    widget->setParent(viewWidget());
    m_widgets.push_back(widget);
    endUpdate();
}

void ScrollWidgetBar::removeWidget(QWidget* widget)
{
    beginUpdate();
    int i = m_widgets.indexOf(widget);
    if (i >= 0)
        m_widgets.remove(i);
    endUpdate();
}

void ScrollWidgetBar::beginUpdate()
{
    m_updateCount++;
}

void ScrollWidgetBar::endUpdate()
{
    Q_ASSERT(m_updateCount > 0);
    m_updateCount--;
    if (m_updateCount == 0)
        updateItemsLayout();
}

int ScrollWidgetBar::maxViewWidgetCount() const
{
    return m_maxViewWidgetCount;
}

void ScrollWidgetBar::setMaxViewWidgetCount(int count)
{
    m_maxViewWidgetCount = count;
}

QSize ScrollWidgetBar::sizeHint() const
{
    QSize ret = QSize(0, 0);

    int count = 0;
    for (QVector<QWidget *>::const_iterator it = m_widgets.constBegin(); it != m_widgets.constEnd(); ++it)
    {
        QWidget* widget = *it;
        if (widget->isHidden())
            continue;
        
        QSize s = widget->sizeHint();

        if (orientation() == Qt::Horizontal)
        {
            ret.rheight() = qMax(ret.height(), s.height());
            if (maxViewWidgetCount() <= 0 || maxViewWidgetCount() > count)
            {
                if (ret.width() > 0)
                    ret.rwidth() += padding();
                ret.rwidth() += s.width();
            }
        }
        else
        {
            ret.rwidth() = qMax(ret.width(), s.width());
            if (maxViewWidgetCount() <= 0 || maxViewWidgetCount() > count)
            {
                if (ret.height() > 0)
                    ret.rheight() += padding();
                ret.rheight() += s.height();
            }
        }
        count++;
    }

    if (orientation() == Qt::Horizontal)
    {
        ret.rwidth() += pageIndent() * 2;
        ret.rheight() += margin() * 2;
    }
    else
    {
        ret.rwidth() += margin() * 2;
        ret.rheight() += pageIndent() * 2;
    }
    return ret;
}

QWidget* ScrollWidgetBar::getWidget(int index) const
{
    return m_widgets[index];
}

int ScrollWidgetBar::getWidgetCount() const
{
    return m_widgets.size();
}

int ScrollWidgetBar::getColumnCount() const
{
    return m_widgets.size();
}

int ScrollWidgetBar::getColumnLength(int column) const
{
    if (orientation() == Qt::Horizontal)
        return m_widgets[column]->width();
    else
        return m_widgets[column]->height();
}

void ScrollWidgetBar::updateItemsLayout()
{
    if (m_updateCount > 0)
        return;

    QPoint topLeft = QPoint(0, 0);
    QPoint offset = orientation() == Qt::Horizontal ? 
        QPoint(pageIndent(), margin()) : QPoint(margin(), pageIndent());
    for (QVector<QWidget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
    {
        QWidget* widget = *it;
        if (widget->isHidden())
            continue;

        QSize s = widget->sizeHint();

        QRect rect = QRect(offset + topLeft, s);
        widget->setGeometry(rect);

        if (orientation() == Qt::Horizontal)
            topLeft.rx() += padding() + s.width();
        else
            topLeft.ry() += padding() + s.height();
    }
}

/* ScrollWidgetBarButton */
ScrollWidgetBarButton::ScrollWidgetBarButton(AbstractScrollWidgetBar* bar)
    : QAbstractButton(bar)
{
    setMouseTracking(true);
}

ScrollWidgetBarButton::~ScrollWidgetBarButton()
{
}

QSize ScrollWidgetBarButton::sizeHint() const
{
    AbstractScrollWidgetBar* bar = qobject_cast<AbstractScrollWidgetBar *>(parentWidget());
    if (bar != Q_NULL)
        return QSize(bar->pageIndent(), bar->pageIndent());
    return QSize(QtnScrollWidgetBarPageIndent, QtnScrollWidgetBarPageIndent);
}

void ScrollWidgetBarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    AbstractScrollWidgetBar* bar = qobject_cast<AbstractScrollWidgetBar *>(parentWidget());
    if (bar == Q_NULL)
        return;
    QColor color = bar->getButtonColor();
    painter.fillRect(QRect(0, 0, width(), height()), color);

    QSize s = QSize(10, 10);
    QPixmap arrow = bar->getArrowIcon(bar->m_prevButton == this).scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QRect r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, s, QRect(0, 0, this->width(), this->height()));
    if (!underMouse())
        painter.setOpacity(0.7);
    painter.drawPixmap(r, arrow);
}

void ScrollWidgetBarButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    update();
}

void ScrollWidgetBarButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    update();
}

void ScrollWidgetBarButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    update();
}
