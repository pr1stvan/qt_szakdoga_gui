#include <algorithm>

#include "frame.h"
#include "framereader.h"

Frame::Frame(QString fileName)
{
    velocity_data_exist=false;
    area_data_exist=false;

    FrameReader freader;

    freader.loadFile(fileName);

    points=freader.getPoints();

    velocity_data_exist=freader.velocityDataExist();
    area_data_exist=freader.areaDataExist();

    if(velocity_data_exist){
        velocities=freader.getVelocities();
    }


    if(area_data_exist){
        areas=freader.getAreas();
        for(int i =0; i<areas.size(); i++){
            if(i%3 ==2){
                vertexAreas.append(areas.at(i-2));
                projectedVertexAreas.append(areas.at(i-1));
                vertexMasses.append(areas.at(i));
            }
        }
    }

    if(freader.indexDataExist()){
        indices=freader.getIndices();
        prefer_drawing_points=false;
    }
    else{
        for(int i=0; i<points.size();i++){
            if(i%3 == 2){
               indices.append(i/3);
            }
        }
        prefer_drawing_points=true;
    }


}

float Frame::getMinPointCoord(int xyz)
{
    if(points.size()==0){
        return 0;
    }

    float min=points[xyz];
    for(int i=xyz+3; i<points.size();i+=3){
        if(points[i]<min){
            min=points[i];
        }
    }
    return min;
}
float Frame::getMaxPointCoord(int xyz)
{
    if(points.size()==0){
        return 0;
    }

    float max=points[xyz];
    for(int i=xyz+3; i<points.size();i+=3){
        if(points[i]>max){
            max=points[i];
        }
    }
    return max;
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

const QVector<float> &Frame::getAreas()
{
    return areas;
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

void Frame::scalePointsAfterLoading(float scale)
{
    for(int i=0;i<points.size();i++){
        points[i]=points[i]*scale;
    }
}

bool Frame::preferDrawingPoints()
{
    return prefer_drawing_points;
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
        if(!velocity_data_exist){
            qDebug() << "setUpFrameColors: no velocity data";
            return;
        }
        weights=toLengthArray(velocities);
    }
    else if(mode == VERTEX_AREA){
        if(!area_data_exist){
            qDebug() << "setUpFrameColors: no area data";
            return;
        }
        weights=vertexAreas;
    }
    else if(mode == PROJECTED_VERTEX_AREA){
        if(!area_data_exist){
            qDebug() << "setUpFrameColors: no area data";
            return;
        }
        weights=projectedVertexAreas;
    }
    else if(mode == VERTEX_MASS){
        if(!area_data_exist){
            qDebug() << "setUpFrameColors: no area data";
            return;
        }
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

