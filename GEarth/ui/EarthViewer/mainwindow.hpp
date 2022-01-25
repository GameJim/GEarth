#pragma once
#include "QtitanRibbon.h"

#include "EarthViewer/dockwidget.hpp"
#include "EarthViewer/mdiarea.hpp"
using namespace Qtitan;

#include "EarthCore/map.h"
#include "EarthCore/type.h"
class CMainWindow : public RibbonMainWindow {
	Q_OBJECT

public:
	CMainWindow(QWidget * parent = Q_NULLPTR);
	~CMainWindow();

protected:
    //用于测试的案例
    void CreateDEMOPage(RibbonPage* pPage);
    

	void CreateSymbolLibraryPage(RibbonPage* pPage);
	void CreateWindowsPage(RibbonPage* pPage);
    

	void CreateOptions();

	protected slots:
	//新建符号库
	void CreateSymbolLibary();
	//打开符号库
	void OpenSymbolLibary();




    //demo
    void CreateMapWindow();
private:

	Qtitan::RibbonStyle* m_pRibbonStyle;
	CDockWidget* m_pLeftDockWidget;// 左侧停靠窗口
	CDockWidget* m_pRightDockWidget;// 右侧停靠窗口
	CMdiArea* m_pMidArea;         //所有符号
	
    std::vector <earth::CRefPtr<earth::CMap>> m_Maps;

};
