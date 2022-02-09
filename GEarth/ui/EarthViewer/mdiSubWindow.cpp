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
    :QMdiSubWindow(parent)
    ,m_map(map)

{
    this->setWindowModality(Qt::WindowModality::NonModal);
    this->resize(400, 400);
    //不绘制背景
    this->setAutoFillBackground(false);
   
  
    //子窗口
    QWidget* pWidget = new QWidget();
    this->setWidget(pWidget);
    m_pView = new earth::CView((void*)widget()->winId());
    
  
    //设置数据
    earth::CMapNode* node = new earth::CMapNode(map);
    m_pView->setSceneData(node);
   
    //设置操作器
    m_pView->setCameraManipulator(new earth::CEarthManipulator());
}

CMdiSubWindow::~CMdiSubWindow()
{

}

earth::CRefPtr<earth::CView> CMdiSubWindow::GetView()
{
    return m_pView;
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

void CMdiSubWindow::closeEvent(QCloseEvent *closeEvent)
{
    //移除渲染
    ((CMdiArea*)this->mdiArea())->asViewer()->removeView(m_pView);
}

