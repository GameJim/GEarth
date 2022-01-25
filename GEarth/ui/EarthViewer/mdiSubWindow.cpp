#include "mdiSubWindow.h"



CMdiSubWindow::CMdiSubWindow(earth::CRefPtr<earth::CMap> map, QWidget * parent /*= Q_NULLPTR*/)
    :QMdiSubWindow(parent),m_map(map)
{
    this->setWindowModality(Qt::WindowModality::NonModal);
    this->resize(200, 200);
    auto rect = this->geometry();
    m_viewer = new earth::CViewer((void*)this->winId());

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
