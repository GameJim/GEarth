
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "GEarth/CMainWindow.h"

#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	ui::CMainWindow* pMainWindow = new ui::CMainWindow();
	a.setStyle(new RibbonStyle(pMainWindow));
   
	pMainWindow->show();
    return a.exec();
}
