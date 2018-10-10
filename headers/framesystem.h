#ifndef FRAMESYSTEM_H
#define FRAMESYSTEM_H
#include <QDir>

#include "frame.h"

class FrameSystem
{
public:

    const QVector<float> &getPoints(int i);
    const QVector<float> &getVelocities(int i);
    const QVector<float> &getAreas(int i);

    const QVector<float> &getVertexAreas(int i);
    const QVector<float> &getProjectedVertexAreas(int i);
    const QVector<float> &getVertexMasses(int i);

    const QVector<unsigned int> &getIndices(int i);

    const QVector<float> &getAmbients(int i);
    const QVector<float> &getDiffuses(int i);
    const QVector<float> &getSpeculars(int i);

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
    void loadFiles(QStringList filePaths);
    void setUpFrameColors();
    QVector3D getPointsAvgAfterLoading();
    void scalePointsAfterLoading();

    float getMin();
    float getMax();

    float getMinPointCoord(int xyz);
    float getMaxPointCoord(int xyz);

    bool velocityDataExist(){
        return velocity_data_exist;
    }
    bool areaDataExist(){
        return area_data_exist;
    }

    ColorMode getColorMode(){
        return colorMode;
    }
    bool preferDrawingPoints(){
        return prefer_drawing_points;
    }
private:
    QVector<Frame> frames;

    ColorMode colorMode;
    QVector3D solidColor;
    QVector3D startColor;
    QVector3D endColor;

    bool prefer_drawing_points;
    bool velocity_data_exist,area_data_exist;

    QStringList getDirectoryFiles(QString directoryPath);


};


#endif // FRAMESYSTEM_H
