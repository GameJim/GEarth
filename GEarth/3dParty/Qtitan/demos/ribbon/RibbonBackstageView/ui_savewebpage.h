/********************************************************************************
** Form generated from reading UI file 'savewebpage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEWEBPAGE_H
#define UI_SAVEWEBPAGE_H

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

class Ui_SaveWebPage
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    Qtitan::RibbonBackstageSeparator *backstageSeparator;
    Qtitan::RibbonBackstageButton *backstageButton;
    QLabel *label_3;

    void setupUi(Qtitan::RibbonBackstagePage *SaveWebPage)
    {
        if (SaveWebPage->objectName().isEmpty())
            SaveWebPage->setObjectName(QStringLiteral("SaveWebPage"));
        SaveWebPage->resize(509, 310);
        horizontalLayout = new QHBoxLayout(SaveWebPage);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, -1, -1);
        label_2 = new QLabel(SaveWebPage);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        backstageSeparator = new Qtitan::RibbonBackstageSeparator(SaveWebPage);
        backstageSeparator->setObjectName(QStringLiteral("backstageSeparator"));

        verticalLayout->addWidget(backstageSeparator);

        backstageButton = new Qtitan::RibbonBackstageButton(SaveWebPage);
        backstageButton->setObjectName(QStringLiteral("backstageButton"));
        backstageButton->setMinimumSize(QSize(0, 0));
        backstageButton->setMaximumSize(QSize(16777215, 16777215));
        backstageButton->setIconSize(QSize(32, 32));
        backstageButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

        verticalLayout->addWidget(backstageButton);

        label_3 = new QLabel(SaveWebPage);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        verticalLayout->setStretch(3, 1);

        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(SaveWebPage);

        QMetaObject::connectSlotsByName(SaveWebPage);
    } // setupUi

    void retranslateUi(Qtitan::RibbonBackstagePage *SaveWebPage)
    {
        SaveWebPage->setWindowTitle(QApplication::translate("SaveWebPage", "SaveWebPage", Q_NULLPTR));
        label_2->setText(QApplication::translate("SaveWebPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Send to Web</span></p></body></html>", Q_NULLPTR));
        backstageButton->setText(QApplication::translate("SaveWebPage", "Send In", Q_NULLPTR));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SaveWebPage: public Ui_SaveWebPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEWEBPAGE_H
