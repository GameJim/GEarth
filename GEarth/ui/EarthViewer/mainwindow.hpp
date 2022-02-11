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
    void AddVector(osg::ref_ptr<osgEarth::Map> map);
    CMainWindow(QWidget * parent = Q_NULLPTR);
	~CMainWindow();

protected:
    CDockWidget* GetOrCreateDockWidget(const QString& sName);

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

    std::map<QString, CDockWidget*>  m_pDockWidget;
	
	CMdiArea* m_pMidArea;         //所有符号
	
    earth::CRefPtr<earth::CGroupNode> m_pData;           //所有数据
};
