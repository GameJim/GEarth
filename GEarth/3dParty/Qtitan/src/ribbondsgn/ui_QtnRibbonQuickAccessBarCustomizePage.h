/********************************************************************************
** Form generated from reading UI file 'QtnRibbonQuickAccessBarCustomizePage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTNRIBBONQUICKACCESSBARCUSTOMIZEPAGE_H
#define UI_QTNRIBBONQUICKACCESSBARCUSTOMIZEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RibbonQuickAccessBarCustomizePage
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelCommandsFrom;
    QComboBox *comboBoxSourceCategory;
    QLabel *labelCommands;
    QListWidget *commandsList;
    QCheckBox *checkBoxQATOn;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *addButton;
    QPushButton *removeButton;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QListWidget *quickAccessBarList;
    QPushButton *pushButtonReset;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *upButton;
    QPushButton *downButton;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *RibbonQuickAccessBarCustomizePage)
    {
        if (RibbonQuickAccessBarCustomizePage->objectName().isEmpty())
            RibbonQuickAccessBarCustomizePage->setObjectName(QStringLiteral("RibbonQuickAccessBarCustomizePage"));
        RibbonQuickAccessBarCustomizePage->resize(600, 380);
        RibbonQuickAccessBarCustomizePage->setMinimumSize(QSize(600, 380));
        horizontalLayout = new QHBoxLayout(RibbonQuickAccessBarCustomizePage);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        labelCommandsFrom = new QLabel(RibbonQuickAccessBarCustomizePage);
        labelCommandsFrom->setObjectName(QStringLiteral("labelCommandsFrom"));
        labelCommandsFrom->setTextFormat(Qt::AutoText);
        labelCommandsFrom->setWordWrap(false);
        labelCommandsFrom->setOpenExternalLinks(false);
        labelCommandsFrom->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        verticalLayout_3->addWidget(labelCommandsFrom);

        comboBoxSourceCategory = new QComboBox(RibbonQuickAccessBarCustomizePage);
        comboBoxSourceCategory->setObjectName(QStringLiteral("comboBoxSourceCategory"));

        verticalLayout_3->addWidget(comboBoxSourceCategory);

        labelCommands = new QLabel(RibbonQuickAccessBarCustomizePage);
        labelCommands->setObjectName(QStringLiteral("labelCommands"));
        labelCommands->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        verticalLayout_3->addWidget(labelCommands);

        commandsList = new QListWidget(RibbonQuickAccessBarCustomizePage);
        commandsList->setObjectName(QStringLiteral("commandsList"));

        verticalLayout_3->addWidget(commandsList);

        checkBoxQATOn = new QCheckBox(RibbonQuickAccessBarCustomizePage);
        checkBoxQATOn->setObjectName(QStringLiteral("checkBoxQATOn"));

        verticalLayout_3->addWidget(checkBoxQATOn);

        verticalLayout_3->setStretch(1, 1);

        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer);

        addButton = new QPushButton(RibbonQuickAccessBarCustomizePage);
        addButton->setObjectName(QStringLiteral("addButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy);
        addButton->setAutoDefault(false);

        verticalLayout->addWidget(addButton);

        removeButton = new QPushButton(RibbonQuickAccessBarCustomizePage);
        removeButton->setObjectName(QStringLiteral("removeButton"));
        sizePolicy.setHeightForWidth(removeButton->sizePolicy().hasHeightForWidth());
        removeButton->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(removeButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer_2);

        verticalLayout->setStretch(2, 1);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        quickAccessBarList = new QListWidget(RibbonQuickAccessBarCustomizePage);
        quickAccessBarList->setObjectName(QStringLiteral("quickAccessBarList"));

        verticalLayout_2->addWidget(quickAccessBarList);

        pushButtonReset = new QPushButton(RibbonQuickAccessBarCustomizePage);
        pushButtonReset->setObjectName(QStringLiteral("pushButtonReset"));
        sizePolicy.setHeightForWidth(pushButtonReset->sizePolicy().hasHeightForWidth());
        pushButtonReset->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(pushButtonReset);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        upButton = new QPushButton(RibbonQuickAccessBarCustomizePage);
        upButton->setObjectName(QStringLiteral("upButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(upButton->sizePolicy().hasHeightForWidth());
        upButton->setSizePolicy(sizePolicy1);
        upButton->setMaximumSize(QSize(30, 30));

        verticalLayout_4->addWidget(upButton);

        downButton = new QPushButton(RibbonQuickAccessBarCustomizePage);
        downButton->setObjectName(QStringLiteral("downButton"));
        sizePolicy1.setHeightForWidth(downButton->sizePolicy().hasHeightForWidth());
        downButton->setSizePolicy(sizePolicy1);
        downButton->setMaximumSize(QSize(30, 30));
        downButton->setFlat(false);

        verticalLayout_4->addWidget(downButton);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);


        horizontalLayout->addLayout(verticalLayout_4);

        horizontalLayout->setStretch(3, 1);
#ifndef QT_NO_SHORTCUT
        labelCommandsFrom->setBuddy(comboBoxSourceCategory);
        labelCommands->setBuddy(commandsList);
#endif // QT_NO_SHORTCUT

        retranslateUi(RibbonQuickAccessBarCustomizePage);

        QMetaObject::connectSlotsByName(RibbonQuickAccessBarCustomizePage);
    } // setupUi

    void retranslateUi(QWidget *RibbonQuickAccessBarCustomizePage)
    {
        RibbonQuickAccessBarCustomizePage->setWindowTitle(QApplication::translate("RibbonQuickAccessBarCustomizePage", "Quick Access Toolbar", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        RibbonQuickAccessBarCustomizePage->setStatusTip(QApplication::translate("RibbonQuickAccessBarCustomizePage", "Customize the Quick Access Toolbar", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        labelCommandsFrom->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "&Choose commands from:", Q_NULLPTR));
        labelCommands->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "C&ommands:", Q_NULLPTR));
        checkBoxQATOn->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "S&how Quick Access Toolbar below the Ribbon", Q_NULLPTR));
        addButton->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "&Add > >", Q_NULLPTR));
        removeButton->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "< < &Remove", Q_NULLPTR));
        pushButtonReset->setText(QApplication::translate("RibbonQuickAccessBarCustomizePage", "Re&set", Q_NULLPTR));
        upButton->setText(QString());
        downButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RibbonQuickAccessBarCustomizePage: public Ui_RibbonQuickAccessBarCustomizePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTNRIBBONQUICKACCESSBARCUSTOMIZEPAGE_H
