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
    //earth::CMap* pMap = new earth::CMap(argc, argv);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
  
    QApplication a(argc, argv);

   /* QTextCodec *codec = QTextCodec::codecForName("GB2312");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);*/


	a.setStyle(new RibbonStyle());
    CMainWindow w;
    w.show();
    return a.exec();
}
