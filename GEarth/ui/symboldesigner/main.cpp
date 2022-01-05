#include "mainwindow.hpp"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setStyle(new RibbonStyle());
    CMainWindow w;
    w.show();
    return a.exec();
}
