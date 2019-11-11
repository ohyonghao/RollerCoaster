#include "MainWindow.h"

#include <QApplication>
#include "GLWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    GLWidget w;
    w.show();
    return a.exec();
}
