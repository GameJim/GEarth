/********************************************************************************
** Form generated from reading UI file 'OfficeStylePopupDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OFFICESTYLEPOPUPDIALOG_H
#define UI_OFFICESTYLEPOPUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PopupDemoDlg
{
public:
    QLabel *label_2;
    QLabel *transparencyLabel;
    QSlider *transparencylSlider;
    QComboBox *animationTypeBox;
    QLabel *label_4;
    QSpinBox *animationSpeedBox;
    QLabel *label_5;
    QSpinBox *displayTimeBox;
    QGroupBox *allowMoveBox;
    QPushButton *showPopupButton;
    QLabel *label;
    QComboBox *lookWindow;

    void setupUi(QWidget *PopupDemoDlg)
    {
        if (PopupDemoDlg->objectName().isEmpty())
            PopupDemoDlg->setObjectName(QStringLiteral("PopupDemoDlg"));
        PopupDemoDlg->resize(340, 260);
        PopupDemoDlg->setMinimumSize(QSize(340, 260));
        PopupDemoDlg->setMaximumSize(QSize(340, 260));
        label_2 = new QLabel(PopupDemoDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(16, 54, 80, 16));
        transparencyLabel = new QLabel(PopupDemoDlg);
        transparencyLabel->setObjectName(QStringLiteral("transparencyLabel"));
        transparencyLabel->setGeometry(QRect(16, 166, 131, 16));
        transparencylSlider = new QSlider(PopupDemoDlg);
        transparencylSlider->setObjectName(QStringLiteral("transparencylSlider"));
        transparencylSlider->setGeometry(QRect(196, 166, 131, 20));
        transparencylSlider->setMaximum(255);
        transparencylSlider->setSliderPosition(255);
        transparencylSlider->setOrientation(Qt::Horizontal);
        animationTypeBox = new QComboBox(PopupDemoDlg);
        animationTypeBox->setObjectName(QStringLiteral("animationTypeBox"));
        animationTypeBox->setGeometry(QRect(196, 52, 131, 20));
        label_4 = new QLabel(PopupDemoDlg);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(16, 94, 151, 16));
        animationSpeedBox = new QSpinBox(PopupDemoDlg);
        animationSpeedBox->setObjectName(QStringLiteral("animationSpeedBox"));
        animationSpeedBox->setGeometry(QRect(196, 88, 131, 22));
        animationSpeedBox->setMaximum(600000);
        animationSpeedBox->setSingleStep(100);
        label_5 = new QLabel(PopupDemoDlg);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(16, 132, 173, 17));
        displayTimeBox = new QSpinBox(PopupDemoDlg);
        displayTimeBox->setObjectName(QStringLiteral("displayTimeBox"));
        displayTimeBox->setGeometry(QRect(196, 128, 131, 22));
        displayTimeBox->setMinimum(-1);
        displayTimeBox->setMaximum(600000);
        displayTimeBox->setSingleStep(100);
        allowMoveBox = new QGroupBox(PopupDemoDlg);
        allowMoveBox->setObjectName(QStringLiteral("allowMoveBox"));
        allowMoveBox->setGeometry(QRect(16, 200, 309, 16));
        allowMoveBox->setFlat(true);
        allowMoveBox->setCheckable(true);
        allowMoveBox->setChecked(false);
        showPopupButton = new QPushButton(PopupDemoDlg);
        showPopupButton->setObjectName(QStringLiteral("showPopupButton"));
        showPopupButton->setGeometry(QRect(112, 232, 111, 23));
        label = new QLabel(PopupDemoDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(16, 20, 121, 16));
        lookWindow = new QComboBox(PopupDemoDlg);
        lookWindow->setObjectName(QStringLiteral("lookWindow"));
        lookWindow->setGeometry(QRect(196, 16, 131, 22));

        retranslateUi(PopupDemoDlg);

        QMetaObject::connectSlotsByName(PopupDemoDlg);
    } // setupUi

    void retranslateUi(QWidget *PopupDemoDlg)
    {
        PopupDemoDlg->setWindowTitle(QApplication::translate("PopupDemoDlg", "Qtitan Popup Widget Demo", Q_NULLPTR));
        label_2->setText(QApplication::translate("PopupDemoDlg", "Animation Type", Q_NULLPTR));
        transparencyLabel->setText(QApplication::translate("PopupDemoDlg", "Transparency:", Q_NULLPTR));
        label_4->setText(QApplication::translate("PopupDemoDlg", "Animation Speed (millisec)", Q_NULLPTR));
        label_5->setText(QApplication::translate("PopupDemoDlg", "DisplayTime (-1 to 600000 millisec)", Q_NULLPTR));
        allowMoveBox->setTitle(QApplication::translate("PopupDemoDlg", "&Moveable Widget", Q_NULLPTR));
        showPopupButton->setText(QApplication::translate("PopupDemoDlg", "Show Popup", Q_NULLPTR));
        label->setText(QApplication::translate("PopupDemoDlg", "Popup Window Look", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PopupDemoDlg: public Ui_PopupDemoDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OFFICESTYLEPOPUPDIALOG_H
