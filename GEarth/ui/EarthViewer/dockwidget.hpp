#pragma once
#include <QDockwidget>
#include <Qnamespace.h>
class CDockWidget : public QDockWidget {
	Q_OBJECT

public:
	CDockWidget(QWidget * parent = Q_NULLPTR);
	~CDockWidget();

private:
	
};
