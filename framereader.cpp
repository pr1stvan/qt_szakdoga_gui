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

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if(reader.name() == "Points") {
           reader.readNextStartElement();
           if(reader.name() == "DataArray"){

              pointArrayString=reader.readElementText();
           }
        }
        if(reader.name() == "PointData") {
           reader.readNextStartElement();
           if(reader.name()== "DataArray" && reader.attributes().value("Name") == "area"){
              areaArrayString=reader.readElementText();
              reader.readNextStartElement();
              if(reader.name()== "DataArray" && reader.attributes().value("Name") == "velocity"){
                   velocityArrayString=reader.readElementText();
              }
           }
        }
        if(reader.name() == "Polys"){
            reader.readNextStartElement();
            if(reader.name()=="DataArray" && reader.attributes().value("Name") == "connectivity"){
                indexArrayString=reader.readElementText();
            }
        }

    }
    if (reader.hasError()) {
           // do error handling
           //qDebug() << "xml reading error";
    }

    QStringList pointStringList = pointArrayString.trimmed().split(" ");
    QStringList velocityStringList = velocityArrayString.trimmed().split(" ");
    QStringList areaStringList = areaArrayString.trimmed().split(" ");
    QStringList indexStringList = indexArrayString.trimmed().split(" ");

    points.clear();
    velocities.clear();
    areas.clear();
    for(int i = 0 ; i< pointStringList.size(); i++){
        points.push_back(pointStringList.at(i).toFloat());
        velocities.push_back(velocityStringList.at(i).toFloat());
        areas.push_back(areaStringList.at(i).toFloat());
    }

    indices.clear();
    for(int i = 0 ; i< indexStringList.size(); i++){
        indices.push_back(indexStringList.at(i).toInt());
    }

    return 0;
}
