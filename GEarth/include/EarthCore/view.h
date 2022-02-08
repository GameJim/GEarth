/*!
*@File    viewer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/25
*/
#pragma once
#include "common/apiDescription.h"
#include "osgViewer/View"
namespace earth
{
    class CORE_PUBLIC CView : public osgViewer::View
    {
    public:
        //创建空白数据
        CView();

        //利用窗口句柄创建
        CView(void* hwnd);

        void InitWindow(void* hwnd);
    };
}



