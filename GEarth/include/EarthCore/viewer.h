/*!
*@File    viewer.h
*@Brief   文件描述:下次一定
*@Date    2022/1/25
*/
#pragma once
#include "common/apiDescription.h"
#include "osgViewer/Viewer"
namespace earth
{
    class CORE_PUBLIC CViewer : public osgViewer::Viewer
    {
    public:
        //创建空白数据
        CViewer();

        //利用窗口句柄创建
        CViewer(void* hwnd);
    };
}



