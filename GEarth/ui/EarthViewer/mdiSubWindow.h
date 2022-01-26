/*!
*@File    CMdiSubWindow.h
*@Brief   文件描述:下次一定
*@Date    2022/1/25
*/
#pragma once
#include "QtWidgets/qmdisubwindow"

#include "EarthCore/map.h"
#include "EarthCore/View.h"
#include "EarthCore/type.h"






class CMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT

public:


    CMdiSubWindow(earth::CRefPtr<earth::CMap> map, earth::CRefPtr<earth::CCompositeViewer> pViewers,QWidget * parent = Q_NULLPTR);
    ~CMdiSubWindow();

    earth::CRefPtr<earth::CView> GetViewer();

    void PrintInfo();
protected:
    
    public slots :
     

private:
    earth::CRefPtr<earth::CMap> m_map;
    earth::CRefPtr<earth::CView> m_viewer;

    earth::CRefPtr<earth::CCompositeViewer> m_pViewers;
};



