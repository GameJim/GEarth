#pragma execution_character_set("utf-8") 
#include "GEarth/CMainWindow.h"
#include <QFileDialog>
#include <QDesktopWidget>




#include <osgViewer/ViewerEventHandlers>
#include <osgDB/WriteFile>
#include <osgEarth/MapNode>
#include <osgEarth/GDAL>
#include <osgEarth/ExampleResources>
#include <osgEarth/EarthManipulator>

#include <osgEarth/Style>
#include <osgEarth/OGRFeatureSource>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/FeatureImageLayer>

#include "GEarth/object_name.h"
#include "GEarth/earth_cfg.h"
#include "GEarth/image_cfg.h"
#include "GEarth/CMapFileWidget.h"
#include "const.h"

#include "util/file_system.h"
#include "util/log_system.h"
#include "util/dynamic_library.h"
using namespace osg;
using namespace osgEarth;
using namespace osgEarth::Util;
namespace ui
{
	// inline CSS, for use with the --script option
	const char* styles_css =
		R"(
    p {
        altitude-clamping: terrain-drape;
        render-backface-culling: false;
    }
    p1: p{ fill: #ff3f3f9f; }
    p2: p{ fill: #3fff3f9f; }
    p3: p{ fill: #3f3fff9f; }
    p4: p{ fill: #ff3fff9f; }
    p5: p{ fill: #ffff3f9f; }
)";

	// JavaScript style selector, for use with the --script option
	const char* script_source =
		R"(
    function getStyleClass()
    {
        // Exclude any countries beginning with the letter A: 
        if ( feature.properties.name.charAt(0) === 'A' )
            return null;
                        
        // If it starts with the letter C, return an inline style:
        if ( feature.properties.name.charAt(0) == 'C' )
            return '{ _fill: #ffc838; stroke: #8f8838; extrusion-height: 250000; }';
                        
        // Otherwise, return a named style based on some calculations:
        var pop = parseFloat(feature.properties.pop);
        if      ( pop <= 14045470 )  return "p1";
        else if ( pop <= 43410900 )  return "p2";
        else if ( pop <= 97228750 )  return "p3";
        else if ( pop <= 258833000 ) return "p4";
        else                         return "p5";
    }
)";



	CMainWindow::CMainWindow(QWidget * parent) : RibbonMainWindow(parent) {
		
		this->InitEarth();
		
		//设置相关布局
		RibbonToolTip::setWrapMode(RibbonToolTip::NativeWrap);
		ribbonBar()->setTitleBarVisible(false);
		//大小
		resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

		//布局，从上向下，从作至有
		this->createPages();

		//中间多窗口
		m_pMidArea = new CMdiArea();
		m_pMidArea->setTabsMovable(true);
		m_pMidArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_pMidArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		this->setCentralWidget(m_pMidArea);


		//创建一个位于中间的球
		//m_pMidArea->createMdiSubWindow(createDefaultMap());

		//创建树控件、绑定callbck
	}

	CMainWindow::~CMainWindow() {

	}


	void CMainWindow::InitEarth()
	{
		//获取系统路径
		g_sModulePath = util::FileSystem::GetModulePath();

		//读取配置
		//EarthCfg::instance().loadCfg(g_sModulePath +"/../cfg/earth.cfg");
		//读取插件，加载插件
		auto plugins = EarthCfg::instance().getPluginsSetting();
		auto registerDynamicLibrary = [&](const EarthCfg::PluginsSetting& pluginSetting)
		{
			std::string name = pluginSetting.path() + pluginSetting.suffix() + ".dll";
			std::string path = pluginSetting.path_root() == "modulePath" ? g_sModulePath + name : name;
			util::DynamicLibraryManager().instance().registerDynamicLibrary(pluginSetting.name(), path);
			if (pluginSetting.active())
				util::DynamicLibraryManager().instance().activeDynamicLibrary(pluginSetting.name());
		};
		for (int i = 0; i < plugins.size();i++)
		{
			registerDynamicLibrary(plugins[i]);
		}

		//读取image配置
		ImageCfg::instance().praseCfg();

		//初始化Earth 环境，读取配置信息等
		osgEarth::initialize();

		//初始化工程
		m_pEarthViewerProject = std::make_unique<EarthViewerProject>();
		LOG_INFO << "环境初始化完成" << std::endl;
		
	}

	void CMainWindow::createPages()
	{
		this->createPageFile();
		this->createPageData();
		this->createPageWinodws();
		this->createPageToos();
		this->createPagePlugin();
		this->createPageSetting();
	}

	void CMainWindow::createPageFile()
	{
		RibbonPage* pPage = ribbonBar()->addPage(tr("文件(&F)"));
		pPage->setObjectName(_file_page);

		//先增加功能
		RibbonGroup* pProjectGroup = pPage->addGroup(tr("工程"));
		RibbonToolBarControl* projectBar = new RibbonToolBarControl();
		QAction* pAction = projectBar->addAction(ImageCfg::instance().getIcon("new_project"), tr("新建"), Qt::ToolButtonTextUnderIcon);
		pAction = projectBar->addAction(ImageCfg::instance().getIcon("open_project"), tr("打开"), Qt::ToolButtonTextUnderIcon);
		pAction = projectBar->addAction(ImageCfg::instance().getIcon("save_project"), tr("保存"), Qt::ToolButtonTextUnderIcon);
		pProjectGroup->addControl(projectBar);
		
		//地图
		RibbonGroup* pMapGroup = pPage->addGroup(tr("地图"));
		RibbonToolBarControl* toolBar = new RibbonToolBarControl();
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("new_map"), tr("新建"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("opem_map"), tr("打开"), Qt::ToolButtonTextUnderIcon);
		connect(pAction, SIGNAL(triggered()), this, SLOT(openMap()));

		pAction = toolBar->addAction(ImageCfg::instance().getIcon("save_map"), tr("保存"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("saveas_map"), tr("另存"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("output_map"), tr("出图"), Qt::ToolButtonTextUnderIcon);
		//pAction = toolBar->addAction(ImageCfg::instance().getIcon("setting"), tr("设置"), Qt::ToolButtonTextUnderIcon);
		pMapGroup->addControl(toolBar);
	}

	void CMainWindow::createPageData()
	{
		RibbonPage* pPage = ribbonBar()->addPage(tr("数据(&D)"));
		pPage->setObjectName(_data_page);
		

		RibbonGroup* pMapGroup = pPage->addGroup(tr("网络地图"));
		RibbonToolBarControl* toolBar = new RibbonToolBarControl();
		QAction* pAction = toolBar->addAction(ImageCfg::instance().getIcon("baidu_map"), tr("百度地图"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("gaode_map"), tr("高德地图"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("mapbox_map"), tr("Maobox"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("osm_map"), tr("OSM"), Qt::ToolButtonTextUnderIcon);
		pMapGroup->addControl(toolBar);


		RibbonGroup* pBaseDataGroup = pPage->addGroup(tr("基础数据"));
		RibbonToolBarControl* pBaseDataToolBar = new RibbonToolBarControl();
		pAction = pBaseDataToolBar->addAction(ImageCfg::instance().getIcon("vector_data"), tr("矢量数据"), Qt::ToolButtonTextUnderIcon);
		pAction = pBaseDataToolBar->addAction(ImageCfg::instance().getIcon("raster_data"), tr("栅格数据"), Qt::ToolButtonTextUnderIcon);
		pAction = pBaseDataToolBar->addAction(ImageCfg::instance().getIcon("annotation_data"), tr("注释数据"), Qt::ToolButtonTextUnderIcon);
		pBaseDataGroup->addControl(pBaseDataToolBar);


		RibbonGroup* pWebDataGroup = pPage->addGroup(tr("网络数据"));
		RibbonToolBarControl* ppWebDataToolBar = new RibbonToolBarControl();
		pAction = ppWebDataToolBar->addAction(ImageCfg::instance().getIcon("arcgis_data"), tr("ArcGIS Server"), Qt::ToolButtonTextUnderIcon);
		pAction = ppWebDataToolBar->addAction(ImageCfg::instance().getIcon("tms_data"), tr("TMS"), Qt::ToolButtonTextUnderIcon);
		pAction = ppWebDataToolBar->addAction(ImageCfg::instance().getIcon("wms_data"), tr("WMS"), Qt::ToolButtonTextUnderIcon);
		pAction = ppWebDataToolBar->addAction(ImageCfg::instance().getIcon("xyz_data"), tr("XYZ"), Qt::ToolButtonTextUnderIcon);
		pWebDataGroup->addControl(ppWebDataToolBar);


		RibbonGroup* pEnvDataGroup = pPage->addGroup(tr("环境数据"));
		RibbonToolBarControl* pEnvDataToolBar = new RibbonToolBarControl();
		pAction = pEnvDataToolBar->addAction(ImageCfg::instance().getIcon("sky_data"), tr("天空"), Qt::ToolButtonTextUnderIcon);
		pAction = pEnvDataToolBar->addAction(ImageCfg::instance().getIcon("ocean_data"), tr("海洋"), Qt::ToolButtonTextUnderIcon);
		pEnvDataGroup->addControl(pEnvDataToolBar);


		RibbonGroup* pModelDataGroup = pPage->addGroup(tr("模型数据"));
		RibbonToolBarControl* ppOtherDataToolBar = new RibbonToolBarControl();
		pAction = ppOtherDataToolBar->addAction(ImageCfg::instance().getIcon("model_data"), tr("模型"), Qt::ToolButtonTextUnderIcon);
		pAction = ppOtherDataToolBar->addAction(ImageCfg::instance().getIcon("tdtiles_data"), tr("3dTiles"), Qt::ToolButtonTextUnderIcon);
		pAction = ppOtherDataToolBar->addAction(ImageCfg::instance().getIcon("video_data"), tr("视频"), Qt::ToolButtonTextUnderIcon);
		pModelDataGroup->addControl(ppOtherDataToolBar);
		
	}

	void CMainWindow::createPageWinodws()
	{
		RibbonPage* pPage = ribbonBar()->addPage(tr("窗口(&W)"));
		
		RibbonGroup* pBaseGroup = pPage->addGroup(tr("基础窗口"));
		RibbonToolBarControl* toolBar = new RibbonToolBarControl();
		QAction* pAction = toolBar->addAction(ImageCfg::instance().getIcon("layer_tree_window"), tr("图层树图"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("data_tree_window"), tr("数据仓图"), Qt::ToolButtonTextUnderIcon);
		pAction = toolBar->addAction(ImageCfg::instance().getIcon("slected_feature_window"), tr("选中要素集"), Qt::ToolButtonTextUnderIcon);
		pBaseGroup->addControl(toolBar);


		RibbonGroup* pWindowGroup = pPage->addGroup(tr("窗口排列"));
		RibbonToolBarControl* pWindowGrouptoolBar = new RibbonToolBarControl();
		pAction = pWindowGrouptoolBar->addAction(ImageCfg::instance().getIcon("pingpu_window"), tr("平铺"), Qt::ToolButtonTextUnderIcon);
		pAction = pWindowGrouptoolBar->addAction(ImageCfg::instance().getIcon("zhedie_window"), tr("折叠"), Qt::ToolButtonTextUnderIcon);
		pAction = pWindowGrouptoolBar->addAction(ImageCfg::instance().getIcon("close_al_window"), tr("关闭"), Qt::ToolButtonTextUnderIcon);
		pWindowGroup->addControl(pWindowGrouptoolBar);

	}

	void CMainWindow::createPageToos()
	{
		RibbonPage* page = ribbonBar()->addPage(tr("工具(&T)"));
		//page->setContextColor(QTITAN_NAMESPACE::RibbonPage::ContextColorOrange);
	}

	void CMainWindow::createPagePlugin()
	{
		RibbonPage* page = ribbonBar()->addPage(tr("插件(&P)"));
	
	}

	void CMainWindow::createPageSetting()
	{
		RibbonPage* page = ribbonBar()->addPage(tr("设置(&S)"));
		
	}


	//osg::ref_ptr<osgEarth::Map> CMainWindow::createDefaultMap(const std::string& sName /*= "default"*/)
	//{
	//	osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map();
	//	map->setMapName(sName);
	//	m_pMapNodes.insert({ sName,map });

	//	// Start with a basemap imagery layer; we'll be using the GDAL driver
	//	// to load a local GeoTIFF file:
	//	GDALImageLayer* basemap = new GDALImageLayer();
	//	basemap->setURL(g_sModulePath + "/../data/world.tif");
	//	map->addLayer(basemap);

	//	// Next we add a layer to provide the feature data.
	//	OGRFeatureSource* features = new OGRFeatureSource();
	//	features->setName("vector-data");

	//	bool useRaster = true;
	//	bool useMem = false;
	//	bool useLabels = false;
	//	bool useDraping = false;
	//	bool useClamping = false;
	//	bool useScript = false;

	//	if (useMem)
	//	{
	//		// the --mem options tells us to just make an in-memory geometry:
	//		Ring* line = new Ring();
	//		line->push_back(osg::Vec3d(-60, 20, 0));
	//		line->push_back(osg::Vec3d(-120, 20, 0));
	//		line->push_back(osg::Vec3d(-120, 60, 0));
	//		line->push_back(osg::Vec3d(-60, 60, 0));
	//		features->setGeometry(line);
	//	}
	//	else
	//	{
	//		features->setURL(g_sModulePath + "/../data/world.shp");
	//	}
	//	map->addLayer(features);

	//	// Define a style for the feature data. Since we are going to render the
	//	// vectors as lines, configure the line symbolizer:
	//	Style style;

	//	LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
	//	ls->stroke()->color() = Color::Yellow;
	//	ls->stroke()->width() = 2.0f;
	//	ls->tessellationSize()->set(100, Units::KILOMETERS);

	//	if (useDraping)
	//	{
	//		AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
	//		alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//		alt->technique() = alt->TECHNIQUE_DRAPE;
	//	}

	//	else if (useClamping)
	//	{
	//		AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
	//		alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//		alt->technique() = alt->TECHNIQUE_GPU;

	//		ls->tessellationSize()->set(100, Units::KILOMETERS);

	//		RenderSymbol* render = style.getOrCreate<RenderSymbol>();
	//		render->depthOffset()->enabled() = true;
	//	}

	//	if (useRaster)
	//	{
	//		FeatureImageLayer* layer = new FeatureImageLayer();
	//		
	//		layer->setFeatureSource(features);
	//		StyleSheet* sheet = new StyleSheet();
	//		sheet->addStyle(style);
	//		layer->setStyleSheet(sheet);
	//		map->addLayer(layer);
	//	}

	//	else
	//	{
	//		FeatureModelLayer* layer = new FeatureModelLayer();
	//		layer->setFeatureSource(features);

	//		StyleSheet* styleSheet = new StyleSheet();

	//		if (useScript)
	//		{
	//			styleSheet->addStylesFromCSS(styles_css);
	//			styleSheet->setScript(new StyleSheet::ScriptDef(script_source));
	//			styleSheet->addSelector(StyleSelector("default", StringExpression("getStyleClass()")));
	//		}
	//		else
	//		{
	//			styleSheet->addStyle(style);
	//		}

	//		layer->setStyleSheet(styleSheet);
	//		map->addLayer(layer);
	//	}

	//	if (useLabels && !useRaster)
	//	{
	//		// set up symbology for drawing labels. We're pulling the label
	//		// text from the name attribute, and its draw priority from the
	//		// population attribute.
	//		Style labelStyle;

	//		TextSymbol* text = labelStyle.getOrCreateSymbol<TextSymbol>();
	//		text->content() = StringExpression("[name]");
	//		text->priority() = NumericExpression("[pop]");
	//		text->size() = 16.0f;
	//		text->alignment() = TextSymbol::ALIGN_CENTER_CENTER;
	//		text->fill()->color() = Color::White;
	//		text->halo()->color() = Color::DarkGray;

	//		StyleSheet* sheet = new StyleSheet();
	//		sheet->addStyle(labelStyle);

	//		// and configure a model layer:
	//		FeatureModelLayer* fml = new FeatureModelLayer();
	//		fml->setName("Labels");
	//		fml->setFeatureSource(features);
	//		fml->setStyleSheet(sheet);
	//		map->addLayer(fml);
	//	}

	//	LayerVector layers;
	//	map->getLayers(layers);
	//	for (LayerVector::const_iterator i = layers.begin(); i != layers.end(); ++i)
	//	{
	//		Layer* layer = i->get();
	//		if (layer->getOpenAutomatically() && !layer->isOpen())
	//		{
	//			OE_WARN << layer->getName() << " : " << layer->getStatus().toString() << std::endl;
	//		}
	//	}

	//	// That's it, the map is ready; now create a MapNode to render the Map:
	//	return map;
	//}

	void CMainWindow::openMap()
	{
		//QWidget widget(this);
		//widget.setWindowTitle("widget窗体");
		////设置窗体大小宽800，高400
		//widget.resize(QSize(800, 400));
		//widget.show();
		CMapFileDialog * dialog = new CMapFileDialog();
		if (dialog->exec() == QFileDialog::Accepted)
		{

		}

		delete dialog;
		//QFileDialog dlg;
		//dlg.setNameFilter("earth file(*.earth ) ;; ALL File (*.*)");
		//dlg.setWindowTitle(tr("打开地图"));
		//if (dlg.exec() == QFileDialog::Accepted)
		//{
		//	QStringList fileList = dlg.selectedFiles();

		//	for (auto file : fileList)
		//	{
		//		std::string sPath = file.toLocal8Bit();
		//		std::string sName = util::FileSystem::instance()->fileStem(sPath);

		//		ReadResult r = osgEarth::URI(file.toLocal8Bit()).readNode();
		//		if (r.succeeded())
		//		{
		//			//添加啊地图
		//			MapNode* pMapNode = r.release<MapNode>();
		//			osg::ref_ptr<osgEarth::Map > pMap = pMapNode->getMap();
		//			//m_pMapNodes.insert({ sName,pMap });

		//			//创建新的窗口
		//			m_pMidArea->createMdiSubWindow(pMapNode);
		//		}
		//	}
		//}
	}

}


