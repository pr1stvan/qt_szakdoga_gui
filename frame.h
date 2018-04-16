#ifndef FRAME_H
#define FRAME_H

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QDebug>

#include "functions.h"

class Frame
{
private:
    QVector<float> points;
    QVector<float> velocities;

    QVector<float> vertexAreas;
    QVector<float> projectedVertexAreas;
    QVector<float> vertexMasses;

    QVector<unsigned int> indices;

    QVector<float> Ambients;
    QVector<float> Diffuses;
    QVector<float> Speculars;

public:
    Frame(){

    }
    Frame(QString fileName);

    float getMin(ColorMode mode);
    float getMax(ColorMode mode);

    QVector<float> getPoints()
    {
        return points;
    }
    QVector<float> getVelocities()
    {
        return velocities;
    }

    const QVector<float> & getVertexAreas();
    const QVector<float> &getProjectedVertexAreas();
    const QVector<float> &getVertexMasses();


    const QVector<float> &getAmbients()
    {
        return Ambients;
    }
    const QVector<float> &getDiffuses()
    {
        return Diffuses;
    }
    const QVector<float> &getSpeculars()
    {
        return Speculars;
    }


    QVector<unsigned int> getIndices(){
        return indices;
    }

    int getPointsSize(){
        return points.size();
    }
    int getIndicesSize(){
        return indices.size();
    }

    void setUpColors(ColorMode mode,QVector3D startColor,QVector3D endColor,float min,float max);
    void setUpVertexAreaColors(QVector3D startColor);
    void setUpProjectedVertexAreaColors();
    void setUpVertexMassColors();
};
#endif // FRAME_H
