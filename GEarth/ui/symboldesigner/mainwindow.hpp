#pragma once
#include "QtitanRibbon.h"

#include "SymbolDesigner/dockwidget.hpp"
#include "SymbolDesigner/mdiarea.hpp"
using namespace Qtitan;
class CMainWindow : public RibbonMainWindow {
	Q_OBJECT

public:
	CMainWindow(QWidget * parent = Q_NULLPTR);
	~CMainWindow();

protected:
	void CreateSymbolLibraryPage(RibbonPage* pPage);
	void CreateWindowsPage(RibbonPage* pPage);

	void CreateOptions();

	protected slots:
	//新建符号库
	void CreateSymbolLibary();
	//打开符号库
	void OpenSymbolLibary();
private:

	Qtitan::RibbonStyle* m_pRibbonStyle;
	CDockWidget* m_pLeftDockWidget;// 左侧停靠窗口
	CDockWidget* m_pRightDockWidget;// 右侧停靠窗口
	CMdiArea* m_pMidArea;         //所有符号
	
};
