#include "mainwindow.hpp"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

#include "common/size.h"

#include <common/log.h>
#include <iostream>

#include "EarthCore/map.h"

int main(int argc, char *argv[])
{
    //earth::CMap* pMap = new earth::CMap(argc, argv);

    QApplication a(argc, argv);
	a.setStyle(new RibbonStyle());
    CMainWindow w;
    w.show();
    return a.exec();
}
