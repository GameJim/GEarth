#include "GEarth/CMdiSubWindow.h"
#include "GEarth/CMdiArea.h"

#if WIN32
#include "osgViewer/api/Win32/GraphicsWindowWin32"
#endif
#include "osg/DisplaySettings"
#include "osgViewer/ViewerEventHandlers"
#include "osgEarth/EarthManipulator"
#include "osgEarth/MapNode"
namespace ui
{

	CMdiSubWindow::CMdiSubWindow(osg::ref_ptr<osgEarth::MapNode> pMapNode, QWidget* parent /*= Q_NULLPTR*/)
		:m_pMapNode(pMapNode)
		, m_pView(new osgViewer::View())
	{
		
		this->resize(400, 400);
		this->setAutoFillBackground(false);
		

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
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->inheritedWindowData = windata;

		//��ʾ����
		osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
		traits->alpha = ds->getMinimumNumAlphaBits();
		traits->stencil = ds->getMinimumNumStencilBits();
		traits->sampleBuffers = ds->getMultiSamples();
		traits->samples = ds->getNumMultiSamples();
		traits->vsync = true;

		osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits);

		osg::ref_ptr<osg::Camera> camera = new osg::Camera();
		camera->setGraphicsContext(gc);
		camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

#endif
		//�������
		m_pView->setCamera(camera);


		m_pView->setSceneData(pMapNode);

		//���ò�����
		m_pView->setCameraManipulator(new osgEarth::EarthManipulator());

	}

	CMdiSubWindow::~CMdiSubWindow()
	{

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
		//�������»���һ��
		if (dynamic_cast<CMdiArea*>(m_pMdiSubWindow->mdiArea()))
		{
			static_cast<CMdiArea*>(m_pMdiSubWindow->mdiArea())->immediatelDraw();
		}
		
	}

}
