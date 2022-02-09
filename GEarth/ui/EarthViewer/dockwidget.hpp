#pragma once
#include <QDockwidget>
#include <Qnamespace.h>
class CDockWidget : public QDockWidget {
	Q_OBJECT

public:
	CDockWidget(const QString& sName,QWidget* child,const Qt::DockWidgetArea& dockWidgetArea,QWidget * parent, const bool& IsShow = true);
	~CDockWidget();

private:
	
};
