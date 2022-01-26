#pragma once
#include <QMdiArea>
#include "EarthCore/type.h"
#include "QTimer"
class CMdiArea : public QMdiArea {
	Q_OBJECT

public:
	CMdiArea(QWidget * parent = Q_NULLPTR);
	~CMdiArea();

    void addMapWindows(QWidget *widget, Qt::WindowFlags flags = Qt::WindowFlags());

    earth::CRefPtr<earth::CCompositeViewer> GetViewer();
public:
public slots:
    void frame();
private:
	
    QTimer* timer;
    earth::CRefPtr<earth::CCompositeViewer> viewer;
};
