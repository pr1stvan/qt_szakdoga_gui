#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QDebug>

#include "framereader.h"

/*
 This class exists for the sole purpose of creating a translation context.
*/
class XmlStreamLint
{
public:
    Q_DECLARE_TR_FUNCTIONS(XmlStreamLint)
};

QVector<float> FrameReader::getPoints()
{
    return points;
}

QVector<float> FrameReader::getVelocities()
{
    return velocities;
}

QVector<float> FrameReader::getAreas()
{
    return areas;
}

QVector<unsigned int> FrameReader::getIndices()
{
    return indices;
}

int FrameReader::getPointsSize()
{
    return points.size();
}

int FrameReader::getIndexSize()
{
    return indices.size();
}

int FrameReader::loadFile(QString fileName)
{
    QFile inputFile(fileName);

    QTextStream errorStream(stderr);


    if (!QFile::exists(fileName))
    {
        errorStream << XmlStreamLint::tr(
                       "File %1 does not exist.\n").arg(fileName);
        return 1;

    } else if (!inputFile.open(QIODevice::ReadOnly)) {
        errorStream << XmlStreamLint::tr(
                       "Failed to open file %1.\n").arg(fileName);
        return 1;
    }


    QXmlStreamReader reader(&inputFile);

    QString pointArrayString;
    QString areaArrayString;
    QString velocityArrayString;
    QString indexArrayString;

    bool wasPoints=false;
    bool wasPointData=false;
    bool wasPolys=false;
    while (!reader.atEnd()) {
        bool res=reader.readNextStartElement();
        if(res){
            //start element
            if(reader.name() == "Points") {
                wasPoints=true;
            }
            else if(reader.name() == "PointData"){
                wasPointData=true;
            }
            else if(reader.name()=="Polys"){
                wasPolys=true;
            }
            else if(reader.name()=="DataArray"){
                if(wasPoints){
                    pointArrayString=reader.readElementText();
                }
                else if(wasPointData){
                    if(reader.attributes().value("Name") == "area"){
                        areaArrayString=reader.readElementText();
                    }
                    else if(reader.attributes().value("Name") == "velocity"){
                        velocityArrayString=reader.readElementText();
                    }
                }
                else if(wasPolys){
                    if(reader.attributes().value("Name") == "connectivity"){
                        indexArrayString=reader.readElementText();
                    }
                }
            }
        }
        else{
            //end element
            if(reader.name() == "Points") {
                wasPoints=false;
            }
            else if(reader.name() == "PointData"){
                wasPointData=false;
            }
            else if(reader.name()=="Polys"){
                wasPolys=false;
            }
        }

    }
    if (reader.hasError()) {
           // do error handling
           //qDebug() << "xml reading error";
    }

    QStringList pointStringList;
    QStringList velocityStringList;
    QStringList areaStringList;
    QStringList indexStringList;

    QStringList newPointStringList;
    QStringList newVelocityStringList;
    QStringList newAreaStringList;
    QStringList newIndexStringList;

    points.clear();
    velocities.clear();
    areas.clear();
    indices.clear();

    points.squeeze();
    velocities.squeeze();
    areas.squeeze();
    indices.squeeze();

    pointStringList = pointArrayString.split(" ");
    velocityStringList = velocityArrayString.split(" ");
    areaStringList = areaArrayString.split(" ");
    indexStringList = indexArrayString.split(" ");

    for(int i=0; i<pointStringList.size(); i++){
        QString actual=pointStringList.at(i).trimmed();
        if(actual.size()!=0){
            newPointStringList << actual;

        }
    }
    for(int i = 0 ; i< velocityStringList.size(); i++){
        QString actual=velocityStringList.at(i).trimmed();
        if(actual.size()!=0){
            newVelocityStringList << actual;
        }
    }
    for(int i = 0 ; i< areaStringList.size(); i++){
        QString actual=areaStringList.at(i).trimmed();
        if(actual.size()!=0){
            newAreaStringList << actual;
        }

    }
    for(int i = 0 ; i< indexStringList.size(); i++){
        QString actual=indexStringList.at(i).trimmed();
        if(actual.size()!=0){
             newIndexStringList << actual;
        }

    }

    pointStringList=newPointStringList;
    velocityStringList=newVelocityStringList;
    areaStringList=newAreaStringList;
    indexStringList=newIndexStringList;

//    qDebug() << pointStringList;
//    qDebug() << velocityStringList;
//    qDebug() << areaStringList;
//    qDebug() << indexStringList;

    for(int i = 0 ; i< newPointStringList.size(); i++){
        points << pointStringList.at(i).toFloat();
    }
    for(int i = 0 ; i< velocityStringList.size(); i++){
        velocities << velocityStringList.at(i).toFloat();
    }
    for(int i = 0 ; i< areaStringList.size(); i++){
        areas << areaStringList.at(i).toFloat();
    }
    for(int i = 0 ; i< indexStringList.size(); i++){
        indices << indexStringList.at(i).toInt();
    }

    //Check if the velocity and area arrays have correct size
    if(velocities.size() !=0 && velocities.size() != points.size()){
        qDebug() << velocities.size() - points.size() << "more velocity numbers.";
        velocities.clear();
        velocities.squeeze();
    }
    if(areas.size()!=0 && areas.size() != points.size()){
        qDebug() << areas.size() - points.size() << "more area numbers.";
        areas.clear();
        areas.squeeze();
    }

    return 0;
}

bool FrameReader::velocityDataExist()
{
    if(velocities.size() > 0){
        return true;
    }
    return false;
}

bool FrameReader::areaDataExist()
{
    if(areas.size() > 0){
        return true;
    }
    return false;
}

bool FrameReader::indexDataExist()
{
    if(indices.size() > 0){
        return true;
    }
    return false;
}
