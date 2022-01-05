/********************************************************************************
** Form generated from reading UI file 'RibbonDesignerDemo.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RIBBONDESIGNERDEMO_H
#define UI_RIBBONDESIGNERDEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "QtnRibbonBar.h"
#include "QtnRibbonGroup.h"
#include "QtnRibbonMainWindow.h"
#include "QtnRibbonPage.h"
#include "QtnRibbonStyle.h"

QT_BEGIN_NAMESPACE

class Ui_RibbonMainWindow
{
public:
    QWidget *centralwidget;
    Qtitan::RibbonBar *ribbonBar;
    Qtitan::RibbonPage *homePage;
    Qtitan::RibbonGroup *ribbonGroupClipboard;
    QStatusBar *statusBar;
    Qtitan::RibbonStyle *ribbonStyle;

    void setupUi(Qtitan::RibbonMainWindow *RibbonMainWindow)
    {
        if (RibbonMainWindow->objectName().isEmpty())
            RibbonMainWindow->setObjectName(QStringLiteral("RibbonMainWindow"));
        RibbonMainWindow->resize(785, 509);
        centralwidget = new QWidget(RibbonMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        RibbonMainWindow->setCentralWidget(centralwidget);
        ribbonBar = new Qtitan::RibbonBar(RibbonMainWindow);
        ribbonBar->setObjectName(QStringLiteral("ribbonBar"));
        homePage = new Qtitan::RibbonPage(ribbonBar);
        homePage->setObjectName(QStringLiteral("homePage"));
        homePage->setGeometry(QRect(0, 23, 785, 93));
        ribbonGroupClipboard = new Qtitan::RibbonGroup(homePage);
        ribbonGroupClipboard->setObjectName(QStringLiteral("ribbonGroupClipboard"));
        ribbonGroupClipboard->setGeometry(QRect(4, 3, 77, 93));
        homePage->addGroup(ribbonGroupClipboard);
        ribbonBar->addPage(homePage);
        RibbonMainWindow->setMenuBar(ribbonBar);
        statusBar = new QStatusBar(RibbonMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RibbonMainWindow->setStatusBar(statusBar);
        ribbonStyle = new Qtitan::RibbonStyle(RibbonMainWindow);
        ribbonStyle->setObjectName(QStringLiteral("ribbonStyle"));
        RibbonMainWindow->setCentralWidget(ribbonStyle);

        retranslateUi(RibbonMainWindow);

        QMetaObject::connectSlotsByName(RibbonMainWindow);
    } // setupUi

    void retranslateUi(Qtitan::RibbonMainWindow *RibbonMainWindow)
    {
        RibbonMainWindow->setWindowTitle(QApplication::translate("RibbonMainWindow", "RibbonMainWindow", Q_NULLPTR));
        homePage->setWindowTitle(QString());
        homePage->setTitle(QApplication::translate("RibbonMainWindow", "&Home", Q_NULLPTR));
        ribbonGroupClipboard->setTitle(QApplication::translate("RibbonMainWindow", "Clipboard", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RibbonMainWindow: public Ui_RibbonMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RIBBONDESIGNERDEMO_H
