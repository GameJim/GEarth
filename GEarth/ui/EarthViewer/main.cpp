#include "mainwindow.hpp"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

#include "common/size.h"

#include <common/log.h>
#include <iostream>

#include "EarthCore/map.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
  
    QApplication a(argc, argv);

	a.setStyle(new RibbonStyle());
    CMainWindow w;
    w.show();
    return a.exec();
}
