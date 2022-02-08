#include "mdiarea.hpp"
#include "mdiSubWindow.h"
CMdiArea::CMdiArea(QWidget * parent) 
    : QMdiArea(parent)
    ,earth::CCompositeViewer() 
{
    timer = new QTimer();
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(Frame()));
  
}

CMdiArea::~CMdiArea() {

}

CMdiSubWindow* CMdiArea::CreateMapWindow(earth::CRefPtr<earth::CMap> pMap)
{
    //flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    //this->addSubWindow(widget, flags);
    //CMdiSubWindow* pSubWindow = (CMdiSubWindow*)widget;

    //viewer->addView(pSubWindow->GetViewer());
    Qt::WindowFlags flags;
    flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    CMdiSubWindow* pSubWindow = new CMdiSubWindow(pMap);

    this->addSubWindow(pSubWindow,flags);
    this->addView(pSubWindow->AsView());

    return pSubWindow;
}

earth::CRefPtr<earth::CCompositeViewer> CMdiArea::asViewer()
{
    return this;
}

void CMdiArea::Frame()
{
    if (this->getNumViews()>0)
    {
        this->frame();
    }
   
}
