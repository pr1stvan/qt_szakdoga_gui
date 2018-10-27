#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QVector>
#include <QDebug>
#include <time.h>
#include "framesystem.h"

int main(int argc, char *argv[])
{
    srand ( time(NULL) );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
