#pragma once
#include <QMdiArea>
#include "EarthCore/type.h"
#include "EarthCore/map.h"
#include "mdiSubWindow.h"
#include "QTimer"
class CMdiArea : public QMdiArea,public earth::CCompositeViewer {
	Q_OBJECT

public:
	CMdiArea(QWidget * parent = Q_NULLPTR);
	~CMdiArea();

    CMdiSubWindow* CreateMapWindow(earth::CRefPtr<earth::CMap> map);

    earth::CCompositeViewer* asViewer();
public:
public slots:
    void Frame();
private:
    //计时器
    QTimer* timer;
};
