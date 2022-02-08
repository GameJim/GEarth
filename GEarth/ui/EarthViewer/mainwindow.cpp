#pragma execution_character_set("utf-8") 
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



CMainWindow::CMainWindow(QWidget * parent)
    : RibbonMainWindow(parent)
{
	RibbonToolTip::setWrapMode(RibbonToolTip::NativeWrap);
	CreateOptions();

	RibbonPage* page = ribbonBar()->addPage(tr("文件"));
	page->setContextColor(QTITAN_NAMESPACE::RibbonPage::ContextColorOrange);
	CreateFilePage(page);

    RibbonPage* pEditsPage = ribbonBar()->addPage(tr("编辑"));
    CreateEmptyPage(pEditsPage);

	RibbonPage* pViews = ribbonBar()->addPage(tr("视图"));
	CreateViewPage(pViews);

    //包含空间工具：测量、分析以及其他
    RibbonPage* pTools = ribbonBar()->addPage(tr("工具"));
    CreateEmptyPage(pTools);

    RibbonPage* pTestPage = ribbonBar()->addPage(tr("测试"));
    CreateTestPage(pTestPage);

    RibbonPage* pDeomPage = ribbonBar()->addPage(tr("案例"));
    CreateDemoPage(pDeomPage);
   
    RibbonPage* pExpandPage = ribbonBar()->addPage(tr("拓展"));
    CreateEmptyPage(pExpandPage);

   

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
    //m_pMidArea->setViewMode(QMdiArea::TabbedView);

    //m_pMidArea->setTabsClosable(true);
    m_pMidArea->setTabsMovable(true);

	this->setCentralWidget(m_pMidArea);

    //临时创建一个地图
    earth::CRefPtr<earth::CMap> pMap = new earth::CMap();
    AddVector(pMap);
    m_Maps.push_back(pMap);
}

CMainWindow::~CMainWindow() {
    m_pMidArea = nullptr;
}

void CMainWindow::CreateDemoPage(RibbonPage* pPage)
{
    RibbonGroup* group = pPage->addGroup(tr("网络地图"));

    RibbonToolBarControl* toolBar = new RibbonToolBarControl();
    QAction* pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("百度地图"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("高德地图"), Qt::ToolButtonTextUnderIcon);
    group->addControl(toolBar);


    RibbonGroup* pMyDemoGroup = pPage->addGroup(tr("案例地图"));
    RibbonToolBarControl* MyDemotoolBar = new RibbonToolBarControl();
    pAction = MyDemotoolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("交通地图"), Qt::ToolButtonTextUnderIcon);
    pAction = MyDemotoolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("高德地图"), Qt::ToolButtonTextUnderIcon);

    pMyDemoGroup->addControl(MyDemotoolBar);
}

void CMainWindow::CreateTestPage(RibbonPage* pPage)
{
    RibbonGroup* group = pPage->addGroup(tr(""));

    RibbonToolBarControl* toolBar = new RibbonToolBarControl();
    //打开，绑定事件
    QAction* pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("多视窗"), Qt::ToolButtonTextUnderIcon);
    connect(pAction, SIGNAL(triggered()), this, SLOT(TestMulitView()));

    group->addControl(toolBar);
}

void CMainWindow::CreateMapWindow()
{
   
    
}

void CMainWindow::TestMulitView()
{
    auto pMap = m_Maps[0];
    {
        CMdiSubWindow* pSubWindow = m_pMidArea->CreateMapWindow(pMap);
        pSubWindow->setWindowTitle(QString::fromStdString(pMap->getName()));
        pSubWindow->show();
    }
    {
        CMdiSubWindow* pSubWindow = m_pMidArea->CreateMapWindow(pMap);
        pSubWindow->setWindowTitle(QString::fromStdString(pMap->getName()));
        pSubWindow->show();
    }
}

void CMainWindow::CreateFilePage(RibbonPage* pPage)
{
	RibbonGroup* group = pPage->addGroup(tr("新建"));
	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
	//打开，绑定事件
	QAction* pAction =toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("项目"), Qt::ToolButtonTextUnderIcon);
	connect(pAction, SIGNAL(triggered()), this, SLOT(CreateScence()));
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("场景"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("数据"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/NewFile.png")), tr("标注"), Qt::ToolButtonTextUnderIcon);
    group->addControl(toolBar);


    group = pPage->addGroup(tr("打开"));
    toolBar = new RibbonToolBarControl();
    //打开，绑定事件
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/OpenFile.png")), tr("项目"), Qt::ToolButtonTextUnderIcon);
    connect(pAction, SIGNAL(triggered()), this, SLOT(OpenScence()));
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/OpenFile.png")), tr("场景"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/OpenFile.png")), tr("数据"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/OpenFile.png")), tr("标注"), Qt::ToolButtonTextUnderIcon);
    group->addControl(toolBar);

	
    group = pPage->addGroup(tr("保存"));
    toolBar = new RibbonToolBarControl();
    //打开，绑定事件
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Save.png")), tr("本地"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Save.png")), tr("发布"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    group->addControl(toolBar);

	
    group = pPage->addGroup(tr("打印"));
    toolBar = new RibbonToolBarControl();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Save.png")), tr("打印"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Save.png")), tr("打印预览"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    group->addControl(toolBar);
  

    group = pPage->addGroup(tr("打印"));
    toolBar = new RibbonToolBarControl();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("显示"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("缓存"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("配置文件"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    group->addControl(toolBar);

}

void CMainWindow::CreateViewPage(RibbonPage* pPage)
{
	RibbonGroup* group = pPage->addGroup(tr("视图"));

	RibbonToolBarControl* toolBar = new RibbonToolBarControl();
    QAction* pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Share.png")), tr("场景视图"), Qt::ToolButtonTextUnderIcon);  //图层结构
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Share.png")), tr("对象视图"), Qt::ToolButtonTextUnderIcon);  //以每个渲染对象来
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Share.png")), tr("工具视图"), Qt::ToolButtonTextUnderIcon);  //分析的工具
	group->addControl(toolBar);

    group = pPage->addGroup(tr("场景视图"));
    toolBar = new RibbonToolBarControl();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("新建三维视图"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("新建二维视图"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("显示所有视图"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("隐藏所有视图"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("关闭所有视图"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    pAction = pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("平铺"), Qt::ToolButtonTextUnderIcon);
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("列表"), Qt::ToolButtonTextUnderIcon);
    toolBar->addSeparator();
    pAction = toolBar->addAction(QIcon(QStringLiteral("./res/ui/EarthViewer/Setting.png")), tr("分割"), Qt::ToolButtonTextUnderIcon);
    group->addControl(toolBar);
}


void CMainWindow::CreateOptions()
{
   
}

void CMainWindow::CreateScence()
{
    
}

void CMainWindow::OpenScence()
{
   
}


void CMainWindow::CreateEmptyPage(RibbonPage* pPage)
{

}
