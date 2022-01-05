/********************************************************************************
** Form generated from reading UI file 'savepage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEPAGE_H
#define UI_SAVEPAGE_H

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

class Ui_SaveSendPage
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayoutMain;
    QVBoxLayout *verticalLayoutTabs;
    QVBoxLayout *verticalLayoutSaveSend;
    QLabel *labelSaveSend;
    Qtitan::RibbonBackstageSeparator *backstageSeparatorFileType;
    QVBoxLayout *verticalLayout;
    Qtitan::RibbonBackstageButton *sendEmailButton;
    Qtitan::RibbonBackstageButton *saveWebButton;
    Qtitan::RibbonBackstageButton *sharePointButton;
    Qtitan::RibbonBackstageButton *blogPostButton;
    QVBoxLayout *verticalLayoutFileTypeCaption;
    QLabel *labelFileTypes;
    Qtitan::RibbonBackstageSeparator *backstageSeparator;
    QVBoxLayout *verticalLayoutFileType;
    Qtitan::RibbonBackstageButton *fileTypeButton;
    Qtitan::RibbonBackstageButton *createDocButton;
    QLabel *labelWrapperNull;
    QLabel *pageWrapper;

    void setupUi(Qtitan::RibbonBackstagePage *SaveSendPage)
    {
        if (SaveSendPage->objectName().isEmpty())
            SaveSendPage->setObjectName(QStringLiteral("SaveSendPage"));
        SaveSendPage->resize(704, 490);
        SaveSendPage->setMinimumSize(QSize(637, 445));
        horizontalLayout_2 = new QHBoxLayout(SaveSendPage);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayoutMain = new QHBoxLayout();
        horizontalLayoutMain->setSpacing(0);
        horizontalLayoutMain->setObjectName(QStringLiteral("horizontalLayoutMain"));
        horizontalLayoutMain->setContentsMargins(14, 24, 0, 14);
        verticalLayoutTabs = new QVBoxLayout();
        verticalLayoutTabs->setSpacing(10);
        verticalLayoutTabs->setObjectName(QStringLiteral("verticalLayoutTabs"));
        verticalLayoutSaveSend = new QVBoxLayout();
        verticalLayoutSaveSend->setSpacing(10);
        verticalLayoutSaveSend->setObjectName(QStringLiteral("verticalLayoutSaveSend"));
        labelSaveSend = new QLabel(SaveSendPage);
        labelSaveSend->setObjectName(QStringLiteral("labelSaveSend"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelSaveSend->sizePolicy().hasHeightForWidth());
        labelSaveSend->setSizePolicy(sizePolicy);

        verticalLayoutSaveSend->addWidget(labelSaveSend);


        verticalLayoutTabs->addLayout(verticalLayoutSaveSend);

        backstageSeparatorFileType = new Qtitan::RibbonBackstageSeparator(SaveSendPage);
        backstageSeparatorFileType->setObjectName(QStringLiteral("backstageSeparatorFileType"));
        backstageSeparatorFileType->setMaximumSize(QSize(270, 16777215));

        verticalLayoutTabs->addWidget(backstageSeparatorFileType);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sendEmailButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        sendEmailButton->setObjectName(QStringLiteral("sendEmailButton"));
        sendEmailButton->setMinimumSize(QSize(282, 47));
        sendEmailButton->setMaximumSize(QSize(282, 47));
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/SendUsing.png"), QSize(), QIcon::Normal, QIcon::Off);
        sendEmailButton->setIcon(icon);
        sendEmailButton->setIconSize(QSize(32, 32));
        sendEmailButton->setCheckable(true);
        sendEmailButton->setChecked(true);
        sendEmailButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        sendEmailButton->setTabStyle(true);
        sendEmailButton->setFlatStyle(true);

        verticalLayout->addWidget(sendEmailButton);

        saveWebButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        saveWebButton->setObjectName(QStringLiteral("saveWebButton"));
        saveWebButton->setMinimumSize(QSize(282, 47));
        saveWebButton->setMaximumSize(QSize(282, 47));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/SaveWeb.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveWebButton->setIcon(icon1);
        saveWebButton->setIconSize(QSize(32, 32));
        saveWebButton->setCheckable(true);
        saveWebButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        saveWebButton->setTabStyle(true);
        saveWebButton->setFlatStyle(true);

        verticalLayout->addWidget(saveWebButton);

        sharePointButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        sharePointButton->setObjectName(QStringLiteral("sharePointButton"));
        sharePointButton->setMinimumSize(QSize(282, 47));
        sharePointButton->setMaximumSize(QSize(282, 47));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/res/SaveSharePoint.png"), QSize(), QIcon::Normal, QIcon::Off);
        sharePointButton->setIcon(icon2);
        sharePointButton->setIconSize(QSize(32, 32));
        sharePointButton->setCheckable(true);
        sharePointButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        sharePointButton->setTabStyle(true);
        sharePointButton->setFlatStyle(true);

        verticalLayout->addWidget(sharePointButton);

        blogPostButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        blogPostButton->setObjectName(QStringLiteral("blogPostButton"));
        blogPostButton->setMinimumSize(QSize(282, 47));
        blogPostButton->setMaximumSize(QSize(282, 47));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/res/Publish.png"), QSize(), QIcon::Normal, QIcon::Off);
        blogPostButton->setIcon(icon3);
        blogPostButton->setIconSize(QSize(32, 32));
        blogPostButton->setCheckable(true);
        blogPostButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        blogPostButton->setTabStyle(true);
        blogPostButton->setFlatStyle(true);

        verticalLayout->addWidget(blogPostButton);


        verticalLayoutTabs->addLayout(verticalLayout);

        verticalLayoutFileTypeCaption = new QVBoxLayout();
        verticalLayoutFileTypeCaption->setSpacing(10);
        verticalLayoutFileTypeCaption->setObjectName(QStringLiteral("verticalLayoutFileTypeCaption"));
        labelFileTypes = new QLabel(SaveSendPage);
        labelFileTypes->setObjectName(QStringLiteral("labelFileTypes"));
        sizePolicy.setHeightForWidth(labelFileTypes->sizePolicy().hasHeightForWidth());
        labelFileTypes->setSizePolicy(sizePolicy);

        verticalLayoutFileTypeCaption->addWidget(labelFileTypes);

        backstageSeparator = new Qtitan::RibbonBackstageSeparator(SaveSendPage);
        backstageSeparator->setObjectName(QStringLiteral("backstageSeparator"));
        backstageSeparator->setMaximumSize(QSize(270, 16777215));

        verticalLayoutFileTypeCaption->addWidget(backstageSeparator);


        verticalLayoutTabs->addLayout(verticalLayoutFileTypeCaption);

        verticalLayoutFileType = new QVBoxLayout();
        verticalLayoutFileType->setSpacing(1);
        verticalLayoutFileType->setObjectName(QStringLiteral("verticalLayoutFileType"));
        fileTypeButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        fileTypeButton->setObjectName(QStringLiteral("fileTypeButton"));
        fileTypeButton->setMinimumSize(QSize(282, 47));
        fileTypeButton->setMaximumSize(QSize(282, 47));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/res/ChangeFileType.png"), QSize(), QIcon::Normal, QIcon::Off);
        fileTypeButton->setIcon(icon4);
        fileTypeButton->setIconSize(QSize(32, 32));
        fileTypeButton->setCheckable(true);
        fileTypeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        fileTypeButton->setTabStyle(true);
        fileTypeButton->setFlatStyle(true);

        verticalLayoutFileType->addWidget(fileTypeButton);

        createDocButton = new Qtitan::RibbonBackstageButton(SaveSendPage);
        createDocButton->setObjectName(QStringLiteral("createDocButton"));
        createDocButton->setMinimumSize(QSize(282, 47));
        createDocButton->setMaximumSize(QSize(282, 47));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/res/CreateDocument.png"), QSize(), QIcon::Normal, QIcon::Off);
        createDocButton->setIcon(icon5);
        createDocButton->setIconSize(QSize(32, 32));
        createDocButton->setCheckable(true);
        createDocButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        createDocButton->setTabStyle(true);
        createDocButton->setFlatStyle(true);

        verticalLayoutFileType->addWidget(createDocButton);

        labelWrapperNull = new QLabel(SaveSendPage);
        labelWrapperNull->setObjectName(QStringLiteral("labelWrapperNull"));

        verticalLayoutFileType->addWidget(labelWrapperNull);


        verticalLayoutTabs->addLayout(verticalLayoutFileType);

        verticalLayoutTabs->setStretch(4, 1);

        horizontalLayoutMain->addLayout(verticalLayoutTabs);

        pageWrapper = new QLabel(SaveSendPage);
        pageWrapper->setObjectName(QStringLiteral("pageWrapper"));
        pageWrapper->setMinimumSize(QSize(0, 0));

        horizontalLayoutMain->addWidget(pageWrapper);

        horizontalLayoutMain->setStretch(1, 1);

        horizontalLayout_2->addLayout(horizontalLayoutMain);


        retranslateUi(SaveSendPage);

        QMetaObject::connectSlotsByName(SaveSendPage);
    } // setupUi

    void retranslateUi(Qtitan::RibbonBackstagePage *SaveSendPage)
    {
        SaveSendPage->setWindowTitle(QApplication::translate("SaveSendPage", "Save & Send", Q_NULLPTR));
        labelSaveSend->setText(QApplication::translate("SaveSendPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Save &amp; Send</span></p></body></html>", Q_NULLPTR));
        sendEmailButton->setText(QApplication::translate("SaveSendPage", "Send Using E-mail", Q_NULLPTR));
        saveWebButton->setText(QApplication::translate("SaveSendPage", "Save to Web", Q_NULLPTR));
        sharePointButton->setText(QApplication::translate("SaveSendPage", "Save to SharePoint", Q_NULLPTR));
        blogPostButton->setText(QApplication::translate("SaveSendPage", "Publish as Blog Post", Q_NULLPTR));
        labelFileTypes->setText(QApplication::translate("SaveSendPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">File Types</span></p></body></html>", Q_NULLPTR));
        fileTypeButton->setText(QApplication::translate("SaveSendPage", "Change File Type", Q_NULLPTR));
        createDocButton->setText(QApplication::translate("SaveSendPage", "Create PDF/XPS Document", Q_NULLPTR));
        labelWrapperNull->setText(QString());
        pageWrapper->setText(QApplication::translate("SaveSendPage", "Wrapper", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SaveSendPage: public Ui_SaveSendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEPAGE_H
