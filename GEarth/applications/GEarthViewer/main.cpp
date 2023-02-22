
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "GEarth/mainwindow.h"

#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setStyle(new RibbonStyle());
    CMainWindow w;
    w.show();
    return a.exec();
}
