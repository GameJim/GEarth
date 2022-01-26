#include "EarthViewer/mdiSubWindow.h"
#include "QLayout"
#include "QLabel"
#include "QLayout"
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
CMdiSubWindow::CMdiSubWindow(earth::CRefPtr<earth::CMap> map, QWidget * parent /*= Q_NULLPTR*/)
    :QMdiSubWindow(parent),m_map(map)
{
    this->setWindowModality(Qt::WindowModality::NonModal);
    this->resize(400, 400);

   
    //子窗口
    QWidget* pWidget = new QWidget();
    this->setWidget(pWidget);
  
    m_viewer = new earth::CViewer((void*)pWidget->winId());

    // make the map scene graph:
    earth::CMapNode* node = new earth::CMapNode(map);
    m_viewer->setSceneData(node);

   
    //运行
    //m_viewer->realize();
    //m_viewer->run();
}

earth::CRefPtr<earth::CViewer> CMdiSubWindow::GetViewer()
{
    return m_viewer;
}

