/*!
*@File    DataTreeWidget.h
*@Brief   文件描述:下次一定
*@Date    2022/2/9
*/
#pragma once

#include <QWidget>
#include <QTreeView>
#include <QStackedWidget>

//文件目录类
class CDirTreeWidget : public QWidget {
    Q_OBJECT

public:
    CDirTreeWidget(QWidget * parent = Q_NULLPTR);
    ~CDirTreeWidget() = default;

    protected slots:
    void ShowDiskTree();
    void ShowDBTree();
private:
    QStackedWidget* m_pViewArea;   //用于显示的区域

};




class CDataTreeWidget : public QWidget {
    Q_OBJECT

public:
    CDataTreeWidget(QWidget * parent = Q_NULLPTR);
    ~CDataTreeWidget();
private:
};



