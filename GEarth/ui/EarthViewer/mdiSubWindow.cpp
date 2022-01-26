#include "EarthViewer/mdiSubWindow.h"
#include "EarthViewer/MdiArea.hpp"
#include "QLayout"
#include "QLabel"
#include "QLayout"
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
#include <QEvent>


#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>


class PrintName :public osgGA::GUIEventHandler
{
public:
    bool handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa)
    {
        std::cout << id << ":-------------------------" << std::endl;

        return false;
    }
    virtual void getUsage(osg::ApplicationUsage&usage)const
    {
        usage.addKeyboardMouseBinding(("world"), "Hello");
    }

    int id;
};

CMdiSubWindow::CMdiSubWindow(earth::CRefPtr<earth::CMap> map, earth::CRefPtr<earth::CCompositeViewer> pViewers, QWidget * parent /*= Q_NULLPTR*/)
    :QMdiSubWindow(parent),m_map(map),m_pViewers(pViewers)
{
    this->setWindowModality(Qt::WindowModality::NonModal);
    this->resize(400, 400);

   
    //子窗口
    QWidget* pWidget = new QWidget();
    this->setWidget(pWidget);
  
    m_viewer = new earth::CView((void*)pWidget->winId());

    //m_viewer->setCameraManipulator(new osgEarth::EarthManipulator());
    m_viewer->addEventHandler(new osgViewer::StatsHandler);
    m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
   

    //设置handle
    //m_viewer->addEventHandler(new osgViewer::StatsHandler());//实现状态信息统计
    //m_viewer->addEventHandler(new osgViewer::WindowSizeHandler());
    //m_viewer->addEventHandler(new osgViewer::ThreadingHandler());
    //m_viewer->addEventHandler(new osgViewer::LODScaleHandler());
    ////m_viewer->addEventHandler(new osgGA::StateSetManipulator(view->getCamera()->getOrCreateStateSet()));//实现几何图形的网格化显示
    //m_viewer->addEventHandler(new osgViewer::RecordCameraPathHandler());
    //m_viewer->addEventHandler(new osgViewer::ScreenCaptureHandler());

    //auto mySelf = new PrintName();
    //mySelf->id = pWidget->winId();
    //m_viewer->addEventHandler(mySelf);

    ////加载操作器
    //auto pEarthManipulator = new osgEarth::EarthManipulator();
    ////pEarthManipulator->addMatrixManipulator()
    //m_viewer->setCameraManipulator(pEarthManipulator);
    //m_viewer->getEventQueue()->keyPress('s');

    // make the map scene graph:
    earth::CMapNode* node = new earth::CMapNode(map);
    m_viewer->setSceneData(node);

   
    //运行
}

CMdiSubWindow::~CMdiSubWindow()
{
    m_pViewers->removeView(m_viewer);
}

earth::CRefPtr<earth::CView> CMdiSubWindow::GetViewer()
{
    return m_viewer;
}

void CMdiSubWindow::PrintInfo()
{
  /*  std::cout << this->winId() << ":-------------------------" << std::endl;
    auto eventQueue = m_viewer->getEventQueue();
    std::cout << "EventQueue:" << eventQueue->empty() << std::endl;
    auto handlers = m_viewer->getEventHandlers();
    std::cout << "EventHandlers:" << handlers.size() << std::endl;
    std::cout << "Key:" << (((osgViewer::StatsHandler*)(handlers.front().get()))->getKeyEventTogglesOnScreenStats());*/
}

