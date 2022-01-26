/*!
*@File    CMdiSubWindow.h
*@Brief   文件描述:下次一定
*@Date    2022/1/25
*/
#pragma once
#include "QtWidgets/qmdisubwindow"

#include "EarthCore/map.h"
#include "EarthCore/viewer.h"
#include "EarthCore/type.h"






class CMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT

public:



    CMdiSubWindow(earth::CRefPtr<earth::CMap> map,QWidget * parent = Q_NULLPTR);
    ~CMdiSubWindow() = default;

    earth::CRefPtr<earth::CViewer> GetViewer();


protected:
    
    public slots:
   
private:
    earth::CRefPtr<earth::CMap> m_map;
    earth::CRefPtr<earth::CViewer> m_viewer;
};



