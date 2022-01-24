/********************************************************************************
** Form generated from reading UI file 'helppage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPPAGE_H
#define UI_HELPPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "QtnRibbonBackstageView.h"

QT_BEGIN_NAMESPACE

class Ui_AboutPage
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelSupport;
    Qtitan::RibbonBackstageSeparator *backstageSeparator;
    Qtitan::RibbonBackstageButton *buttonHelp;
    QLabel *labelWFill;
    Qtitan::RibbonBackstageSeparator *backstageSeparator_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelLogo;
    QLabel *labelAbout;
    Qtitan::RibbonBackstageSeparator *backstageSeparator_3;
    QLabel *labelVersion;

    void setupUi(Qtitan::RibbonBackstagePage *AboutPage)
    {
        if (AboutPage->objectName().isEmpty())
            AboutPage->setObjectName(QStringLiteral("AboutPage"));
        AboutPage->resize(674, 445);
        AboutPage->setMinimumSize(QSize(637, 445));
        horizontalLayout_2 = new QHBoxLayout(AboutPage);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, 0, -1, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 13, -1, -1);
        labelSupport = new QLabel(AboutPage);
        labelSupport->setObjectName(QStringLiteral("labelSupport"));

        verticalLayout->addWidget(labelSupport);

        backstageSeparator = new Qtitan::RibbonBackstageSeparator(AboutPage);
        backstageSeparator->setObjectName(QStringLiteral("backstageSeparator"));

        verticalLayout->addWidget(backstageSeparator);

        buttonHelp = new Qtitan::RibbonBackstageButton(AboutPage);
        buttonHelp->setObjectName(QStringLiteral("buttonHelp"));
        buttonHelp->setMinimumSize(QSize(0, 0));
        buttonHelp->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonHelp->setIcon(icon);
        buttonHelp->setIconSize(QSize(64, 64));
        buttonHelp->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        verticalLayout->addWidget(buttonHelp);

        labelWFill = new QLabel(AboutPage);
        labelWFill->setObjectName(QStringLiteral("labelWFill"));

        verticalLayout->addWidget(labelWFill);

        verticalLayout->setStretch(3, 1);

        horizontalLayout->addLayout(verticalLayout);

        backstageSeparator_2 = new Qtitan::RibbonBackstageSeparator(AboutPage);
        backstageSeparator_2->setObjectName(QStringLiteral("backstageSeparator_2"));
        backstageSeparator_2->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(backstageSeparator_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(16);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(16, 12, -1, 0);
        labelLogo = new QLabel(AboutPage);
        labelLogo->setObjectName(QStringLiteral("labelLogo"));
        labelLogo->setPixmap(QPixmap(QString::fromUtf8(":/res/devmachines.png")));

        verticalLayout_2->addWidget(labelLogo);

        labelAbout = new QLabel(AboutPage);
        labelAbout->setObjectName(QStringLiteral("labelAbout"));

        verticalLayout_2->addWidget(labelAbout);

        backstageSeparator_3 = new Qtitan::RibbonBackstageSeparator(AboutPage);
        backstageSeparator_3->setObjectName(QStringLiteral("backstageSeparator_3"));

        verticalLayout_2->addWidget(backstageSeparator_3);

        labelVersion = new QLabel(AboutPage);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));
        labelVersion->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(labelVersion);

        verticalLayout_2->setStretch(3, 1);

        horizontalLayout->addLayout(verticalLayout_2);

        horizontalLayout->setStretch(2, 1);

        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(AboutPage);

        QMetaObject::connectSlotsByName(AboutPage);
    } // setupUi

    void retranslateUi(Qtitan::RibbonBackstagePage *AboutPage)
    {
        AboutPage->setWindowTitle(QApplication::translate("AboutPage", "About", Q_NULLPTR));
        labelSupport->setText(QApplication::translate("AboutPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Support</span></p></body></html>", Q_NULLPTR));
        buttonHelp->setText(QApplication::translate("AboutPage", "Get help using Developer Machines Support", Q_NULLPTR));
        labelWFill->setText(QString());
        labelLogo->setText(QString());
        labelAbout->setText(QApplication::translate("AboutPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">About QtitanRibbon by Developer Machines</span></p></body></html>", Q_NULLPTR));
        labelVersion->setText(QApplication::translate("AboutPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Version:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Copyright (c) 2009-2017 Developer Machines. All Rights Reserved.</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutPage: public Ui_AboutPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPPAGE_H
