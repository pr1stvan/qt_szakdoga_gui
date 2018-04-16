#include <algorithm>

#include "frame.h"
#include "framereader.h"

Frame::Frame(QString fileName)
{
    FrameReader freader;

    freader.loadFile(fileName);

    points=freader.getPoints();
    velocities=freader.getVelocities();
    QVector<float> areas=freader.getAreas();
    indices=freader.getIndices();

    for(int i =0; i<areas.size(); i++){
        if(i%3 ==2){
            vertexAreas.append(areas.at(i-2));
            projectedVertexAreas.append(areas.at(i-1));
            vertexMasses.append(areas.at(i));
        }
    }
}

float Frame::getMin(ColorMode mode)
{
    switch(mode) {
    case VELOCITY:
        return minLength(toQVector3DArray(velocities)).length();
        break;
    case VERTEX_AREA:
        return *std::min_element(vertexAreas.constBegin(),vertexAreas.constEnd());
        break;
    case PROJECTED_VERTEX_AREA:
        return *std::min_element(projectedVertexAreas.constBegin(),projectedVertexAreas.constEnd());
        break;
    case VERTEX_MASS:
        return *std::min_element(vertexMasses.constBegin(),vertexMasses.constEnd());
        break;
    default:
        return 0;
    }
}

float Frame::getMax(ColorMode mode)
{
    switch(mode) {
    case VELOCITY:
        return maxLength(toQVector3DArray(velocities)).length();
        break;
    case VERTEX_AREA:
        return *std::max_element(vertexAreas.constBegin(),vertexAreas.constEnd());
        break;
    case PROJECTED_VERTEX_AREA:
        return *std::max_element(projectedVertexAreas.constBegin(),projectedVertexAreas.constEnd());
        break;
    case VERTEX_MASS:
        return *std::max_element(vertexMasses.constBegin(),vertexMasses.constEnd());
        break;
    default:
        return 0;
    }
}

const QVector<float> &Frame::getVertexAreas()
{
    return vertexAreas;
}

const QVector<float> &Frame::getProjectedVertexAreas()
{
    return projectedVertexAreas;
}

const QVector<float> &Frame::getVertexMasses()
{
    return vertexMasses;
}

void Frame::setUpColors(ColorMode mode, QVector3D startColor, QVector3D endColor, float min, float max)
{
    QVector<float> weights;
    if(mode == SOLID){
        QVector<float> newAmbients;
        QVector<float> newDiffuses;
        QVector<float> newSpeculars;

        for(int i =0 ; i< points.size(); i++){
            if(i%3 == 0){
                newAmbients.append(startColor.x());
                newDiffuses.append(startColor.x());
                newSpeculars.append(1.0f);
            }
            else if(i%3 == 1){
                newAmbients.append(startColor.y());
                newDiffuses.append(startColor.y());
                newSpeculars.append(1.0f);
            }
            else if(i%3 == 2){
                newAmbients.append(startColor.z());
                newDiffuses.append(startColor.z());
                newSpeculars.append(1.0f);
            }
        }

        Ambients=newAmbients;
        Diffuses=newDiffuses;
        Speculars=newSpeculars;

        return;
    }
    else if(mode == VELOCITY){
        weights=toLengthArray(velocities);
    }
    else if(mode == VERTEX_AREA){
        weights=vertexAreas;
    }
    else if(mode == PROJECTED_VERTEX_AREA){
        weights=projectedVertexAreas;
    }
    else if(mode == VERTEX_MASS){
        weights=vertexMasses;
    }
    if(weights.size() == 0)return;


    QVector<float> newAmbients;
    QVector<float> newDiffuses;
    QVector<float> newSpeculars;

    for(int i=0; i< weights.size(); i++){
        float rate=(weights.at(i)-min)/(max-min);

        QVector3D actualColor = (1-rate)*startColor+rate*endColor;

        newAmbients.append(actualColor.x());
        newAmbients.append(actualColor.y());
        newAmbients.append(actualColor.z());

        newDiffuses.append(actualColor.x());
        newDiffuses.append(actualColor.y());
        newDiffuses.append(actualColor.z());

        newSpeculars.append(1.0f);
        newSpeculars.append(1.0f);
        newSpeculars.append(1.0f);
    }

    Ambients=newAmbients;
    Diffuses=newDiffuses;
    Speculars=newSpeculars;

}

