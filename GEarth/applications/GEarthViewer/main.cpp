
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
	/*QMainWindow window;
	
	window.setWindowTitle("My Window");
	window.setStyleSheet("QMainWindow {background-color: #f2f2f2; border: none;}"
		"QMainWindow::title {background-color: #000000; color: #000000; border:none;}");
	window.show();*/
    return a.exec();
}
