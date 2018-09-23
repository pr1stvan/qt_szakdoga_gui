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



//int main()
//{

//    FrameSystem fs;

//    fs.loadDirectory(QString("C:/Users/THe_KinG/Desktop/000/ArOnly"));

//    qDebug() << "jonnek a frame -ek:" << endl;
//    for(int i = 0; i<fs.size(); i++){
//        qDebug() << "points:" << fs.getPoints(i);
//        qDebug() << "velocities:" << fs.getVelocities(i);
//        qDebug() << "areas:" << fs.getAreas(i);
//        qDebug() << "indexes:" << fs.getIndices(i);

//        qDebug() << "";


//        qDebug() << "velocity data exist:" << fs.velocityDataExist();
//        qDebug() << "area data exist:" << fs.areaDataExist();
////        qDebug() << "vertex areas: " << fs.getVertexAreas(i);
////        qDebug() << "projected vertex areas: " << fs.getProjectedVertexAreas(i);
////        qDebug() << "vertex masses: " << fs.getVertexMasses(i);
//        qDebug() << "";

//    }

//    return 0;

//}
