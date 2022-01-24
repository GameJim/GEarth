/********************************************************************************
** Form generated from reading UI file 'QtnRibbonBarCustomizePage.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTNRIBBONBARCUSTOMIZEPAGE_H
#define UI_QTNRIBBONBARCUSTOMIZEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RibbonBarCustomizePage
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelCommandsFrom;
    QComboBox *comboBoxSourceCategory;
    QTreeWidget *treeCommands;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QPushButton *addButton;
    QPushButton *removeButton;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelCustomizeRibbon;
    QComboBox *switchTabsBox;
    QTreeWidget *treeRibbon;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *newTabButton;
    QPushButton *newGroupButton;
    QPushButton *renameButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelCustomizations;
    QPushButton *resetButton;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *upButton;
    QPushButton *downButton;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *RibbonBarCustomizePage)
    {
        if (RibbonBarCustomizePage->objectName().isEmpty())
            RibbonBarCustomizePage->setObjectName(QStringLiteral("RibbonBarCustomizePage"));
        RibbonBarCustomizePage->resize(600, 380);
        RibbonBarCustomizePage->setMinimumSize(QSize(600, 380));
        horizontalLayout = new QHBoxLayout(RibbonBarCustomizePage);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelCommandsFrom = new QLabel(RibbonBarCustomizePage);
        labelCommandsFrom->setObjectName(QStringLiteral("labelCommandsFrom"));

        verticalLayout->addWidget(labelCommandsFrom);

        comboBoxSourceCategory = new QComboBox(RibbonBarCustomizePage);
        comboBoxSourceCategory->setObjectName(QStringLiteral("comboBoxSourceCategory"));

        verticalLayout->addWidget(comboBoxSourceCategory);

        treeCommands = new QTreeWidget(RibbonBarCustomizePage);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeCommands->setHeaderItem(__qtreewidgetitem);
        treeCommands->setObjectName(QStringLiteral("treeCommands"));
        treeCommands->setHeaderHidden(true);

        verticalLayout->addWidget(treeCommands);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        addButton = new QPushButton(RibbonBarCustomizePage);
        addButton->setObjectName(QStringLiteral("addButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(addButton);

        removeButton = new QPushButton(RibbonBarCustomizePage);
        removeButton->setObjectName(QStringLiteral("removeButton"));
        sizePolicy.setHeightForWidth(removeButton->sizePolicy().hasHeightForWidth());
        removeButton->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(removeButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        verticalLayout_3->setStretch(2, 1);

        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labelCustomizeRibbon = new QLabel(RibbonBarCustomizePage);
        labelCustomizeRibbon->setObjectName(QStringLiteral("labelCustomizeRibbon"));

        verticalLayout_2->addWidget(labelCustomizeRibbon);

        switchTabsBox = new QComboBox(RibbonBarCustomizePage);
        switchTabsBox->setObjectName(QStringLiteral("switchTabsBox"));

        verticalLayout_2->addWidget(switchTabsBox);

        treeRibbon = new QTreeWidget(RibbonBarCustomizePage);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeRibbon->setHeaderItem(__qtreewidgetitem1);
        treeRibbon->setObjectName(QStringLiteral("treeRibbon"));
        treeRibbon->header()->setVisible(false);
        treeRibbon->header()->setStretchLastSection(true);

        verticalLayout_2->addWidget(treeRibbon);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        newTabButton = new QPushButton(RibbonBarCustomizePage);
        newTabButton->setObjectName(QStringLiteral("newTabButton"));

        horizontalLayout_2->addWidget(newTabButton);

        newGroupButton = new QPushButton(RibbonBarCustomizePage);
        newGroupButton->setObjectName(QStringLiteral("newGroupButton"));

        horizontalLayout_2->addWidget(newGroupButton);

        renameButton = new QPushButton(RibbonBarCustomizePage);
        renameButton->setObjectName(QStringLiteral("renameButton"));

        horizontalLayout_2->addWidget(renameButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, -1, -1);
        labelCustomizations = new QLabel(RibbonBarCustomizePage);
        labelCustomizations->setObjectName(QStringLiteral("labelCustomizations"));

        horizontalLayout_3->addWidget(labelCustomizations);

        resetButton = new QPushButton(RibbonBarCustomizePage);
        resetButton->setObjectName(QStringLiteral("resetButton"));

        horizontalLayout_3->addWidget(resetButton);

        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, -1, -1);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        upButton = new QPushButton(RibbonBarCustomizePage);
        upButton->setObjectName(QStringLiteral("upButton"));

        verticalLayout_4->addWidget(upButton);

        downButton = new QPushButton(RibbonBarCustomizePage);
        downButton->setObjectName(QStringLiteral("downButton"));

        verticalLayout_4->addWidget(downButton);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);


        horizontalLayout->addLayout(verticalLayout_4);

#ifndef QT_NO_SHORTCUT
        labelCommandsFrom->setBuddy(comboBoxSourceCategory);
        labelCustomizeRibbon->setBuddy(treeRibbon);
#endif // QT_NO_SHORTCUT

        retranslateUi(RibbonBarCustomizePage);

        QMetaObject::connectSlotsByName(RibbonBarCustomizePage);
    } // setupUi

    void retranslateUi(QWidget *RibbonBarCustomizePage)
    {
        RibbonBarCustomizePage->setWindowTitle(QApplication::translate("RibbonBarCustomizePage", "Customize Ribbon", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        RibbonBarCustomizePage->setStatusTip(QApplication::translate("RibbonBarCustomizePage", "Customize the Ribbon", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        labelCommandsFrom->setText(QApplication::translate("RibbonBarCustomizePage", "&Choose commands from:", Q_NULLPTR));
        addButton->setText(QApplication::translate("RibbonBarCustomizePage", "&Add > >", Q_NULLPTR));
        removeButton->setText(QApplication::translate("RibbonBarCustomizePage", "< < &Remove", Q_NULLPTR));
        labelCustomizeRibbon->setText(QApplication::translate("RibbonBarCustomizePage", "Customize the Ri&bbon:", Q_NULLPTR));
        newTabButton->setText(QApplication::translate("RibbonBarCustomizePage", "Ne&w Tab", Q_NULLPTR));
        newGroupButton->setText(QApplication::translate("RibbonBarCustomizePage", "&New Group", Q_NULLPTR));
        renameButton->setText(QApplication::translate("RibbonBarCustomizePage", "Rena&me...", Q_NULLPTR));
        labelCustomizations->setText(QApplication::translate("RibbonBarCustomizePage", "Customizations:", Q_NULLPTR));
        resetButton->setText(QApplication::translate("RibbonBarCustomizePage", "Re&set", Q_NULLPTR));
        upButton->setText(QString());
        downButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RibbonBarCustomizePage: public Ui_RibbonBarCustomizePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTNRIBBONBARCUSTOMIZEPAGE_H
