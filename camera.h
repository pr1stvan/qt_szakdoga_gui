#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>


class Camera : public QObject
{
    Q_OBJECT

private:
    QVector3D position;
    QVector3D lookAtPoint;
    QVector3D up;
    QVector3D right;

    int canvasWidth;
    int canvasHeight;

    int xAngle_degrees;
    int yAngle_degrees;
    int distance;
    int viewAngle;

    int distanceVelocity;

public slots:
    void setXRotation(int degrees);
    void setYRotation(int degrees);
    void setDistance(int distance);
    void setViewAngle(int degrees);


signals:
    void xRotationChanged(int degrees);
    void yRotationChanged(int degrees);
    void distanceChanged(int distance);
    void viewAngleChanged(int degrees);

public:


    Camera();

    QMatrix4x4 V();
    QMatrix4x4 P();

    void updateSignal();
    void rotateToNewPosition();
    void setPosition(QVector3D position);

    void setLookAtPoint(QVector3D lookAtPoint);

    void setCanvas(int width,int height){
        canvasWidth=width;
        canvasHeight=height;
    }

    void rotateHorizontal(int dh_degrees);
    void rotateVertical(int dv_degrees);
    void increaseDistance(int dDistance);

};

#endif // CAMERA_H
