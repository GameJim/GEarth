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


class CMdiSubWindow : public QMdiSubWindow,public earth::CView{
    Q_OBJECT

public:
    CMdiSubWindow(earth::CRefPtr<earth::CMap> map, QWidget* parent = Q_NULLPTR);
    ~CMdiSubWindow();

    earth::CRefPtr<earth::CView> AsView();

    void PrintInfo();
protected:
    
    public slots :
     

private:
    //数据资源
    earth::CRefPtr<earth::CMap> m_map;

};



