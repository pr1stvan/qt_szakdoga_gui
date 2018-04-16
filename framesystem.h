#ifndef FRAMESYSTEM_H
#define FRAMESYSTEM_H
#include <QDir>

#include "frame.h"

class FrameSystem
{
public:

    QVector<float> getPoints(int i);
    QVector<float> getVelocities(int i);
    QVector<float> getAreas(int i);

    const QVector<float> &getVertexAreas(int i);
    QVector<float> getProjectedVertexAreas(int i);
    QVector<float> getVertexMasses(int i);

    QVector<unsigned int> getIndices(int i);

    QVector<float> getAmbients(int i);
    QVector<float> getDiffuses(int i);
    QVector<float> getSpeculars(int i);

    void setColorMode(ColorMode mode);
    void setSolidColor(QColor color);
    void setStartColor(QColor color);
    void setEndColor(QColor color);

    QColor getSolidColor();
    QColor getStartColor();
    QColor getEndColor();

    FrameSystem();
    QVector3D getStartColorVector(){
        return startColor;
    }
    QVector3D getEndColorVector(){
        return endColor;
    }

    int getVertexAllocationSize();
    int getIndexAllocationSize();

    int size(){
        return frames.size();
    }
    void loadDirectory(QString directoryPath);
    void setUpFrameColors();
    QVector3D getPointsAvgAfterLoading();

    float getMin();
    float getMax();


private:
    QVector<Frame> frames;

    ColorMode colorMode;
    QVector3D solidColor;
    QVector3D startColor;
    QVector3D endColor;


    QStringList getDirectoryFiles(QString directoryPath);


};


#endif // FRAMESYSTEM_H
