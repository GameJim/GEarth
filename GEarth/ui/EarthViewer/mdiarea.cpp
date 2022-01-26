#include "mdiarea.hpp"
#include "mdiSubWindow.h"
CMdiArea::CMdiArea(QWidget * parent) : QMdiArea(parent) {
    viewer = new earth::CCompositeViewer();

    timer = new QTimer();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(frame()));
}

CMdiArea::~CMdiArea() {
	
}

void CMdiArea::addMapWindows(QWidget *widget, Qt::WindowFlags flags /*= Qt::WindowFlags()*/)
{
    flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    this->addSubWindow(widget, flags);
    CMdiSubWindow* pSubWindow = (CMdiSubWindow*)widget;

    viewer->addView(pSubWindow->GetViewer());

    //viewer->frame();
}

void CMdiArea::frame()
{
    if (viewer->getNumViews() != 0)
    {
        viewer->frame();
    }
    
}
