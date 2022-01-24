#include "mainwindow.hpp"
#include "EarthCore/map.h"

#include <QFileDialog>
CMainWindow::CMainWindow(QWidget * parent) : RibbonMainWindow(parent) {
	RibbonToolTip::setWrapMode(RibbonToolTip::NativeWrap);
	CreateOptions();
	
	RibbonPage* page = ribbonBar()->addPage(tr("&Home"));
	page->setContextColor(QTITAN_NAMESPACE::RibbonPage::ContextColorOrange);
	CreateSymbolLibraryPage(page);

	RibbonPage* pWindows = ribbonBar()->addPage(tr("Windows"));
	CreateWindowsPage(pWindows);

	ribbonBar()->setTitleBarVisible(false);

	//添加菜单
	m_pLeftDockWidget = new CDockWidget(this);
	this->addDockWidget(Qt::LeftDockWidgetArea, m_pLeftDockWidget);
	m_pLeftDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);//设置可停靠区域为主窗口左边和右边
	m_pLeftDockWidget->hide();

	m_pRightDockWidget = new CDockWidget(this);
	this->addDockWidget(Qt::RightDockWidgetArea, m_pRightDockWidget);
	m_pRightDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);//设置可停靠区域为主窗口左边和右边
	m_pRightDockWidget->hide();

	m_pMidArea = new CMdiArea();
	this->setCentralWidget(m_pMidArea);
}

CMainWindow::~CMainWindow() {
	
}


void CMainWindow::CreateSymbolLibraryPage(RibbonPage* pPage)
{
	RibbonGroup* group = pPage->addGroup(tr("Windows States"));

	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
	//打开，绑定事件
	QAction* pAction =toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("New"), Qt::ToolButtonTextUnderIcon);
	connect(pAction, SIGNAL(triggered()), this, SLOT(CreateSymbolLibary()));


	pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/OpenFile.png")), tr("Open"), Qt::ToolButtonTextUnderIcon);
	connect(pAction, SIGNAL(triggered()), this, SLOT(OpenSymbolLibary()));

	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Save.png")), tr("Save"), Qt::ToolButtonTextUnderIcon);
	toolBar->addSeparator();
	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Share.png")), tr("Share"), Qt::ToolButtonTextUnderIcon);
	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("Setiting"), Qt::ToolButtonTextUnderIcon);
	group->addControl(toolBar);
}

void CMainWindow::CreateWindowsPage(RibbonPage* pPage)
{
	RibbonGroup* group = pPage->addGroup(tr("Symobl Library"));

	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
	/*toolBar->addAction(QIcon(QStringLiteral("./res/NewFile.png")), tr("New"), Qt::ToolButtonTextUnderIcon);
	toolBar->addAction(QIcon(QStringLiteral("./res/OpenFile.png")), tr("Open"), Qt::ToolButtonTextUnderIcon);
	toolBar->addAction(QIcon(QStringLiteral("./res/Save.png")), tr("Save"), Qt::ToolButtonTextUnderIcon);*/

	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Share.png")), tr("Share"), Qt::ToolButtonTextUnderIcon);
	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("Setiting"), Qt::ToolButtonTextUnderIcon);
	toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("Setiting2"), Qt::ToolButtonTextUnderIcon);
	group->addControl(toolBar);
}

void CMainWindow::CreateOptions()
{

}

void CMainWindow::CreateSymbolLibary()
{
    earth::CMap* pMap = new earth::CMap();
}

void CMainWindow::OpenSymbolLibary()
{
   
}
