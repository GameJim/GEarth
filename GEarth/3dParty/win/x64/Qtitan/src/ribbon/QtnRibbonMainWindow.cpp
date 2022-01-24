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
#include <QPainter>
#include <QLayout>

#include "QtnRibbonBar.h"
#include "QtnRibbonStyle.h"
#include "QtnRibbonMainWindow.h"
#include "QtnOfficeFrameHelper.h"
#ifdef DEBUG_MEMORY_ENABLED
#include "QtitanMSVSDebug.h"
#endif

QTITAN_BEGIN_NAMESPACE
/*!
    \class RibbonMainWindow
    \inmodule QtitanRibbon
    \brief RibbonMainWindow class represents the main window of the application used to create the Ribbon UI.
*/

/*!
\brief Constructs RibbonMainWindow object with the given \a parent and \a flags.
*/ 
RibbonMainWindow::RibbonMainWindow(QWidget* parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
{
    RibbonBar::loadTranslation();
    setObjectName("RibbonMainWindow");
}

/*!
\brief Destructor of the RibbonMainWindow object.
*/ 
RibbonMainWindow::~RibbonMainWindow()
{
}

/*
\brief Returns a pointer to RibbonBar object. If RibbonBar does not exist, then the function will create an empty Ribbon UI panel.
*/
RibbonBar* RibbonMainWindow::ribbonBar() const
{
    RibbonBar* ribbonBar = Q_NULL;

    QWidget* menu = menuWidget();
    if (menu)
        ribbonBar = qobject_cast<RibbonBar*>(menu);

    if (!menu && !ribbonBar) 
    {
        RibbonMainWindow* self = const_cast<RibbonMainWindow*>(this);
        ribbonBar = new RibbonBar(self);
        self->setMenuWidget(ribbonBar);
    }
    return ribbonBar;
}

/*
\brief Sets the RibbonBar to the main window. Object RibbonMainWindow takes ownership of the \a ribbonBar. The method is used when needed to set the custom Ribbon Bar object, whose class, inherited from RibbonBar.
*/
void RibbonMainWindow::setRibbonBar(RibbonBar* ribbonBar)
{
    setMenuWidget(ribbonBar);
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
/*! \internal */
void RibbonMainWindow::setCentralWidget(QStyle* style)
{
    Q_UNUSED(style);
}
#endif
QTITAN_END_NAMESPACE
