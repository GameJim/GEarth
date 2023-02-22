#pragma once
#include "QtitanRibbon.h"
#include "GEarth/api_description.h"


using namespace Qtitan;
class PUBLIC_API CMainWindow : public RibbonMainWindow {
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

};
