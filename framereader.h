#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include <QVector>

class FrameReader
{
private:
    QVector<float> points;
    QVector<float> velocities;
    QVector<float> areas;
    QVector<unsigned int> indices;

public:

    QVector<float> getPoints();
    QVector<float> getVelocities();
    QVector<float> getAreas();
    QVector<unsigned int> getIndices();

    int getPointsSize();
    int getIndexSize();

    int loadFile(QString fileName);
};

#endif // FRAMEREADER_H
