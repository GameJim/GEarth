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
	void CreateFilePage(RibbonPage* pPage);
	void CreateViewPage(RibbonPage* pPage);
    

    //用于测试的案例
    void CreateTestPage(RibbonPage* pPage);
    void CreateDemoPage(RibbonPage* pPage);

	void CreateOptions();

	protected slots:
	//新建符号库
	void CreateScence();
	//打开符号库
	void OpenScence();

    void CreateEmptyPage(RibbonPage* pPage);

    //demo
    void CreateMapWindow();

protected slots:
    void TestMulitView();
private:

	Qtitan::RibbonStyle* m_pRibbonStyle;
	CDockWidget* m_pLeftDockWidget;// 左侧停靠窗口
	CDockWidget* m_pRightDockWidget;// 右侧停靠窗口
	CMdiArea* m_pMidArea;         //所有符号
	
    std::vector <earth::CRefPtr<earth::CMap>> m_Maps;

};
