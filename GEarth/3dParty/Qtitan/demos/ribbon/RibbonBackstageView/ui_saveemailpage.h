/********************************************************************************
** Form generated from reading UI file 'saveemailpage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEEMAILPAGE_H
#define UI_SAVEEMAILPAGE_H

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

class Ui_SaveEmailPage
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    Qtitan::RibbonBackstageSeparator *backstageSeparator;
    QHBoxLayout *horizontalLayout;
    Qtitan::RibbonBackstageButton *backstageButton;
    QLabel *label;
    QLabel *label_3;

    void setupUi(Qtitan::RibbonBackstagePage *SaveEmailPage)
    {
        if (SaveEmailPage->objectName().isEmpty())
            SaveEmailPage->setObjectName(QStringLiteral("SaveEmailPage"));
        SaveEmailPage->resize(437, 295);
        horizontalLayout_2 = new QHBoxLayout(SaveEmailPage);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(-1, 0, 0, -1);
        label_2 = new QLabel(SaveEmailPage);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        backstageSeparator = new Qtitan::RibbonBackstageSeparator(SaveEmailPage);
        backstageSeparator->setObjectName(QStringLiteral("backstageSeparator"));

        verticalLayout->addWidget(backstageSeparator);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        backstageButton = new Qtitan::RibbonBackstageButton(SaveEmailPage);
        backstageButton->setObjectName(QStringLiteral("backstageButton"));
        backstageButton->setMinimumSize(QSize(0, 0));
        backstageButton->setMaximumSize(QSize(16777215, 16777215));
        backstageButton->setIconSize(QSize(32, 32));
        backstageButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

        horizontalLayout->addWidget(backstageButton);

        label = new QLabel(SaveEmailPage);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        label_3 = new QLabel(SaveEmailPage);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        verticalLayout->setStretch(3, 1);

        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(SaveEmailPage);

        QMetaObject::connectSlotsByName(SaveEmailPage);
    } // setupUi

    void retranslateUi(Qtitan::RibbonBackstagePage *SaveEmailPage)
    {
        SaveEmailPage->setWindowTitle(QApplication::translate("SaveEmailPage", "SaveEmailPage", Q_NULLPTR));
        label_2->setText(QApplication::translate("SaveEmailPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Send Using E-mail</span></p></body></html>", Q_NULLPTR));
        backstageButton->setText(QApplication::translate("SaveEmailPage", "Send as \n"
" Attachment", Q_NULLPTR));
        label->setText(QApplication::translate("SaveEmailPage", "Attach a copy of this document to an e-mail ", Q_NULLPTR));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SaveEmailPage: public Ui_SaveEmailPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEEMAILPAGE_H
