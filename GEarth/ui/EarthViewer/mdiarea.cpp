#include "mdiarea.hpp"
#include "mdiSubWindow.h"
CMdiArea::CMdiArea(QWidget * parent) : QMdiArea(parent) {
    viewer = new earth::CCompositeViewer();
    //viewer->realize();
    timer = new QTimer();
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(frame()));
    //viewer->setStartTick(10);
}

CMdiArea::~CMdiArea() {
	
}

void CMdiArea::addMapWindows(QWidget *widget, Qt::WindowFlags flags /*= Qt::WindowFlags()*/)
{
    flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    this->addSubWindow(widget, flags);
    CMdiSubWindow* pSubWindow = (CMdiSubWindow*)widget;

    viewer->addView(pSubWindow->GetViewer());

    //

}

earth::CRefPtr<earth::CCompositeViewer> CMdiArea::GetViewer()
{
    return viewer;
}

void CMdiArea::frame()
{
    if (viewer->getNumViews() != 0)
    {
        std::cout << "" << viewer->getFrameStamp()->getFrameNumber() << std::endl;
        auto& SubWindows = this->subWindowList();
        for (auto widget : SubWindows)
        {
            CMdiSubWindow* pSubWindow = (CMdiSubWindow*)widget;
            pSubWindow->PrintInfo();
        }

       
        viewer->frame();
    }
    
}
