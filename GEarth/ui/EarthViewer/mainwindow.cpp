#include "mainwindow.hpp"
#include "mdiSubWindow.h"
#include "EarthCore/map.h"

#include <osgEarth/MapNode>
#include <osgEarth/GDAL>
#include <osgEarth/ExampleResources>
#include <osgEarth/EarthManipulator>

#include <osgEarth/Style>
#include <osgEarth/OGRFeatureSource>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/FeatureImageLayer>

using namespace osgEarth;

void AddVector(osg::ref_ptr<osgEarth::Map> map)
{
    //
    GDALImageLayer* basemap = new GDALImageLayer();
    basemap->setURL("D:/data/osgEarth/data/world.tif");
    map->addLayer(basemap);

    // Next we add a layer to provide the feature data.
    OGRFeatureSource* features = new OGRFeatureSource();
    features->setName("vector-data");

    //if (useMem)
    //{
    //    // the --mem options tells us to just make an in-memory geometry:
    //    Ring* line = new Ring();
    //    line->push_back(osg::Vec3d(-60, 20, 0));
    //    line->push_back(osg::Vec3d(-120, 20, 0));
    //    line->push_back(osg::Vec3d(-120, 60, 0));
    //    line->push_back(osg::Vec3d(-60, 60, 0));
    //    features->setGeometry(line);
    //}
  //  else
    {
        features->setURL("D:/data/osgEarth/data/world.shp");
    }
    map->addLayer(features);

    // Define a style for the feature data. Since we are going to render the
    // vectors as lines, configure the line symbolizer:
    Style style;

    LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
    ls->stroke()->color() = Color::Yellow;
    ls->stroke()->width() = 2.0f;
    ls->tessellationSize()->set(100, Units::KILOMETERS);

    //if (useDraping)
    {
        AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
        alt->clamping() = alt->CLAMP_TO_TERRAIN;
        alt->technique() = alt->TECHNIQUE_DRAPE;
    }

    //else if (useClamping)
    //{
    //    AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
    //    alt->clamping() = alt->CLAMP_TO_TERRAIN;
    //    alt->technique() = alt->TECHNIQUE_GPU;

    //    ls->tessellationSize()->set(100, Units::KILOMETERS);

    //    RenderSymbol* render = style.getOrCreate<RenderSymbol>();
    //    render->depthOffset()->enabled() = true;
    //}

   // if (useRaster)
    {
        FeatureImageLayer* layer = new FeatureImageLayer();
        layer->setFeatureSource(features);
        StyleSheet* sheet = new StyleSheet();
        sheet->addStyle(style);
        layer->setStyleSheet(sheet);
        map->addLayer(layer);
    }

   /* else
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
*/
    //if (useLabels && !useRaster)
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
        if (layer->getStatus().isError() &&
            layer->getEnabled())
        {
            OE_WARN << layer->getName() << " : " << layer->getStatus().toString() << std::endl;
        }
    }
}



CMainWindow::CMainWindow(QWidget * parent) : RibbonMainWindow(parent) {
	RibbonToolTip::setWrapMode(RibbonToolTip::NativeWrap);
	CreateOptions();
	
	RibbonPage* page = ribbonBar()->addPage(tr("&Home"));
	page->setContextColor(QTITAN_NAMESPACE::RibbonPage::ContextColorOrange);
	CreateSymbolLibraryPage(page);

	RibbonPage* pWindows = ribbonBar()->addPage(tr("Windows"));
	CreateWindowsPage(pWindows);


    RibbonPage* pDeomPage = ribbonBar()->addPage(tr("Demo"));
    CreateDEMOPage(pDeomPage);

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
   /* m_pMidArea->setViewMode(QMdiArea::TabbedView);

    m_pMidArea->setTabsClosable(true);
    m_pMidArea->setTabsMovable(true);*/

	this->setCentralWidget(m_pMidArea);

    //临时创建一个地图
    earth::CRefPtr<earth::CMap> pMap = new earth::CMap();
    AddVector(pMap);
    m_Maps.push_back(pMap);
}

CMainWindow::~CMainWindow() {
	
}


void CMainWindow::CreateDEMOPage(RibbonPage* pPage)
{
    RibbonGroup* group = pPage->addGroup(tr("Create Windows"));

    RibbonToolBarControl* toolBar = new RibbonToolBarControl();
    //打开，绑定事件
    QAction* pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("New Window"), Qt::ToolButtonTextUnderIcon);
    connect(pAction, SIGNAL(triggered()), this, SLOT(CreateMapWindow()));

    group->addControl(toolBar);
}

void CMainWindow::CreateMapWindow()
{
    auto pMap = m_Maps[0];
    {
        CMdiSubWindow* pSubWindow = new CMdiSubWindow(pMap);
        pSubWindow->setWindowTitle(QString::fromStdString(pMap->getName()));
        m_pMidArea->addMapWindows(pSubWindow);
        pSubWindow->show();
    }
    
   /* {
        CMdiSubWindow* pSubWindow = new CMdiSubWindow(pMap);
        pSubWindow->setWindowTitle(QString::fromStdString(pMap->getName()));
        m_pMidArea->addMapWindows(pSubWindow);
        m_pMidArea->show();
    }*/
   
   
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
    
}

void CMainWindow::OpenSymbolLibary()
{
   
}
