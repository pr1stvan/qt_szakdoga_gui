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
    bool velocity_data_exist,area_data_exist;
    bool prefer_drawing_points;

    QVector<float> points;
    QVector<float> velocities;

    QVector<float> areas;
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

    bool getVelocityDataExist(){
        return velocity_data_exist;
    }
    bool getAreaDataExist(){
        return area_data_exist;
    }

    float getMinPointCoord(int xyz);
    float getMaxPointCoord(int xyz);

    float getMin(ColorMode mode);
    float getMax(ColorMode mode);

    const QVector<float> &getPoints()
    {
        return points;
    }
     const QVector<float> &getVelocities()
    {
        return velocities;
    }

    const QVector<float> &getAreas();
    const QVector<float> &getVertexAreas();
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


    const QVector<unsigned int> &getIndices(){
        return indices;
    }
    void scalePointsAfterLoading(float scale);

    int getPointsSize(){
        return points.size();
    }
    int getIndicesSize(){
        return indices.size();
    }

    bool preferDrawingPoints();
    bool velocityDataExist();
    bool areaDataExist();
    bool indexDataExist();

    void setUpColors(ColorMode mode,QVector3D startColor,QVector3D endColor,float min,float max);
    void setUpVertexAreaColors(QVector3D startColor);
    void setUpProjectedVertexAreaColors();
    void setUpVertexMassColors();
};
#endif // FRAME_H
