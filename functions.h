#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>
#include <QVector3D>

QVector<QVector3D> toQVector3DArray(const QVector<float> & array);
QVector<float> toFloatArray(const QVector<QVector3D> & array);
QVector3D maxLength(const QVector<QVector3D> & vectors);
QVector3D minLength(const QVector<QVector3D> & vectors);
QVector<float> toLengthArray(const QVector<float> & array);
float calculateScaleMultiplier(float number, float multiplier);


enum ColorMode
{
   SOLID,
   VELOCITY,
   VERTEX_AREA,
   PROJECTED_VERTEX_AREA,
   VERTEX_MASS
};

#endif // FUNCTIONS_H
