#include "EarthViewer/mdiSubWindow.h"
#include "EarthViewer/MdiArea.hpp"
#include "QLayout"
#include "QLabel"
#include "QLayout"
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
#include <QEvent>

#include "EarthCore/EarthManipulator.h"


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

CMdiSubWindow::CMdiSubWindow(earth::CRefPtr<earth::CMap> map, QWidget * parent /*= Q_NULLPTR*/)
    :QMdiSubWindow(parent),earth::CView()
    ,m_map(map)

{
    this->setWindowModality(Qt::WindowModality::NonModal);
    this->resize(400, 400);

  
    //子窗口
    QWidget* pWidget = new QWidget();
    this->setWidget(pWidget);
    this->InitWindow((void*)widget()->winId());
  
    //设置数据
    earth::CMapNode* node = new earth::CMapNode(map);
    this->setSceneData(node);
   
    //设置操作器
    this->setCameraManipulator(new earth::CEarthManipulator());
}

CMdiSubWindow::~CMdiSubWindow()
{
    ((CMdiArea*)this->mdiArea())->asViewer()->removeView(this);
}

earth::CRefPtr<earth::CView> CMdiSubWindow::AsView()
{
    return this;
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

