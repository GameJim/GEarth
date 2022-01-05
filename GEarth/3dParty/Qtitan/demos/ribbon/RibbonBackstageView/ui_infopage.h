/********************************************************************************
** Form generated from reading UI file 'infopage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOPAGE_H
#define UI_INFOPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "QtnRibbonBackstageView.h"

QT_BEGIN_NAMESPACE

class Ui_InfoPage
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    Qtitan::RibbonBackstageSeparator *backstageSeparator;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_3;
    Qtitan::RibbonBackstageButton *protectDocumentBut;
    QLabel *label;
    Qtitan::RibbonBackstageSeparator *backstageSeparator_2;
    QFormLayout *formLayout;
    Qtitan::RibbonBackstageButton *checkIssuesBut;
    QLabel *label_2;
    Qtitan::RibbonBackstageSeparator *backstageSeparator_3;
    QFormLayout *formLayout_2;
    Qtitan::RibbonBackstageButton *mngVersionsBut;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    Qtitan::RibbonBackstageSeparator *backstageSeparator_4;
    QLabel *label_4;

    void setupUi(Qtitan::RibbonBackstagePage *InfoPage)
    {
        if (InfoPage->objectName().isEmpty())
            InfoPage->setObjectName(QStringLiteral("InfoPage"));
        InfoPage->resize(919, 505);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InfoPage->sizePolicy().hasHeightForWidth());
        InfoPage->setSizePolicy(sizePolicy);
        InfoPage->setMinimumSize(QSize(637, 401));
        InfoPage->setMaximumSize(QSize(16777215, 16777215));
        InfoPage->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout_2 = new QHBoxLayout(InfoPage);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(18);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 10, 18, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(12);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(23, -1, -1, -1);
        label_5 = new QLabel(InfoPage);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(label_5);

        backstageSeparator = new Qtitan::RibbonBackstageSeparator(InfoPage);
        backstageSeparator->setObjectName(QStringLiteral("backstageSeparator"));

        verticalLayout_2->addWidget(backstageSeparator);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        verticalLayout->setContentsMargins(0, -1, -1, -1);
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setSizeConstraint(QLayout::SetMinAndMaxSize);
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_3->setHorizontalSpacing(20);
        protectDocumentBut = new Qtitan::RibbonBackstageButton(InfoPage);
        protectDocumentBut->setObjectName(QStringLiteral("protectDocumentBut"));
        sizePolicy1.setHeightForWidth(protectDocumentBut->sizePolicy().hasHeightForWidth());
        protectDocumentBut->setSizePolicy(sizePolicy1);
        protectDocumentBut->setMinimumSize(QSize(0, 0));
        protectDocumentBut->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/ProtectDocument.png"), QSize(), QIcon::Normal, QIcon::Off);
        protectDocumentBut->setIcon(icon);
        protectDocumentBut->setIconSize(QSize(32, 32));
        protectDocumentBut->setPopupMode(QToolButton::InstantPopup);
        protectDocumentBut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, protectDocumentBut);

        label = new QLabel(InfoPage);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(16777215, 16777215));
        label->setTextFormat(Qt::AutoText);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, label);


        verticalLayout->addLayout(formLayout_3);

        backstageSeparator_2 = new Qtitan::RibbonBackstageSeparator(InfoPage);
        backstageSeparator_2->setObjectName(QStringLiteral("backstageSeparator_2"));

        verticalLayout->addWidget(backstageSeparator_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        formLayout->setHorizontalSpacing(20);
        formLayout->setVerticalSpacing(7);
        checkIssuesBut = new Qtitan::RibbonBackstageButton(InfoPage);
        checkIssuesBut->setObjectName(QStringLiteral("checkIssuesBut"));
        sizePolicy1.setHeightForWidth(checkIssuesBut->sizePolicy().hasHeightForWidth());
        checkIssuesBut->setSizePolicy(sizePolicy1);
        checkIssuesBut->setMinimumSize(QSize(0, 0));
        checkIssuesBut->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/CheckIssues.png"), QSize(), QIcon::Normal, QIcon::Off);
        checkIssuesBut->setIcon(icon1);
        checkIssuesBut->setIconSize(QSize(32, 32));
        checkIssuesBut->setPopupMode(QToolButton::InstantPopup);
        checkIssuesBut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        formLayout->setWidget(0, QFormLayout::LabelRole, checkIssuesBut);

        label_2 = new QLabel(InfoPage);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(16777215, 16777215));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_2->setWordWrap(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, label_2);


        verticalLayout->addLayout(formLayout);

        backstageSeparator_3 = new Qtitan::RibbonBackstageSeparator(InfoPage);
        backstageSeparator_3->setObjectName(QStringLiteral("backstageSeparator_3"));

        verticalLayout->addWidget(backstageSeparator_3);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_2->setRowWrapPolicy(QFormLayout::DontWrapRows);
        formLayout_2->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        formLayout_2->setHorizontalSpacing(20);
        formLayout_2->setVerticalSpacing(0);
        formLayout_2->setContentsMargins(-1, -1, -1, 2);
        mngVersionsBut = new Qtitan::RibbonBackstageButton(InfoPage);
        mngVersionsBut->setObjectName(QStringLiteral("mngVersionsBut"));
        sizePolicy1.setHeightForWidth(mngVersionsBut->sizePolicy().hasHeightForWidth());
        mngVersionsBut->setSizePolicy(sizePolicy1);
        mngVersionsBut->setMinimumSize(QSize(0, 0));
        mngVersionsBut->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/res/MngVersions.png"), QSize(), QIcon::Normal, QIcon::Off);
        mngVersionsBut->setIcon(icon2);
        mngVersionsBut->setIconSize(QSize(32, 32));
        mngVersionsBut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, mngVersionsBut);

        label_3 = new QLabel(InfoPage);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(16777215, 81));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_3->setWordWrap(true);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_3);


        verticalLayout->addLayout(formLayout_2);

        verticalLayout->setStretch(4, 1);

        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);

        backstageSeparator_4 = new Qtitan::RibbonBackstageSeparator(InfoPage);
        backstageSeparator_4->setObjectName(QStringLiteral("backstageSeparator_4"));
        backstageSeparator_4->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(backstageSeparator_4);

        label_4 = new QLabel(InfoPage);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/res/ScreenPreview.png")));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout->addWidget(label_4);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(InfoPage);

        QMetaObject::connectSlotsByName(InfoPage);
    } // setupUi

    void retranslateUi(Qtitan::RibbonBackstagePage *InfoPage)
    {
        InfoPage->setWindowTitle(QApplication::translate("InfoPage", "Info", Q_NULLPTR));
        label_5->setText(QApplication::translate("InfoPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">Information about document</span></p></body></html>", Q_NULLPTR));
        protectDocumentBut->setText(QApplication::translate("InfoPage", "Protect Document", Q_NULLPTR));
        label->setText(QApplication::translate("InfoPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Permissions</span><span style=\" font-size:8.25pt;\"> </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Anyone can open, copy and change any part of this document</p></body></html>", Q_NULLPTR));
        checkIssuesBut->setText(QApplication::translate("InfoPage", "Check for Issues", Q_NULLPTR));
        label_2->setText(QApplication::translate("InfoPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Prepare for Sharing</span> </p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Before sharing this file, be aware that it contains:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; mar"
                        "gin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Document properties and author's name</span></p></body></html>", Q_NULLPTR));
        mngVersionsBut->setText(QApplication::translate("InfoPage", "Manage Versions", Q_NULLPTR));
        label_3->setText(QApplication::translate("InfoPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Versions</span> </p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">There are no previous vertion of this file.</span></p></body></html>", Q_NULLPTR));
        label_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InfoPage: public Ui_InfoPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOPAGE_H
