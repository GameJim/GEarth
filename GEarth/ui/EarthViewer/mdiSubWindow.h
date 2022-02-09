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


class CMdiSubWindow : public QMdiSubWindow{
    Q_OBJECT

public:
    CMdiSubWindow(earth::CRefPtr<earth::CMap> map, QWidget* parent = Q_NULLPTR);
    ~CMdiSubWindow();

    earth::CRefPtr<earth::CView> GetView();

    void PrintInfo();
protected slots:
    void closeEvent(QCloseEvent *closeEvent) Q_DECL_OVERRIDE;
   
     

private:
    //数据资源
    earth::CRefPtr<earth::CView> m_pView;
    earth::CRefPtr<earth::CMap> m_map;

};



