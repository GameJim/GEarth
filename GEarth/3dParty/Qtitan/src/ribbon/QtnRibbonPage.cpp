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
#include <QApplication>
#include <QPainter>
#include <QStyleOption>
#include <QEvent>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif

#include "QtnRibbonStyle.h"
#include "QtnStyleHelpers.h"
#include "QtnRibbonPage.h"
#include "QtnRibbonPagePrivate.h"
#include "QtnRibbonGroup.h"
#include "QtnRibbonBar.h"
#include "QtnRibbonBarPrivate.h"
#include "QtnRibbonGroupPrivate.h"
#include "QtnRibbonDef.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif // Q_OS_WIN
#ifdef DEBUG_MEMORY_ENABLED
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE


static const int pageMarginPopup = 2;
static const int minRibbonWidth  = 100;

/* RibbonPagePrivate */
RibbonPagePrivate::RibbonPagePrivate()
{
  m_animationStep = 0.0;
  m_timerElapse = 0;

  m_buttonScrollGroupLeft = Q_NULL;    // Button to draw left scroll
  m_buttonScrollGroupRight = Q_NULL;   // Button to draw right scroll

  m_title = RibbonBar::tr_compatible(QtnRibbonUntitledString);
  m_visibleUpdateCount = 0;
  m_animation = false;
  m_contextColor = RibbonPage::ContextColorNone;
  m_layoutDirection = Qt::RightToLeft;
  m_associativeTab = Q_NULL;
  m_groupsHeight = -1;
  m_groupScrollPos = 0;
  m_pageScrollPos = 0;
  m_scrollPosTarget = 0;
}

RibbonPagePrivate::~RibbonPagePrivate()
{
}

RibbonPagePrivate* RibbonPagePrivate::_get(RibbonPage* page)
{
    return &page->qtn_d();
}

void RibbonPagePrivate::init()
{
  QTN_P(RibbonPage);
  m_buttonScrollGroupLeft = new RibbonGroupScroll(&p, true);
  m_buttonScrollGroupLeft->setVisible(false);

  m_buttonScrollGroupRight = new RibbonGroupScroll(&p, false);
  m_buttonScrollGroupRight->setVisible(false);

  QObject::connect(m_buttonScrollGroupLeft, SIGNAL(pressed()), this, SLOT(pressLeftScrollButton()));
  QObject::connect(m_buttonScrollGroupRight, SIGNAL(pressed()), this, SLOT(pressRightScrollButton()));

  QObject::connect(m_buttonScrollGroupLeft, SIGNAL(stopScrollTimer()), this, SLOT(forcedStopScrollTimer()));
  QObject::connect(m_buttonScrollGroupRight, SIGNAL(stopScrollTimer()), this, SLOT(forcedStopScrollTimer()));
}

bool RibbonPagePrivate::collapseGroups(int& leftOffset, int actualWidth, RibbonControlSizeDefinition::GroupSize size, bool adjust)
{
    Q_ASSERT(m_layoutDirection == Qt::RightToLeft || m_layoutDirection == Qt::LeftToRight);

    bool ret = false;
    if (leftOffset > actualWidth)
    {
        QListIterator<RibbonGroup *> iterator(m_listGroups);
        bool normDir = m_layoutDirection == Qt::RightToLeft;
        if (normDir)
            iterator.toBack();

        while (normDir ? iterator.hasPrevious() : iterator.hasNext())
        {
            RibbonGroup* group = normDir ? iterator.previous() : iterator.next();
            if (group->isHidden())
                continue;

            if (group->currentSize() <= size && (adjust || RibbonGroupPrivate::_get(group)->canReduce()))
            {
                int width_0 = group->sizeHint().width();
                if (adjust)
                    RibbonGroupPrivate::_get(group)->adjustCurrentSize(false);
                else
                    RibbonGroupPrivate::_get(group)->reduce();

                int width_1 = group->sizeHint().width();
                int delta = width_0 - width_1;
                leftOffset -= delta;
                ret = ret || (delta != 0);
            }
            if (leftOffset <= actualWidth)
                break;
        }
    }
    return ret;
}

bool RibbonPagePrivate::expandGroups(int& leftOffset, int actualWidth, RibbonControlSizeDefinition::GroupSize size, bool adjust)
{
    Q_ASSERT(m_layoutDirection == Qt::RightToLeft || m_layoutDirection == Qt::LeftToRight);

    bool ret = false;
    bool normDir = m_layoutDirection == Qt::RightToLeft;
    QListIterator<RibbonGroup *> iterator(m_listGroups);
    if (!normDir)
        iterator.toBack();

    while (normDir ? iterator.hasNext() : iterator.hasPrevious())
    {
        RibbonGroup* group = normDir ? iterator.next() : iterator.previous();
        if (group->isHidden())
            continue;
        if (group->currentSize() >= size && (adjust || RibbonGroupPrivate::_get(group)->canExpand()))
        {
            int width_0 = group->sizeHint().width();
            if (adjust)
                RibbonGroupPrivate::_get(group)->adjustCurrentSize(true);
            else
                RibbonGroupPrivate::_get(group)->expand();

            int width_1 = group->sizeHint().width();
            int delta = width_0 - width_1;
            leftOffset -= delta;
            ret = ret || (delta != 0);
        }
        if (leftOffset > actualWidth)
            break;
    }
    return ret;
}

RibbonControlSizeDefinition::GroupSize RibbonPagePrivate::getMinGroupSize() const
{
    RibbonControlSizeDefinition::GroupSize size = RibbonControlSizeDefinition::GroupPopup;
    for (QList<RibbonGroup*>::const_iterator it = m_listGroups.constBegin(); it != m_listGroups.constEnd(); ++it)
    {
        RibbonGroup* group = *it;
        if (group->isHidden())
            continue;
        size = qMin(size, group->currentSize());
        if (size == RibbonControlSizeDefinition::GroupLarge)
            break;
    }
    return size;
}

RibbonControlSizeDefinition::GroupSize RibbonPagePrivate::getMaxGroupSize() const
{
    RibbonControlSizeDefinition::GroupSize size = RibbonControlSizeDefinition::GroupLarge;
    for (QList<RibbonGroup*>::const_iterator it = m_listGroups.constBegin(); it != m_listGroups.constEnd(); ++it)
    {
        RibbonGroup* group = *it;
        if (group->isHidden())
            continue;
        size = qMax(size, group->currentSize());
    }
    return size;
}

bool RibbonPagePrivate::canReduce() const
{
    for (QList<RibbonGroup*>::const_iterator it = m_listGroups.constBegin(); it != m_listGroups.constEnd(); ++it)
    {
        RibbonGroup* group = *it;
        if (RibbonGroupPrivate::_get(group)->canReduce())
            return true;
    }
    return false;
}

bool RibbonPagePrivate::isPopupMode() const
{
    QTN_P(const RibbonPage);
    return p.ribbonBar() != Q_NULL && p.ribbonBar()->isMinimized();
}
static RibbonControlSizeDefinition::GroupSize qtn_size_inc(RibbonControlSizeDefinition::GroupSize& size)
{
    RibbonControlSizeDefinition::GroupSize ret = size;
    size = (RibbonControlSizeDefinition::GroupSize)(size + 1);
    return ret;
}

static RibbonControlSizeDefinition::GroupSize qtn_size_dec(RibbonControlSizeDefinition::GroupSize& size)
{
    RibbonControlSizeDefinition::GroupSize ret = size;
    size = (RibbonControlSizeDefinition::GroupSize)(size - 1);
    return ret;
}

int RibbonPagePrivate::adjustGroups(int leftOffset, int actualWidth)
{
    RibbonControlSizeDefinition::GroupSize size = getMaxGroupSize();
    while (size > RibbonControlSizeDefinition::GroupLarge && leftOffset < actualWidth)
    {
        if (expandGroups(leftOffset, actualWidth, size, true))
            continue;
        if (expandGroups(leftOffset, actualWidth, qtn_size_dec(size), false))
            continue;
    }

    if (leftOffset > actualWidth)
    {
        size = getMinGroupSize();
        int iCount = 0, iMaxCollapseCount = 100; // �������ѹ������
        while (canReduce() && leftOffset > actualWidth)
        {
            iCount++;
            if (iCount > iMaxCollapseCount)
            {
                break;
            }

            if (collapseGroups(leftOffset, actualWidth, size, true))
                continue;
            if (collapseGroups(leftOffset, actualWidth, qtn_size_inc(size), false))
                continue;
        }
    }
    return leftOffset;
}

int RibbonPagePrivate::calculateGroupsWidth()
{
    int width = 0;
    for (int i = 0, count = m_listGroups.size(); i < count; ++i)
    {
        RibbonGroup* group = m_listGroups.at(i);
        if (group->isHidden())
            continue;
        width += group->sizeHint().width();
    }
    return width;
}

void RibbonPagePrivate::updateLayout(bool updateScroll)
{
    QTN_P(RibbonPage);
    int leftOffset = 0;
    int actualWidth = p.width();
    if (RibbonBar* ribbonBar = p.ribbonBar())
    {
        if (RibbonBarPrivate::_get(ribbonBar)->isUpdating())
            return;

        if (!isPopupMode() && 
            !RibbonBarPrivate::_get(ribbonBar)->m_logotypeLabel->isHidden())
        {
            QRect rectLogotype = RibbonBarPrivate::_get(ribbonBar)->m_logotypeLabel->geometry();
            if (!rectLogotype.isNull())
            {
                actualWidth -= rectLogotype.width();

                if (RibbonBarPrivate::_get(ribbonBar)->m_logotypeLabel->alignment() == Qt::AlignLeft)
                    leftOffset += rectLogotype.width();
            }
        }
    }

    bool isMinimized = actualWidth < minRibbonWidth;
    if (!isMinimized)
    {
        int gropsWidth = calculateGroupsWidth() + leftOffset;
        adjustGroups(gropsWidth, actualWidth);
    }

    static int margin = 4;
    for (QList<RibbonGroup*>::iterator it = m_listGroups.begin(); it != m_listGroups.end(); ++it)
    {
        RibbonGroup* group = *(it);
        if (group->isHidden())
            continue;
         
        if (it == m_listGroups.begin())
          leftOffset += margin;

        QSize groupSizeHint = group->sizeHint();

        int controlsLeftOffset = 0;
        QWidget* parentWidget = group;
        if (group->isReduced())
        {
            // Is the group in a popup stage then create a sub-window RibbonGroupPopup
            RibbonGroupPrivate::_get(group)->reposition(QRect(QPoint(-m_groupScrollPos + leftOffset, margin - 1), QSize(groupSizeHint.width(), p.size().height())));
            parentWidget = RibbonGroupPrivate::_get(group)->groupPopupWidget();
        }

        if (!(group->controlsAlignment() & Qt::AlignLeft))
        {
            QSize contentSize = RibbonGroupPrivate::_get(group)->updateControlsLayout(Q_NULL);
            QSize parentSize = parentWidget->sizeHint();
            controlsLeftOffset = QStyle::alignedRect(Qt::LeftToRight, group->controlsAlignment(), contentSize, QRect(QPoint(0, 0), parentSize)).left();
        }

        RibbonGroupPrivate::_get(group)->updateControlsLayout(parentWidget, controlsLeftOffset);

        if (!group->isReduced())
            RibbonGroupPrivate::_get(group)->reposition(QRect(QPoint(-m_groupScrollPos + leftOffset, margin - 1), QSize(groupSizeHint.width(), p.size().height())));

        RibbonGroupPrivate::_get(group)->updateOptionButtonLayout();
        leftOffset += groupSizeHint.width();
    }

    if (updateScroll)
        showGroupScroll(false);
}

void RibbonPagePrivate::removeGroup(int index, bool deleteGroup)
{
    if (validateGroupIndex(index)) 
    {
        RibbonGroup* group = m_listGroups.at(index);

        group->removeEventFilter(this);
        QList<QAction*> actList = group->actions();

        for (int i = actList.size()-1; i >= 0; --i) 
            m_listShortcuts.removeOne(actList.at(i));

        m_listGroups.removeAt(index);

        if (deleteGroup)
            delete group;
        else
            group->setParent( Q_NULL );

        updateLayout();
    }
    else
        Q_ASSERT(false);
}

int RibbonPagePrivate::groupIndex(RibbonGroup* group) const
{
    return m_listGroups.indexOf(group);
}

int RibbonPagePrivate::calcReducedGroupsWidth() const
{
    QTN_P(const RibbonPage);
    static int margin = 4;
    int totalWidth = 0;
    for (QList<RibbonGroup*>::const_iterator it = m_listGroups.constBegin(); it != m_listGroups.constEnd(); ++it)
    {
        RibbonGroup* group = *(it);
        if (group->isHidden())
            continue;

        if (RibbonGroupPrivate::_get(group)->canReduce())
        {
            totalWidth = 0;
            break;
        }

        if (it == m_listGroups.begin())
            totalWidth = margin * 2;

        QRect rect = group->geometry();
        totalWidth += rect.width();
    }

    int width = 0;
    if (RibbonBar* ribbon = p.ribbonBar())
    {
        if (!isPopupMode() && 
            !RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->isHidden())
        {
            QRect rect = RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->geometry();
            if (!rect.isNull())
                width = rect.width();
        }
    }

    return totalWidth + width;
}

void RibbonPagePrivate::enableGroupScroll(bool scrollLeft, bool scrollRight)
{
    QTN_P(RibbonPage);

    Q_ASSERT(m_buttonScrollGroupLeft);
    Q_ASSERT(m_buttonScrollGroupRight);

    if (!m_buttonScrollGroupRight || !m_buttonScrollGroupLeft)
        return;

    QRect rcPage(p.geometry());

    if (scrollLeft)
    {
        m_buttonScrollGroupLeft->raise();
        m_buttonScrollGroupLeft->setVisible(true);

        int leftOffset = 0;
        if (RibbonBar* ribbon = p.ribbonBar())
        {
            if (!isPopupMode() && 
                !RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->isHidden() && 
                RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->alignment() == Qt::AlignLeft)
            {
                QRect rect = RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->geometry();
                if (!rect.isNull())
                    leftOffset = rect.width() - 2;
            }
        }

        QRect rc(QPoint(0 + leftOffset - 1, 1), QSize(12, rcPage.height() - 1));

        if (isPopupMode())
            rc.setHeight(rc.height() + pageMarginPopup);

        m_buttonScrollGroupLeft->setGeometry(rc);
    }
    else
    {
        m_buttonScrollGroupLeft->setDown(false);
        m_buttonScrollGroupLeft->setVisible(false);
    }

    if (scrollRight)
    {
        m_buttonScrollGroupRight->raise();
        m_buttonScrollGroupRight->setVisible(true);

        int leftOffset = 0;
        if (RibbonBar* ribbon = p.ribbonBar())
        {
            if (!isPopupMode() && 
                !RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->isHidden() &&
                RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->alignment() == Qt::AlignRight)
            {
                QRect rectLogotype = RibbonBarPrivate::_get(ribbon)->m_logotypeLabel->geometry();
                if (!rectLogotype.isNull())
                    leftOffset = rectLogotype.width() - 3;
            }
        }

        QRect rc(QPoint(rcPage.width() - leftOffset - 12 + 1 , 1), QSize(12, rcPage.height() - 1));

        if (isPopupMode())
            rc.setHeight(rc.height() + pageMarginPopup);

        m_buttonScrollGroupRight->setGeometry(rc);
    }
    else
    {
        m_buttonScrollGroupRight->setDown(false);
        m_buttonScrollGroupRight->setVisible(false);
    }
    if (m_buttonScrollGroupLeft->isHidden() && m_buttonScrollGroupRight->isHidden())
        m_pageScrollPos = 0;
}

void RibbonPagePrivate::showGroupScroll(bool onlyCalc)
{
    QTN_P(RibbonPage);
    int totalWidth = calcReducedGroupsWidth();
    if (totalWidth == 0 && m_buttonScrollGroupLeft->isHidden() && m_buttonScrollGroupRight->isHidden())
        return;

    int groupsLength = p.width();
    int scrollPos = m_groupScrollPos;

    if (totalWidth > groupsLength)
    {
        if (scrollPos > totalWidth - groupsLength)
            scrollPos = totalWidth - groupsLength;
    }
    else
        scrollPos = 0;

    if (scrollPos < 0) 
        scrollPos = 0;

    m_groupScrollPos = scrollPos;
    if (!onlyCalc)
        enableGroupScroll(scrollPos > 0, totalWidth - groupsLength - scrollPos > 0);
}

void RibbonPagePrivate::scrollGroupAnimate()
{
    if (qAbs(m_groupScrollPos - m_scrollPosTarget) > qFabs(m_animationStep))
    {
        m_groupScrollPos = int((double)m_groupScrollPos + m_animationStep);
        updateLayout(false);
    }
    else
    {
        m_animation = false;
        m_groupScrollPos = m_scrollPosTarget;
        m_timerElapse = 0;
        m_scrollTimer.stop();
        showGroupScroll(false);
    }
}

void RibbonPagePrivate::startScrollGropsAnimate(int groupScrollPos, int scrollPosTarget)
{
    QTN_P(RibbonPage)
    m_animation = true;
    m_scrollPosTarget = scrollPosTarget;
    m_groupScrollPos = groupScrollPos;
    m_timerElapse = QApplication::doubleClickInterval() * 4 / 5;
    m_animationStep = double(m_scrollPosTarget - m_groupScrollPos) / (m_timerElapse > 200 ? 8.0 : 3.0);

    if (m_animationStep > 0 && m_animationStep < 1) m_animationStep = 1;
    if (m_animationStep < 0 && m_animationStep > -1) m_animationStep = -1;

    int nms = 40;
    m_scrollTimer.start(nms, &p);
    scrollGroupAnimate();
}

static void listPageWidth(int totalWidth, int realWidth, QList<int>& pagesWidth) 
{
    if (totalWidth > realWidth)
    {
        pagesWidth << realWidth;
        listPageWidth(totalWidth - realWidth, realWidth, pagesWidth);
    }
    else
        pagesWidth << totalWidth;
    return;
}

void RibbonPagePrivate::pressLeftScrollButton()
{
    QTN_P(RibbonPage)
    QList<int> pagesWidth;
    listPageWidth(calcReducedGroupsWidth(), p.width(), pagesWidth);

    if (0 < m_pageScrollPos)
        m_pageScrollPos--; 

    int scrollPos = m_groupScrollPos;
    m_groupScrollPos -= pagesWidth[m_pageScrollPos];
    showGroupScroll(true);
    startScrollGropsAnimate(scrollPos, m_groupScrollPos);
}

void RibbonPagePrivate::pressRightScrollButton()
{
    QTN_P(RibbonPage)
    QList<int> pagesWidth;
    listPageWidth(calcReducedGroupsWidth(), p.width(), pagesWidth);

    m_groupScrollPos += pagesWidth[m_pageScrollPos];
    showGroupScroll(true);

    if (pagesWidth.size() - 1 > m_pageScrollPos)
        m_pageScrollPos++; 

    startScrollGropsAnimate(0, m_groupScrollPos);
}

void RibbonPagePrivate::forcedStopScrollTimer()
{
    m_scrollTimer.stop();
    showGroupScroll(false);
}

/*! \reimp */
bool RibbonPagePrivate::eventFilter(QObject* obj, QEvent* event)
{
    bool res = QObject::eventFilter(obj, event);

    if (!qobject_cast<RibbonGroup*>(obj))
        return res;

    switch (event->type()) 
    {
        case QEvent::ActionAdded: 
            if (QActionEvent* actEvent = static_cast<QActionEvent*>(event))
            {
                m_listShortcuts.append(actEvent->action());
                if (m_associativeTab)
                    m_associativeTab->addAction(actEvent->action());
            }
            break;
        case QEvent::ActionRemoved: 
            if (QActionEvent* actEvent = static_cast<QActionEvent*>(event))
            {
                if (m_associativeTab)
                    m_associativeTab->removeAction(actEvent->action());

                int index = m_listShortcuts.indexOf(actEvent->action());
                if (index != -1)
                    m_listShortcuts.removeAt(index);
            }
            break;
        default:
            break;
    }

    return res;
}

/*!
\class RibbonPage 
\inmodule QtitanRibbon
*/
RibbonPage::RibbonPage(QWidget* parent)
    : QWidget(parent)
{
    QTN_INIT_PRIVATE(RibbonPage);
    QTN_D(RibbonPage);
    d.init();
}

RibbonPage::RibbonPage(RibbonBar* ribbonBar, const QString& title)
    : QWidget(ribbonBar)
{
    QTN_INIT_PRIVATE(RibbonPage);
    QTN_D(RibbonPage);
    d.init();
    setTitle(title);
}

RibbonPage::~RibbonPage()
{
    QTN_D(RibbonPage);
    if (d.m_associativeTab)
        d.m_associativeTab->setPage(Q_NULL);

    if (RibbonBar* _ribbonBar = ribbonBar())
        _ribbonBar->detachPage(this);
    QTN_FINI_PRIVATE();
}

/*!
Returns the ribbon bar that holds this page.
*/
RibbonBar* RibbonPage::ribbonBar() const
{
    QTN_D(const RibbonPage);
    if (d.m_associativeTab != Q_NULL)
        return d.m_associativeTab->ribbonBar();
    else
        return Q_NULL;
}

/*!
Overloaded method. Returns true if page is visible in the ribbon bar.
*/
bool RibbonPage::isVisible() const
{
    QTN_D(const RibbonPage);
    if (d.m_associativeTab != Q_NULL)
        return !d.m_associativeTab->isHidden();
    else
        return QWidget::isVisible();
}

/*! \reimp */
void RibbonPage::setVisible(bool visible)
{
    QTN_D(RibbonPage);
    if (d.m_associativeTab != Q_NULL)
        d.m_associativeTab->update();

    bool guard = d.m_visibleUpdateCount > 0;
    if (guard)
    {
        QWidget::setVisible(visible);
        return;
    }

    if (d.m_associativeTab != Q_NULL)
    {
        d.m_associativeTab->setContextColor(d.m_contextColor);
        d.m_associativeTab->setContextText(d.m_contextTitle);
        d.m_associativeTab->setVisible(visible);
        visible = visible && d.m_associativeTab->isSelected();
        if (!visible)
            qtn_set_tab_selected(d.m_associativeTab, false);
    }
    QWidget::setVisible(visible);

    RibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == Q_NULL)
        return;

    int index = _ribbonBar->currentPageIndex();
    int newIndex = index;
    RibbonPage* page = _ribbonBar->getPage(index);
    if (page == Q_NULL || !page->isVisible())
    {
        newIndex = -1;
        for (int i = 0, count = _ribbonBar->getPageCount(); count > i; ++i)
        {
            RibbonPage* page = _ribbonBar->getPage(i);
            if (page->isVisible())
            {
                newIndex = i;
                break;
            }
        }
    }

    if (newIndex != index)
        _ribbonBar->setCurrentPageIndex(newIndex);

    _ribbonBar->updateLayout();
}

/*!
Adds existing group to the ribbon page.
*/
void RibbonPage::addGroup(RibbonGroup* group)
{
    insertGroup(-1, group);
}

/*!
Adds group with given \a title to the ribbon page.
*/
RibbonGroup* RibbonPage::addGroup(const QString& title)
{
    return insertGroup(-1, title);
}

/*!
Adds group with given \a icon and \a title to the ribbon page.
*/
RibbonGroup* RibbonPage::addGroup(const QIcon& icon, const QString& title)
{
    return insertGroup(-1, icon, title);
}

void RibbonPage::insertGroup(int index, RibbonGroup* group)
{
    QTN_D(RibbonPage);

    group->setParent(this);

    if (RibbonBar* _ribbonBar = ribbonBar())
        group->setProperty(_qtn_TitleGroupsVisible, _ribbonBar->isTitleGroupsVisible());

    if (!d.validateGroupIndex(index)) 
    {
        index = d.m_listGroups.count();
        d.m_listGroups.append(group);
    } 
    else 
        d.m_listGroups.insert(index, group);

    connect(group, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)), Qt::QueuedConnection);
    connect(group, SIGNAL(released()), this, SLOT(released()));

    group->show();
    d.updateLayout();
}

RibbonGroup* RibbonPage::insertGroup(int index, const QString& title)
{
    RibbonGroup* group = new RibbonGroup(Q_NULL, title);
    insertGroup(index, group);
    return group;
}

RibbonGroup* RibbonPage::insertGroup(int index, const QIcon& icon, const QString& title)
{
    if (RibbonGroup* group = insertGroup(index, title))
    {
        group->setIcon(icon);
        return group;
    }
    return Q_NULL;
}

void RibbonPage::removeGroup(RibbonGroup* group)
{
    QTN_D(RibbonPage);
    removeGroup(d.groupIndex(group));
}

void RibbonPage::removeGroup(int index)
{
    QTN_D(RibbonPage);
    d.removeGroup(index, true);
}

void RibbonPage::detachGroup(RibbonGroup* group)
{
    QTN_D(RibbonPage);
    detachGroup(d.groupIndex(group));
}

void RibbonPage::detachGroup(int index)
{
    QTN_D(RibbonPage);
    d.removeGroup(index, false);
}

void RibbonPage::clearGroups()
{
    QTN_D(RibbonPage);
    for(int i = (int)d.m_listGroups.count()-1; i >= 0; i--)
        removeGroup(i);
}

QAction* RibbonPage::defaultAction() const
{
    QTN_D(const RibbonPage);
    if (d.m_associativeTab)
        return d.m_associativeTab->defaultAction();
    return Q_NULL;
}

/*!
Returns ribbon group count in the page.
*/
int RibbonPage::groupCount() const
{
    QTN_D(const RibbonPage);
    return d.m_listGroups.count();
}

/*!
Returns ribbon group by \a index.
*/
RibbonGroup* RibbonPage::getGroup(int index) const
{
    QTN_D(const RibbonPage);
    if (index < 0 || index >= d.m_listGroups.size())
        return Q_NULL;
    return d.m_listGroups[index];
}

int RibbonPage::groupIndex(RibbonGroup* group) const
{
    QTN_D(const RibbonPage);
    Q_ASSERT(group != Q_NULL);
    if (group && d.m_listGroups.contains(group))
        return d.m_listGroups.indexOf(group);
    return -1;
}

QList<RibbonGroup*> RibbonPage::groups() const
{
    QTN_D(const RibbonPage);
    return d.m_listGroups;
}

/*!
Sets the predefined \a color for the contextual page.
 */ 
void RibbonPage::setContextColor(ContextColor color)
{
    QTN_D(RibbonPage);
    d.m_contextColor = color;

    if (d.m_associativeTab)
    {
        d.m_associativeTab->setContextColor(color);
        update();
        if (parentWidget())
            parentWidget()->update();
#ifdef Q_OS_WIN
        if (RibbonBar* bar = ribbonBar())
            bar->updateWindowTitle();
#endif //Q_OS_WIN
    }
}

/*!
Returns the predefined color for the contextual page.
 */ 
RibbonPage::ContextColor RibbonPage::contextColor() const
{
    QTN_D(const RibbonPage);
    return d.m_contextColor;
}

void RibbonPage::setTitle(const QString& title)
{
    QTN_D(RibbonPage);

    if (d.m_title == title)
        return;

    d.m_title = title;

    if (d.m_associativeTab)
        d.m_associativeTab->setText(d.m_title);

    emit titleChanged(d.m_title);
}

/*!
\property RibbonPage::title
Sets the title of the page.
\sa RibbonPage::contextTitle
\inmodule QtitanRibbon
*/
const QString& RibbonPage::title() const
{
    QTN_D(const RibbonPage);
    return d.m_associativeTab ? d.m_associativeTab->text() : d.m_title;
}

void RibbonPage::setContextTitle(const QString& title)
{
    QTN_D(RibbonPage);
    d.m_contextTitle = title;

    if (d.m_associativeTab)
    {
        d.m_associativeTab->setContextText(d.m_contextTitle);
#ifdef Q_OS_WIN
        if (RibbonBar* _ribbonBar = ribbonBar())
            _ribbonBar->updateWindowTitle();
#endif //Q_OS_WIN
    }
}

void RibbonPage::released()
{
    RibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == Q_NULL)
        return;

    if (_ribbonBar->isMinimized() && isVisible() && QApplication::activePopupWidget() == this)
        hide();
}

void RibbonPage::actionTriggered(QAction* action)
{
    RibbonBar* _ribbonBar = ribbonBar();
    if (_ribbonBar == Q_NULL)
        return;

    QTN_D(RibbonPage);

    if (d.m_listShortcuts.indexOf(action) != -1)
        return;

    if (_ribbonBar->isMinimized() && isVisible())
    {
        if (action->menu())
            return;
        close();
    }
}

/*!
\property RibbonPage::contextTitle
Sets the context title of the page. Context title is located on main window frame if page is contextual.
\sa RibbonPage::title
\inmodule QtitanRibbon
*/

const QString& RibbonPage::contextTitle() const
{
    QTN_D(const RibbonPage);
    return d.m_associativeTab ? d.m_associativeTab->contextText() : d.m_title;
}

void RibbonPage::setContextGroupName(const QString& groupName)
{
    QTN_D(RibbonPage);
    d.m_contextGroupName = groupName;

    if (d.m_associativeTab)
    {
        d.m_associativeTab->setContextGroupName(d.m_contextGroupName);
#ifdef Q_OS_WIN
        if (RibbonBar* _ribbonBar = ribbonBar())
            _ribbonBar->updateWindowTitle();
#endif //Q_OS_WIN
    }
}

/*!
\property RibbonPage::tabWidth
Sets the width of the tab in pixels. By default the width is calculated automaticaly relative to the font of this page. 
\inmodule QtitanRibbon
*/
void RibbonPage::setTabWidth(int width)
{
    QTN_D(RibbonPage);
    if (d.m_associativeTab == Q_NULL)
        return;
    d.m_associativeTab->setTabWidth(width);
}

int RibbonPage::tabWidth() const
{
    QTN_D(const RibbonPage);
    if (d.m_associativeTab == Q_NULL)
        return -1;
    return d.m_associativeTab->tabWidth();
}

void RibbonPage::updateLayout()
{
    QTN_D(RibbonPage);
    d.updateLayout();
}

void RibbonPage::setAssociativeTab(RibbonTab* tab)
{
    QTN_D(RibbonPage);
    if (d.m_associativeTab == tab)
        return;
    d.m_associativeTab = tab;
    if (d.m_associativeTab == Q_NULL)
        return;

    d.m_associativeTab->setPage(this);
    d.m_associativeTab->setContextColor(d.m_contextColor);
    d.m_associativeTab->setContextText(d.m_contextTitle);

    for (int j = 0, count = groupCount(); count > j; ++j)
    {
        if (RibbonGroup* group = getGroup(j))
        {
            QList<QAction*> actList = group->actions();

            for (int i = 0, actCount = actList.size(); actCount > i; ++i) 
            {
                QAction* a = actList.at(i);
                d.m_listShortcuts.append(a);
                d.m_associativeTab->addAction(a);
            }
        }
    }
}

RibbonTab* RibbonPage::associativeTab() const
{
    QTN_D(const RibbonPage);
    return d.m_associativeTab;
}

bool RibbonPage::canActivate()
{
    bool ret = true;
    emit activating(ret);
    return ret;
}

void RibbonPage::setGroupsHeight(int height)
{
    QTN_D(RibbonPage);
    d.m_groupsHeight = height;
}

/*! \reimp */
bool RibbonPage::event(QEvent* event)
{
    QTN_D(RibbonPage);
    switch(event->type())
    {
        case QEvent::LayoutRequest:
                d.updateLayout();
            break;
        case QEvent::Timer:
            {
                QTimerEvent* timerEvent = static_cast<QTimerEvent*>(event);
                if (d.m_scrollTimer.timerId() == timerEvent->timerId()) 
                {
                    d.scrollGroupAnimate();
                    event->accept();
                    return true;
                }
            }
            break;
        case QEvent::StyleChange:
        case QEvent::FontChange:
            {
                qtn_set_font_to_ribbon_children(this, font());
            }
            break;
        case QEvent::WinIdChange:
            break;
        case QEvent::ChildAdded :
            {
                QChildEvent* childEvent = static_cast<QChildEvent*>(event);
                if (RibbonGroup* group = qobject_cast<RibbonGroup*>(childEvent->child()))
                    group->installEventFilter(&d);

                if (childEvent->added())
                {
                    QWidget* widget = qobject_cast<QWidget *>(childEvent->child());
                    if (widget != Q_NULL)
                    {
                        if (qobject_cast<RibbonGroup *>(widget) != Q_NULL)
                            qobject_cast<RibbonGroup *>(widget)->setFont(font());
                        else
                        {
                            widget->setFont(font());
                            qtn_set_font_to_ribbon_children(widget, font());
                        }
                    }
                }
            }
            break;
        case QEvent::ChildRemoved :
            {
                QChildEvent* childEvent = static_cast<QChildEvent*>(event);
                if (RibbonGroup* group = qobject_cast<RibbonGroup*>(childEvent->child()))
                    group->removeEventFilter(&d);
            }
            break;
        default:
            break;
    }

    return QWidget::event(event);
}

/*! \reimp */
QSize RibbonPage::sizeHint() const
{
    QTN_D(const RibbonPage);
    QSize resultSize = QWidget::sizeHint();

    // Calculate a max height of the group. Group may contain a different number of rows(3 - rows, 5 - rows)
    int maxHeight = 0;
    for (int i = 0, count = groupCount(); count > i; i++)
    {
        if (RibbonGroup* group = getGroup(i))
            maxHeight = qMax(maxHeight, group->sizeHint().height());
    }
    resultSize.setHeight(maxHeight);

    if (d.isPopupMode())
        resultSize.setHeight(resultSize.height() - pageMarginPopup);

    return resultSize;
}

/*! \reimp */
void RibbonPage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QTN_D(RibbonPage);
    if (d.isPopupMode())
    {
        QPainter p(this);
        StyleOptionRibbon opt;
        opt.init(this);
        opt.rect.adjust( 0, 0, 0, pageMarginPopup);
        style()->drawControl((QStyle::ControlElement)RibbonStyle::CE_RibbonGroups, &opt, &p, this->parentWidget());
    }
}

/*! \reimp */
void RibbonPage::changeEvent(QEvent* event)
{
    QTN_D(RibbonPage);
    switch (event->type()) 
    {
        case QEvent::StyleChange:
                d.updateLayout();
            break;
        case QEvent::FontChange: 
            {
                for (int i = 0; i < d.m_listGroups.size (); i++)
                {
                    RibbonGroup* group = d.m_listGroups.at(i);
                    group->setFont(font());
                }
                d.updateLayout();
            }
            break;
        default:
            break;
    };
    return QWidget::changeEvent(event);
}

/*! \reimp */
void RibbonPage::resizeEvent(QResizeEvent* event)
{
    QTN_D(RibbonPage);
    QSize delta = event->size() - event->oldSize();
    if (delta.isNull())
        return;
    d.updateLayout();
}


/*! \reimp */
#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool RibbonPage::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    bool res = QWidget::nativeEvent(eventType, message, result);
    MSG* msg = static_cast<MSG *>(message);
#else
bool RibbonPage::winEvent(MSG* message, long* result)
{
    bool res = QWidget::winEvent(message, result);
    MSG* msg = static_cast<MSG *>(message);
#endif
    if (!res)
    {
        if (msg->message == WM_LBUTTONDOWN) 
        {
            if (QWidget* activePopupWidget = QApplication::activePopupWidget())
            {
                if (activePopupWidget == this)
                {
                    POINT curPos = msg->pt;
                    QPoint globalPos(curPos.x, curPos.y);

                    QPoint pos = mapFromGlobal(globalPos);

                    QToolButton* toolButton = qobject_cast<QToolButton*>(activePopupWidget->childAt(pos));

                    if (toolButton && toolButton->isEnabled())
                    {
                        if (QAction* action = toolButton->defaultAction())
                        {
                            if (action->menu())
                            {
                                int button = Qt::LeftButton;
                                int state = 0;

                                pos = toolButton->mapFromGlobal(globalPos);

                                QMouseEvent e(QEvent::MouseButtonPress, pos, globalPos,
                                    Qt::MouseButton(button),
                                    Qt::MouseButtons(state & Qt::MouseButtonMask),
                                    Qt::KeyboardModifiers(state & Qt::KeyboardModifierMask));

                                res = QApplication::sendEvent(toolButton, &e);
                                res = res && e.isAccepted();
                            }
                        }
                    }
                }
            }
        }
    }
    return res;
}
#endif

void Qtitan::qtn_set_page_visible(RibbonPage* page, bool visible)
{
    RibbonPageVisibleAutoUpdater autoUpdater(RibbonPagePrivate::_get(page));
    page->setVisible(visible);
}


