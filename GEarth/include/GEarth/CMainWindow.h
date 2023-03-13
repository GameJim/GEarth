#pragma once
#include <map>
#include "QtitanRibbon.h"

#include "GEarth/CMdiArea.h"
#include "GEarth/earth_viewer_project.h"
namespace osgEarth
{
	class Map;
	class MapNode;
}

namespace ui
{
	using namespace Qtitan;
	class EARTH_API CMainWindow : public RibbonMainWindow {
		Q_OBJECT
	public:
		CMainWindow(QWidget * parent = Q_NULLPTR);
		~CMainWindow();

	protected:
		virtual void InitEarth();
	protected:
		//createPage
		void createPages();
		void createPageFile();
		void createPageData();
		void createPageWinodws();
		void createPageToos();
		void createPagePlugin();
		void createPageSetting();

	protected: 
		//创建一个空的Map
		//osg::ref_ptr<osgEarth::Map> createDefaultMap(const std::string& sName = "default");
		private slots:
		void openMap();
	protected:

	private:
		Qtitan::RibbonStyle* m_pRibbonStyle;
		//ui 资源
		CMdiArea* m_pMidArea;

		//数据资源
		std::unique_ptr<EarthViewerProject> m_pEarthViewerProject;
	};
}

