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
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleOption>
#include <QPainter>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLayout>
#include <QWidgetAction>
#include <qevent.h>
#include <QTranslator>

#include "QtnRibbonPrivate.h"
#include "QtnStyleHelpers.h"
#include "QtnRibbonButtonPrivate.h"
#include "QtnRibbonStyle.h"
#include "QtnRibbonBarPrivate.h"
#include "QtnOfficeFrameHelper.h"
#include "QtnRibbonSystemPopupBar.h"
#include "QtnRibbonBackstageView.h"
#include "QtnRibbonBackstageViewPrivate.h"
#include "QtnRibbonToolTip.h"
#include "QtnRibbonGroup.h"
#include "QtnRibbonGallery.h"
#include "QtnRibbonCustomizeManager.h"
#include "QtnRibbonPagePrivate.h"
#include "QtnRibbonDef.h"
#include "QtnOfficePopupMenu.h"
#ifdef Q_OS_WIN
#include "QtnOfficeFrameHelper_win.h"
#endif // Q_OS_WIN
#ifdef DEBUG_MEMORY_ENABLED
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE

const QString strCustomizeQAToolBar = QString("customizeQAToolBar");
const QString strCustomizeRibbonBar = QString("customizeRibbonBar");


/*!
*/
LogotypeLabel::LogotypeLabel(QWidget* parent)
    : QLabel( parent )
    , m_alignment(Qt::AlignRight)
{
}

/*! \reimp */
void LogotypeLabel::paintEvent(QPaintEvent* event)
{
    bool visible = true;
    if (RibbonBar* ribbonBar = qobject_cast<RibbonBar*>(parentWidget()))
        visible = ribbonBar->isMaximized();

    if (!visible)
        return;

    QPainter p(this);
    StyleOptionRibbon opt;
    opt.init(this);

    QRect rect = parentWidget()->geometry();
    if (m_alignment == Qt::AlignRight)
    {
        QPoint pnt = mapFromParent(rect.topLeft());
        opt.rect.setLeft(pnt.x());
        opt.rect.setWidth(rect.width());
    }
    else
        opt.rect.setWidth(rect.width());

    style()->drawControl((QStyle::ControlElement)RibbonStyle::CE_RibbonGroups, &opt, &p, this);

    QLabel::paintEvent(event);
}


/*!
    \enum RibbonBar::QuickAccessBarPosition
    This enum describes the position of quick access bar widget on ribbon.
    \inmodule QtitanRibbon
    \value QATopPosition quick access bar is placed on the title (or the top) of the ribbon.
    \value QABottomPosition quick access bar is placed on the bottom of the ribbon.    
*/

/*!
\property RibbonBar::currentIndexPage
\inmodule QtitanRibbon
*/

/*!
\property RibbonBar::minimized
\inmodule QtitanRibbon
*/

/*!
\property RibbonBar::maximized
\inmodule QtitanRibbon
*/

RibbonBarPagePopup::RibbonBarPagePopup(RibbonBar* ribbonBar)
    : QWidget(ribbonBar, Qt::Popup), m_ribbonBar(ribbonBar)
{
}

RibbonBar* RibbonBarPagePopup::ribbonBar() const
{
    return m_ribbonBar;
}

void RibbonBarPagePopup::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
}

bool RibbonBarPagePopup::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::Show:
        {
        }
        break;
#ifdef Q_OS_MAC
        case QEvent::Close:
        {
            if (event->spontaneous())
            {
                event->ignore();
                return true;
            }
        }
        break;
#endif
        case QEvent::Hide:
        {
            RibbonBar* _ribbonBar = ribbonBar();
            if (_ribbonBar == Q_NULL)
                break;
            _ribbonBar->update();
        }
        break;
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        {
            RibbonBar* _ribbonBar = ribbonBar();
            if (_ribbonBar == Q_NULL)
                break;
            if (_ribbonBar->isMinimized())
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QWidget* widget = QApplication::widgetAt(mouseEvent->globalPos());
                RibbonTab* tab = qobject_cast<RibbonTab *>(widget);
                if (tab != Q_NULL)
                {
                    if (mouseEvent->type() == QEvent::MouseButtonDblClick)
                        QMetaObject::invokeMethod(tab, "dblClicked", Qt::QueuedConnection);
                    else
                        QMetaObject::invokeMethod(tab, "clicked", Qt::QueuedConnection);
                    return true;
                }
            }
        }
        break;
        default:
        break;
    }
    return QWidget::event(event);
}

/*!
\class RibbonBarPrivate
\internal
*/
RibbonBarPrivate::RibbonBarPrivate()
{
    m_tabBar = Q_NULL;
    m_pagePopupWidget = Q_NULL;
    m_quickAccessBar = Q_NULL;
    m_controlSystemButton = Q_NULL;
    m_rowItemHeight = 22;
    m_rowItemCount = 3;
    m_updateLayoutCount = 0;
    m_heightTabs = -1;
    m_frameHelper = Q_NULL;
    m_showTitleAlways = true;
    m_ribbonBarVisible = true;
    m_minimizationEnabled = true;
    m_keyboardState = false;
    m_altPressed = false;
    m_keyTipsComplement = true;
    m_keyTipsEnabled = true;
    m_keyTipsShowing = false;
    m_minimized = false;
    m_titleGroupsVisible = true;
    m_keyboardFocusWidget = Q_NULL;
    m_quickAccessBarPosition = RibbonBar::QATopPosition;
    m_tabBarPosition = RibbonBar::TabBarTopPosition;
    m_hoveredSubControl = QStyle::SC_None;
    m_activeSubControl = QStyle::SC_None;
    m_countKey = 0;
    m_customizeManager = Q_NULL;
    m_customizeDialog = Q_NULL;
}

RibbonBarPrivate::~RibbonBarPrivate()
{
    removeAllContextHeaders();
}

RibbonBarPrivate* RibbonBarPrivate::_get(RibbonBar* ribbon)
{
    return &ribbon->qtn_d();
}

const RibbonBarPrivate* RibbonBarPrivate::_get(const RibbonBar* ribbon)
{
    return &ribbon->qtn_d();
}

void RibbonBarPrivate::init()
{
    QTN_P(RibbonBar);
    p.ensurePolished();
    p.setNativeMenuBar(false);
    RibbonStyle* ribbonStyle = qobject_cast<RibbonStyle*>(p.style());
    if (ribbonStyle)
        p.setFont(ribbonStyle->font(&p));
    else
        p.setFont(QApplication::font("Qtitan::RibbonBar"));

    p.setAttribute(Qt::WA_Hover, true);
    p.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum); 

    m_tabBar = new RibbonTabBar(&p);
    m_tabBar->show();
    QObject::connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(currentTabBarChanged(int)));

    m_pagePopupWidget = new RibbonBarPagePopup(&p);

    m_quickAccessBar = new RibbonQuickAccessBar(&p);
    m_quickAccessBar->setObjectName("qtn_Ribbon_Quick_Access_Bar");
    m_quickAccessBar->setVisible(false);

    m_logotypeLabel = new LogotypeLabel(&p);
    m_logotypeLabel->setVisible(false);
}

void RibbonBarPrivate::calcHeightItem()
{
    QTN_P(RibbonBar);
    QFont fontRegular = p.font();
    QFontMetrics fm(fontRegular);
    m_rowItemHeight = qMax(fm.height() + ((fm.descent() + fm.ascent()) / 3), 17) + 3;

    QLineEdit ed;
    ed.setAttribute(Qt::WA_MacSmallSize);
    ed.setFont(fontRegular);
    QSize sizeHint = ed.sizeHint();
    m_rowItemHeight = qMax(sizeHint.height(), m_rowItemHeight);

    QComboBox box1;
    box1.setAttribute(Qt::WA_MacSmallSize);
    box1.setFont(fontRegular);
    sizeHint = box1.sizeHint();
    m_rowItemHeight = qMax(sizeHint.height(), m_rowItemHeight);

    QComboBox box2;
    box2.setAttribute(Qt::WA_MacSmallSize);
    box2.setEditable(true); //Affects on MacOSX widget
    box2.setFont(fontRegular);
    sizeHint = box2.sizeHint();
    m_rowItemHeight = qMax(sizeHint.height(), m_rowItemHeight);

    QSpinBox box3;
    box3.setAttribute(Qt::WA_MacSmallSize);
    box3.setFont(fontRegular);
    sizeHint = box3.sizeHint();
    m_rowItemHeight = qMax(sizeHint.height(), m_rowItemHeight);

    QTabBar tab;
    tab.setFont(fontRegular);
    tab.setAttribute(Qt::WA_MacSmallSize);
    tab.insertTab(0, QString("TEXT"));

    bool isDPIAware = true;
    RibbonStyle* ribbonStyle = qobject_cast<RibbonStyle*>(p.style());
    if (ribbonStyle)
        isDPIAware = ribbonStyle->isDPIAware();

    int hframe = qMax(tab.sizeHint().height(), (isDPIAware ? int(DrawHelpers::dpiScaled(24.)) : 24));
    int heightTab = qMax(22, hframe + 1);
    m_heightTabs = heightTab > 22 ? heightTab : 23;
}

void RibbonBarPrivate::layoutRibbon()
{
    QTN_P(RibbonBar);
    /* In MacOSX the font has been selected in RibbonStyle::polish(), so we have non valid reference to the tabbar. */
    if (m_tabBar == Q_NULL || m_quickAccessBar == Q_NULL)
        return;

    p.setUpdatesEnabled(false);

    calcHeightItem();

    bool saveRibbonBarVisible = m_ribbonBarVisible;
    m_ribbonBarVisible = true;

    QStyleOption opt;
    opt.init(&p);
    const int vmargin = p.style()->pixelMetric(QStyle::PM_MenuBarVMargin, &opt, &p);
    int heightGroup = maxGroupHeight();
    const int tabsHeight = m_tabBar->sizeHint().height();
    const int pageAreaHeight = heightGroup;

    int quickAccessHeight = m_quickAccessBar->sizeHint().height();

    bool frameThemeEnabled = p.isFrameThemeEnabled();
    bool showTitle = frameThemeEnabled || m_showTitleAlways;

    int frameBorder = 4;

    QRect rcTabBar(QPoint(2, 0), QSize(opt.rect.width() - 2, tabsHeight));
    QRect pageRect(QPoint(0, tabsHeight), QSize(opt.rect.width(), pageAreaHeight + vmargin * 2 + 2 + 1));

    if (p.tabBarPosition() == RibbonBar::TabBarBottomPosition)
    {
        rcTabBar.translate(0, pageRect.height() - 2);
        pageRect.translate(0, -tabsHeight);
    }

    m_rcTitle = QRect();
    m_rcHeader = QRect();
    m_rcTitleText = QRect();

    if (showTitle)
    {
        frameBorder = m_frameHelper ? m_frameHelper->frameBorder() : 0;
        int ribbonTopBorder = topBorder();
        int titleOffset = ribbonTopBorder - frameBorder;

        m_rcTitle = QRect(QPoint(-frameBorder, titleOffset), 
            QPoint(opt.rect.width() + frameBorder, p.titleBarHeight() + ribbonTopBorder));

        rcTabBar.translate(0, m_rcTitle.height() + titleOffset);
        pageRect.translate(0, m_rcTitle.height() + titleOffset);

        m_rcHeader = QRect(0, 0, opt.rect.width(), m_rcTitle.bottom());
        m_rcTitleText  = QRect(0, 0, opt.rect.width(), m_rcTitle.bottom());

        if (!m_frameHelper)
            m_rcTitleText = QRect();
    }

    if (m_controlSystemButton && m_controlSystemButton->toolButtonStyle() == Qt::ToolButtonTextBesideIcon)
        m_controlSystemButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    int corner = quickAccessHeight + tabsHeight - 3;

    if (!m_controlSystemButton || m_controlSystemButton->toolButtonStyle() != Qt::ToolButtonFollowStyle)
    {
        StyleOptionTitleBar titleBarOpt;
        getTitleBarOption(titleBarOpt);

        QSize szIcon = m_frameHelper ? m_frameHelper->sizeSystemIcon(titleBarOpt.icon, titleBarOpt.rect) : QSize(16, 16);
        corner = 5 + (!titleBarOpt.icon.isNull() ? szIcon.width() : 0);
    }

    QSize szBar = m_quickAccessBar->sizeHint();
    if (!m_quickAccessBar->isHidden())
    {
        if (m_quickAccessBarPosition == RibbonBar::QATopPosition)
        {
            m_rcQuickAccess = QRect(QPoint(5 + corner, m_rcTitle.top() + frameBorder), szBar);
        }
        else if (m_quickAccessBarPosition == RibbonBar::QABottomPosition)
        {
            if (m_minimized)
                m_rcQuickAccess = QRect(QPoint(0, m_rcTabBar.bottom()), QPoint(pageRect.right(), m_rcTabBar.bottom() + quickAccessHeight));
            else if (m_tabBarPosition == RibbonBar::TabBarTopPosition)
                m_rcQuickAccess = QRect(QPoint(0, pageRect.bottom()), QPoint(pageRect.right(), pageRect.bottom() + quickAccessHeight));
            else if (m_tabBarPosition == RibbonBar::TabBarBottomPosition)
                m_rcQuickAccess = QRect(QPoint(0, rcTabBar.bottom() + 2), QPoint(pageRect.right(), rcTabBar.bottom() + quickAccessHeight + 2));
        }
    }

    QPoint posAccessBar = m_rcQuickAccess.topLeft();
    m_quickAccessBar->move(posAccessBar);
    m_quickAccessBar->resize(m_rcQuickAccess.size());

    int left = 2;
    int right = opt.rect.width() - 2;
    if (m_controlSystemButton)
    {
        QSize szControl(corner + 1, corner + 1);
        int top = m_rcTitle.top() + frameBorder;
        left = 0;
        int nGap = 0;

        if (p.tabBarPosition() == RibbonBar::TabBarBottomPosition && m_controlSystemButton->toolButtonStyle() == Qt::ToolButtonFollowStyle)
        {
            top = pageRect.bottom() - m_controlSystemButton->sizeHint().height();
        }
        else if (m_controlSystemButton->toolButtonStyle() != Qt::ToolButtonFollowStyle)
        {
            szControl = m_controlSystemButton->sizeHint();
            szControl.setHeight(tabsHeight);

            top = rcTabBar.top() + (tabsHeight - szControl.height()) * 0.5;
            nGap = 3;
        }
        else if (!showTitle)
        {
            szControl = m_controlSystemButton->sizeHint();
            top = rcTabBar.top() + (tabsHeight - szControl.height()) * 0.5;
        }
        else
        {
            if (top == 0)
                top += frameBorder/2;
            szControl -= QSize(2, 2);
        }

        int add = p.tabBarPosition() == RibbonBar::TabBarBottomPosition ? 2 : 0;
        szControl.setHeight(szControl.height() - add);
        QRect rcControl(QPoint(left + 1, top + add), szControl);
        left += szControl.width() + nGap;
        m_controlSystemButton->raise();
        m_controlSystemButton->setGeometry(rcControl);
    }

    bool quickAccessNearTabs = !m_quickAccessBar->isHidden() && m_quickAccessBarPosition == RibbonBar::QATopPosition && !showTitle;

    if (quickAccessNearTabs)
    {
        m_rcQuickAccess.setLeft(left);
        m_rcQuickAccess.setTop(m_rcTabBar.top());
        m_rcQuickAccess.setBottom(m_rcTabBar.bottom());

        m_quickAccessBar->move(m_rcQuickAccess.topLeft());

        left += szBar.width();
    }

    if (left + 6 > right)
        m_rcTabBar = QRect(QPoint(0, rcTabBar.top()), QPoint(0, rcTabBar.bottom() + 1));
    else
        m_rcTabBar = QRect(QPoint(left, rcTabBar.top()), QPoint(right, rcTabBar.bottom() + 1));

    // It's needed to apply a width of the logo if exists.
    QRect logoRect = !m_logotypeLabel->isHidden() ? m_logotypeLabel->geometry() : QRect();
    if (!logoRect.isNull() && m_logotypeLabel->alignment() == Qt::AlignLeft)
        m_rcTabBar.setLeft(qMax(m_rcTabBar.left(), logoRect.width()));

    m_tabBar->setGeometry(m_rcTabBar);
    m_tabBar->layoutWidgets();

    if (!m_logotypeLabel->isHidden())
    {
        if (m_logotypeLabel->alignment() == Qt::AlignRight)
            pageRect.adjust(0, 0, -4, 0);
        else
            pageRect.adjust(2, 0, 0, 0);
    }

    m_rcPageArea = pageRect;

    if (p.isMinimized())
    {
        m_rcPageArea.setHeight(0);
        pageRect.moveTo(0, 0);
    }

    for (QList<RibbonPage*>::iterator it = m_listPages.begin(); it != m_listPages.end(); ++it)
    {
        RibbonPage* page = *(it);
        page->setGroupsHeight(heightGroup);
        page->setGeometry(pageRect);
    }

    layoutTitleButton();

    if (m_frameHelper)
        m_frameHelper->setHeaderRect(m_rcHeader);

    m_ribbonBarVisible = saveRibbonBarVisible;

    bool ribbonBarVisible = true;
    if (p.isTitleBarVisible())
    {
        ribbonBarVisible = m_rcHeader.width() > 3;
        
        if (m_quickAccessBarPosition == RibbonBar::QABottomPosition)
        {
            if (m_controlSystemButton && m_controlSystemButton->toolButtonStyle() == Qt::ToolButtonFollowStyle)
            {
                const int wd = m_controlSystemButton->sizeHint().width();
                ribbonBarVisible = m_rcHeader.width() - (wd*2) > 3;
            }
            else
            {
                const int left = m_rcHeader.left();
                ribbonBarVisible = m_rcHeader.width() - left * 2 > 3;
            }
        }
    }
    else
        ribbonBarVisible = opt.rect.width() >= 250;

    if (m_ribbonBarVisible != ribbonBarVisible && p.QMenuBar::isVisible())
    {
        m_ribbonBarVisible = ribbonBarVisible;
        if (m_frameHelper)
            m_frameHelper->postRecalcFrameLayout();
    }

    if (!m_ribbonBarVisible && m_frameHelper && m_frameHelper->canHideTitle())
    {
        m_rcTitleText.setLeft(m_rcQuickAccess.left());
        m_rcTitleText.setRight(m_rcHeader.right());
        m_quickAccessBar->setGeometry(QRect());
    }

    if (showTitle && m_ribbonBarVisible)
        layoutContextHeaders();
    else
        removeAllContextHeaders();

    p.setUpdatesEnabled(true);

    if (!m_logotypeLabel->isHidden())
    {
        QRect rectLogotype;
        int width = m_logotypeLabel->pixmap()->width();
        int height = m_logotypeLabel->pixmap()->height();

        if (height < m_rcPageArea.height())
            rectLogotype.setTop(m_rcPageArea.top());
        else
        {
            rectLogotype.setTop(0);
        }

        if ( m_logotypeLabel->alignment() == Qt::AlignRight)
            rectLogotype.setLeft(m_rcPageArea.right() - width + 4);

        rectLogotype.setWidth(width);
        rectLogotype.setHeight(m_rcPageArea.height());

        m_logotypeLabel->setGeometry(rectLogotype);
        m_logotypeLabel->raise();
    }

    RibbonPage* currentPage = validIndex(m_tabBar->currentIndex()) ? m_listPages.at(m_tabBar->currentIndex()) : Q_NULL;
    if (currentPage != Q_NULL)
        currentPage->updateLayout();
}

static bool buttonVisible(const QStyle::SubControl sc, const StyleOptionTitleBar* tb)
{
    bool isMinimized = tb->titleBarState & Qt::WindowMinimized;
    bool isMaximized = tb->titleBarState & Qt::WindowMaximized;
    const uint flags = tb->titleBarFlags;
    bool retVal = false;
    switch (sc) 
    {
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
        default :
            retVal = true;
    }
    return retVal;
}

void RibbonBarPrivate::layoutTitleButton()
{
    QTN_P(RibbonBar);
    if (!p.isFrameThemeEnabled())
        return;

    StyleOptionTitleBar titleBarOpt;
    getTitleBarOption(titleBarOpt);

    QRect rcQuickAccess = m_rcQuickAccess;
    if (m_ribbonBarVisible)
    {
        if (m_quickAccessBarPosition != RibbonBar::QATopPosition || !m_quickAccessBar->isVisible())
        {
            if (!m_controlSystemButton)
                m_rcHeader.setLeft(m_rcHeader.left() + titleBarOpt.icon.actualSize(QSize(16, 16)).width() + 2);
            else
            {
                if (m_controlSystemButton->toolButtonStyle() == Qt::ToolButtonFollowStyle)
                    m_rcHeader.setLeft(m_rcHeader.left() + m_controlSystemButton->sizeHint().width() - 5);
                else
                    m_rcHeader.setLeft(m_rcHeader.left() + titleBarOpt.icon.actualSize(QSize(16, 16)).width() + 2);
            }

            if (m_controlSystemButton)
                rcQuickAccess = m_controlSystemButton->rect();
            else
                rcQuickAccess = QRect();
        }
        else
        {
            rcQuickAccess.setRight(rcQuickAccess.left() + (m_quickAccessBar->isVisible() ? m_quickAccessBar->width() + 2 : 12));
            m_rcHeader.setLeft(rcQuickAccess.right());
        }
    }
    else
    {
        int corner = 2 + (!titleBarOpt.icon.isNull() ? titleBarOpt.icon.actualSize(QSize(16, 16)).width() : 0);
        m_rcHeader.setLeft(corner);
    }

    bool dwmEnabled = m_frameHelper->isDwmEnabled();

    int frameBorder = m_frameHelper && dwmEnabled ? m_frameHelper->frameBorder() : 0;
    m_rcHeader.adjust(0, frameBorder, 0, 0);
    QRect rcButtons = m_rcHeader;
    m_actCloseButton = addTitleButton(QStyle::SC_TitleBarCloseButton, !dwmEnabled && ::buttonVisible(QStyle::SC_TitleBarCloseButton, &titleBarOpt), rcButtons);
    m_actNormalButton = addTitleButton(QStyle::SC_TitleBarNormalButton, !dwmEnabled && ::buttonVisible(QStyle::SC_TitleBarNormalButton, &titleBarOpt), rcButtons);
    m_actMaxButton = addTitleButton(QStyle::SC_TitleBarMaxButton, !dwmEnabled && ::buttonVisible(QStyle::SC_TitleBarMaxButton, &titleBarOpt), rcButtons);
    m_actMinButton = addTitleButton(QStyle::SC_TitleBarMinButton, !dwmEnabled && ::buttonVisible(QStyle::SC_TitleBarMinButton, &titleBarOpt), rcButtons);
    m_rcHeader = rcButtons;

#ifdef Q_OS_WIN
    if (dwmEnabled)
    {
        int width = (::GetSystemMetrics(SM_CXSIZE) + ::GetSystemMetrics(SM_CXBORDER) + 8) * 3;
        m_rcHeader.setRight(m_rcHeader.right() - width);
    }
#endif // Q_OS_WIN
    m_rcTitleText = m_rcHeader;

    QString strTitle = p.parentWidget() ? p.parentWidget()->windowTitle() : QString();

    QFont font;
    if (!m_frameHelper->getCaptionQtnFont(1, &font))
        font = QApplication::font("QMdiSubWindowTitleBar");
    const QFontMetrics fm(font);

    QRect br(fm.boundingRect(strTitle));

    int captionLength = br.width();

    if (dwmEnabled)
        captionLength += 2 * 10;// glowSize = 10 for Windows7 (Vista)

    captionLength = qMin(m_rcTitleText.width(), captionLength);

    QRect rcTitleText(m_rcTitleText);
    rcTitleText.setLeft(rcTitleText.left() + (rcTitleText.width() - captionLength) * 0.5);
    if (rcTitleText.left() < m_rcHeader.left())
        rcTitleText.setLeft(m_rcHeader.left());

    rcTitleText.setRight(rcTitleText.left() + captionLength);

    if (rcQuickAccess.intersects(rcTitleText))
    {
        captionLength = qMin(m_rcHeader.width(), captionLength);
        rcTitleText.setRight(rcTitleText.left() + captionLength);
    }
    else if (!rcQuickAccess.isValid() || !m_quickAccessBar->isVisible())
    {
        captionLength = qMin(m_rcHeader.width(), captionLength);
        if (dwmEnabled)
            rcTitleText.setLeft(m_rcHeader.left() + 10);
        else
            rcTitleText.setLeft(m_rcHeader.left() + (m_rcHeader.width() - captionLength) * 0.5);
    }

    if (rcTitleText.right() > m_rcHeader.right())
        rcTitleText.setRight(m_rcHeader.right());

    m_rcTitleText = rcTitleText;

    if (m_controlSystemButton && (bool)p.style()->styleHint((QStyle::StyleHint)RibbonStyle::SH_RibbonBackstageHideTabs))
    {
        if (RibbonBackstageView* backstage = qobject_cast<RibbonBackstageView*>(m_controlSystemButton->backstage()))
        {
            if (backstage->isVisible())
            {
                m_rcTitleText.setLeft(backstage->menuGeometry().width());
                m_rcTitleText.setRight(m_rcHeader.right());
            }
        }
    }
}

void RibbonBarPrivate::layoutContextHeaders()
{
    QTN_P(RibbonBar);
    removeAllContextHeaders();

    if (!(p.isFrameThemeEnabled() || m_showTitleAlways))
        return;

    ContextHeader* prevContextHeader = Q_NULL;

    int count = m_tabBar->getTabCount();
    if (count == 0)
        return;

    if (m_quickAccessBarPosition == RibbonBar::QATopPosition && m_quickAccessBar->isVisible() && !m_frameHelper)
        m_rcHeader.setLeft(m_rcQuickAccess.right() + (m_quickAccessBar ? m_quickAccessBar->sizeHint().width() + 2 : 12));

    QRect rectTitleText = m_rcTitleText.isEmpty() ? m_rcHeader : m_rcTitleText;

    for (int i = 0; i < count; ++i)
    {
        RibbonTab* tab = m_tabBar->getTab(i);

        if (!tab->isVisible())
            continue;

        if (tab->contextColor() == RibbonPage::ContextColorNone || tab->contextText().isEmpty())
        {
            prevContextHeader = Q_NULL;
            continue;
        }

        QRect rect = tab->geometry();
        QPoint topLeft = m_tabBar->mapToParent(rect.topLeft());
        QPoint bottomRight = m_tabBar->mapToParent(rect.bottomRight());

        if (prevContextHeader && !prevContextHeader->strGroupName.isEmpty() && prevContextHeader->strGroupName == tab->contextGroupName())
        {
            prevContextHeader->rcRect.setRight(bottomRight.x());
            prevContextHeader->lastTab = tab;
            tab->setContextHeader(prevContextHeader);
        }
        else
        {
            ContextHeader* header = new ContextHeader(m_tabBar, tab);
            header->rcRect = QRect(QPoint(topLeft.x(), rectTitleText.top()), QPoint(bottomRight.x(), rectTitleText.bottom() + 1));
            m_listContextHeaders.append(header);
            prevContextHeader = header;
        }
    }

    count = m_listContextHeaders.count();
    if (count == 0)
        return;

    if (!reduceContextHeaders())
    {
        removeAllContextHeaders();
        return;
    }

    QRect rcHeaders(QPoint(m_listContextHeaders.at(0)->rcRect.left(), m_rcTitle.top() + 6),
        QPoint(m_listContextHeaders.at(count - 1)->rcRect.right(), m_rcTitle.bottom()));

    int captionLength = m_rcTitleText.width();
    QRect rcCaptionText(m_rcTitleText);

    if (rcCaptionText.intersects(rcHeaders))
    {
        if (rcCaptionText.center().x() > rcHeaders.center().x())
        {
            if (m_rcHeader.right() - rcHeaders.right() < captionLength)
            {
                m_rcTitleText.setLeft(rcHeaders.right());
                m_rcTitleText.setRight(m_rcHeader.right());
            }
            else
            {
                m_rcTitleText.setLeft(rcHeaders.right());
                m_rcTitleText.setRight(rcHeaders.right() + captionLength);
            }
        }
        else
        {
            if (rcHeaders.left() - m_rcHeader.left() < captionLength)
            {
                m_rcTitleText.setLeft(m_rcHeader.left());
                m_rcTitleText.setRight(rcHeaders.left());
            }
            else
            {
                m_rcTitleText.setLeft(rcHeaders.left() - captionLength);
                m_rcTitleText.setRight(rcHeaders.left());
            }
        }
    }
}

void RibbonBarPrivate::layoutCorner()
{
    QTN_P(RibbonBar);
    if (QWidget* rightWidget = p.cornerWidget(Qt::TopRightCorner)) 
    {
        int hmargin = p.style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, &p);

        if (rightWidget && rightWidget->isVisible()) 
        {
            QRect rcTab = m_tabBar->geometry();
            QRect rc = rightWidget->geometry();
            QPoint pos(p.width() - rc.width() - hmargin + 1, rcTab.top() + (rcTab.height() - rc.height())/2);
            QRect vRect = QStyle::visualRect(p.layoutDirection(), p.rect(), QRect(pos, rc.size()));
            rightWidget->setGeometry(vRect);
        }
    }
}

bool RibbonBarPrivate::reduceContextHeaders()
{
    int left = m_rcHeader.left();
    int right = m_rcHeader.right();

    int count = m_listContextHeaders.count();

    for (int i = 0; i < count; i++)
    {
        ContextHeader* header =  m_listContextHeaders.at(i);
        if (header->rcRect.left() < left)
        {
            header->rcRect.setLeft(left);
            if (header->rcRect.width()  < 40)
                header->rcRect.setRight(left + 40);

            left = header->rcRect.right();
        }
        else break;
    }
    for (int i = count - 1; i >= 0; i--)
    {
        ContextHeader* header =  m_listContextHeaders.at(i);
        if (header->rcRect.right() > right)
        {
            header->rcRect.setRight(right);
            if (header->rcRect.width() < 40)
                header->rcRect.setLeft(right - 40);

            right = header->rcRect.left();

            if (right < left)
                return false;
        }
        else break;
    }
    return true;
}

void RibbonBarPrivate::removeAllContextHeaders()
{
    for (int i = 0; i < m_listContextHeaders.count(); i++)
        delete m_listContextHeaders.at(i);
    m_listContextHeaders.clear();
}


void RibbonBarPrivate::setVisibleBackstage(bool visible)
{
    QTN_P(RibbonBar);
    if ((bool)p.style()->styleHint((QStyle::StyleHint)RibbonStyle::SH_RibbonBackstageHideTabs))
    {
        if (visible)
        {
            if (m_visibleWidgets.size() == 0 && m_frameHelper)
                m_frameHelper->frameChanged();

            if (m_tabBar && m_tabBar->isVisible())
            {
                m_visibleWidgets << m_tabBar;
                m_tabBar->setVisible(false);
            }

            if (m_quickAccessBar && m_quickAccessBar->isVisible())
            {
                m_visibleWidgets << m_quickAccessBar;
                m_quickAccessBar->setVisible(false);
            }

            if (m_controlSystemButton && m_controlSystemButton->isVisible())
            {
                m_visibleWidgets << m_controlSystemButton;
                m_controlSystemButton->setVisible(false);
            }
        }
        else
        {
            RibbonBarAutoUpdater autoUpdater(&p);

            for (QVector<QWidget*>::ConstIterator it = m_visibleWidgets.begin(); it != m_visibleWidgets.end(); ++it)
                (*it)->setVisible(true);
            m_visibleWidgets.clear();

            if (m_frameHelper) 
                m_frameHelper->frameChanged();
        }
    }
}

QAction* RibbonBarPrivate::createSystemButton(const QIcon& icon, const QString& text)
{
    QTN_P(RibbonBar);
    QAction* actionSystemPopupBar = Q_NULL;
    if (icon.isNull())
    {
        QIcon iconQtnLogo;
        QIcon qtitan(":/res/qtitan.png");
        iconQtnLogo.addPixmap(qtitan.pixmap(QSize(64,56), QIcon::Disabled));
        QIcon icon32(":/res/qtitanlogo32x32.png");
        iconQtnLogo.addPixmap(icon32.pixmap(QSize(32,32), QIcon::Disabled));
        actionSystemPopupBar = new QAction(iconQtnLogo, text, &p);
    }
    else
        actionSystemPopupBar = new QAction(icon, text, &p);

    m_controlSystemButton = new RibbonSystemButton(&p);
    m_controlSystemButton->show();
    m_controlSystemButton->setAutoRaise(true);
    m_controlSystemButton->setFocusPolicy(Qt::NoFocus);
    m_controlSystemButton->setDefaultAction(actionSystemPopupBar);

    m_controlSystemButton->setPopupMode(QToolButton::InstantPopup);
    m_controlSystemButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    actionSystemPopupBar->setIconText(actionSystemPopupBar->text());

    return actionSystemPopupBar;
}

void RibbonBarPrivate::insertPage(int indTab, int index, RibbonPage* page)
{
    Q_ASSERT(page != Q_NULL);
    QTN_P(RibbonBar);

    if (!validIndex(index)) 
    {
        index = m_listPages.count();
        m_listPages.append(page);
    }
    else
        m_listPages.insert(index, page);

    if (RibbonTab* tab = m_tabBar->getTab(indTab))
        page->setAssociativeTab(tab);

    preparePages();
    p.updateLayout();
}

void RibbonBarPrivate::removePage(int index, bool deletePage)
{
    if (validIndex(index))
    {
        removeAllContextHeaders();
        RibbonPage* page = m_listPages[index];
        m_listPages.removeAt(index);
        if (deletePage)
            delete page;
        else
        {
            page->setAssociativeTab(Q_NULL);
            page->setParent(Q_NULL);
        }
    }
}

int RibbonBarPrivate::pageIndex(RibbonPage* page) const
{
    return m_listPages.indexOf(page);
}

int RibbonBarPrivate::heightTabs() const
{
    return m_heightTabs;
}

int RibbonBarPrivate::topBorder() const
{
    return m_frameHelper && m_frameHelper->isDwmEnabled() ? m_frameHelper->frameBorder() : 0;
}

void RibbonBarPrivate::showPagePopup(RibbonPage* page)
{
    QTN_P(RibbonBar);

    QStyleOption opt;
    opt.init(page);

    const int vmargin = p.style()->pixelMetric(QStyle::PM_MenuBarVMargin, &opt, &p);
    int heightGroup = maxGroupHeight();
    int height = heightGroup + vmargin * 2 + vmargin + 1;
    page->setGroupsHeight(heightGroup);

    QPoint pnt(0, 0);
    QRect rect = page->rect();
    rect.setTop(m_rcTabBar.top());
    rect.setBottom(m_rcTabBar.bottom());

    QRect screen = QApplication::desktop()->availableGeometry(&p);
    int h = height;
    if (p.mapToGlobal(QPoint(0, rect.bottom())).y() + h <= screen.height())
        pnt = p.mapToGlobal(rect.bottomLeft());
    else
        pnt = p.mapToGlobal(rect.topLeft() - QPoint(0, h));

    QRect rc(pnt, QSize(rect.width(), height));
    m_pagePopupWidget->setGeometry(rc);
    m_pagePopupWidget->show();
}

void RibbonBarPrivate::hidePagePopup()
{
    m_pagePopupWidget->hide();
}

bool RibbonBarPrivate::isPagePopupVisible() const
{
    return m_pagePopupWidget->isVisible();
}

QWidget* RibbonBarPrivate::getPageParent()
{
    QTN_P(RibbonBar)
    if (p.isMinimized())
        return m_pagePopupWidget;
    else
        return &p;
}

bool RibbonBarPrivate::isUpdating() const
{
    Q_ASSERT(m_updateLayoutCount >= 0);
    return m_updateLayoutCount > 0;
}

void RibbonBarPrivate::preparePages()
{
    const QList<RibbonTab*>& tabs = m_tabBar->tabs();
    QWidget* parent = getPageParent();
    for (QList<RibbonTab*>::const_iterator it = tabs.constBegin(); it != tabs.constEnd(); ++it)
    {
        RibbonTab* tab = *it;
        if (tab->page() != Q_NULL && tab->page()->parent() != parent)
        {
            RibbonPageVisibleAutoUpdater autoUpdater(RibbonPagePrivate::_get(tab->page()));
            tab->page()->setParent(parent);
            tab->page()->setVisible(tab->isSelected());
        }
    }
}

void RibbonBarPrivate::currentTabBarChanged(int index)
{
    QTN_P(RibbonBar);
    RibbonTab* tab = m_tabBar->getTab(index);
    emit p.currentPageIndexChanged(index);
    emit p.currentPageChanged(tab != Q_NULL ? tab->page() : Q_NULL);
    if (p.currentPageIndex() != -1 && p.currentPageIndex() != index)
    {
        if (p.parentWidget() != Q_NULL)
            p.parentWidget()->setFocus();
    }
}

void RibbonBarPrivate::getTitleBarOption(StyleOptionTitleBar& option) const
{
    QTN_P(const RibbonBar);
    QWidget* widget = p.parentWidget();
    if (!widget)
        return;

    option.initFrom(widget);
    option.frameHelper = m_frameHelper;

    if (m_activeSubControl != QStyle::SC_None) 
    {
        if (m_hoveredSubControl == m_activeSubControl) 
        {
            option.state |= QStyle::State_Sunken;
            option.activeSubControls = m_activeSubControl;
        }
    }
    else if (p.style()->styleHint(QStyle::SH_TitleBar_AutoRaise, 0, widget) && 
             m_hoveredSubControl != QStyle::SC_None &&
             m_hoveredSubControl != QStyle::SC_TitleBarLabel)
    {
        option.state |= QStyle::State_MouseOver;
        option.activeSubControls = m_hoveredSubControl;
    }
    else
    {
        option.state &= ~QStyle::State_MouseOver;
        option.activeSubControls = QStyle::SC_None;
    }

    option.subControls = QStyle::SC_All;
    option.titleBarFlags = widget->windowFlags();
    option.titleBarState = widget->windowState();

    if (option.titleBarFlags & Qt::WindowTitleHint)
    {
        QIcon icon = p.parentWidget() ? p.parentWidget()->windowIcon() : QIcon();
        QSize s = icon.actualSize(QSize(64, 64));
        option.icon = icon.pixmap(s);
    }

    if (m_frameHelper && option.icon.isNull())
        option.icon = m_frameHelper->getFrameSmallIcon();

    if (m_controlSystemButton && m_controlSystemButton->isVisible())
        option.drawIcon = !m_controlSystemButton  || m_controlSystemButton->toolButtonStyle() != Qt::ToolButtonFollowStyle;

    int border = p.style()->styleHint(QStyle::SH_TitleBar_NoBorder, &option, widget) ? 4 : 0;

    int paintHeight = p.titleBarHeight();
    paintHeight -= p.isMinimized() ? 2 * border : border;
    option.rect = m_rcTitle;
    
    RibbonBarPrivate* that = const_cast<RibbonBarPrivate*>(this);
    that->m_windowTitle = p.parentWidget() ? p.parentWidget()->windowTitle() : QString();

    if (!m_windowTitle.isEmpty()) 
    {
        // Set the text here before asking for the width of the title bar label
        // in case people uses the actual text to calculate the width.
        option.text = m_windowTitle;
        const QFont font = QApplication::font("QMdiSubWindowTitleBar");
        option.fontMetrics = QFontMetrics(font);
        int width = m_rcHeader.width();
        // Set elided text if we don't have enough space for the entire title.
        option.text = option.fontMetrics.elidedText(m_windowTitle, Qt::ElideRight, width);
    }

    option.rcTitleText = m_rcTitleText;
    int visCount = (m_quickAccessBar != Q_NULL) ? m_quickAccessBar->visibleCount() : int(0);
    option.quickAccessVisibleCount = visCount;
    option.quickAccessOnTop = m_quickAccessBarPosition == RibbonBar::QATopPosition;
    QSize szAccessBar =  (m_quickAccessBar != Q_NULL) ? m_quickAccessBar->size() : QSize();
    option.quickAccessVisible = (m_quickAccessBar != Q_NULL) && m_quickAccessBar->isVisible() && !szAccessBar.isNull();
    option.rcQuickAccess = m_rcQuickAccess;
    option.existSysButton = m_controlSystemButton;
    option.normalSysButton = m_controlSystemButton ? (m_controlSystemButton->toolButtonStyle() != Qt::ToolButtonFollowStyle) : true;
}

QStyle::SubControl RibbonBarPrivate::getSubControl(const QPoint& pos) const
{
    QTN_P(const RibbonBar);
    StyleOptionTitleBar opt;
    getTitleBarOption(opt);
    return p.style()->hitTestComplexControl(QStyle::CC_TitleBar, &opt, pos, p.parentWidget());
}

RibbonTitleButton* RibbonBarPrivate::findTitleButton(QStyle::SubControl subControl) const
{
    QTN_P(const RibbonBar);
    QList<RibbonTitleButton*> buttons_list = p.findChildren<RibbonTitleButton*>();
    foreach (RibbonTitleButton* but, buttons_list) 
    {
        if (but->subControl() == subControl)
            return but;
    }
    return Q_NULL;
}

QAction* RibbonBarPrivate::addTitleButton(QStyle::SubControl subControl, bool add, QRect& rcControl)
{
    QTN_P(RibbonBar);

    RibbonTitleButton* but = findTitleButton(subControl);

    QAction* act = but ? but->defaultAction() : Q_NULL;

    if (!but && add)
    {
        act = new QAction(&p);
        but = new RibbonTitleButton(&p, subControl);
        but->show();
        but->setDefaultAction(act);
        connect(act, SIGNAL(triggered()), this, SLOT(processClickedSubControl()));
        act->setToolTip(m_frameHelper->getSystemMenuString(subControl));
    }
    else if (but && !add)
    {
        act = but->defaultAction();
        but->setVisible(false);
        but->setParent(Q_NULL);
        but->deleteLater();
        but = Q_NULL;
        act->deleteLater();
        act = Q_NULL;
    }

    if (add)
    {
        QRect rcButton(rcControl.right() - rcControl.height(), rcControl.top(), rcControl.right()-(rcControl.right() - rcControl.height()), rcControl.bottom());
        rcButton.adjust(0, 0, -1, 0);
        if (m_frameHelper)
        {
            int iCaptionWidth, iCaptionHeight;
            if (m_frameHelper->clientMetrics(iCaptionWidth, iCaptionHeight))
            {
                int sysBtnEdge = qMin(iCaptionHeight, rcControl.height () - 2);
                rcButton.setTopLeft(QPoint(rcControl.right() - iCaptionWidth, rcControl.top()));
                rcButton.setWidth(iCaptionWidth);
                rcButton.setHeight(sysBtnEdge);
            }
        }

        but->setGeometry(rcButton);
        rcControl.setWidth(rcControl.width() - rcButton.width());
    }
    return act;
}

ContextHeader* RibbonBarPrivate::hitTestContextHeaders(const QPoint& point) const
{
    for ( int i = 0, count = m_listContextHeaders.count(); i < count; i++)
    {
        ContextHeader* header =  m_listContextHeaders.at(i);
        if (header->rcRect.contains(point))
             return header;
    }
    return Q_NULL;
}

QMenu* RibbonBarPrivate::createContextMenu()
{
    QTN_P(RibbonBar)
    if (p.isBackstageVisible())
        return Q_NULL;

    QMenu* popup = new QMenu(&p);

    QAction* action = Q_NULL;
    if (p.isQuickAccessVisible())
    {
        if (RibbonQuickAccessBar* quickAccessBar = p.quickAccessBar())
        {
            QList<QAction*> actions = quickAccessBar->actions();
            if (actions.size() > 0)
            {
                action = popup->addAction(RibbonBar::tr_compatible(QtnRibbonCustomizeQuickAccessToolBarDotString));
                action->setObjectName(strCustomizeQAToolBar);
                connect(action, SIGNAL(triggered()), this, SLOT(toggledCustomizeBar()));

                action = popup->addAction(p.quickAccessBarPosition() == RibbonBar::QATopPosition ? RibbonBar::tr_compatible(QtnRibbonShowQuickAccessToolBarBelowString)
                    : RibbonBar::tr_compatible(QtnRibbonShowQuickAccessToolBarAboveString));
                action->setObjectName(strCustomizeRibbonBar);
                connect(action, SIGNAL(triggered()), this, SLOT(toggledQuickAccessBarPos()));
            }
        }
        popup->addSeparator();
    }

    action = popup->addAction(RibbonBar::tr_compatible(QtnRibbonCustomizeActionString));
    action->setObjectName(strCustomizeRibbonBar);
    connect(action, SIGNAL(triggered()), this, SLOT(toggledCustomizeBar()));

    if (p.isMinimizationEnabled())
    {
        action = popup->addAction(RibbonBar::tr_compatible(QtnRibbonMinimizeActionString));
        action->setCheckable(true);
        action->setChecked(p.isMinimized());
        connect(action, SIGNAL(triggered()), this, SLOT(toggledMinimized()));
    }
    return popup;
}

static QString findAccel(const QString &text)
{
    if (text.isEmpty())
        return text;

    int fa = 0;
    QChar ac;
    while ((fa = text.indexOf(QLatin1Char('&'), fa)) != -1) 
    {
        ++fa;
        if (fa < text.length()) 
        {
            // ignore "&&"
            if (text.at(fa) == QLatin1Char('&')) 
            {
                ++fa;
                continue;
            } 
            else 
            {
                ac = text.at(fa);
                break;
            }
        }
    }
    if (ac.isNull())
        return QString();
    return (QString)ac.toUpper();
}

void RibbonBarPrivate::setKeyboardMode(bool b)
{
    QTN_P(RibbonBar);
    if (b && !p.style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, &p)) 
        return;

    m_keyboardState = b;

    if (m_keyboardState)
    {
        QWidget* fw = QApplication::focusWidget();
        if (fw != &p)
            m_keyboardFocusWidget = fw;
        p.setFocus(Qt::MenuBarFocusReason);

        if (m_levels.indexOf(&p) == -1)
        {
            m_keyTipsShowing = true;
            emit p.keyTipsShowed(true);
            showKeyTips(&p);
        }
    }
    else
    {
        hideKeyTips();
        m_levels.clear();

        if (m_keyboardFocusWidget)
        {
            if (QApplication::focusWidget() == &p)
                m_keyboardFocusWidget->setFocus(Qt::MenuBarFocusReason);
            m_keyboardFocusWidget = 0;
        }
    }
}

void RibbonBarPrivate::showKeyTips(QWidget* w)
{
    hideKeyTips();
    m_countKey = 0;

    if (qobject_cast<RibbonBar*>(w))
        createKeyTips();
    else if (qobject_cast<RibbonPage*>(w))
        createGroupKeyTips();
    else if (QMenu* menu = qobject_cast<QMenu*>(w))
        createPopupKeyTips(menu);

    int count = m_keyTips.count();
    if (count == 0)
        return;

    for (int i = count - 1; i >= 0; i--)
    {
        RibbonKeyTip* keyTip = m_keyTips.at(i);
        keyTip->setExplicit(!keyTip->getStringTip().isEmpty());

        if (keyTip->isExplicit())
            continue;

        QString strCaption = keyTip->getCaption();
        if (strCaption.isEmpty())
        {
            keyTip->close();
            keyTip->deleteLater();
            m_keyTips.removeAt(i);
            continue;
        }

        strCaption = strCaption.toUpper();
        keyTip->setCaption(strCaption);

        if (keyTip->getPrefix().isEmpty())
        {
            QString key = ::findAccel(strCaption);
            if (key.isEmpty())
            {
                if (!strCaption.isEmpty())
                    keyTip->setStringTip(QChar(strCaption[0]));
            }
            else
                keyTip->setStringTip(key);
        }
        else
        {
            keyTip->setStringTip(keyTip->getPrefix());
        }
    }

    count = m_keyTips.count();
    for (int i = 0; i < count; i++)
    {
        RibbonKeyTip* keyTip = m_keyTips.at(i);

        QString strTip = keyTip->getStringTip();

        QList<RibbonKeyTip*> list;
        list.append(keyTip);

        QString strUsed("& ");
        for ( int j = i + 1; j < count; j++)
        {
            RibbonKeyTip* keyTipWidget = m_keyTips.at(j);;
            if (keyTipWidget->getStringTip()[0] == strTip[0])
            {
                list.append(keyTipWidget);
                if (keyTipWidget->getStringTip().length() == 2)
                    strUsed += keyTipWidget->getStringTip()[1];
            }
        }

        if (list.count() < 2)
            continue;

        int k = 1;
        for (int j = 0; j < list.count(); j++)
        {
            RibbonKeyTip* keyTipWidget = list.at(j);
            if (keyTipWidget->getStringTip().length() > 1)
                continue;

            QChar chAccel(0);
            QString strCaption = keyTipWidget->getCaption();
            for (int n = 0; n < strCaption.length() - 1; n++)
            {
                if ((strCaption[n] == ' ') && (strUsed.indexOf(strCaption[n + 1]) == -1))
                {
                    chAccel = strCaption[n + 1];
                    strUsed += chAccel;
                    break;
                }
            }

            if (chAccel.isNull())
            {
                int start = strCaption[0] == QLatin1Char('&') ? 1 : 0;
                if (strCaption[start] == keyTipWidget->getStringTip()[0])
                    start++;

                for (int n = start; n < strCaption.length(); n++)
                {
                    if (strUsed.indexOf(strCaption[n]) == -1)
                    {
                        chAccel = keyTipWidget->getCaption()[n];
                        strUsed += chAccel;
                        break;
                    }
                }
            }
            if (chAccel.isNull())
            {
                QString str = QString("%1").arg(k);
                chAccel = str[0];
                k++;
            }
            keyTipWidget->setStringTip(keyTipWidget->getStringTip() + chAccel);
        }
    }

    calcPositionKeyTips();

    if (m_keyTips.count() > 0)
        qApp->installEventFilter(this);

    QTN_P(RibbonBar);
    p.setFocus();
}

bool RibbonBarPrivate::hideKeyTips()
{
    bool res = false;
    if (m_keyTips.count() > 0)
    {
        qApp->removeEventFilter(this);
        res = true;
    }
    destroyKeyTips();
    m_countKey = 0;
    return res;
}

void RibbonBarPrivate::createKeyTips()
{
    QTN_P(RibbonBar);
    m_levels.push(&p);

    QList<QToolButton*> list = m_quickAccessBar->findChildren<QToolButton*>();
    for (int i = 0, count = list.count(); i < count; i++)
    {
        QToolButton* button = list.at(i);
        if (!button->isVisible() || button->objectName() == QLatin1String("QtnRibbonQuickAccessButton"))
            continue;

        int k = 1;
        QString strCaption;
        if (QAction* act = button->defaultAction())
        {
            strCaption = act->property(_qtn_KeyTip).toString();
            if (strCaption.isEmpty())
            {
                QList<QAction*> listActs = m_quickAccessBar->actions();
                int index = listActs.indexOf(act);
                if (index != -1)
                    k = index;
            }
        }

        if (strCaption.isEmpty())
        {
            if (k < 10)
                strCaption = QString("%1").arg(k);
            else if (k < 19)
                strCaption = QString("%1").arg(k - 9);
            else 
                break;
        }

        m_keyTips.append(new RibbonKeyTip(&p, button, strCaption, button->mapToGlobal(QPoint(button->rect().center().x(), button->rect().bottom() - 11)), 
            RibbonKeyTip::AlignTipCenter | RibbonKeyTip::AlignTipTop, button->isEnabled()));
    }

    if (m_tabBar && m_tabBar->isVisible())
    {
        for (int i = 0, count = m_tabBar->getTabCount(); i < count; i++)
        {
            RibbonTab* tab = m_tabBar->getTab(i);

            bool propKeyTip = true;
            QString strCaption = tab->defaultAction()->property(_qtn_KeyTip).toString();

            if (strCaption.isEmpty())
            {
                strCaption = tab->text();
                propKeyTip = false;
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!strCaption.isEmpty() && tab->isEnabled() && tab->isVisible())
            {
                QPoint pt(tab->rect().center().x(), tab->rect().bottom() - 9);
                RibbonKeyTip* keyTip = new RibbonKeyTip(&p, tab, strCaption, tab->mapToGlobal(pt), 
                    RibbonKeyTip::AlignTipCenter | RibbonKeyTip::AlignTipTop, true);
                m_keyTips.append(keyTip);
            }
        }
    }

    if (m_controlSystemButton)
    {
        QPoint pt(m_controlSystemButton->rect().center().x(), m_controlSystemButton->rect().center().y());

        QString strCaption = m_controlSystemButton->defaultAction()->property(_qtn_KeyTip).toString();
        if (strCaption.isEmpty())
            strCaption = m_controlSystemButton->text();

        m_keyTips.append(new RibbonKeyTip(&p, m_controlSystemButton, strCaption, 
            m_controlSystemButton->mapToGlobal(pt), RibbonKeyTip::AlignTipCenter | RibbonKeyTip::AlignTipVCenter, m_controlSystemButton->isEnabled()));
    }
}

void RibbonBarPrivate::createGroupKeyTips()
{
    int index = m_tabBar->currentIndex();
    if (!validIndex(index))
        return;

    RibbonPage* page = m_listPages.at(index);
    m_levels.push(page);
    for (int i = 0, count = page->groupCount(); i < count; i++)
    {
        RibbonGroup* group = page->getGroup(i);
        bool visible = group->isVisible();

        QList<QWidget*> list = group->findChildren<QWidget*>();
        for (int j = 0; j < list.count(); j++)
        {
            QWidget* widget = list.at(j);
            createWidgetKeyTips(group, widget, QString(), page->rect().translated(page->mapToGlobal(page->rect().topLeft())), visible);
        }
    }
}

static QString getTextAction(const RibbonGroup& group, QWidget& widget, bool& propKeyTip)
{
    QString strCaption;
    foreach (QAction* action, group.actions())
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(action))
        {
            if (&widget == widgetAction->defaultWidget())
            {
                strCaption = widgetAction->property(_qtn_KeyTip).toString();
                if (strCaption.isEmpty())
                {
                    strCaption = widgetAction->text();
                    propKeyTip = false;
                }
                break;
            }
        }
    }
    return strCaption;
}

void RibbonBarPrivate::createWidgetKeyTips(RibbonGroup* group, QWidget* widget, const QString& prefix, const QRect& rcGroups, bool visible)
{
    QTN_P(RibbonBar);

    if (!widget)
        return;

    bool propKeyTip = true;
    QString strCaption;
    if (QAbstractButton* button = qobject_cast<QAbstractButton*>(widget))
    {
        strCaption = ::getTextAction(*group, *button, propKeyTip);
        if(strCaption.isEmpty())
        {
            if (QToolButton* toolButton = qobject_cast<QToolButton*>(button))
            {
                if (QAction* action = toolButton->defaultAction())
                {
                    strCaption = action->property(_qtn_KeyTip).toString();
                    if (strCaption.isEmpty())
                    {
                        strCaption = action->text();
                        propKeyTip = false;
                    }
                }

                if (strCaption.isEmpty())
                {
                    if (RibbonGallery* ribGallery = qobject_cast<RibbonGallery*>(toolButton->parentWidget()))
                    {
                        if (toolButton->property(_qtn_PopupButtonGallery).toBool())
                        {
                            if (OfficePopupMenu* menu = ribGallery->popupMenu())
                            {
                                if (QAction* act = menu->defaultAction())
                                    strCaption = act->text();
                            }
                            if (strCaption.isEmpty())
                                strCaption = ::getTextAction(*group, *toolButton->parentWidget(), propKeyTip);
                        }
                    }
                    else if (strCaption.isEmpty())
                        strCaption = group->title();
                }
            }
            else
            {
                strCaption = ::getTextAction(*group, *widget, propKeyTip);
                if (strCaption.isEmpty())
                    strCaption = button->text();
            }
        }
    }

    bool textWidget = true;
    if (RibbonGallery* gallery = qobject_cast<RibbonGallery*>(widget))
        textWidget = !gallery->popupMenu();

    if (strCaption.isEmpty() && textWidget)
        strCaption = ::getTextAction(*group, *widget, propKeyTip);

    if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
        strCaption = QString();

    if (strCaption.isEmpty())
        return;

    QRect rc = widget->rect();
    rc.translate(widget->mapToGlobal(rc.topLeft()));

    if (rc.isEmpty())
        return;

    QPoint pt(rc.left() + 21, rc.bottom() - 4);
    uint align = RibbonKeyTip::AlignTipTop;

    QStyleOptionGroupBox opt;
    opt.init(&p);
    opt.text = group->title();

    QRect rcGroup = group->rect();
    rcGroup.translate(group->mapToGlobal(rcGroup.topLeft()));

    const int heightCaptionGroup = p.style()->pixelMetric((QStyle::PixelMetric)RibbonStyle::PM_RibbonHeightCaptionGroup, &opt, group);
    QRect rcCaption(QPoint(rcGroup.left(), rcGroup.bottom() - heightCaptionGroup + 2), QPoint(rcGroup.right(), rcGroup.bottom()));
    rcGroup.setBottom(rcCaption.top());

    QFontMetrics fm(p.font());
    const int h = fm.boundingRect('X').height() / 2;

    if (rc.intersects(rcCaption))
    {
        pt = QPoint(rc.center().x(), rcGroups.bottom() - 6);
        align = RibbonKeyTip::AlignTipTop | RibbonKeyTip::AlignTipCenter;
    }
    else if ((Qt::ToolButtonStyle)widget->property("toolButtonStyle").toInt() == Qt::ToolButtonTextUnderIcon && rc.height() > rcGroup.height() * 0.5)
    {
        pt = QPoint(rc.center().x(), rc.bottom() + h);
        align = RibbonKeyTip::AlignTipCenter | RibbonKeyTip::AlignTipVCenter;
    }
    else if (qobject_cast<RibbonGallery*>(widget))
    {
        pt = QPoint(rc.right(), rcGroup.bottom() - 2);
        align = RibbonKeyTip::AlignTipCenter | RibbonKeyTip::AlignTipVCenter;
    }
    else if (rc.center().y() < rcGroup.top() + rcGroup.height() * 1 / 3)
    {
        /// top
        pt = QPoint(rc.left() + 11, rcGroup.top());
        align = RibbonKeyTip::AlignTipLeft | RibbonKeyTip::AlignTipVCenter;
    }
    else if (rc.center().y() > rcGroup.top() + rcGroup.height() * 2 / 3)
    {
        pt = QPoint(rc.left() + 11, rcGroup.bottom());
        align = RibbonKeyTip::AlignTipLeft | RibbonKeyTip::AlignTipVCenter;
    }
    else
    {
        if (qobject_cast<RibbonButton*>(widget) && qobject_cast<RibbonToolBarControl*>(widget->parentWidget()))
            pt = QPoint(rc.left() + 11, rc.bottom() + h * 2);
        else
            pt = QPoint(rc.left() + 11, rcGroup.center().y() - 1);
        align = RibbonKeyTip::AlignTipLeft | RibbonKeyTip::AlignTipVCenter;
    }

    if (qobject_cast<QCheckBox*>(widget) || qobject_cast<QRadioButton*>(widget))
        pt.setX(rc.left() + 6);

    RibbonKeyTip* keyTip = new RibbonKeyTip(&p, widget, strCaption, pt, align, widget->isEnabled());

    if (!prefix.isEmpty())
        keyTip->setPrefix(prefix);

    QRect rcIntersect; 
    rcIntersect = rcGroups.intersected(rc);
    keyTip->setVisibleTip(visible && (rcIntersect == rc) && (widget->isVisible()));
    m_keyTips.append(keyTip);
}

bool RibbonBarPrivate::createPopupKeyTips(QMenu* levalMenu)
{
    QTN_P(RibbonBar);
    QMenu* menu = levalMenu ? levalMenu : qobject_cast<QMenu*>(QApplication::activePopupWidget());
    if (menu)
    {
        m_levels.push(menu);

        QList<QAction*> list = menu->actions();
        for (int i = 0; i < list.count(); i++)
        {
            QAction* action = list.at(i);

            if (action->text().count(QString(_qtn_PopupLable)) != 0)
                continue;

            bool propKeyTip = true;
            QString strCaption = action->property(_qtn_KeyTip).toString();

            if (strCaption.isEmpty())
            {
                strCaption = action->text();
                propKeyTip = false;
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!action->isVisible() || strCaption.isEmpty())
                continue;

            QRect rect = menu->actionGeometry(action);
            QPoint pt(rect.left() + 12, rect.bottom() - 11);
            RibbonKeyTip* keyTip = new RibbonKeyTip(&p, menu, strCaption, menu->mapToGlobal(pt), RibbonKeyTip::AlignTipTop, action->isEnabled(), action);

            m_keyTips.append(keyTip);
        }

        QList<QWidget*> listWidget = menu->findChildren<QWidget*>();
        for (int i = 0; i < listWidget.count(); i++)
        {
            QWidget* widget = listWidget.at(i);

            bool propKeyTip = true;
            QString strCaption;

            if (QToolButton* toolButton = qobject_cast<QToolButton*>(widget))
            {
                if (QAction* action = toolButton->defaultAction())
                {
                    strCaption = action->property(_qtn_KeyTip).toString();
                    if (strCaption.isEmpty())
                    {
                        strCaption = action->text();
                        propKeyTip = false;
                    }
                }
            }

            if (!m_keyTipsComplement && !propKeyTip && ::findAccel(strCaption).isEmpty())
                strCaption = QString();

            if (!widget->isVisible() || strCaption.isEmpty())
                continue;

            QPoint pt(widget->rect().left() + 12, widget->rect().bottom() - 11);
            RibbonKeyTip* keyTip = new RibbonKeyTip(&p, widget, strCaption, widget->mapToGlobal(pt), 
                RibbonKeyTip::AlignTipTop, widget->isEnabled());
            m_keyTips.append(keyTip);
        }
        return true;
    }
    return false;
}

void RibbonBarPrivate::destroyKeyTips()
{
    for (int i = 0; i < m_keyTips.count(); i++)
    {
        RibbonKeyTip* keyTip = m_keyTips.at(i);
        keyTip->close();
        keyTip->deleteLater();
    }
    m_keyTips.clear();
}

void RibbonBarPrivate::calcPositionKeyTips()
{
    for (int i = 0, count = m_keyTips.count(); i < count; i++)
    {
        RibbonKeyTip* keyTip = m_keyTips.at(i);
        if (!keyTip->isVisibleTip())
            continue;

        QSize sz(0, 0);
        QFontMetrics fontMetrics(keyTip->font());
        QRect rect = fontMetrics.boundingRect(keyTip->getStringTip());
        sz = QSize(qMax(rect.width() + 6, 16), qMax(rect.height() + 2, 15));

        QPoint pt = keyTip->posTip();
        uint flagAlign = keyTip->getAlign();

        QRect rc(QPoint(pt.x() - ((flagAlign & RibbonKeyTip::AlignTipCenter) ?  sz.width() * 0.5 : (flagAlign & RibbonKeyTip::AlignTipRight) ? sz.width() : 0),
            pt.y() - (flagAlign & RibbonKeyTip::AlignTipVCenter ?  sz.height() * 0.5 : flagAlign & RibbonKeyTip::AlignTipBottom ? sz.height() : 0)), sz);

        keyTip->setGeometry(rc);
        keyTip->show();
    }
}

int RibbonBarPrivate::rowItemHeight() const
{
    return m_rowItemHeight;
}

int RibbonBarPrivate::rowItemCount() const
{
    return m_rowItemCount;
}

int RibbonBarPrivate::maxGroupHeight() const
{
    QTN_P(const RibbonBar);
    int ret = rowItemHeight() * rowItemCount();
    QStyleOption opt;
    opt.init(&p);
    ret += p.style()->pixelMetric((QStyle::PixelMetric)RibbonStyle::PM_RibbonHeightCaptionGroup, &opt, Q_NULL) + 4;
    for (QList<RibbonPage*>::const_iterator it = m_listPages.begin(); it != m_listPages.end(); ++it)
    {
        RibbonPage* page = (*it);
        for (int g = 0, countG = page->groupCount(); countG > g; g++)
        {
            if (RibbonGroup* group = page->getGroup(g))
                ret = qMax(ret, group->sizeHint().height());
        }
    }
    return ret;
}

bool RibbonBarPrivate::pressTipCharEvent(const QKeyEvent* key)
{
    QTN_P(RibbonBar);
    QString str = key->text().toUpper();
    if (str.isEmpty())
        return false;
    for (int i = 0; i < m_keyTips.count(); i++)
    {
        RibbonKeyTip* keyTip = m_keyTips.at(i);
        int length = keyTip->getStringTip().length();
        if (length > m_countKey)
        {
            if (keyTip->getStringTip()[m_countKey] == str[0])
            {
                if (length - 1 > m_countKey)
                {
                    m_countKey++;

                    QString str = keyTip->getStringTip().left(m_countKey);
                    for (int j = m_keyTips.count() - 1; j >= 0; j--)
                    {
                        keyTip = m_keyTips.at(j);
                        if (keyTip->getStringTip().left(m_countKey) != str)
                        {
                            m_keyTips.removeAt(j);
                            keyTip->hide();
                            keyTip->deleteLater();
                        }
                    }
                }
                else
                {
                    if (QAbstractButton* absButton = qobject_cast<QAbstractButton*>(keyTip->getOwner()))
                    {
                        if (keyTip->isEnabledTip() && keyTip->isVisibleTip())
                        {
                            m_keyTipsShowing = false;
                            emit p.keyTipsShowed(false);

                            bool clearLevel = true;
                            bool showMenu = false;
                            if (QToolButton* button = qobject_cast<QToolButton*>(absButton))
                            {
                                if (button->popupMode() == QToolButton::InstantPopup)
                                    clearLevel = false;
                                else if (button->popupMode() == QToolButton::MenuButtonPopup)
                                {
                                    button->showMenu();
                                    clearLevel = false;
                                    showMenu = true;
                                }
                            }
                            if (clearLevel)
                                m_levels.clear();

                            if (!showMenu)
                                absButton->animateClick(0);
                        }
                        return false;
                    }
                    else if (qobject_cast<QMenu*>(keyTip->getOwner()) && keyTip->isEnabledTip() && keyTip->isVisibleTip())
                    {
                        hideKeyTips();

                        m_keyTipsShowing = false;
                        emit p.keyTipsShowed(false);

                        if (QAction* action = keyTip->getAction())
                        {
                            if (qobject_cast<RibbonPageSystemPopup*>(action->menu()))
                            {
                                if (QMenu* activPopup = qobject_cast<RibbonSystemPopupBar*>(QApplication::activePopupWidget()))
                                    activPopup->setActiveAction(action);
                                return false;
                            }
                            else
                            {
                                m_levels.clear();
                                action->trigger();
                                QWidget* owner = keyTip->getOwner();
                                owner->close();

                                if (qobject_cast<RibbonPageSystemPopup*>(owner))
                                {
                                    if (QWidget* activPopup = qobject_cast<RibbonSystemPopupBar*>(QApplication::activePopupWidget()))
                                        activPopup->close();
                                }
                                return false;
                            }
                        }
                    } 
                    else if (qobject_cast<RibbonTab*>(keyTip->getOwner()))
                    {
                        if (RibbonTab* tab = qobject_cast<RibbonTab*>(keyTip->getOwner()))
                        {
                            int index = m_tabBar->getTabIndex(tab);
                            if (index != -1)
                            {
                                p.setCurrentPageIndex(index);
                                showKeyTips(p.getPage(index));
                            }
                        }
                        return false;
                    }
                    else if (keyTip->getOwner() && keyTip->isEnabledTip() && keyTip->isVisibleTip())
                    {
                        hideKeyTips();

                        m_keyTipsShowing = false;
                        emit p.keyTipsShowed(false);

                        m_levels.clear();
                        keyTip->getOwner()->setFocus();
                    }
                }
                return false;
            }
        }
    }
    return false;
}

bool RibbonBarPrivate::eventFilter(QObject* obj, QEvent* event)
{
    QTN_P(RibbonBar)
    switch (event->type()) 
    {
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::NonClientAreaMouseButtonPress :
        case QEvent::NonClientAreaMouseButtonRelease :
        case QEvent::NonClientAreaMouseButtonDblClick :
            {
                m_keyTipsShowing = false;
                emit p.keyTipsShowed(false);

                hideKeyTips();
                m_levels.clear();
            }
            break;
        case QEvent::Show :
            if (QMenu* menu = qobject_cast<QMenu*>(obj))
            {
                if (m_levels.size() > 0 || ((qobject_cast<RibbonPageSystemPopup*>(obj) || qobject_cast<RibbonSystemPopupBar*>(obj))))
                {
                    if (m_levels.indexOf(menu) == -1)
                    {
                        m_keyTipsShowing = true;
                        emit p.keyTipsShowed(true);
                        hideKeyTips();
                        showKeyTips(menu);
                    }
                }
            }
            else if (qobject_cast<RibbonPageSystemPopup*>(obj))
            {
                m_keyTipsShowing = false;
                hideKeyTips();
                m_levels.clear();
            }
            break;
        case QEvent::KeyPress :
            if (QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event))
            {
                if (keyEvent->key() != Qt::Key_Escape)
                {
                    if ((qobject_cast<QMainWindow*>(obj) || (m_minimized && qobject_cast<RibbonPage*>(obj))))
                        pressTipCharEvent(keyEvent);
                    else if (qobject_cast<QMenu*>(obj))
                        return !pressTipCharEvent(keyEvent);
                }
                else
                {
                    if (m_levels.size() > 1 && m_levels.pop())
                        showKeyTips(m_levels.pop());
                    else if (m_levels.size() > 0 && m_levels.pop())
                    {
                        hideKeyTips();

                        m_keyTipsShowing = false;
                        emit p.keyTipsShowed(false);
                    }
                }
            }
            break;
        case QEvent::Close :
            if (m_minimized && m_levels.size() > 1 && m_levels.pop())
                showKeyTips(m_levels.pop());
            break;
        default:
            break;
    }
    return false;
}

void RibbonBarPrivate::processClickedSubControl()
{
    QTN_P(RibbonBar);
    if (QAction* action = qobject_cast<QAction *>(sender()))
    {
        QWidget* widget = p.parentWidget();
        if (widget == Q_NULL )
            return;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        if (m_frameHelper)
        {
            if (m_actCloseButton == action)
                m_frameHelper->processClickedSubControl(QStyle::SC_TitleBarCloseButton);
            else if (m_actNormalButton == action)
                m_frameHelper->processClickedSubControl(QStyle::SC_TitleBarNormalButton);
            else if (m_actMaxButton == action)
                m_frameHelper->processClickedSubControl(QStyle::SC_TitleBarMaxButton);
            else if (m_actMinButton == action)
                m_frameHelper->processClickedSubControl(QStyle::SC_TitleBarMinButton);
        }
#else
        if (m_actCloseButton == action)
            widget->close();
        else if (m_actNormalButton == action)
            widget->showNormal();
        else if (m_actMaxButton == action)
            widget->showMaximized();
        else if (m_actMinButton == action)
            widget->showMinimized();
#endif
    }
}

void RibbonBarPrivate::createCustomizeDialog()
{
    QTN_P(RibbonBar)
    if (m_customizeDialog != Q_NULL)
        return;

    m_customizeDialog = new RibbonCustomizeDialog(p.parentWidget());
    // Create "Customize QAT" page:
    RibbonQuickAccessBarCustomizePage* quickAccessBarCustomizePage = new RibbonQuickAccessBarCustomizePage(&p);
    m_customizeDialog->addPage(quickAccessBarCustomizePage);
    // Create "Customize Ribbon" page:
    RibbonBarCustomizePage* ribbonBarCustomizePage = new RibbonBarCustomizePage(&p);
    m_customizeDialog->addPage(ribbonBarCustomizePage);
}

void RibbonBarPrivate::toggledCustomizeBar()
{
    QTN_P(RibbonBar)
    p.showCustomizeDialog();
}

void RibbonBarPrivate::toggledQuickAccessBarPos()
{
    QTN_P(RibbonBar)
    p.setQuickAccessBarPosition(p.quickAccessBarPosition() == RibbonBar::QATopPosition ? RibbonBar::QABottomPosition : RibbonBar::QATopPosition);
}

void RibbonBarPrivate::toggledMinimized()
{
    QTN_P(RibbonBar)
    p.setMinimized(!p.isMinimized());
}


/*!
\class RibbonBar
\inmodule QtitanRibbon
*/
RibbonBar::RibbonBar(QWidget* parent)
    : QMenuBar(parent)
{
    QTN_INIT_PRIVATE(RibbonBar);
    QTN_D(RibbonBar);
    d.init();
}

RibbonBar::~RibbonBar()
{
    QTN_FINI_PRIVATE();
}

/*!
Returns the visible flag of the Ribbon Bar.
*/ 
bool RibbonBar::isVisible() const
{
    QTN_D(const RibbonBar);
    return QMenuBar::isVisible() && d.m_ribbonBarVisible;
}

RibbonPage* RibbonBar::addPage(const QString& text)
{
    return insertPage(-1, text);
}

void RibbonBar::addPage(RibbonPage* page)
{
    insertPage(-1, page);
}

RibbonPage* RibbonBar::insertPage(int index, const QString& text)
{
    RibbonBarAutoUpdater autoUpdater(this);
    QTN_D(RibbonBar);
    int indTab = d.m_tabBar->insertTab(index, text);
    RibbonPage* page = new RibbonPage(this);
    d.insertPage(indTab, index, page);
    page->setTitle(text);
    return page;
}

void RibbonBar::insertPage(int index, RibbonPage* page)
{
    Q_ASSERT(page != Q_NULL);
    QTN_D(RibbonBar);
    if (d.pageIndex(page) != -1)
    {
        qWarning("RibbonBar::insertPage: Can't add page twice.");
        return;
    }
    RibbonBarAutoUpdater autoUpdater(this);
    page->setParent(this);
    int indTab = d.m_tabBar->insertTab(index, page->title());
    d.insertPage(indTab, index, page);
}

void RibbonBar::movePage(RibbonPage* page, int newIndex)
{
    QTN_D(RibbonBar);
    movePage(d.pageIndex(page), newIndex);
}

void RibbonBar::movePage(int index, int newIndex)
{
    QTN_D(RibbonBar);
    if (d.validIndex(index) && d.validIndex(newIndex))
    {
        RibbonBarAutoUpdater autoUpdater(this);
        d.m_tabBar->moveTab(index, newIndex);
        d.m_listPages.move(index, newIndex);
    }
}

void RibbonBar::removePage(RibbonPage* page)
{
    QTN_D(RibbonBar);
    int index = d.pageIndex(page);
    if (index != -1)
        removePage(index);
}

void RibbonBar::removePage(int index)
{
    QTN_D(RibbonBar);
    RibbonBarAutoUpdater autoUpdater(this);
    d.removePage(index);
    d.m_tabBar->removeTab(index);
}

void RibbonBar::detachPage(RibbonPage* page)
{
    QTN_D(RibbonBar);
    int index = d.pageIndex(page);
    if (index != -1)
        detachPage(index);
}

void RibbonBar::detachPage(int index)
{
    QTN_D(RibbonBar);
    RibbonBarAutoUpdater autoUpdater(this);
    d.removePage(index, false);
    d.m_tabBar->removeTab(index);
}

/*!
Removes all pages from the ribbon bar.
*/
void RibbonBar::clearPages()
{
    for (int i = getPageCount() - 1; i >= 0; i--)
        removePage(i);
}

/*!
Returns the current state of showing Key-Tips. If true then Key-Tips visible on screen at the moment.
*/ 
bool RibbonBar::isKeyTipsShowing() const
{
    QTN_D(const RibbonBar);
    return d.m_keyTipsShowing;
}

/*!
Returns the flag is mechanizm of Key-Tips enabled or not.
*/ 
bool RibbonBar::keyTipsEnabled() const
{
    QTN_D(const RibbonBar);
    return d.m_keyTipsEnabled;
}

/*!
Enabled or disabled mechanizm of Key-Tips.
*/ 
void RibbonBar::setKeyTipsEnable(bool enable)
{
    QTN_D(RibbonBar);
    d.m_keyTipsEnabled = enable;
}


/*!
Returns the policy of assignment for KeyTips of the buttons and other controls. If return value
is true then the keytips assigned automatically, based on keyboard shortcuts <&File, &Open...>. 
If return value is false keytips will be assigned only if user set it by RibbonBar::setKeyTip() function.
\sa setKeyTip()
*/ 
bool RibbonBar::isKeyTipsComplement() const
{
    QTN_D(const RibbonBar);
    return d.m_keyTipsComplement;
}

/*!
The function sets the policy of assignment for KeyTips of the buttons and other controls. If the parameter \a complement
is true then the keytips assigned automatically, based on keyboard shortcuts <&File, &Open...>. 
If parameter \a complement is false keytips will be assigned only if user set it by RibbonBar::setKeyTip() function.
\sa setKeyTip()
*/ 
void RibbonBar::setKeyTipsComplement(bool complement)
{
    QTN_D(RibbonBar);
    d.m_keyTipsComplement = complement;
}

/*!
Sets the \a keyTip for the control associated with given \a action. Function is taken effect if RibbonBar::isKeyTipsComplement() return true.
\sa setKeyTipsComplement()
*/
void RibbonBar::setKeyTip(QAction* action, const QString& keyTip)
{
    if (!action)
    {
        Q_ASSERT(false);
        return;
    }
    action->setProperty(_qtn_KeyTip, keyTip);
}

/*!
Sets custom logo for the ribbon bar. Logo can be aligned at the left or at the right of the ribbon bar. 
*/
void RibbonBar::setLogoPixmap(Qt::AlignmentFlag alignment, const QPixmap& logotype)
{
    QTN_D(RibbonBar);

    if ( alignment == Qt::AlignRight || alignment == Qt::AlignLeft)
    {
        if (logotype.isNull())
        {
            Q_ASSERT(false);
            return;
        }
        d.m_logotypeLabel->setPixmap(logotype);
        d.m_logotypeLabel->setAlignment(alignment);
        d.m_logotypeLabel->setVisible(!logotype.isNull());
        updateLayout();

        int index = -1;// currentPageIndex();
        if (index != -1)
        {
            if (RibbonPage* page = getPage(index))
                page->updateLayout();
        }
        return;
    }
    Q_ASSERT(false);
}

/*!
Returns the logo of the ribbon bar. Out parametent \a alignment contains a alignment property of the logo.
*/
QPixmap RibbonBar::logoPixmap(Qt::AlignmentFlag& alignment) const
{
    QTN_D(const RibbonBar);
    
    alignment = d.m_logotypeLabel->alignment();
    return !d.m_logotypeLabel->isHidden() ? *d.m_logotypeLabel->pixmap() : QPixmap();
}

/*!
Sets the background image for the title of the ribbon bar. 
*/
void RibbonBar::setTitleBackground(const QPixmap& px)
{
    QTN_D(RibbonBar);
    d.m_pixTitleBackground = px;
    update();
}

/*!
Returns the current background image of the ribbon bar title. 
*/
const QPixmap& RibbonBar::titleBackground() const
{
    QTN_D(const RibbonBar);
    return d.m_pixTitleBackground;
}

/*!
Returns pointer to the quick access bar. See description of class RibbonQuickAccessBar for more details.
*/
RibbonQuickAccessBar* RibbonBar::quickAccessBar() const
{
    QTN_D(const RibbonBar);
    return d.m_quickAccessBar;
}

/*!
Returns pointer to the quick access bar. See description of class RibbonQuickAccessBar for more details.
*/
RibbonQuickAccessBar* RibbonBar::getQuickAccessBar() const
{
    return quickAccessBar();
}

/*!
Sets the visibility of the quick access bar. 
*/
void RibbonBar::showQuickAccess(bool show)
{
    QTN_D(RibbonBar);
    d.m_quickAccessBar->setVisible(show);
}

/*!
Retuns the visibility of the quick access bar. 
*/
bool RibbonBar::isQuickAccessVisible() const
{
    QTN_D(const RibbonBar);
    return d.m_quickAccessBar && d.m_quickAccessBar->isVisible();
}

/*!
Sets quick access bar position on the ribbon. \sa QuickAccessBarPosition
*/
void RibbonBar::setQuickAccessBarPosition(RibbonBar::QuickAccessBarPosition position)
{
    QTN_D(RibbonBar);
    if (d.m_quickAccessBarPosition != position)
    {
        d.m_quickAccessBarPosition = position;
        d.layoutCorner();
        updateLayout();
        updateGeometry();
    }
}

/*!
Retruns current position of quick access bar on the ribbon. \sa QuickAccessBarPosition
*/
RibbonBar::QuickAccessBarPosition RibbonBar::quickAccessBarPosition() const
{
    QTN_D(const RibbonBar);
    return d.m_quickAccessBarPosition;
}

void RibbonBar::setTabBarPosition(RibbonBar::TabBarPosition position)
{
    QTN_D(RibbonBar);
    if (d.m_tabBarPosition != position)
    {
        d.m_tabBarPosition = position;
        d.layoutCorner();
        updateLayout();
        updateGeometry();
    }
}

RibbonBar::TabBarPosition RibbonBar::tabBarPosition() const
{
    QTN_D(const RibbonBar);
    RibbonBar::TabBarPosition pos = d.m_tabBarPosition;
    if (pos == RibbonBar::TabBarBottomPosition && isMinimized())
        pos = RibbonBar::TabBarTopPosition;
    return pos;
}

/*! \internal */
void RibbonBar::setMinimizedFlag(bool flag)
{
    QTN_D(RibbonBar);
    if (d.m_minimized != flag)
    {
        d.m_minimized = flag;
        d.m_pagePopupWidget->hide();
        d.m_tabBar->refresh();
        updateLayout();
        d.preparePages();
        if (QLayout* layout = parentWidget()->layout())
            layout->invalidate();

        emit minimizationChanged(d.m_minimized);
    }
}

#ifdef Q_OS_WIN
void RibbonBar::updateWindowTitle()
{
    QTN_D(RibbonBar);
    if (d.m_frameHelper)
        updateLayout();
}
#endif // Q_OS_WIN

/*!
Do minimize the ribbon bar. This is equivalent of setMinimized(true) or setMaximized(false) call.
*/
void RibbonBar::minimize()
{
    setMinimizedFlag(true);
}

/*!
Returns true if ribbon bar is minimized. Otherwise returns false.
*/
bool RibbonBar::isMinimized() const
{
    QTN_D(const RibbonBar);
    return d.m_minimized;
}

/*!
Sets minimized \a flag for the ribbon bar.
*/
void RibbonBar::setMinimized(bool flag)
{
    setMinimizedFlag(flag);
}

/*!
Do maximize the ribbon bar. This is equivalent of setMaximized(true) or setMinimized(false) call.
*/
void RibbonBar::maximize()
{
    setMinimizedFlag(false);
}

/*!
Returns true if ribbon bar is maximized. Otherwise returns false.
*/
bool RibbonBar::isMaximized() const
{
    return !isMinimized();
}

/*!
Sets maximized \a flag for the ribbon bar.
*/
void RibbonBar::setMaximized(bool flag)
{
    setMinimizedFlag(!flag);
}

/*!
Sets the minimization feature of the Ribbon Bar to the \a enabled.
*/ 
void RibbonBar::setMinimizationEnabled(bool enabled)
{
    QTN_D(RibbonBar);
    d.m_minimizationEnabled = enabled;
}

/*!
Returns the enable flag of the minimization feature of the Ribbon Bar.
*/ 
bool RibbonBar::isMinimizationEnabled() const
{
    QTN_D(const RibbonBar);
    return d.m_minimizationEnabled;
}

/*!
Returns customize manager. It is used to configure customization via a special dialog RibbonCustomizeDialog.
\sa RibbonCustomizeDialog
*/ 
RibbonCustomizeManager* RibbonBar::customizeManager()
{
    QTN_D(RibbonBar);
    if (d.m_customizeManager == Q_NULL)
        d.m_customizeManager = new RibbonCustomizeManager(this);
    return d.m_customizeManager;
}

/*!
Returns customize dialog. You can use the reference to add custom setting pages to the standard dialog. 
\sa RibbonCustomizeManager, showCustomizeDialog()
*/
RibbonCustomizeDialog* RibbonBar::customizeDialog()
{
    QTN_D(RibbonBar);
    d.createCustomizeDialog();
    return d.m_customizeDialog;
}

/*!
Shows customization dialog related to customizeDialog() function. Dialog is shown in modal mode to window that holds a ribbon bar.
\sa customizeDialog
*/
void RibbonBar::showCustomizeDialog()
{
    QTN_D(RibbonBar)
    RibbonCustomizeDialog* dialog = customizeDialog();
    Q_ASSERT(dialog != Q_NULL);
    if (QAction* action = qobject_cast<QAction*>(d.sender()))
    {
        if (action->objectName() == strCustomizeQAToolBar)
        {
            RibbonQuickAccessBarCustomizePage* page = Q_NULL;
            for (int i = 0; i < dialog->pageCount(); ++i)
            {
                page = qobject_cast<RibbonQuickAccessBarCustomizePage *>(dialog->pageByIndex(i));
                if (page != Q_NULL)
                {
                    dialog->setCurrentPage(page);
                    break;
                }
            }
        }
        else if (action->objectName() == strCustomizeRibbonBar)
        {
            RibbonBarCustomizePage* page = Q_NULL;
            for (int i = 0; i < dialog->pageCount(); ++i)
            {
                page = qobject_cast<RibbonBarCustomizePage *>(dialog->pageByIndex(i));
                if (page != Q_NULL)
                {
                    dialog->setCurrentPage(page);
                    break;
                }
            }
        }
    }
    dialog->exec();
}

/*!
Make a page in the position specified by \a index, the current one.
*/ 
void RibbonBar::setCurrentPageIndex(int index)
{
    QTN_D(RibbonBar);
    d.m_tabBar->setCurrentIndex(index);
}

/*!
Returns the index of the current page.
*/ 
int RibbonBar::currentPageIndex() const
{
    QTN_D(const RibbonBar);
    return d.m_tabBar->currentIndex();
}

/*!
Returns the list of all pages in the ribbon bar.
*/
QList<RibbonPage*> RibbonBar::pages() const
{
    QTN_D(const RibbonBar);
    return d.m_listPages;
}

/*!
Returns the ribbon page by index. See description of class RibbonPage for more details.
*/ 
RibbonPage* RibbonBar::getPage(int index) const
{
    QTN_D(const RibbonBar);
    if (d.validIndex(index))
        return d.m_listPages[index];
    return Q_NULL;
}

/*!
Returns the number of the pages inside ribbon bar.
*/ 
int RibbonBar::getPageCount() const
{
    QTN_D(const RibbonBar);
    return d.m_listPages.count();
}

QMenu* RibbonBar::addMenu(const QString& text)
{
    QTN_D(RibbonBar);
    return d.m_tabBar->addMenu(text);
}

QAction* RibbonBar::addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu)
{
    QTN_D(RibbonBar);
    QAction* action = d.m_tabBar->addAction(icon, text, style, menu);
    QMenuBar::addAction(action);
    return action;
}

QAction* RibbonBar::addAction(QAction* action, Qt::ToolButtonStyle style)
{
    QTN_D(RibbonBar);
    QMenuBar::addAction(action);
    return d.m_tabBar->addAction(action, style);
}

QAction* RibbonBar::addSystemButton(const QString& text)
{
    return addSystemButton(QIcon(), text);
}

QAction* RibbonBar::addSystemButton(const QIcon& icon, const QString& text)
{
    QTN_D(RibbonBar);
    QAction* actionSystemPopupBar = d.m_controlSystemButton ? 
        d.m_controlSystemButton->defaultAction() : d.createSystemButton(icon, text);

    if (QMenu* menu = actionSystemPopupBar->menu())
        delete menu;

    actionSystemPopupBar->setMenu(new RibbonSystemPopupBar(this));

    return actionSystemPopupBar;
}

RibbonSystemButton* RibbonBar::getSystemButton() const
{
    QTN_D(const RibbonBar);
    return d.m_controlSystemButton;
}

/*!
Returns the visibility of the BackStage view at the moment.
*/ 
bool RibbonBar::isBackstageVisible() const
{
    QTN_D(const RibbonBar);
    if (d.m_controlSystemButton)
    {
        if (RibbonBackstageView* backstage = qobject_cast<RibbonBackstageView*>(d.m_controlSystemButton->backstage()))
            return backstage->isVisible();
    }
    return false;
}

void RibbonBar::setFrameThemeEnabled(bool enable)
{
#ifdef Q_OS_WIN
    QTN_D(RibbonBar);
    if (!enable)
    {
        if (d.m_frameHelper)
        {
            delete d.m_frameHelper;
            d.m_frameHelper = Q_NULL;
        }

        d.addTitleButton(QStyle::SC_TitleBarCloseButton, false, d.m_rcHeader);
        d.addTitleButton(QStyle::SC_TitleBarNormalButton, false, d.m_rcHeader);
        d.addTitleButton(QStyle::SC_TitleBarMaxButton, false, d.m_rcHeader);
        d.addTitleButton(QStyle::SC_TitleBarMinButton, false, d.m_rcHeader);
        adjustSize();
    }
    else if (!d.m_frameHelper)
    {
        d.m_frameHelper = new OfficeFrameHelperWin(this);
        d.m_frameHelper->enableOfficeFrame(this->parentWidget());
        d.m_frameHelper->setContextHeader(&d.m_listContextHeaders);
    }
    d.layoutCorner();
    updateLayout();
#else // Q_OS_WIN
    Q_UNUSED(enable)
#endif // Q_OS_WIN
}

/*!
\property RibbonBar::frameThemeEnabled
This property holds whether display the modern Office 2007, 2010, 2013 frame and activate Windows Air support (applications under Windows 8, 7,Vista) is enabled. By default parameter is enabled. Used on Windows platform only.
\inmodule QtitanRibbon
*/ 
bool RibbonBar::isFrameThemeEnabled() const
{
    QTN_D(const RibbonBar);
    return d.m_frameHelper;
}

int RibbonBar::backstageViewTop() const
{
    QTN_D(const RibbonBar);
    return titleBarHeight() + d.topBorder() + tabBarHeight(true);
}

int RibbonBar::tabBarHeight(bool isVisible) const
{
    QTN_D(const RibbonBar);
    return (d.m_tabBar && (d.m_tabBar->isVisible()||isVisible)) ? d.m_rcTabBar.height() : 2;
}

bool RibbonBar::isTitleBarVisible() const
{
    QTN_D(const RibbonBar);
    return isFrameThemeEnabled() || d.m_showTitleAlways;
}

void RibbonBar::setTitleBarVisible(bool visible)
{
    QTN_D(RibbonBar);
    if (d.m_showTitleAlways != visible)
    {
        d.m_showTitleAlways = visible;
        updateLayout();
    }
}

/*!
\property RibbonBar::titleGroupsVisible
Sets the visibility of title on all ribbon groups. 
\inmodule QtitanRibbon
*/
bool RibbonBar::isTitleGroupsVisible() const
{
    QTN_D(const RibbonBar);
    return d.m_titleGroupsVisible;
}

void RibbonBar::setTitleGroupsVisible(bool visible)
{
    QTN_D(RibbonBar);
    if (d.m_titleGroupsVisible != visible)
    {
        d.m_titleGroupsVisible = visible;
        for (QList<RibbonPage*>::iterator it = d.m_listPages.begin(); it != d.m_listPages.end(); ++it)
        {
            RibbonPage* page = (*it);
            for (int j = 0, size = page->groupCount(); size > j; ++j)
            {
                if (RibbonGroup* group = page->getGroup(j))
                    group->setProperty(_qtn_TitleGroupsVisible, d.m_titleGroupsVisible);
            }
        }
        d.layoutCorner();
        updateLayout();
        updateGeometry();
    }
}

int RibbonBar::titleBarHeight() const
{
    QTN_D(const RibbonBar);
    if (!isFrameThemeEnabled() && !d.m_showTitleAlways)
        return 0;

    int height = d.m_frameHelper ? d.m_frameHelper->titleBarSize() : style()->pixelMetric(QStyle::PM_TitleBarHeight, 0);
    if (d.m_tabBar == Q_NULL)
        return height;

    int quickAccessHeight = quickAccessBar() ? quickAccessBar()->height() : height;
    quickAccessHeight = qMax(quickAccessHeight, d.m_tabBar->height() - 2);

    if (height >= quickAccessHeight)
        return height;

    return quickAccessHeight;
}

int RibbonBar::rowItemHeight() const
{
    QTN_D(const RibbonBar);
    return d.rowItemHeight();
}

int RibbonBar::rowItemCount() const
{
    QTN_D(const RibbonBar);
    return d.rowItemCount();
}

void RibbonBar::updateLayout()
{
    QTN_D(RibbonBar);
    if (d.m_updateLayoutCount > 0)
        return;
    d.layoutRibbon();
}

void RibbonBar::beginUpdate()
{
    QTN_D(RibbonBar);
    d.m_updateLayoutCount++;
    Q_ASSERT(d.m_updateLayoutCount > 0);
}

void RibbonBar::endUpdate()
{
    QTN_D(RibbonBar);
    d.m_updateLayoutCount--;
    if (d.isUpdating())
        return;
    updateLayout();
}

void Qtitan::qtn_set_font_to_ribbon_children(QWidget* parent, const QFont& font)
{
    QObjectList list = parent->children();
    for (QObjectList::iterator it = list.begin(); it != list.end(); ++it)
    {
        QWidget* widget = qobject_cast<QWidget*>(*it);
        if (widget == Q_NULL)
            continue;

        QMenu* menu = qobject_cast<QMenu*>(widget);
        RibbonPage* page = qobject_cast<RibbonPage*>(widget);
        RibbonGroup* group = qobject_cast<RibbonGroup*>(widget);
        if (menu != Q_NULL)
          menu->setFont(QApplication::font(menu));
        else if (page != Q_NULL)
            page->setFont(font);
        else if (group != Q_NULL)
            group->setFont(font);
        else
        {
            widget->setFont(font);
            qtn_set_font_to_ribbon_children(widget, font);
        }
    }
}

/*! \reimp */
bool RibbonBar::event(QEvent* event)
{
    QTN_D(RibbonBar);

    bool result = QMenuBar::event(event);

    switch(event->type())
    {
        case QEvent::ChildAdded:
            {
                QChildEvent* e = static_cast<QChildEvent *>(event);
                if (e->added())
                {
                    QWidget* widget = qobject_cast<QWidget *>(e->child());
                    if (widget != Q_NULL)
                    {
                        if (qobject_cast<RibbonPage *>(widget) != Q_NULL)
                            qobject_cast<RibbonPage *>(widget)->setFont(font());
                        else
                        {
                            widget->setFont(font());
                            qtn_set_font_to_ribbon_children(widget, font());
                        }
                    }
                }
            }
            break;
        case QEvent::StyleChange:
        case QEvent::FontChange:
            {
                qtn_set_font_to_ribbon_children(this, font());
                d.layoutCorner();
                updateLayout();
            }
            break;
        case QEvent::WindowStateChange: 
            {
                d.layoutCorner();
                updateLayout();
            }
            break;
        case QEvent::WindowTitleChange :
            {
                d.m_windowTitle = parentWidget() ? parentWidget()->windowTitle() : QString();
                updateLayout();
            }
            break;
        case QEvent::WindowIconChange:
            if (d.m_frameHelper)
                d.m_frameHelper->resetWindowIcon();
            break;
        case QEvent::ActionRemoved :
        case QEvent::ActionChanged :
                QApplication::sendEvent(d.m_tabBar, event);
            break;
        case QEvent::Show :
        case QEvent::Hide :
            {
                bool show = event->type() == QEvent::Show;
                if (isFrameThemeEnabled() && show != d.m_ribbonBarVisible)
                {
                    d.m_ribbonBarVisible = show;
                    d.m_frameHelper->recalcFrameLayout();
                }
                if (show)
                    updateLayout();
            }
            break;
        default:
            break;
    }

    return result;
}

/*! \reimp */
bool RibbonBar::eventFilter(QObject* object, QEvent* event)
{
    QTN_D(RibbonBar);
    const QEvent::Type type = event->type();
    if (object == cornerWidget(Qt::TopRightCorner) || object == cornerWidget(Qt::TopLeftCorner)) 
    {
        switch (event->type()) 
        {
            case QEvent::ShowToParent:
            case QEvent::HideToParent:
                d.layoutCorner();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
                return true;
#else
                break;
#endif
            default:
                break;
        }
    }


    if (type == QEvent::Resize)
    {
        if (object == parentWidget() && !isVisible())
        {
            updateLayout();
            return true;
        }
    } 

    if (style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, this)) 
    {
        if (d.m_altPressed) 
        {
            switch (event->type()) 
            {
                case QEvent::KeyPress:
                case QEvent::KeyRelease:
                {
                    QKeyEvent* key = static_cast<QKeyEvent*>(event);
                    if (key->key() == Qt::Key_Alt || key->key() == Qt::Key_Meta)
                    {
                        if (event->type() == QEvent::KeyPress) // Alt-press does not interest us, we have the shortcut-override event
                            break;

                        if (keyTipsEnabled())
                            d.setKeyboardMode(!d.m_keyboardState);
                    }
                }
//                break;
                // fall through
                case QEvent::MouseButtonPress:
                case QEvent::MouseButtonRelease:
                case QEvent::MouseMove:
                case QEvent::FocusIn:
                case QEvent::FocusOut:
                case QEvent::ActivationChange:
                    d.m_altPressed = false;
                    qApp->removeEventFilter(this);
                    break;
                default:
                    break;
            }
        }
        else if (isVisible() && !isBackstageVisible())
        {
            if (event->type() == QEvent::ShortcutOverride) 
            {
                QKeyEvent* key = static_cast<QKeyEvent*>(event);
                if ((key->key() == Qt::Key_Alt || key->key() == Qt::Key_Meta) && key->modifiers() == Qt::AltModifier)
                {
                    d.m_altPressed = true;
                    qApp->installEventFilter(this);
                }
            }
        }
    }
    return false;
}

/*! \reimp */
void RibbonBar::paintEvent(QPaintEvent* event)
{
    QTN_D(RibbonBar);
    
    QWidget* leftCorner = cornerWidget(Qt::TopLeftCorner);
    if(leftCorner && leftCorner->isVisible())
        leftCorner->setVisible(false);

    // draw ribbon
    QPainter p(this);
    QRect rectEvent = event->rect();
    p.setClipRect(rectEvent);

    StyleOptionRibbon opt;
    opt.init(this);
    opt.frameHelper = d.m_frameHelper;
    opt.rectTabBar = d.m_rcTabBar;
    opt.minimized = d.m_minimized;
    opt.isBackstageVisible = isBackstageVisible();
    opt.pixTitleBackground = d.m_pixTitleBackground;
    opt.titleBarHeight = titleBarHeight();
    opt.airRegion = event->region();
    opt.tabBarPosition = d.m_tabBarPosition;

    p.setBackgroundMode(Qt::TransparentMode);

    QStyle* ribbonStyle = style();
    if (rectEvent.intersects(opt.rect))
        ribbonStyle->drawControl((QStyle::ControlElement)RibbonStyle::CE_RibbonBar, &opt, &p, this);

    if ((isFrameThemeEnabled() || d.m_showTitleAlways) &&  d.m_rcTitle.isValid())
    {
        StyleOptionTitleBar titleBarOption;
        d.getTitleBarOption(titleBarOption);
        titleBarOption.frameHelper = d.m_frameHelper;
        titleBarOption.rectTabBar = d.m_rcTabBar;
        titleBarOption.rcRibbonClient = rect();
        titleBarOption.listContextHeaders = &d.m_listContextHeaders;
        titleBarOption.airRegion = event->region();
        titleBarOption.isBackstageVisible = isBackstageVisible();

        if (rectEvent.intersects(titleBarOption.rect))
        {
            QFont saveFont = p.font();
            const QFont font = QApplication::font("QMdiSubWindowTitleBar");
            p.setFont(font);
            ribbonStyle->drawComplexControl(QStyle::CC_TitleBar, &titleBarOption, &p, this);
            p.setFont(saveFont);
        }
    }

    if (!d.m_minimized && getPageCount() > 0 && rectEvent.intersects(opt.rect))
    {
        opt.rect = d.m_rcPageArea;
        if (!d.m_logotypeLabel->isHidden())
        {
            QRect rectPage = opt.rect;
            QRect rect = d.m_logotypeLabel->geometry();
            if (d.m_logotypeLabel->alignment() == Qt::AlignRight)
            {
                rectPage.adjust(0, 0, 4, 0);
                rectPage.setRight(rectPage.right() - rect.width());
                p.setClipRect(rectPage);
            }
            else    
            {
                rectPage.adjust(-2, 0, 0, 0);
                rectPage.setLeft(rect.width());
                p.setClipRect(rectPage);
            }
        }
        ribbonStyle->drawControl((QStyle::ControlElement)RibbonStyle::CE_RibbonGroups, &opt, &p, this);
    }
}

/*! \reimp */
void RibbonBar::changeEvent(QEvent* event)
{
    return QMenuBar::changeEvent(event);
}

/*! \reimp */
void RibbonBar::resizeEvent(QResizeEvent* event)
{
    QTN_D(RibbonBar);
    if (event->oldSize().width() != event->size().width())
    {
        d.layoutCorner();
        updateLayout();
    }
}

/*! \reimp */
void RibbonBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    QMenuBar::mouseDoubleClickEvent(event);

    if (event->button() != Qt::LeftButton)
        return;

    QTN_D(RibbonBar);
    if (isFrameThemeEnabled() && isTitleBarVisible())
    {
        if (d.hitTestContextHeaders(event->pos()))
        {
            QWidget* widget = parentWidget();
            if (widget == Q_NULL )
                return;

            StyleOptionTitleBar titleBarOpt;
            d.getTitleBarOption(titleBarOpt);

            bool buttonMax = ::buttonVisible(QStyle::SC_TitleBarMaxButton, &titleBarOpt);
            bool buttonNormal = ::buttonVisible(QStyle::SC_TitleBarNormalButton, &titleBarOpt);

            if (buttonMax)
                widget->showMaximized();
            else if (buttonNormal)
                widget->showNormal();

            QSize s = widget->size();
            if (s.isValid())
                widget->resize(s);

            return;
        }
    }
}

/*! \reimp */
void RibbonBar::mousePressEvent(QMouseEvent* event)
{
    // block the function call QMenuBar
    QWidget::mousePressEvent(event);

    QTN_D(RibbonBar);
    if (event->button() == Qt::LeftButton)
    {
        if (ContextHeader* header = d.hitTestContextHeaders(event->pos()))
        {
            for (int i = 0, count = d.m_tabBar->getTabCount(); count > i; i++)
            {
                if (RibbonTab* tab = d.m_tabBar->getTab(i))
                {
                    if (tab == header->firstTab)
                    {
                        setCurrentPageIndex(i);
                        break;
                    }
                }
            }
        }
    }
}

/*! \reimp */
void RibbonBar::wheelEvent(QWheelEvent* event)
{
    QTN_D(RibbonBar);
    if (QApplication::activePopupWidget())
        return;

    if (!isMaximized())
        return;

    QPoint pos = event->pos();

    if (!geometry().contains(pos))
        return;

    d.m_tabBar->nextTab(event->delta() < 0);
}

void RibbonBar::contextMenuEvent(QContextMenuEvent* event)
{
    QTN_D(RibbonBar)
    if (QMenu* menu = d.createContextMenu()) 
    {
        menu->setAttribute(Qt::WA_DeleteOnClose);

        event->accept();

        emit showRibbonContextMenu(menu, event);
        
        if (event->isAccepted())
            menu->exec(event->globalPos());
    }
}

int RibbonBar::heightForWidth(int) const
{
    QTN_D(const RibbonBar);

    const int tabsHeight = d.m_rcTabBar.height();
    const int pageAreaHeight = d.m_rcPageArea.height();
    const int ribbonTopBorder = d.m_frameHelper && d.m_frameHelper->isDwmEnabled() ? d.m_frameHelper->frameBorder() : 0;
    const int heightTitle = d.m_frameHelper ? titleBarHeight() : d.m_rcTitle.height();

    if (!d.m_ribbonBarVisible && d.m_frameHelper && d.m_frameHelper->canHideTitle())
        return heightTitle;

    int height = tabsHeight + pageAreaHeight + heightTitle + ribbonTopBorder;

    if (d.m_quickAccessBarPosition != RibbonBar::QATopPosition)
        height += d.m_rcQuickAccess.height();

    if (d.m_minimized)
        height += 3;

    if (d.m_ribbonBarVisible)
        return height;

    return 0;
}

QSize RibbonBar::sizeHint() const
{
    return QSize(rect().width(), heightForWidth(0)).expandedTo(QApplication::globalStrut());
}

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool RibbonBar::nativeEvent(const QByteArray& eventType, void* message, long* result)
#else
bool RibbonBar::winEvent(MSG* message, long* result)
#endif
{
    QTN_D(RibbonBar);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (d.m_frameHelper)
    {
        MSG* msg = static_cast<MSG *>(message);
        if (msg->message == WM_NCHITTEST)
        {
            *result = HTTRANSPARENT;
            return true;
        }
    }
    return QMenuBar::nativeEvent(eventType, message, result);
#else
    if (d.m_frameHelper /*&& d.m_frameHelper->winEvent(message, result)*/)
    {
        MSG* msg = static_cast<MSG *>(message);
        if (msg->message == WM_NCHITTEST)
        {
            *result = HTTRANSPARENT;
            return true;
        }
    }

    return QMenuBar::winEvent(message, result);
#endif
}
#endif // Q_OS_WIN

/*!
Installs translator to QApplication based on \a country variable. Format variable is de_de, ru_ru, fr_fr. If \a country is empty then loaded QTranslator based on current locale setting.
*/
bool RibbonBar::loadTranslation(const QString& country)
{
    static bool loaded = false;
    if (loaded)
        return true;

    const QString directory = QLatin1String(":/translations/ribbon");

    QTranslator* translator = new QTranslator(qApp);
    if (country.isEmpty())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
        foreach(const QLocale locale, QLocale().uiLanguages())
        {
            if (translator->load(locale, QLatin1String(""), QLatin1String(""), directory))
            {
                qApp->installTranslator(translator);
                loaded = true;
                return true;
            }
        }
#endif
    }
    else
    {
        if (translator->load(country, directory))
        {
            qApp->installTranslator(translator);
            loaded = true;
            return true;
        }
    }

    delete translator;
    return false;
}

QString RibbonBar::tr_compatible(const char *s, const char *c)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return QCoreApplication::translate("Qtitan::RibbonBar", s, c);
#else
    return QCoreApplication::translate("Qtitan::RibbonBar", s, c, QCoreApplication::CodecForTr);
#endif
}
