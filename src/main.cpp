#include "include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //disable dark mode by default
    w.toggleDarkMode(false);
    return a.exec();
}
