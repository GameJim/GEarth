
#include <QApplication>
#include "%RibbonClassName:l%.%CppHeaderSuffix%"

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    %RibbonClassName% w;
    w.show();
    return app.exec();
}
