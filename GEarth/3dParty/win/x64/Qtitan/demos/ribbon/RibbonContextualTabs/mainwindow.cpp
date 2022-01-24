#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

#include "aboutdialog.h"


/* MainWindow */
MainWindow::MainWindow(QWidget* parent)
    : RibbonWindow(parent)
{
    RibbonWorkspace* workspace = new RibbonWorkspace;
    setCentralWidget(workspace);
    createRibbon();
    createStatusBar();

    move(QPoint(200, 200));
    QRect geom = QApplication::desktop()->availableGeometry();
    resize(2 * geom.width() / 3, 2 * geom.height() / 3);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createRibbon()
{
    RibbonPage* page = ribbonBar()->addPage(tr("Page"));
    RibbonGroup* group = page->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button"), Qt::ToolButtonTextUnderIcon);

    QString strContextTitle(tr("Contextual Group"));
    QString strContextGroupName("id_contextual_group");
    RibbonPage* contextPage1 = ribbonBar()->addPage(tr("Page 1"));
    group = contextPage1->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button 1"), Qt::ToolButtonTextUnderIcon);

    contextPage1->setContextColor(RibbonPage::ContextColorRed);
    contextPage1->setContextTitle(strContextTitle);
    contextPage1->setContextGroupName(strContextGroupName);

    RibbonPage* contextPage2 = ribbonBar()->addPage(tr("Page 2"));
    group = contextPage2->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button 2"), Qt::ToolButtonTextUnderIcon);
    contextPage2->setContextColor(RibbonPage::ContextColorRed);
    contextPage2->setContextTitle(strContextTitle);
    contextPage2->setContextGroupName(strContextGroupName);

    RibbonPage* contextPage3 = ribbonBar()->addPage(tr("Page 3"));
    group = contextPage3->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button 3"), Qt::ToolButtonTextUnderIcon);
    contextPage3->setContextColor(RibbonPage::ContextColorRed);
    contextPage3->setContextTitle(strContextTitle);
    contextPage3->setContextGroupName(strContextGroupName);

    page = ribbonBar()->addPage(tr("Page"));
    group = page->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button"), Qt::ToolButtonTextUnderIcon);

    RibbonPage* context = ribbonBar()->addPage(tr("Page"));
    group = context->addGroup(tr("Group"));
    group->addAction(QIcon(":/shared/res/qtitanlogo32x32.png"), tr("Button"), Qt::ToolButtonTextUnderIcon);

    context->setContextColor(RibbonPage::ContextColorBlue);
    context->setContextTitle(tr("Contextual"));

    ribbonBar()->setFrameThemeEnabled();
}

void MainWindow::createStatusBar()
{
    RibbonSliderPane* sliderPane = new RibbonSliderPane();
    sliderPane->setScrollButtons(true);
    sliderPane->setRange(0, 100);
    sliderPane->setMaximumWidth(130);
    sliderPane->setSingleStep(10);
    sliderPane->setSliderPosition(50);
    statusBar()->addPermanentWidget(sliderPane);
    statusBar();
}
