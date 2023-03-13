#include "GEarth/CMdiSubWindow.h"
#include "GEarth/CMdiArea.h"

#if WIN32
#include "osgViewer/api/Win32/GraphicsWindowWin32"
#endif
#include "osg/DisplaySettings"
#include "osgViewer/ViewerEventHandlers"
#include "osgEarth/EarthManipulator"
#include "osgEarth/MapNode"
#include <osgEarth/Ephemeris>
#include <osgEarth/Sky>
#include "const.h"
namespace ui
{

	CMdiSubWindow::CMdiSubWindow(osg::ref_ptr<osgEarth::MapNode> pMapNode, QWidget* parent /*= Q_NULLPTR*/)
		:m_pMapNode(pMapNode)
		, m_pView(new osgViewer::View())
	{
		
		this->resize(400, 400);
		//this->setAutoFillBackground(false);
		

		QWidget* pWidget = new CCanva(this);
		this->setWidget(pWidget);

		void* hwnd = (void*)pWidget->winId();

#if WIN32
		RECT rect;
		::GetWindowRect((HWND)hwnd, &rect);

		auto windata = new osgViewer::GraphicsWindowWin32::WindowData((HWND)hwnd);
		auto traits = new osg::GraphicsContext::Traits();
		traits->x = rect.left;
		traits->y = rect.top;
		traits->width = rect.right - rect.left;
		traits->height = rect.bottom - rect.top;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->inheritedWindowData = windata;
		traits->vsync = false;

		//显示设置
		osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
		traits->alpha = ds->getMinimumNumAlphaBits();
		traits->stencil = ds->getMinimumNumStencilBits();
		traits->sampleBuffers = ds->getMultiSamples();
		traits->samples = ds->getNumMultiSamples();
		

		osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits);

		osg::ref_ptr<osg::Camera> camera = new osg::Camera();
		camera->setGraphicsContext(gc);
		camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		
#endif
		//设置相机
		m_pView->setCamera(camera);


		{
			//osgEarth::DateTime dateTime(2022, 7, 17, 20);		//格林尼治时间

			//osgEarth::Util::Ephemeris* ephemeris = new osgEarth::Util::Ephemeris;

			//osgEarth::Util::SkyOptions skyOptions;

			//osgEarth::Config conf = skyOptions.getConfig();
			//osgEarth::optional<osgEarth::URI> path;
			//path = g_sModulePath + "/../data/moon_1024x512.jpg";
			//conf.set("moon_image", path);
			//skyOptions.merge(conf);
			////skyOptions.ambient() = 0.4;//控制黑夜部分明暗程度，数值越小，越黑暗
			////skyOptions.quality() = osgEarth::Util::SkyOptions::Quality::QUALITY_HIGH;//控制黑夜部分明暗程度，数值越小，越黑暗

			//osgEarth::Util::SkyNode* m_pSkyNode = osgEarth::Util::SkyNode::create(skyOptions);
		
			///*m_pSkyNode->setStarsVisible(true);
			//m_pSkyNode->setAtmosphereVisible(true);
			//m_pSkyNode->setSunVisible(true);
			//m_pSkyNode->setMoonVisible(true);*/


			//m_pSkyNode->setName("SkyNode");
			//m_pSkyNode->setEphemeris(ephemeris);
			//m_pSkyNode->setDateTime(dateTime);
			//m_pSkyNode->attach(m_pView,0);
			//m_pSkyNode->setLighting(false);
			//m_pSkyNode->addChild(pMapNode);

			m_pView->setSceneData(pMapNode);
		}
		

		//设置操作器
		m_pView->setCameraManipulator(new osgEarth::EarthManipulator());

	}

	CMdiSubWindow::~CMdiSubWindow()
	{
		if (dynamic_cast<CMdiArea*>(mdiArea()))
		{
			static_cast<CMdiArea*>(mdiArea())->destoryMdiSubWindow(this);
		}
	}

	osg::ref_ptr<osgViewer::View> CMdiSubWindow::getView() const
	{
		return m_pView;
	}

	osg::ref_ptr<osgEarth::MapNode> CMdiSubWindow::getMapNode() const
	{
		return m_pMapNode;
	}

	void CMdiSubWindow::closeEvent(QCloseEvent *closeEvent)
	{

	}

	

	CCanva::CCanva(CMdiSubWindow* pMdiSubWindow, QWidget* parent /*= Q_NULLPTR*/)
		:m_pMdiSubWindow(pMdiSubWindow)
	{
		this->setAutoFillBackground(false);
		this->setWindowFlag(Qt::FramelessWindowHint);
		this->setAttribute(Qt::WA_PaintOnScreen);
		
	}

	QPaintEngine* CCanva::paintEngine() const
	{
		return 0;
	}
	void CCanva::paintEvent(QPaintEvent *paintEvent)
	{
		//立即重新绘制一次
		if (dynamic_cast<CMdiArea*>(m_pMdiSubWindow->mdiArea()))
		{
			//static_cast<CMdiArea*>(m_pMdiSubWindow->mdiArea())->immediatelDraw();
		}
		
	}

}

