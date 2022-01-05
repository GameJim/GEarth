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
#ifndef QTN_RIBBONGROUPPRIVATE_H
#define QTN_RIBBONGROUPPRIVATE_H

#include <QIcon>
#include "QtnRibbonGroup.h"
#include "QtnRibbonControls.h"
#include "QtitanDef.h"

class QAction;
class QToolButton;
class QStyleOptionGroupBox;

QTITAN_BEGIN_NAMESPACE

class RibbonGroupOption;
class RibbonGroupScroll;
class RibbonControl;
class RibbonGroupPopup;

/* RibbonGroupPrivate */   
class RibbonGroupPrivate : public QObject
{
    Q_OBJECT
public:
    QTN_DECLARE_PUBLIC(RibbonGroup)
public:
    explicit RibbonGroupPrivate();
    virtual ~RibbonGroupPrivate();
    static RibbonGroupPrivate* _get(RibbonGroup* group);
public:
    void init();
    void clear(bool deleteControls);
    void enableGroupScroll(bool scrollLeft, bool scrollRight);
    void showGroupScroll();

    void initStyleOption(QStyleOptionGroupBox& opt) const;
    int calcMinimumWidth(RibbonControlSizeDefinition::GroupSize size) const;
    int arrangeRowContent(QList<RibbonControl *>& row, int leftOffset, int rowItemHeight, int rowItemCount, QWidget* parent) const;
    void reposition(const QRect& rect);
    void updateOptionButtonLayout();
    QSize updateControlsLayout(QWidget* parent, int leftOffset = 0) const;
    void adjustCurrentSize(bool expand);
    void updateLayoutParent();
    Qt::TextElideMode titleTextElideMode() const;
    QSize sizeHint() const;

    void setReductionSize(RibbonControlSizeDefinition::GroupSize size);
    void reduce();
    void expand();
    bool canReduce();
    bool canExpand();
    RibbonGroupPopup* groupPopupWidget();
public Q_SLOTS:
    void pressLeftScrollButton();
    void pressRightScrollButton();
    void hidePopupMode(QAction* action);
public:
    static bool visualIndexLessThan(RibbonControl* first, RibbonControl* second);
    RibbonGroupOption* m_optionButton;
    RibbonGroupScroll* m_buttonScrollGroupLeft;    // Button to draw left scroll
    RibbonGroupScroll* m_buttonScrollGroupRight;   // Button to draw right scroll
    RibbonGroupPopup* m_groupPopupWidget;
    RibbonToolBarControl* m_toolBar;

    QList<RibbonControl*> m_controls;
    RibbonControlSizeDefinition::GroupSize m_currentSize;
    Qt::Alignment m_contentAlignment;
    Qt::Alignment m_controlsAlignment;

    bool m_butOptionVisible;
    bool m_dirty;

    QAction* m_actOption;
    QString m_title;
    QIcon m_icon;
    QFont* m_font;
    QColor m_color;
    int m_pageScrollPos;
    int m_groupScrollPos;  // Scroll position
    Qt::TextElideMode m_titleElideMode;
private:
    Q_DISABLE_COPY(RibbonGroupPrivate)
};

QTITAN_END_NAMESPACE

#endif // QTN_RIBBONGROUPPRIVATE_H


