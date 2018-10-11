#include <QDebug>

#include <QColor>
#include <QVector3D>
#include <QVector>

#include "framesystem.h"
#include "framereader.h"
#include "framesystem.h"

FrameSystem::FrameSystem():
    colorMode(SOLID),
    solidColor(0,0,0),
    startColor(0,0,0),
    endColor(0,0,0),
    prefer_drawing_points(false),
    velocity_data_exist(false),
    area_data_exist(false)
{

}

QColor vectorTocolor(QVector3D vec){
    QColor color;

    color.setRgbF((qreal)vec.x(),(qreal)vec.y(),(qreal)vec.z());
    return color;
}

void FrameSystem::loadFiles(QStringList filePaths)
{
   frames.clear();
   frames.squeeze();
//   QStringList files = getDirectoryFiles(directoryPath);

   prefer_drawing_points=false;
   if(filePaths.size() == 0){
       velocity_data_exist=false;
       area_data_exist=false;
   }
   else{
       Frame frame(filePaths.at(0));
       frames.push_back(frame);

       velocity_data_exist=frame.getVelocityDataExist();
       area_data_exist=frame.getAreaDataExist();
       prefer_drawing_points=frame.preferDrawingPoints();

       for(int i = 1 ; i< filePaths.size(); i++){
           Frame frame(filePaths.at(i));
           frames.push_back(frame);
       }
       colorMode=SOLID;
       if(!velocity_data_exist && colorMode==VELOCITY){
           colorMode=SOLID;
       }
       if(!area_data_exist && colorMode!=VELOCITY){
           colorMode=SOLID;
       }
       setUpFrameColors();
       scalePointsAfterLoading();
   }
}

QVector3D FrameSystem::getPointsAvgAfterLoading()
{
    if(frames.size() == 0){
        return QVector3D(0,0,0);
    }

    QVector<float> points=getPoints(0);

    float avgX=0;
    float avgY=0;
    float avgZ=0;
    float count=0;
    for(int i=0; i<points.size(); i++){
        if(i%3 == 0){
            //x coordinates
            count +=1;

            avgX += points[i];
        }
        else if(i%3 == 1){
            //y coordinates
            avgY+=points[i];
        }
        else if(i%3 == 2){
            //z coordinates
            avgZ+=points[i];
        }
    }

    return QVector3D(avgX/count,avgY/count,avgZ/count);
}

void FrameSystem::scalePointsAfterLoading()
{
    QVector<float> t;
    t << getMaxPointCoord(0)-getMinPointCoord(0);
    t << getMaxPointCoord(1)-getMinPointCoord(1);
    t << getMaxPointCoord(2)-getMinPointCoord(2);

//    qDebug() << "x:" << t[0];
//    qDebug() << "y:" << t[1];
//    qDebug() << "z:" << t[2];
//    float avg=0;
//    for(auto e : t){
//        avg+=e;
//    }
//    avg/=3.0f;
//    qDebug() << "avg:" << avg;

    float maxCoordinateDistance=*std::max_element(t.constBegin(),t.constEnd());
//    qDebug() << "max distance:" << maxCoordinateDistance;
    float scale=26.0f/maxCoordinateDistance;
//    qDebug() << "scale:" << scale;
    for(int i=0; i<frames.size(); i++){
        frames[i].scalePointsAfterLoading(scale);
    }


    t.clear();
    t.squeeze();
    t << getMaxPointCoord(0)-getMinPointCoord(0);
    t << getMaxPointCoord(1)-getMinPointCoord(1);
    t << getMaxPointCoord(2)-getMinPointCoord(2);

//    qDebug() << "new x:" << t[0];
//    qDebug() << "new y:" << t[1];
//    qDebug() << "new z:" << t[2];

    maxCoordinateDistance=*std::max_element(t.constBegin(),t.constEnd());
//    qDebug() <<"new max distance:" << maxCoordinateDistance;

}

float FrameSystem::getMin()
{
    if(frames.size()==0){
        return 0;
    }

    float min= frames[0].getMin(colorMode);

    for(int i=1; i< frames.size(); i++){
        float actual=frames[i].getMin(colorMode);
        if(actual < min){
            min = actual;
        }
    }

    return min;
}

float FrameSystem::getMax()
{
    if(frames.size()==0){
        return 0;
    }

    float max= frames[0].getMax(colorMode);

    for(int i=1; i< frames.size(); i++){
        float actual=frames[i].getMax(colorMode);
        if(actual > max){
            max = actual;
        }
    }

    return max;
}

float FrameSystem::getMinPointCoord(int xyz)
{
    if(frames.size() == 0)return 0;

    float min=frames[0].getMinPointCoord(xyz);
    for(int i=1; i<frames.size();i++){
        float actual=frames[i].getMinPointCoord(xyz);
        if(actual < min){
            min=actual;
        }
    }
    return min;

}

float FrameSystem::getMaxPointCoord(int xyz)
{
    if(frames.size() == 0)return 0;

    float max=frames[0].getMaxPointCoord(xyz);
    for(int i=1; i<frames.size();i++){
        float actual=frames[i].getMaxPointCoord(xyz);
        if(actual > max){
            max=actual;
        }
    }
    return max;

}

QStringList FrameSystem::getDirectoryFiles(QString directoryPath)
{
    QDir currentDir(directoryPath);

    QStringList fileNames=currentDir.entryList(QStringList(QString("*.vtp")), QDir::Files | QDir::NoSymLinks);

    QStringList files;
    for(QString file : fileNames){
        files.append(directoryPath + QLatin1Char('/') + file);
    }
    files.sort();

    return files;
}

void FrameSystem::setUpFrameColors()
{
    if(frames.size()<1){
        return;
    }

    QVector3D start,end;
    float min,max;
    if(colorMode==SOLID){
        start=solidColor;
        end=QVector3D(0,0,0);
        max=0,min=0;
    }
    else {
        start=startColor;
        end=endColor;
        min=getMin();
        max=getMax();
    }

    for(int i= 0; i< frames.size(); i++){
        frames[i].setUpColors(colorMode,start,
                              end,
                              min,
                              max);
    }
}

const QVector<float> &FrameSystem::getPoints(int i)
{
    return frames[i].getPoints();
}
const QVector<float> &FrameSystem::getVelocities(int i)
{
    return frames[i].getVelocities();
}

const QVector<float> &FrameSystem::getAreas(int i)
{
    return frames[i].getAreas();
}

const QVector<float> &FrameSystem::getVertexAreas(int i)
{
    return frames[i].getVertexAreas();
}

const QVector<float> &FrameSystem::getProjectedVertexAreas(int i)
{
    return frames[i].getProjectedVertexAreas();
}

const QVector<float> &FrameSystem::getVertexMasses(int i)
{
    return frames[i].getVertexMasses();
}

const QVector<unsigned int> &FrameSystem::getIndices(int i)
{
    return frames[i].getIndices();
}
const QVector<float> &FrameSystem::getAmbients(int i)
{
    return frames[i].getAmbients();
}
const QVector<float> &FrameSystem::getDiffuses(int i)
{
    return frames[i].getDiffuses();
}

const QVector<float> &FrameSystem::getSpeculars(int i)
{
    return frames[i].getSpeculars();
}

void FrameSystem::setColorMode(ColorMode mode)
{
    colorMode=mode;
    setUpFrameColors();
}

void FrameSystem::setSolidColor(QColor color)
{
    solidColor=QVector3D((float)color.redF(),
                                 (float)color.greenF(),
                                 (float)color.blueF());
    setUpFrameColors();
//    qDebug() << "setSolid: " <<  QVector3D(solidColor.x()*255,solidColor.y()*255,solidColor.z()*255);
}

void FrameSystem::setStartColor(QColor color)
{
    startColor=QVector3D((float)color.redF(),
                                 (float)color.greenF(),
                                 (float)color.blueF());

    setUpFrameColors();

//    qDebug() << "setStart: " << QVector3D(startColor.x()*255,startColor.y()*255,startColor.z()*255);
}
void FrameSystem::setEndColor(QColor color)
{
    endColor=QVector3D((float)color.redF(),
                                 (float)color.greenF(),
                                 (float)color.blueF());
    setUpFrameColors();
//    qDebug() << "setEnd: " << QVector3D(endColor.x()*255,endColor.y()*255,endColor.z()*255);
}

QColor FrameSystem::getSolidColor()
{
    return vectorTocolor(solidColor);
}

QColor FrameSystem::getStartColor()
{
    return vectorTocolor(startColor);
}

QColor FrameSystem::getEndColor()
{
    return vectorTocolor(endColor);
}

int FrameSystem::getVertexAllocationSize()
{
    if(frames.size()>0){
        return frames[0].getPointsSize();
    }
    return 0;
}
int FrameSystem::getIndexAllocationSize()
{
    if(frames.size()>0){
        return frames[0].getIndicesSize();
    }
    return 0;
}


