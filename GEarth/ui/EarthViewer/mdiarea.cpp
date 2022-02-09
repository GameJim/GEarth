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
    
    Qt::WindowFlags flags;
    flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    CMdiSubWindow* pSubWindow = new CMdiSubWindow(pMap);

    this->addSubWindow(pSubWindow,flags);
    this->addView(pSubWindow->GetView());

    return pSubWindow;
}

earth::CCompositeViewer* CMdiArea::asViewer()
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
