#pragma execution_character_set("utf-8") 
#include "GEarth/CMainWindow.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include "GEarth/object_name.h"

#include <osgEarth/MapNode>

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

#include "GEarth/earth_cfg.h"
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
		//读取配置
		EarthCfg* pEarthCfg = new EarthCfg();

		//初始化Earth 环境，读取配置信息等
		osgEarth::initialize();

		
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
		this->setCentralWidget(m_pMidArea);



		//创建一个位于中间的球
		m_pMidArea->createMdiSubWindow(createDefaultMap());

		
	}

	CMainWindow::~CMainWindow() {

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
		RibbonPage* page = ribbonBar()->addPage(tr("文件(&F)"));
		page->setObjectName(_file_page);
		
	}

	void CMainWindow::createPageData()
	{
		RibbonPage* page = ribbonBar()->addPage(tr("数据(&D)"));
		page->setObjectName(_data_page);
		
	}

	void CMainWindow::createPageWinodws()
	{
		RibbonPage* page = ribbonBar()->addPage(tr("窗口(&W)"));
		//page->setContextColor(QTITAN_NAMESPACE::RibbonPage::ContextColorOrange);
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


	osg::ref_ptr<osgEarth::Map> CMainWindow::createDefaultMap(const std::string& sName /*= "default"*/)
	{
		osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map();
		map->setMapName(sName);
		m_pMapNodes.insert({ sName,map });

		// Start with a basemap imagery layer; we'll be using the GDAL driver
		// to load a local GeoTIFF file:
		GDALImageLayer* basemap = new GDALImageLayer();
		basemap->setURL("../data/world.tif");
		map->addLayer(basemap);

		// Next we add a layer to provide the feature data.
		OGRFeatureSource* features = new OGRFeatureSource();
		features->setName("vector-data");

		bool useRaster = true;
		bool useMem = false;
		bool useLabels = false;
		bool useDraping = false;
		bool useClamping = false;
		bool useScript = false;

		if (useMem)
		{
			// the --mem options tells us to just make an in-memory geometry:
			Ring* line = new Ring();
			line->push_back(osg::Vec3d(-60, 20, 0));
			line->push_back(osg::Vec3d(-120, 20, 0));
			line->push_back(osg::Vec3d(-120, 60, 0));
			line->push_back(osg::Vec3d(-60, 60, 0));
			features->setGeometry(line);
		}
		else
		{
			features->setURL("../data/world.shp");
		}
		map->addLayer(features);

		// Define a style for the feature data. Since we are going to render the
		// vectors as lines, configure the line symbolizer:
		Style style;

		LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
		ls->stroke()->color() = Color::Yellow;
		ls->stroke()->width() = 2.0f;
		ls->tessellationSize()->set(100, Units::KILOMETERS);

		if (useDraping)
		{
			AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
			alt->clamping() = alt->CLAMP_TO_TERRAIN;
			alt->technique() = alt->TECHNIQUE_DRAPE;
		}

		else if (useClamping)
		{
			AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
			alt->clamping() = alt->CLAMP_TO_TERRAIN;
			alt->technique() = alt->TECHNIQUE_GPU;

			ls->tessellationSize()->set(100, Units::KILOMETERS);

			RenderSymbol* render = style.getOrCreate<RenderSymbol>();
			render->depthOffset()->enabled() = true;
		}

		if (useRaster)
		{
			FeatureImageLayer* layer = new FeatureImageLayer();
			layer->setFeatureSource(features);
			StyleSheet* sheet = new StyleSheet();
			sheet->addStyle(style);
			layer->setStyleSheet(sheet);
			map->addLayer(layer);
		}

		else
		{
			FeatureModelLayer* layer = new FeatureModelLayer();
			layer->setFeatureSource(features);

			StyleSheet* styleSheet = new StyleSheet();

			if (useScript)
			{
				styleSheet->addStylesFromCSS(styles_css);
				styleSheet->setScript(new StyleSheet::ScriptDef(script_source));
				styleSheet->addSelector(StyleSelector("default", StringExpression("getStyleClass()")));
			}
			else
			{
				styleSheet->addStyle(style);
			}

			layer->setStyleSheet(styleSheet);
			map->addLayer(layer);
		}

		if (useLabels && !useRaster)
		{
			// set up symbology for drawing labels. We're pulling the label
			// text from the name attribute, and its draw priority from the
			// population attribute.
			Style labelStyle;

			TextSymbol* text = labelStyle.getOrCreateSymbol<TextSymbol>();
			text->content() = StringExpression("[name]");
			text->priority() = NumericExpression("[pop]");
			text->size() = 16.0f;
			text->alignment() = TextSymbol::ALIGN_CENTER_CENTER;
			text->fill()->color() = Color::White;
			text->halo()->color() = Color::DarkGray;

			StyleSheet* sheet = new StyleSheet();
			sheet->addStyle(labelStyle);

			// and configure a model layer:
			FeatureModelLayer* fml = new FeatureModelLayer();
			fml->setName("Labels");
			fml->setFeatureSource(features);
			fml->setStyleSheet(sheet);
			map->addLayer(fml);
		}

		LayerVector layers;
		map->getLayers(layers);
		for (LayerVector::const_iterator i = layers.begin(); i != layers.end(); ++i)
		{
			Layer* layer = i->get();
			if (layer->getOpenAutomatically() && !layer->isOpen())
			{
				OE_WARN << layer->getName() << " : " << layer->getStatus().toString() << std::endl;
			}
		}

		// That's it, the map is ready; now create a MapNode to render the Map:
		return map;
	}

}


//void CMainWindow::CreateSymbolLibraryPage(RibbonPage* pPage)
//{
//	RibbonGroup* group = pPage->addGroup(tr("Windows States"));
//
//	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
//	//打开，绑定事件
//	QAction* pAction =toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/NewFile.png")), tr("New"), Qt::ToolButtonTextUnderIcon);
//	connect(pAction, SIGNAL(triggered()), this, SLOT(CreateSymbolLibary()));
//
//
//	pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/OpenFile.png")), tr("Open"), Qt::ToolButtonTextUnderIcon);
//	connect(pAction, SIGNAL(triggered()), this, SLOT(OpenSymbolLibary()));
//
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Save.png")), tr("Save"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addSeparator();
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Share.png")), tr("Share"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Setting.png")), tr("Setiting"), Qt::ToolButtonTextUnderIcon);
//	group->addControl(toolBar);
//}
//
//void CMainWindow::CreateWindowsPage(RibbonPage* pPage)
//{
//	RibbonGroup* group = pPage->addGroup(tr("Symobl Library"));
//
//	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
//	/*toolBar->addAction(QIcon(QStringLiteral("./res/NewFile.png")), tr("New"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addAction(QIcon(QStringLiteral("./res/OpenFile.png")), tr("Open"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addAction(QIcon(QStringLiteral("./res/Save.png")), tr("Save"), Qt::ToolButtonTextUnderIcon);*/
//
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Share.png")), tr("Share"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Setting.png")), tr("Setiting"), Qt::ToolButtonTextUnderIcon);
//	toolBar->addAction(QIcon(QStringLiteral("./res/ui/symboldesigner/Setting.png")), tr("Setiting2"), Qt::ToolButtonTextUnderIcon);
//	group->addControl(toolBar);
//}
//
//void CMainWindow::CreateOptions()
//{
//
//}
//
//void CMainWindow::CreateSymbolLibary()
//{
//    QString sName = QString("../%1%2").arg(2021).arg(6);
//    auto path = QFileDialog::getSaveFileName(this, tr("New Symbol Library"), sName, tr("Symbol Libary(*.gsl)"));
//  
//}
//
//void CMainWindow::OpenSymbolLibary()
//{
//    auto path = QFileDialog::getOpenFileName(this, tr("Open Symbol Library"), "", tr("Symbol Libary(*.gsl)"));
// 
//}
