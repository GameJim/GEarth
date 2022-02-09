#include "dockwidget.hpp"
#include "mainwindow.hpp"
//CDockWidget::CDockWidget(QWidget * parent) : QDockWidget(parent) {
//
//}

CDockWidget::CDockWidget(const QString& sName, QWidget* child, const Qt::DockWidgetArea& dockWidgetArea, QWidget * parent,const bool& IsShow)
    :QDockWidget(parent)
{
    ((CMainWindow*)parent)->addDockWidget(dockWidgetArea, this);
   
    //窗口
    this->setWidget(child);
    //名称
    this->setWindowTitle(sName);


    if (IsShow)
    {
        this->show();
    }
    else
    {
        this->hide();
    }
}

CDockWidget::~CDockWidget() {
	
}
