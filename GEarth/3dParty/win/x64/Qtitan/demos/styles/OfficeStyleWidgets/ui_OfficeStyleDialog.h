/********************************************************************************
** Form generated from reading UI file 'OfficeStyleDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OFFICESTYLEDIALOG_H
#define UI_OFFICESTYLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StyleDialog
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QGroupBox *groupBox_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QGroupBox *groupBox_3;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QComboBox *comboBox_2;
    QLabel *label_3;
    QTreeWidget *treeWidget;
    QLabel *label_4;
    QLabel *label_5;
    QListWidget *listWidget;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QWidget *tab_5;
    QWidget *widget;
    QSlider *horizontalSlider;
    QLabel *label_7;
    QLabel *label_8;
    QScrollBar *horizontalScrollBar;
    QLabel *label_9;
    QSlider *horizontalSlider_2;
    QScrollBar *horizontalScrollBar_2;
    QTableWidget *tableWidget;
    QSlider *horizontalSlider_3;
    QScrollBar *horizontalScrollBar_3;
    QWidget *tab_2;

    void setupUi(QDialog *StyleDialog)
    {
        if (StyleDialog->objectName().isEmpty())
            StyleDialog->setObjectName(QStringLiteral("StyleDialog"));
        StyleDialog->setEnabled(true);
        StyleDialog->resize(527, 420);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StyleDialog->sizePolicy().hasHeightForWidth());
        StyleDialog->setSizePolicy(sizePolicy);
        tabWidget = new QTabWidget(StyleDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 511, 401));
        tabWidget->setMaximumSize(QSize(511, 401));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 151, 131));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 20, 111, 23));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 56, 111, 23));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(false);
        pushButton_3->setGeometry(QRect(20, 90, 111, 23));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(170, 10, 91, 131));
        radioButton = new QRadioButton(groupBox_2);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(15, 28, 64, 17));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(groupBox_2);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(15, 56, 56, 17));
        radioButton_3 = new QRadioButton(groupBox_2);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));
        radioButton_3->setEnabled(false);
        radioButton_3->setGeometry(QRect(15, 84, 63, 17));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(270, 10, 91, 131));
        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 28, 64, 17));
        checkBox->setChecked(true);
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(10, 56, 56, 17));
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setEnabled(false);
        checkBox_3->setGeometry(QRect(10, 84, 63, 17));
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(370, 28, 131, 20));
        comboBox->setEditable(true);
        comboBox->setFrame(true);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(370, 10, 113, 16));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(370, 78, 117, 16));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(370, 96, 133, 20));
        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setEnabled(false);
        lineEdit_2->setGeometry(QRect(370, 121, 133, 20));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setEnabled(false);
        comboBox_2->setGeometry(QRect(370, 53, 131, 20));
        comboBox_2->setEditable(false);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 148, 81, 16));
        treeWidget = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(16, 168, 117, 110));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(140, 148, 91, 16));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(384, 148, 77, 16));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(380, 168, 117, 110));
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(16, 300, 185, 69));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        tabWidget_2->addTab(tab_3, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        tabWidget_2->addTab(tab_5, QString());
        widget = new QWidget();
        widget->setObjectName(QStringLiteral("widget"));
        tabWidget_2->addTab(widget, QString());
        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(212, 299, 130, 19));
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 280, 73, 16));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(216, 280, 105, 16));
        horizontalScrollBar = new QScrollBar(tab);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setGeometry(QRect(360, 300, 140, 16));
        horizontalScrollBar->setMaximum(3);
        horizontalScrollBar->setSingleStep(0);
        horizontalScrollBar->setPageStep(10);
        horizontalScrollBar->setSliderPosition(0);
        horizontalScrollBar->setTracking(true);
        horizontalScrollBar->setOrientation(Qt::Horizontal);
        horizontalScrollBar->setInvertedControls(true);
        label_9 = new QLabel(tab);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(364, 280, 97, 16));
        horizontalSlider_2 = new QSlider(tab);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(212, 324, 130, 21));
        horizontalSlider_2->setValue(38);
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        horizontalSlider_2->setInvertedAppearance(false);
        horizontalSlider_2->setTickPosition(QSlider::TicksAbove);
        horizontalScrollBar_2 = new QScrollBar(tab);
        horizontalScrollBar_2->setObjectName(QStringLiteral("horizontalScrollBar_2"));
        horizontalScrollBar_2->setGeometry(QRect(360, 320, 140, 28));
        horizontalScrollBar_2->setMaximum(10);
        horizontalScrollBar_2->setOrientation(Qt::Horizontal);
        tableWidget = new QTableWidget(tab);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(136, 168, 241, 110));
        horizontalSlider_3 = new QSlider(tab);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(212, 348, 130, 19));
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        horizontalSlider_3->setTickPosition(QSlider::TicksBelow);
        horizontalScrollBar_3 = new QScrollBar(tab);
        horizontalScrollBar_3->setObjectName(QStringLiteral("horizontalScrollBar_3"));
        horizontalScrollBar_3->setGeometry(QRect(360, 352, 140, 16));
        horizontalScrollBar_3->setOrientation(Qt::Horizontal);
        horizontalScrollBar_3->setInvertedAppearance(true);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(StyleDialog);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StyleDialog);
    } // setupUi

    void retranslateUi(QDialog *StyleDialog)
    {
        StyleDialog->setWindowTitle(QApplication::translate("StyleDialog", "Style", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("StyleDialog", "Push Buttons", Q_NULLPTR));
        pushButton->setText(QApplication::translate("StyleDialog", "Default", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("StyleDialog", "Normal", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("StyleDialog", "Disabled", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("StyleDialog", "Radio Buttons", Q_NULLPTR));
        radioButton->setText(QApplication::translate("StyleDialog", "Selected", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("StyleDialog", "Normal", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("StyleDialog", "Disabled", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("StyleDialog", "GroupBox", Q_NULLPTR));
        checkBox->setText(QApplication::translate("StyleDialog", "Selected", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("StyleDialog", "Normal", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("StyleDialog", "Disabled", Q_NULLPTR));
        label->setText(QApplication::translate("StyleDialog", "Combo Box:", Q_NULLPTR));
        label_2->setText(QApplication::translate("StyleDialog", "Line Edit:", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("StyleDialog", "Disabled", Q_NULLPTR));
        label_3->setText(QApplication::translate("StyleDialog", "Tree Widget:", Q_NULLPTR));
        label_4->setText(QApplication::translate("StyleDialog", "Table Widget:", Q_NULLPTR));
        label_5->setText(QApplication::translate("StyleDialog", "List Widget:", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("StyleDialog", "Tab 1", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("StyleDialog", "Tab 2", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(widget), QApplication::translate("StyleDialog", "Tab 3", Q_NULLPTR));
        label_7->setText(QApplication::translate("StyleDialog", "Tab Widget:", Q_NULLPTR));
        label_8->setText(QApplication::translate("StyleDialog", "Slider:", Q_NULLPTR));
        label_9->setText(QApplication::translate("StyleDialog", "Scroll Bar:", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("StyleDialog", "Widgets", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("StyleDialog", "Style and Themes", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StyleDialog: public Ui_StyleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OFFICESTYLEDIALOG_H
